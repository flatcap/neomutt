#include "config.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "imap/imap_private.h"
#include "maildir/maildir_private.h"
#include "notmuch/notmuch_private.h"
#include "pop/pop_private.h"
#include "email/lib.h"
#include "conn/conn.h"
#include "account.h"
#include "compress.h"
#include "context.h"
#include "globals.h"
#include "mailbox.h"
#include "mbox/mbox.h"
#include "nntp/nntp.h"
#include "notmuch/mutt_notmuch.h"

void dot_type_bool(FILE *fp, const char *name, bool val)
{
  static const char *values[] = { "false", "true" };
  fprintf(fp, "\t\t<tr>\n");
  fprintf(fp, "\t\t\t<td border=\"0\" align=\"left\">%s</td>\n", name);
  fprintf(fp, "\t\t\t<td border=\"0\">=</td>\n");
  fprintf(fp, "\t\t\t<td border=\"0\" align=\"left\">%s</td>\n", values[val]);
  fprintf(fp, "\t\t</tr>\n");
}

void dot_type_date(char *buf, size_t buflen, time_t timestamp)
{
  struct tm *tm = localtime(&timestamp);

  snprintf(buf, buflen, "%d-%02d-%02d %02d:%02d", tm->tm_year + 1900,
           tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min);
}

void dot_type_file(FILE *fp, const char *name, FILE *struct_fp)
{
  fprintf(fp, "\t\t<tr>\n");
  fprintf(fp, "\t\t\t<td border=\"0\" align=\"left\">%s</td>\n", name);
  fprintf(fp, "\t\t\t<td border=\"0\">=</td>\n");
  if (struct_fp)
  {
    fprintf(fp, "\t\t\t<td border=\"0\" align=\"left\">%p (%d)</td>\n",
            (void *) struct_fp, struct_fp->_fileno);
  }
  else
  {
    fprintf(fp, "\t\t\t<td border=\"0\" align=\"left\">NULL</td>\n");
  }
  fprintf(fp, "\t\t</tr>\n");
}

void dot_type_number(FILE *fp, const char *name, int num)
{
  fprintf(fp, "\t\t<tr>\n");
  fprintf(fp, "\t\t\t<td border=\"0\" align=\"left\">%s</td>\n", name);
  fprintf(fp, "\t\t\t<td border=\"0\">=</td>\n");
  fprintf(fp, "\t\t\t<td border=\"0\" align=\"left\">%d</td>\n", num);
  fprintf(fp, "\t\t</tr>\n");
}

void dot_type_string_escape(char *buf, size_t buflen)
{
  for (; buf[0]; buf++)
  {
    if (buf[0] == '<')
      mutt_str_inline_replace(buf, buflen, 1, "&lt;");
    else if (buf[0] == '>')
      mutt_str_inline_replace(buf, buflen, 1, "&gt;");
  }
}

void dot_type_string(FILE *fp, const char *name, const char *str)
{
  char buf[1024] = "[NULL]";

  if (str)
  {
    mutt_str_strfcpy(buf, str, sizeof(buf));
    dot_type_string_escape(buf, sizeof(buf));
  }

  bool quoted = ((buf[0] != '[') && (buf[0] != '*'));

  fprintf(fp, "\t\t<tr>\n");
  fprintf(fp, "\t\t\t<td border=\"0\" align=\"left\">%s</td>\n", name);
  fprintf(fp, "\t\t\t<td border=\"0\">=</td>\n");
  if (quoted)
    fprintf(fp, "\t\t\t<td border=\"0\" align=\"left\">\"%s\"</td>\n", buf);
  else
    fprintf(fp, "\t\t\t<td border=\"0\" align=\"left\">%s</td>\n", buf);
  fprintf(fp, "\t\t</tr>\n");
}

void dot_type_umask(char *buf, size_t buflen, int umask)
{
  snprintf(buf, buflen, "0%03o", umask);
}

void dot_ptr_name(char *buf, size_t buflen, void *ptr)
{
  snprintf(buf, buflen, "obj_%p", ptr);
}

void dot_ptr(FILE *fp, const char *name, void *ptr, const char *colour)
{
  fprintf(fp, "\t\t<tr>\n");
  fprintf(fp, "\t\t\t<td border=\"0\" align=\"left\">%s</td>\n", name);
  fprintf(fp, "\t\t\t<td border=\"0\">=</td>\n");
  if (colour && ptr)
  {
    fprintf(fp, "\t\t\t<td border=\"0\" align=\"left\" bgcolor=\"%s\">%p</td>\n",
            colour, ptr);
  }
  else
  {
    fprintf(fp, "\t\t\t<td border=\"0\" align=\"left\">%p</td>\n", ptr);
  }
  fprintf(fp, "\t\t</tr>\n");
}

void dot_add_link(struct ListHead *links, void *src, void *dst, const char *label, bool back)
{
  if (!src || !dst)
    return;

  char obj1[16] = { 0 };
  char obj2[16] = { 0 };
  char text[256] = { 0 };
  char lstr[128] = { 0 };

  dot_ptr_name(obj1, sizeof(obj1), src);
  dot_ptr_name(obj2, sizeof(obj2), dst);

  if (label)
    snprintf(lstr, sizeof(lstr), "edgetooltip=\"%s\"", label);

  snprintf(text, sizeof(text), "%s -> %s [ %s %s ]", obj1, obj2,
           back ? "dir=back" : "", lstr);
  mutt_list_insert_tail(links, mutt_str_strdup(text));
}

void dot_graph_header(FILE *fp)
{
  fprintf(fp, "digraph neomutt\n");
  fprintf(fp, "{\n\n");

  fprintf(fp, "\tgraph [\n");
  fprintf(fp, "\t\trankdir=\"TB\"\n");
  fprintf(fp, "\t\tnodesep=\"0.5\"\n");
  fprintf(fp, "\t\tranksep=\"0.5\"\n");
  fprintf(fp, "\t];\n");
  fprintf(fp, "\n");
  fprintf(fp, "\tnode [\n");
  fprintf(fp, "\t\tshape=\"plain\"\n");
  fprintf(fp, "\t];\n");
  fprintf(fp, "\n");
  fprintf(fp, "\tedge [\n");
  fprintf(fp, "\t\tpenwidth=\"4.5\"\n");
  fprintf(fp, "\t\tarrowsize=\"1.0\"\n");
  fprintf(fp, "\t\tcolor=\"#c0c0c0\"\n");
  fprintf(fp, "\t];\n");
  fprintf(fp, "\n");
}

void dot_graph_footer(FILE *fp, struct ListHead *links)
{
  fprintf(fp, "\n");
  struct ListNode *np = NULL;
  STAILQ_FOREACH(np, links, entries)
  {
    fprintf(fp, "\t%s;\n", np->data);
  }
  fprintf(fp, "\n}\n");
}

void dot_object_header(FILE *fp, void *ptr, const char *name, const char *colour)
{
  char obj[16] = { 0 };
  dot_ptr_name(obj, sizeof(obj), ptr);

  if (!colour)
    colour = "#ffff80";

  fprintf(fp, "\t%s [\n", obj);
  fprintf(fp, "\t\tlabel=<<table cellspacing=\"0\" border=\"1\" rows=\"*\" "
              "color=\"#d0d0d0\">\n");
  fprintf(fp, "\t\t<tr>\n");
  fprintf(fp, "\t\t\t<td border=\"0\" align=\"left\" bgcolor=\"%s\" port=\"top\" colspan=\"3\"><font color=\"#000000\" point-size=\"20\"><b>%s</b></font> <font point-size=\"12\">(%p)</font></td>\n",
          colour, name, ptr);
  fprintf(fp, "\t\t</tr>\n");
}

void dot_object_footer(FILE *fp)
{
  fprintf(fp, "\t\t</table>>\n");
  fprintf(fp, "\t];\n");
  fprintf(fp, "\n");
}

void dot_node(FILE *fp, void *ptr, const char *name, const char *colour)
{
  char obj[16] = { 0 };
  dot_ptr_name(obj, sizeof(obj), ptr);

  fprintf(fp, "\t%s [\n", obj);
  fprintf(fp, "\t\tlabel=<<table cellspacing=\"0\" border=\"1\" rows=\"*\" "
              "color=\"#d0d0d0\">\n");
  fprintf(fp, "\t\t<tr>\n");
  fprintf(fp, "\t\t\t<td border=\"0\" bgcolor=\"%s\" port=\"top\"><font color=\"#000000\" point-size=\"20\"><b>%s</b></font></td>\n",
          colour, name);
  fprintf(fp, "\t\t</tr>\n");
  dot_object_footer(fp);
}

void dot_node_link(FILE *fp, void *ptr, const char *name, void *link, const char *colour)
{
  char obj[16] = { 0 };
  dot_ptr_name(obj, sizeof(obj), ptr);

  fprintf(fp, "\t%s [\n", obj);
  fprintf(fp, "\t\tlabel=<<table cellspacing=\"0\" border=\"1\" rows=\"*\" "
              "color=\"#d0d0d0\">\n");
  fprintf(fp, "\t\t<tr>\n");
  fprintf(fp, "\t\t\t<td border=\"0\" bgcolor=\"%s\" port=\"top\"><font color=\"#000000\" point-size=\"20\"><b>%s</b></font></td>\n",
          colour, name);
  fprintf(fp, "\t\t</tr>\n");

  fprintf(fp, "\t\t<tr>\n");
  fprintf(fp, "\t\t\t<td border=\"0\" align=\"left\" bgcolor=\"%s\">%p</td>\n", colour, link);
  fprintf(fp, "\t\t</tr>\n");

  dot_object_footer(fp);
}

void dot_path_fs(char *buf, size_t buflen, const char *path)
{
  const char *slash = strrchr(path, '/');
  if (slash)
    slash++;
  else
    slash = path;

  mutt_str_strfcpy(buf, slash, buflen);
}

void dot_path_imap(char *buf, size_t buflen, const char *path)
{
  char tmp[1024] = { 0 };
  mutt_str_strfcpy(tmp, path, sizeof(tmp));

  struct Url *u = url_parse(tmp);

  if (u->path && (u->path[0] != '\0'))
    mutt_str_strfcpy(buf, u->path, buflen);
  else
    snprintf(buf, buflen, "%s:%s", u->host, u->user);

  url_free(&u);
}

void dot_comp(FILE *fp, struct CompressInfo *ci, struct ListHead *links)
{
  dot_object_header(fp, ci, "CompressInfo", "#c0c060");
  dot_type_string(fp, "append", ci->cmd_append);
  dot_type_string(fp, "close", ci->cmd_close);
  dot_type_string(fp, "open", ci->cmd_open);
  dot_object_footer(fp);
}

void dot_mailbox_type(FILE *fp, const char *name, enum MailboxType type)
{
  const char *typestr = NULL;

  switch (type)
  {
    case MUTT_MBOX:
      typestr = "MBOX";
      break;
    case MUTT_MMDF:
      typestr = "MMDF";
      break;
    case MUTT_MH:
      typestr = "MH";
      break;
    case MUTT_MAILDIR:
      typestr = "MAILDIR";
      break;
    case MUTT_NNTP:
      typestr = "NNTP";
      break;
    case MUTT_IMAP:
      typestr = "IMAP";
      break;
    case MUTT_NOTMUCH:
      typestr = "NOTMUCH";
      break;
    case MUTT_POP:
      typestr = "POP";
      break;
    case MUTT_COMPRESSED:
      typestr = "COMPRESSED";
      break;
    default:
      typestr = "UNKNOWN";
  }

  fprintf(fp, "\t\t<tr>\n");
  fprintf(fp, "\t\t\t<td border=\"0\" align=\"left\">%s</td>\n", name);
  fprintf(fp, "\t\t\t<td border=\"0\">=</td>\n");
  fprintf(fp, "\t\t\t<td border=\"0\" align=\"left\">%s</td>\n", typestr);
  fprintf(fp, "\t\t</tr>\n");
}

void dot_mailbox_imap(FILE *fp, struct ImapMboxData *mdata, struct ListHead *links)
{
  dot_object_header(fp, mdata, "ImapMboxData", "#60c060");
  dot_type_string(fp, "name", mdata->name);
  dot_type_string(fp, "munge_name", mdata->munge_name);
  dot_type_string(fp, "real_name", mdata->real_name);
  dot_object_footer(fp);
}

void dot_mailbox_maildir(FILE *fp, struct MaildirMboxData *mdata, struct ListHead *links)
{
  char buf[64] = { 0 };

  dot_object_header(fp, mdata, "MaildirMboxData", "#60c060");

  dot_type_date(buf, sizeof(buf), mdata->mtime_cur.tv_sec);
  dot_type_string(fp, "mtime_cur", buf);

  dot_type_umask(buf, sizeof(buf), mdata->mh_umask);
  dot_type_string(fp, "mh_umask", buf);
  dot_object_footer(fp);
}

void dot_mailbox_mbox(FILE *fp, struct MboxAccountData *mdata, struct ListHead *links)
{
  char buf[64] = { 0 };

  dot_object_header(fp, mdata, "MboxAccountData", "#60c060");
  dot_ptr(fp, "fp", mdata->fp, NULL);

  dot_type_date(buf, sizeof(buf), mdata->atime.tv_sec);
  dot_type_string(fp, "atime", buf);

  dot_object_footer(fp);

#if 0
  FILE *fp;
  struct timespec atime;
  bool locked : 1;
  bool append : 1;
#endif
}

void dot_mailbox_nntp(FILE *fp, struct NntpMboxData *mdata, struct ListHead *links)
{
  dot_object_header(fp, mdata, "NntpMboxData", "#60c060");
  dot_type_string(fp, "group", mdata->group);
  dot_type_string(fp, "desc", mdata->desc);

  dot_type_number(fp, "first_message", mdata->first_message);
  dot_type_number(fp, "last_message", mdata->last_message);
  dot_type_number(fp, "last_loaded", mdata->last_loaded);
  dot_type_number(fp, "last_cached", mdata->last_cached);
  dot_type_number(fp, "unread", mdata->unread);

  dot_type_bool(fp, "subscribed", mdata->subscribed);
  dot_type_bool(fp, "new", mdata->new);
  dot_type_bool(fp, "allowed", mdata->allowed);
  dot_type_bool(fp, "deleted", mdata->deleted);

  dot_object_footer(fp);

#if 0
  unsigned int newsrc_len;
  struct NewsrcEntry *newsrc_ent;
  struct NntpAccountData *adata;
  struct NntpAcache acache[NNTP_ACACHE_LEN];
  struct BodyCache *bcache;
#endif
}

void dot_mailbox_notmuch(FILE *fp, struct NmMboxData *mdata, struct ListHead *links)
{
  dot_object_header(fp, mdata, "NmMboxData", "#60c060");
  dot_type_number(fp, "db_limit", mdata->db_limit);
  dot_object_footer(fp);

#if 0
  char *db_query;
  int db_limit;
  enum NmQueryType query_type;
  struct Progress progress;
  int oldmsgcount;
  int ignmsgcount;
  bool noprogress : 1;
  bool longrun : 1;
  bool trans : 1;
  bool progress_ready : 1;
#endif
}

void dot_mailbox_pop(FILE *fp, struct PopAccountData *mdata, struct ListHead *links)
{
  dot_object_header(fp, mdata, "PopAccountData", "#60c060");
  dot_ptr(fp, "conn", mdata->conn, "#ff8080");
  dot_object_footer(fp);

#if 0
  unsigned int status : 2;
  bool capabilities : 1;
  unsigned int use_stls : 2;
  bool cmd_capa : 1;
  bool cmd_stls : 1;
  unsigned int cmd_user : 2;
  unsigned int cmd_uidl : 2;
  unsigned int cmd_top : 2;
  bool resp_codes : 1;
  bool expire : 1;
  bool clear_cache : 1;
  size_t size;
  time_t check_time;
  time_t login_delay;
  char *auth_list;
  char *timestamp;
  struct BodyCache *bcache;
  char err_msg[POP_CMD_RESPONSE];
  struct PopCache cache[POP_CACHE_LEN];
#endif
}

void dot_mailbox(FILE *fp, struct Mailbox *m, struct ListHead *links)
{
  char buf[64] = { 0 };

  dot_object_header(fp, m, "Mailbox", "#80ff80");
  dot_mailbox_type(fp, "type", m->magic);

  if ((m->magic == MUTT_IMAP) || (m->magic == MUTT_POP))
  {
    dot_path_imap(buf, sizeof(buf), mutt_b2s(m->pathbuf));
    dot_type_string(fp, "pathbuf", buf);
    dot_path_imap(buf, sizeof(buf), m->realpath);
    dot_type_string(fp, "realpath", buf);
  }
  else
  {
    dot_path_fs(buf, sizeof(buf), mutt_b2s(m->pathbuf));
    dot_type_string(fp, "pathbuf", buf);
    dot_path_fs(buf, sizeof(buf), m->realpath);
    dot_type_string(fp, "realpath", buf);
  }

  // dot_ptr(fp, "mdata", m->mdata, "#e0e060");
  dot_ptr(fp, "account", m->account, "#80ffff");

  if (m->desc)
    dot_type_string(fp, "desc", m->desc);
  dot_object_footer(fp);

  // dot_add_link(links, m, m->mdata, false);

  if (m->mdata)
  {
    if (m->magic == MUTT_MAILDIR)
      dot_mailbox_maildir(fp, m->mdata, links);
    else if (m->magic == MUTT_IMAP)
      dot_mailbox_imap(fp, m->mdata, links);
    else if (m->magic == MUTT_POP)
      dot_mailbox_pop(fp, m->mdata, links);
    else if (m->magic == MUTT_MBOX)
      dot_mailbox_mbox(fp, m->mdata, links);
    else if (m->magic == MUTT_NNTP)
      dot_mailbox_nntp(fp, m->mdata, links);
    else if (m->magic == MUTT_NOTMUCH)
      dot_mailbox_notmuch(fp, m->mdata, links);

    dot_add_link(links, m, m->mdata, "Mailbox->mdata", false);
  }

  if (m->compress_info)
  {
    dot_comp(fp, m->compress_info, links);
    dot_add_link(links, m, m->compress_info, "Mailbox->compress_info", false);
  }

#if 0
  off_t size
  bool has_new
  int msg_count
  int msg_unread
  int msg_flagged
  struct Email **hdrs
  int hdrmax
  int *v2r
  int vcount
  bool notified
  bool newly_created
  struct timespec mtime
  struct timespec last_visited
  struct timespec stats_last_checked
  void *mdata
  void (*free_data)(void **)
  const struct MxOps *mx_ops
  bool changed : 1
  bool readonly : 1
  bool quiet : 1
  bool closing : 1
  unsigned char rights[(RIGHTSMAX + 7) / 8]
  struct Hash *id_hash
  struct Hash *subj_hash
  struct Hash *label_hash
  int flags
#endif
}

void dot_mailbox_node(FILE *fp, struct MailboxNode *mn, struct ListHead *links)
{
  dot_node(fp, mn, "MN", "#80ff80");

  dot_mailbox(fp, mn->mailbox, links);

  dot_add_link(links, mn, mn->mailbox, "MailboxNode->mailbox", false);

  struct Buffer buf;
  mutt_buffer_init(&buf);

  char name[256] = { 0 };
  mutt_buffer_addstr(&buf, "{ rank=same ");

  dot_ptr_name(name, sizeof(name), mn);
  mutt_buffer_add_printf(&buf, "%s ", name);

  dot_ptr_name(name, sizeof(name), mn->mailbox);
  mutt_buffer_add_printf(&buf, "%s ", name);

  if (mn->mailbox->mdata)
  {
    dot_ptr_name(name, sizeof(name), mn->mailbox->mdata);
    mutt_buffer_add_printf(&buf, "%s ", name);
  }

  mutt_buffer_addstr(&buf, "}");

  mutt_list_insert_tail(links, buf.data);
  buf.data = NULL;
}

void dot_mailbox_list(FILE *fp, struct MailboxList *ml, struct ListHead *links, bool abbr)
{
  struct MailboxNode *prev = NULL;
  struct MailboxNode *np = NULL;
  STAILQ_FOREACH(np, ml, entries)
  {
    if (abbr)
      dot_node_link(fp, np, "MN", np->mailbox, "#80ff80");
    else
      dot_mailbox_node(fp, np, links);
    if (prev)
      dot_add_link(links, prev, np, "MailboxNode->next", false);
    prev = np;
  }
}

void dot_connection(FILE *fp, struct Connection *c, struct ListHead *links)
{
  dot_object_header(fp, c, "Connection", "#ff8080");
  dot_type_string(fp, "user", c->account.user);
  dot_type_string(fp, "host", c->account.host);
  dot_type_number(fp, "port", c->account.port);
  // dot_ptr(fp, "data", c->data, "#60c0c0");
  dot_type_number(fp, "fd", c->fd);
  dot_object_footer(fp);
}

void dot_account_imap(FILE *fp, struct ImapAccountData *adata, struct ListHead *links)
{
  dot_object_header(fp, adata, "ImapAccountData", "#60c0c0");
  // dot_type_string(fp, "mbox_name", adata->mbox_name);
  // dot_type_string(fp, "login", adata->conn_account.login);
  dot_type_string(fp, "user", adata->conn_account.user);
  dot_type_string(fp, "pass", adata->conn_account.pass[0] ? "***" : "");
  dot_type_number(fp, "port", adata->conn_account.port);
  // dot_ptr(fp, "conn", adata->conn, "#ff8080");
  dot_ptr(fp, "mailbox", adata->mailbox, "#80ff80");
  dot_object_footer(fp);

  if (adata->conn)
  {
    dot_connection(fp, adata->conn, links);
    dot_add_link(links, adata, adata->conn, "ImapAccountData->conn", false);
  }
}

void dot_account_mbox(FILE *fp, struct MboxAccountData *adata, struct ListHead *links)
{
  char buf[64] = { 0 };

  dot_object_header(fp, adata, "MboxAccountData", "#60c0c0");
  dot_ptr(fp, "fp", adata->fp, NULL);

  dot_type_date(buf, sizeof(buf), adata->atime.tv_sec);
  dot_type_string(fp, "atime", buf);
  dot_type_bool(fp, "locked", adata->locked);
  dot_type_bool(fp, "append", adata->append);

  dot_object_footer(fp);
}

void dot_account_nntp(FILE *fp, struct NntpAccountData *adata, struct ListHead *links)
{
  dot_object_header(fp, adata, "NntpAccountData", "#60c0c0");
  dot_type_number(fp, "groups_num", adata->groups_num);

  dot_type_bool(fp, "hasCAPABILITIES", adata->hasCAPABILITIES);
  dot_type_bool(fp, "hasSTARTTLS", adata->hasSTARTTLS);
  dot_type_bool(fp, "hasDATE", adata->hasDATE);
  dot_type_bool(fp, "hasLIST_NEWSGROUPS", adata->hasLIST_NEWSGROUPS);
  dot_type_bool(fp, "hasXGTITLE", adata->hasXGTITLE);
  dot_type_bool(fp, "hasLISTGROUP", adata->hasLISTGROUP);
  dot_type_bool(fp, "hasLISTGROUPrange", adata->hasLISTGROUPrange);
  dot_type_bool(fp, "hasOVER", adata->hasOVER);
  dot_type_bool(fp, "hasXOVER", adata->hasXOVER);
  dot_type_bool(fp, "cacheable", adata->cacheable);
  dot_type_bool(fp, "newsrc_modified", adata->newsrc_modified);

  dot_type_string(fp, "authenticators", adata->authenticators);
  dot_type_string(fp, "overview_fmt", adata->overview_fmt);
  dot_type_string(fp, "newsrc_file", adata->newsrc_file);
  dot_type_file(fp, "newsrc_fp", adata->fp_newsrc);

  dot_type_number(fp, "groups_num", adata->groups_num);
  dot_type_number(fp, "groups_max", adata->groups_max);

  char buf[128];
  dot_type_date(buf, sizeof(buf), adata->mtime);
  dot_type_string(fp, "mtime", buf);
  dot_type_date(buf, sizeof(buf), adata->newgroups_time);
  dot_type_string(fp, "newgroups_time", buf);
  dot_type_date(buf, sizeof(buf), adata->check_time);
  dot_type_string(fp, "check_time", buf);

  dot_object_footer(fp);

  if (adata->conn)
  {
    dot_connection(fp, adata->conn, links);
    dot_add_link(links, adata, adata->conn, "NntpAccountData->conn", false);
  }

#if 0
  unsigned int use_tls    : 3;
  unsigned int status     : 3;
  off_t size;
  void **groups_list;
  struct Hash *groups_hash;
#endif
}

void dot_account_notmuch(FILE *fp, struct NmAccountData *adata, struct ListHead *links)
{
  dot_object_header(fp, adata, "NmAccountData", "#60c0c0");
  dot_ptr(fp, "db", adata->db, NULL);
  dot_object_footer(fp);

#if 0
  notmuch_database_t *db;
  struct Url db_url;
  char *db_url_holder;
#endif
}

void dot_account_pop(FILE *fp, struct PopAccountData *adata, struct ListHead *links)
{
  char buf[64] = { 0 };

  dot_object_header(fp, adata, "PopAccountData", "#60c0c0");

  dot_type_date(buf, sizeof(buf), adata->check_time);
  dot_type_string(fp, "check_time", buf);

  dot_type_string(fp, "login", adata->conn_account.login);
  dot_type_string(fp, "user", adata->conn_account.user);
  dot_type_string(fp, "pass", adata->conn_account.pass[0] ? "***" : "");
  dot_type_number(fp, "port", adata->conn_account.port);
  // dot_ptr(fp, "conn", adata->conn, "#ff8080");
  dot_object_footer(fp);

  if (adata->conn)
  {
    dot_connection(fp, adata->conn, links);
    dot_add_link(links, adata, adata->conn, "PopAccountData->conn", false);
  }

#if 0
  unsigned int status : 2;
  bool capabilities : 1;
  unsigned int use_stls : 2;
  bool cmd_capa : 1;         /**< optional command CAPA */
  bool cmd_stls : 1;         /**< optional command STLS */
  unsigned int cmd_user : 2; /**< optional command USER */
  unsigned int cmd_uidl : 2; /**< optional command UIDL */
  unsigned int cmd_top : 2;  /**< optional command TOP */
  bool resp_codes : 1;       /**< server supports extended response codes */
  bool expire : 1;           /**< expire is greater than 0 */
  bool clear_cache : 1;
  size_t size;
  time_t check_time;
  time_t login_delay; /**< minimal login delay  capability */
  char *auth_list;    /**< list of auth mechanisms */
  char *timestamp;
  struct BodyCache *bcache; /**< body cache */
  char err_msg[POP_CMD_RESPONSE];
  struct PopCache cache[POP_CACHE_LEN];
#endif
}

void dot_account(FILE *fp, struct Account *a, struct ListHead *links)
{
  dot_object_header(fp, a, "Account", "#80ffff");
  dot_mailbox_type(fp, "magic", a->magic);
  // dot_ptr(fp, "adata", a->adata, "#60c0c0");
  dot_object_footer(fp);

  if (a->adata)
  {
    if (a->magic == MUTT_IMAP)
      dot_account_imap(fp, a->adata, links);
    else if (a->magic == MUTT_POP)
      dot_account_pop(fp, a->adata, links);
    else if (a->magic == MUTT_MBOX)
      dot_account_mbox(fp, a->adata, links);
    else if (a->magic == MUTT_NNTP)
      dot_account_nntp(fp, a->adata, links);
    else if (a->magic == MUTT_NOTMUCH)
      dot_account_notmuch(fp, a->adata, links);

    dot_add_link(links, a, a->adata, "Account->adata", false);
  }

  struct MailboxNode *first = STAILQ_FIRST(&a->mailboxes);
  dot_add_link(links, a, first, "Account->mailboxes", false);
  dot_mailbox_list(fp, &a->mailboxes, links, false);

#if 0
  struct Account *account
#endif
}

void dot_allaccounts(FILE *fp, struct AccountList *al, struct ListHead *links)
{
  struct Account *prev = NULL;
  struct Account *np = NULL;
  TAILQ_FOREACH(np, al, entries)
  {
    dot_account(fp, np, links);
    if (prev)
      dot_add_link(links, prev, np, "Account->next", false);

    prev = np;
  }
}

void dot_context(FILE *fp, struct Context *ctx, struct ListHead *links)
{
#if 0
  dot_node(fp, Context, "Context", "#ff80ff");
#else
  dot_object_header(fp, ctx, "Context", "#ff80ff");
  dot_ptr(fp, "mailbox", ctx->mailbox, "#80ff80");
  // dot_type_number(fp, "vsize", ctx->vsize);
  // dot_type_string(fp, "pattern", ctx->pattern);
  // dot_type_number(fp, "tagged", ctx->tagged);
  // dot_type_number(fp, "new", ctx->new);
  // dot_type_number(fp, "deleted", ctx->deleted);
  // dot_type_bool(fp, "dontwrite", ctx->dontwrite);
  // dot_type_bool(fp, "append", ctx->append);
  // dot_type_bool(fp, "collapsed", ctx->collapsed);
  // dot_type_bool(fp, "peekonly", ctx->peekonly);
  dot_object_footer(fp);
#endif

#if 0
  if (ctx)
  {
#if 0
    dot_mailbox(fp, ctx->mailbox, links);
    dot_add_link(links, ctx, ctx->mailbox, "Context->mailbox", false);
#else
    void *dummy = (void *) 0x43214321;
    dot_add_link(links, ctx, dummy, "Context->mailbox", false);
    dot_node_link(fp, dummy, "Mailbox", ctx->mailbox, "#80ff80");
#endif
  }
#endif

#if 0
  struct Pattern *limit_pattern
  struct Email *last_tag
  struct MuttThread *tree
  struct Hash *thread_hash
  int msgnotreadyet
  struct Menu *menu
  struct Mailbox *mailbox
#endif
}

void dot_dump(const char *title)
{
  char name[256] = { 0 };
  struct ListHead links = STAILQ_HEAD_INITIALIZER(links);

  time_t now = time(NULL);
  if (title)
  {
    char date[128];
    strftime(date, sizeof(date), "%R", localtime(&now));
    snprintf(name, sizeof(name), "%s-%s.gv", date, title);
  }
  else
  {
    strftime(name, sizeof(name), "%R.gv", localtime(&now));
  }

  umask(022);
  FILE *fp = fopen(name, "w");
  if (!fp)
    return;

  dot_graph_header(fp);

#if 0
  dot_node(fp, &AllMailboxes, "AllMailboxes", "#80ff80");
  dot_add_link(&links, &AllMailboxes, STAILQ_FIRST(&AllMailboxes), "Mailbox->next", false);
  dot_mailbox_list(fp, &AllMailboxes, &links, true);
#endif

#if 1
  dot_node(fp, &AllAccounts, "AllAccounts", "#80ffff");
  dot_add_link(&links, &AllAccounts, TAILQ_FIRST(&AllAccounts), "AllAccounts->first", false);
  dot_allaccounts(fp, &AllAccounts, &links);
#endif

#if 0
  void *connections = (void *) 0x12341234;
  dot_node(fp, connections, "Connections", "#ff8080");
  struct ConnectionList *cl = mutt_socket_head();
  dot_add_link(&links, connections, TAILQ_FIRST(cl), "Connections->first", false);
#endif

#if 1
  if (Context)
    dot_context(fp, Context, &links);

  /* Globals */
  fprintf(fp, "\t{ rank=same ");
  if (Context)
  {
    dot_ptr_name(name, sizeof(name), Context);
    fprintf(fp, "%s ", name);
  }
  dot_ptr_name(name, sizeof(name), &AllAccounts);
  fprintf(fp, "%s ", name);
  // dot_ptr_name(name, sizeof(name), connections);
  // fprintf(fp, "%s ", name);
  fprintf(fp, "}\n");
#endif

  fprintf(fp, "\t{ rank=same ");
  struct Account *np = NULL;
  TAILQ_FOREACH(np, &AllAccounts, entries)
  {
    dot_ptr_name(name, sizeof(name), np);
    fprintf(fp, "%s ", name);
  }
  fprintf(fp, "}\n");

  dot_graph_footer(fp, &links);
  fclose(fp);
  mutt_list_free(&links);
}
