#include "config.h"
#include <string.h>
#include <stdlib.h>
#include "mail_account.h"
#include "config_set.h"
#include "mutt/mutt.h"

struct MailAccount *accounts = NULL;
struct MailAccount *current_account = NULL;

struct MailAccount *mail_account_create(const char *name)
{
  struct MailAccount *last = NULL;

  for (last = accounts; last && last->next; last = last->next)
    ;

  struct MailAccount *ma = mutt_mem_calloc(1, sizeof(*ma));

  ma->name = strdup(name);
  ma->config = config_set_new();
  ma->next = NULL;

  if (last)
    last->next = ma;
  else
    accounts = ma;

  return ma;
}

struct MailAccount *mail_account_find(const char *name)
{
  for (struct MailAccount *ma = accounts; ma; ma = ma->next)
  {
    if (strcmp(ma->name, name) == 0)
      return ma;
  }

  return NULL;
}

