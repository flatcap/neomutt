/**
 * @file
 * Usenet network mailbox type; talk to an NNTP server
 *
 * @authors
 * Copyright (C) 1998 Brandon Long <blong@fiction.net>
 * Copyright (C) 1999 Andrej Gritsenko <andrej@lucky.net>
 * Copyright (C) 2000-2012 Vsevolod Volkov <vvv@mutt.org.ua>
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
 * @page nntp NNTP: Usenet network mailbox type; talk to an NNTP server
 *
 * Usenet network mailbox type; talk to an NNTP server
 *
 * | File          | Description          |
 * | :------------ | :------------------- |
 * | nntp/newsrc.c | @subpage nntp_newsrc |
 * | nntp/nntp.c   | @subpage nntp_nntp   |
 */

#ifndef MUTT_NNTP_NNTP_H
#define MUTT_NNTP_NNTP_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include "format_flags.h"
#include "mx.h"

struct Account;
struct Header;
struct Context;

/* These Config Variables are only used in nntp/nntp.c */
extern char *NntpAuthenticators;
extern short NntpContext;
extern bool  NntpListgroup;
extern bool  NntpLoadDescription;
extern short NntpPoll;
extern bool  ShowNewNews;

/* These Config Variables are only used in nntp/newsrc.c */
extern char *NewsCacheDir;
extern char *Newsrc;

/* article number type and format */
#define anum_t uint32_t
#define ANUM "%u"

/**
 * struct NntpServer - NNTP-specific server data
 */
struct NntpServer
{
  bool hasCAPABILITIES    : 1;
  bool hasSTARTTLS        : 1;
  bool hasDATE            : 1;
  bool hasLIST_NEWSGROUPS : 1;
  bool hasXGTITLE         : 1;
  bool hasLISTGROUP       : 1;
  bool hasLISTGROUPrange  : 1;
  bool hasOVER            : 1;
  bool hasXOVER           : 1;
  unsigned int use_tls    : 3;
  unsigned int status     : 3;
  bool cacheable          : 1;
  bool newsrc_modified    : 1;
  FILE *newsrc_fp;
  char *newsrc_file;
  char *authenticators;
  char *overview_fmt;
  off_t size;
  time_t mtime;
  time_t newgroups_time;
  time_t check_time;
  unsigned int groups_num;
  unsigned int groups_max;
  void **groups_list;
  struct Hash *groups_hash;
  struct Connection *conn;
};

/**
 * struct NntpHeaderData - NNTP-specific header data
 */
struct NntpHeaderData
{
  anum_t article_num;
  bool parsed : 1;
};

/**
 * struct NntpAcache - NNTP article cache
 */
struct NntpAcache
{
  unsigned int index;
  char *path;
};

/**
 * struct NewsrcEntry - An entry in a .newsrc (subscribed newsgroups)
 */
struct NewsrcEntry
{
  anum_t first;
  anum_t last;
};

/* number of entries in article cache */
#define NNTP_ACACHE_LEN 10

/**
 * struct NntpData - NNTP-specific server data
 */
struct NntpData
{
  char *group;
  char *desc;
  anum_t first_message;
  anum_t last_message;
  anum_t last_loaded;
  anum_t last_cached;
  anum_t unread;
  bool subscribed : 1;
  bool new        : 1;
  bool allowed    : 1;
  bool deleted    : 1;
  unsigned int newsrc_len;
  struct NewsrcEntry *newsrc_ent;
  struct NntpServer *nserv;
  struct NntpAcache acache[NNTP_ACACHE_LEN];
  struct BodyCache *bcache;
};

struct NntpServer *nntp_select_server(struct Context *ctx, char *server, bool leave_lock);
struct NntpData *mutt_newsgroup_subscribe(struct NntpServer *nserv, char *group);
struct NntpData *mutt_newsgroup_unsubscribe(struct NntpServer *nserv, char *group);
struct NntpData *mutt_newsgroup_catchup(struct Context *ctx, struct NntpServer *nserv, char *group);
struct NntpData *mutt_newsgroup_uncatchup(struct Context *ctx, struct NntpServer *nserv, char *group);
int nntp_active_fetch(struct NntpServer *nserv, bool new);
int nntp_newsrc_update(struct NntpServer *nserv);
int nntp_post(struct Context *ctx, const char *msg);
int nntp_check_msgid(struct Context *ctx, const char *msgid);
int nntp_check_children(struct Context *ctx, const char *msgid);
int nntp_newsrc_parse(struct NntpServer *nserv);
void nntp_newsrc_close(struct NntpServer *nserv);
void nntp_mailbox(struct Mailbox *mailbox, char *buf, size_t buflen);
void nntp_expand_path(char *buf, size_t buflen, struct Account *acct);
void nntp_clear_cache(struct NntpServer *nserv);
const char *nntp_format_str(char *buf, size_t buflen, size_t col, int cols, char op,
                            const char *src, const char *prec, const char *if_str,
                            const char *else_str, unsigned long data, enum FormatFlag flags);

void nntp_article_status(struct Mailbox *mailbox, struct Header *hdr, char *group, anum_t anum);

extern struct NntpServer *CurrentNewsSrv;

int nntp_compare_order(const void *a, const void *b);
int nntp_path_probe(const char *path, const struct stat *st);

extern struct MxOps mx_nntp_ops;

#endif /* MUTT_NNTP_NNTP_H */
