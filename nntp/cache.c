/**
 * @file
 * NNTP caching
 *
 * @authors
 * Copyright (C) 2018 Richard Russon <rich@flatcap.org>
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @page nntp_cache NNTP caching
 *
 * NNTP caching
 */

#include "config.h"
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "nntp_private.h"
#include "mutt/mutt.h"
#include "email/lib.h"
#include "conn/conn.h"
#include "bcache.h"
#include "globals.h"
#include "mutt_account.h"
#include "muttlib.h"
#include "nntp.h"

/**
 * cache_expand - Make fully qualified cache file name
 * @param dst    Buffer for filename
 * @param dstlen Length of buffer
 * @param acct   Account
 * @param src    Path to add to the URL
 */
void cache_expand(char *dst, size_t dstlen, struct ConnAccount *acct, const char *src)
{
  char *c = NULL;
  char file[PATH_MAX];

  /* server subdirectory */
  if (acct)
  {
    struct Url url;

    mutt_account_tourl(acct, &url);
    url.path = mutt_str_strdup(src);
    url_tostring(&url, file, sizeof(file), U_PATH);
    FREE(&url.path);
  }
  else
    mutt_str_strfcpy(file, src ? src : "", sizeof(file));

  snprintf(dst, dstlen, "%s/%s", NewsCacheDir, file);

  /* remove trailing slash */
  c = dst + strlen(dst) - 1;
  if (*c == '/')
    *c = '\0';
  mutt_expand_path(dst, dstlen);
  mutt_encode_path(dst, dstlen, dst);
}

#ifdef USE_HCACHE
/**
 * nntp_hcache_namer - Compose hcache file names - Implements ::hcache_namer_t
 */
static int nntp_hcache_namer(const char *path, char *dest, size_t destlen)
{
  int count = snprintf(dest, destlen, "%s.hcache", path);

  /* Strip out any directories in the path */
  char *first = strchr(dest, '/');
  char *last = strrchr(dest, '/');
  if (first && last && (last > first))
  {
    memmove(first, last, strlen(last) + 1);
    count -= (last - first);
  }

  return count;
}

/**
 * nntp_hcache_open - Open newsgroup hcache
 * @param m Mailbox
 * @retval ptr  Header cache
 * @retval NULL Error
 */
header_cache_t *nntp_hcache_open(struct Mailbox *m)
{
  struct NntpMboxData *mdata = nntp_mdata_get(m);
  struct NntpAccountData *adata = nntp_adata_get(m);

  if (!adata || !mdata || !mdata->adata || !mdata->adata->cacheable ||
      !mdata->adata->conn || !mdata->group ||
      !(mdata->newsrc_ent || mdata->subscribed || SaveUnsubscribed))
  {
    return NULL;
  }

  struct Url url;
  char file[PATH_MAX];

  mutt_account_tourl(&adata->conn->account, &url);
  url.path = mdata->group;
  url_tostring(&url, file, sizeof(file), U_PATH);
  return mutt_hcache_open(NewsCacheDir, file, nntp_hcache_namer);
}

/**
 * nntp_hcache_update - Remove stale cached headers
 * @param m  Mailbox
 * @param hc Header cache
 */
void nntp_hcache_update(struct Mailbox *m, header_cache_t *hc)
{
  struct NntpMboxData *mdata = nntp_mdata_get(m);
  if (!mdata || !hc)
    return;

  char buf[16];
  bool old = false;
  anum_t first = 0, last = 0;

  /* fetch previous values of first and last */
  void *hdata = mutt_hcache_fetch_raw(hc, "index", 5);
  if (hdata)
  {
    mutt_debug(2, "mutt_hcache_fetch index: %s\n", (char *) hdata);
    if (sscanf(hdata, ANUM " " ANUM, &first, &last) == 2)
    {
      old = true;
      mdata->last_cached = last;

      /* clean removed headers from cache */
      for (anum_t current = first; current <= last; current++)
      {
        if (current >= mdata->first_message && current <= mdata->last_message)
          continue;

        snprintf(buf, sizeof(buf), "%u", current);
        mutt_debug(2, "mutt_hcache_delete %s\n", buf);
        mutt_hcache_delete(hc, buf, strlen(buf));
      }
    }
    mutt_hcache_free(hc, &hdata);
  }

  /* store current values of first and last */
  if (!old || mdata->first_message != first || mdata->last_message != last)
  {
    snprintf(buf, sizeof(buf), "%u %u", mdata->first_message, mdata->last_message);
    mutt_debug(2, "mutt_hcache_store index: %s\n", buf);
    mutt_hcache_store_raw(hc, "index", 5, buf, strlen(buf) + 1);
  }
}

/**
 * nntp_hcache_check_files - Read the cached list of articles
 * @param m     Mailbox
 * @param adata NNTP server
 */
void nntp_hcache_check_files(struct Mailbox *m, struct NntpAccountData *adata)
{
  if (!adata || !adata->cacheable || !adata->newsrc_file)
    return;

  DIR *dp = opendir(adata->newsrc_file);
  if (!dp)
    return;

  struct dirent *entry = NULL;
  while ((entry = readdir(dp)))
  {
    char *group = entry->d_name;

    char *p = group + strlen(group) - 7;
    if ((strlen(group) < 8) || (strcmp(p, ".hcache") != 0))
      continue;

    *p = '\0';
    struct NntpMboxData *mdata = mutt_hash_find(adata->groups_hash, group);
    if (!mdata)
      continue;

    header_cache_t *hc = nntp_hcache_open(m);
    if (!hc)
      continue;

    /* fetch previous values of first and last */
    void *hdata = mutt_hcache_fetch_raw(hc, "index", 5);
    if (hdata)
    {
      anum_t first = 0, last = 0;

      if (sscanf(hdata, ANUM " " ANUM, &first, &last) == 2)
      {
        if (mdata->deleted)
        {
          mdata->first_message = first;
          mdata->last_message = last;
        }
        if ((last >= mdata->first_message) && (last <= mdata->last_message))
        {
          mdata->last_cached = last;
          mutt_debug(2, "%s last_cached=%u\n", mdata->group, last);
        }
      }
      mutt_hcache_free(hc, &hdata);
    }
    mutt_hcache_close(hc);
  }
  closedir(dp);
}
#endif

/**
 * nntp_bcache_delete - Remove bcache file - Implements ::bcache_list_t
 * @retval 0 Always
 */
static int nntp_bcache_delete(const char *id, struct BodyCache *bcache, void *data)
{
  struct NntpMboxData *mdata = data;
  anum_t anum;
  char c;

  if (!mdata || sscanf(id, ANUM "%c", &anum, &c) != 1 ||
      anum < mdata->first_message || anum > mdata->last_message)
  {
    if (mdata)
      mutt_debug(2, "mutt_bcache_del %s\n", id);
    mutt_bcache_del(bcache, id);
  }
  return 0;
}

/**
 * nntp_bcache_update - Remove stale cached messages
 * @param mdata NNTP Mailbox data
 */
void nntp_bcache_update(struct NntpMboxData *mdata)
{
  mutt_bcache_list(mdata->bcache, nntp_bcache_delete, mdata);
}

/**
 * nntp_delete_group_cache - Remove hcache and bcache of newsgroup
 * @param mdata NNTP Mailbox data
 */
void nntp_delete_group_cache(struct NntpMboxData *mdata)
{
  if (!mdata || !mdata->adata || !mdata->adata->cacheable)
    return;

#ifdef USE_HCACHE
  char file[PATH_MAX];
  nntp_hcache_namer(mdata->group, file, sizeof(file));
  cache_expand(file, sizeof(file), &mdata->adata->conn->account, file);
  unlink(file);
  mdata->last_cached = 0;
  mutt_debug(2, "%s\n", file);
#endif

  if (!mdata->bcache)
  {
    mdata->bcache = mutt_bcache_open(&mdata->adata->conn->account, mdata->group);
  }
  if (mdata->bcache)
  {
    mutt_debug(2, "%s/*\n", mdata->group);
    mutt_bcache_list(mdata->bcache, nntp_bcache_delete, NULL);
    mutt_bcache_close(&mdata->bcache);
  }
}

/**
 * nntp_clear_cache - Clear the NNTP cache
 * @param adata NNTP server
 *
 * Remove hcache and bcache of all unexistent and unsubscribed newsgroups
 */
void nntp_clear_cache(struct NntpAccountData *adata)
{
  char file[PATH_MAX];
  char *fp = NULL;
  struct dirent *entry = NULL;
  DIR *dp = NULL;

  if (!adata || !adata->cacheable)
    return;

  cache_expand(file, sizeof(file), &adata->conn->account, NULL);
  dp = opendir(file);
  if (dp)
  {
    mutt_str_strncat(file, sizeof(file), "/", 1);
    fp = file + strlen(file);
    while ((entry = readdir(dp)))
    {
      char *group = entry->d_name;
      struct stat sb;
      struct NntpMboxData *mdata = NULL;
      struct NntpMboxData tmp_mdata;

      if ((mutt_str_strcmp(group, ".") == 0) || (mutt_str_strcmp(group, "..") == 0))
        continue;
      *fp = '\0';
      mutt_str_strncat(file, sizeof(file), group, strlen(group));
      if (stat(file, &sb))
        continue;

#ifdef USE_HCACHE
      if (S_ISREG(sb.st_mode))
      {
        char *ext = group + strlen(group) - 7;
        if (strlen(group) < 8 || (mutt_str_strcmp(ext, ".hcache") != 0))
          continue;
        *ext = '\0';
      }
      else
#endif
          if (!S_ISDIR(sb.st_mode))
        continue;

      mdata = mutt_hash_find(adata->groups_hash, group);
      if (!mdata)
      {
        mdata = &tmp_mdata;
        mdata->adata = adata;
        mdata->group = group;
        mdata->bcache = NULL;
      }
      else if (mdata->newsrc_ent || mdata->subscribed || SaveUnsubscribed)
        continue;

      nntp_delete_group_cache(mdata);
      if (S_ISDIR(sb.st_mode))
      {
        rmdir(file);
        mutt_debug(2, "%s\n", file);
      }
    }
    closedir(dp);
  }
}
