#ifndef _MUTT_MAIL_ACCOUNT_H
#define _MUTT_MAIL_ACCOUNT_H

struct MailAccount
{
  const char *name;
  struct ConfigSet *config;
  struct MailAccount *next;
};

struct MailAccount *mail_account_create(const char *name);
struct MailAccount *mail_account_find  (const char *name);

extern struct MailAccount *accounts;
extern struct MailAccount *current_account;

#endif /* _MUTT_MAIL_ACCOUNT_H */
