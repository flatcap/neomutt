#include "config.h"
#include <stdio.h>
#include "imap/imap_private.h"
#include "conn/conn.h"
#include "account.h"
#include "globals.h"
#include "mutt_account.h"
#include "nntp/nntp.h"

const char *account_flags(int flags)
{
  static char str[64];
  str[0] = '\0';

  if (flags & MUTT_ACCT_PORT)
    mutt_str_strcat(str, sizeof(str), "port ");
  if (flags & MUTT_ACCT_USER)
    mutt_str_strcat(str, sizeof(str), "user ");
  if (flags & MUTT_ACCT_LOGIN)
    mutt_str_strcat(str, sizeof(str), "login ");
  if (flags & MUTT_ACCT_PASS)
    mutt_str_strcat(str, sizeof(str), "pass ");
  if (flags & MUTT_ACCT_SSL)
    mutt_str_strcat(str, sizeof(str), "ssl ");

  if (str[0] == '\0')
    return "NONE";

  return str;
}

const char *account_type(int type)
{
  if (type == MUTT_ACCT_TYPE_IMAP)
    return "IMAP";
  if (type == MUTT_ACCT_TYPE_POP)
    return "POP";
  if (type == MUTT_ACCT_TYPE_SMTP)
    return "SMTP";
  if (type == MUTT_ACCT_TYPE_NNTP)
    return "NNTP";

  return "UNKNOWN";
}

const char *mbox_account_type(int type)
{
  switch (type)
  {
    case MUTT_MBOX:
      return "MBOX";
    case MUTT_MMDF:
      return "MMDF";
    case MUTT_MH:
      return "MH";
    case MUTT_MAILDIR:
      return "MAILDIR";
    case MUTT_NNTP:
      return "NNTP";
    case MUTT_IMAP:
      return "IMAP";
    case MUTT_NOTMUCH:
      return "NOTMUCH";
    case MUTT_POP:
      return "POP";
    case MUTT_COMPRESSED:
      return "COMPRESSED";
    default:
      return "UNKNOWN";
  }
}

void dump_accounts(FILE *fp)
{
  struct Account *np = NULL;
  TAILQ_FOREACH(np, &AllAccounts, entries)
  {
    fprintf(fp, "account: %s, %p\n", mbox_account_type(np->magic), np->adata);
    struct MailboxNode *mp = NULL;
    STAILQ_FOREACH(mp, &np->mailboxes, entries)
    {
      fprintf(fp, "  mailbox: %s\n", mutt_b2s(mp->mailbox->pathbuf));
    }
  }
}

void dump_nntp(FILE *fp)
{
  if (!CurrentNewsSrv)
    return;

  fprintf(fp, "nntp %p\n", (void *) CurrentNewsSrv);
#if 0
  struct ConnectionList *cl = mutt_socket_head();
  struct Connection *cnp = NULL;
  TAILQ_FOREACH(cnp, cl, entries)
  {
    fprintf(fp, "\tconnection: %p\n", (void *) cnp);
    fprintf(fp, "\t\tuser:  %s\n", cnp->account.user);
    fprintf(fp, "\t\tlogin: %s\n", cnp->account.login);
    fprintf(fp, "\t\tpass:  %s\n", cnp->account.pass);
    fprintf(fp, "\t\thost:  %s\n", cnp->account.host);
    fprintf(fp, "\t\tport:  %d\n", cnp->account.port);
    fprintf(fp, "\t\ttype:  %d\n", cnp->account.type);
    fprintf(fp, "\t\tflags: %d\n", cnp->account.flags);
  }
#endif
}

#if 0
void dump_connections(FILE *fp)
{
  struct ConnectionList *cl = mutt_socket_head();
  struct Connection *cnp = NULL;
  TAILQ_FOREACH(cnp, cl, entries)
  {
    fprintf(fp, "connection: %p\n", (void *) cnp);
    fprintf(fp, "\tuser:  %s\n", cnp->account.user);
    fprintf(fp, "\tlogin: %s\n", cnp->account.login);
    fprintf(fp, "\tpass:  %s\n", cnp->account.pass);
    fprintf(fp, "\thost:  %s\n", cnp->account.host);
    fprintf(fp, "\tport:  %d\n", cnp->account.port);
    fprintf(fp, "\ttype:  %s\n", account_type(cnp->account.type));
    fprintf(fp, "\tflags: %s\n", account_flags(cnp->account.flags));
  }
}
#endif

const char *imap_state_str(unsigned char state)
{
  switch (state)
  {
    case IMAP_DISCONNECTED:
      return "disconnected";
    case IMAP_CONNECTED:
      return "connected";
    case IMAP_AUTHENTICATED:
      return "authenticated";
    case IMAP_SELECTED:
      return "selected";
    default:
      return "UNKNOWN";
  }
}

const char *imap_status_str(unsigned char status)
{
  switch (status)
  {
    case IMAP_FATAL:
      return "fatal";
    case IMAP_BYE:
      return "bye";
    default:
      return "UNKNOWN";
  }
}

#if 0
void imap_dump(struct Mailbox *m, FILE *fp)
{
  const char *account_flags(int flags);
  const char *account_type(int type);

  struct ConnectionList *cl = mutt_socket_head();
  struct Connection *cnp = NULL;
  TAILQ_FOREACH(cnp, cl, entries)
  {
    fprintf(fp, "connection: %p\n", (void *) cnp);
    fprintf(fp, "\tuser:  %s\n", cnp->account.user);
    fprintf(fp, "\tlogin: %s\n", cnp->account.login);
    fprintf(fp, "\tpass:  %s\n", cnp->account.pass);
    fprintf(fp, "\thost:  %s\n", cnp->account.host);
    fprintf(fp, "\tport:  %d\n", cnp->account.port);
    fprintf(fp, "\ttype:  %s\n", account_type(cnp->account.type));
    fprintf(fp, "\tflags: %s\n", account_flags(cnp->account.flags));

    struct ImapAccountData *adata = cnp->data;
    fprintf(fp, "\t\tcapstr:    %.80s\n", adata->capstr);
    fprintf(fp, "\t\tstate:     %s\n", imap_state_str(adata->state));
    fprintf(fp, "\t\tstatus:    %s\n", imap_status_str(adata->status));
  }
}
#endif
