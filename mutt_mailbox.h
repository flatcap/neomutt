#ifndef MUTT_MUTT_MAILBOX_H
#define MUTT_MUTT_MAILBOX_H

#include <stddef.h>
#include <stdbool.h>
#include "mutt_commands.h"

struct Buffer;
struct ConfigSubset;
struct Mailbox;
struct stat;

/* These Config Variables are only used in mutt_mailbox.c */
extern short C_MailCheck;
extern bool  C_MailCheckStats;
extern short C_MailCheckStatsInterval;

/* force flags passed to mutt_mailbox_check() */
#define MUTT_MAILBOX_CHECK_FORCE       (1 << 0)
#define MUTT_MAILBOX_CHECK_FORCE_STATS (1 << 1)

int  mutt_mailbox_check       (struct Mailbox *m_cur, int force);
void mutt_mailbox_cleanup     (const char *path, struct stat *st);
bool mutt_mailbox_list        (void);
void mutt_mailbox_next        (struct Mailbox *m_cur, char *s, size_t slen);
void mutt_mailbox_next_buffer (struct Mailbox *m_cur, struct Buffer *s);
bool mutt_mailbox_notify      (struct Mailbox *m_cur);
void mutt_mailbox_set_config  (struct Mailbox *m, struct ConfigSubset *parent);
void mutt_mailbox_set_notified(struct Mailbox *m);

enum CommandResult mutt_parse_mailbox  (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult mutt_parse_unmailbox(struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);

#endif /* MUTT_MUTT_MAILBOX_H */
