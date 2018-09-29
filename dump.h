#ifndef MUTT_DUMP_H
#define MUTT_DUMP_H

#include <stdio.h>

struct Mailbox;

const char *account_flags(int flags);
const char *account_type(int type);
const char *mbox_account_type(int type);
void dump_accounts(FILE *fp);
void dump_nntp(FILE *fp);
// void dump_connections(FILE *fp);
const char *imap_state_str(unsigned char state);
const char *imap_status_str(unsigned char status);
// void imap_dump(struct Mailbox *m, FILE *fp);

#endif /* MUTT_DUMP_H */
