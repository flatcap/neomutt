#include "config.h"
#include "mutt/mutt.h"
#include "config/lib.h"
#include "email/lib.h"
#include "core/lib.h"
#include "hook.h"

const char *MagicValues[] = {
  NULL, "mbox", "MMDF", "MH", "Maildir", "nntp", "imap", "notmuch", "pop", "compressed",
};

const char *dump_config2(int subtype, intptr_t event)
{
  static char buf[256];
  const char *desc = "unknown C";

  switch (subtype)
  {
    case NT_CONFIG_SET:
      desc = "SET";
      break;
    case NT_CONFIG_RESET:
      desc = "RESET";
      break;
    case NT_CONFIG_INITIAL_SET:
      desc = "INITIAL_SET";
      break;
  }

  struct EventConfig *ec = (struct EventConfig *) event;
  snprintf(buf, sizeof(buf), "Config %s: %s", desc, ec->name);
  return buf;
}

const char *dump_account(int subtype, intptr_t event)
{
  static char buf[256] = { 0 };
  const char *desc = "unknown A";

  switch (subtype)
  {
    case NT_ACCOUNT_ADD:
      desc = "ADD";
      break;
    case NT_ACCOUNT_REMOVE:
      desc = "REMOVE";
      break;
  }

  struct EventAccount *ev_a = (struct EventAccount *) event;
  if (ev_a && ev_a->account)
  {
    int magic = ev_a->account->magic;
    if ((magic > 0) && (magic < mutt_array_size(MagicValues)))
      snprintf(buf, sizeof(buf), "Account %s (%s)", desc, MagicValues[magic]);
    else
      snprintf(buf, sizeof(buf), "Account %s (%d)", desc, magic);
  }
  return buf;
}

void process_emails(int subtype, intptr_t event)
{
  struct EventEmail *ev_m = (struct EventEmail *) event;

  int limit = MIN(5, ev_m->num_emails);
  for (int i = 0; i < limit; i++)
  {
    struct Email *e = ev_m->emails[i];

    mutt_debug(LL_NOTIFY, "    Subject: %s\n", e->env->subject);
  }

  if (ev_m->num_emails > limit)
  {
    mutt_debug(LL_NOTIFY, "    ... and %d more\n", ev_m->num_emails - limit);
  }
}

const char *dump_email(int subtype, intptr_t event)
{
  static char buf[256];
  const char *desc = "unknown E";

  switch (subtype)
  {
    case NT_EMAIL_ADD:
      desc = "ADD";
      break;
    case NT_EMAIL_NEW:
      desc = "NEW";
      break;
    case NT_EMAIL_REMOVE:
      desc = "REMOVE";
      break;
  }

  struct EventEmail *ev_m = (struct EventEmail *) event;
  snprintf(buf, sizeof(buf), "Email %s - %d", desc, ev_m->num_emails);
  return buf;
}

const char *dump_global(int subtype, intptr_t event)
{
  const char *desc = "unknown G";

  switch (subtype)
  {
    case NT_GLOBAL_SHUTDOWN:
      desc = "Global SHUTDOWN";
      break;
    case NT_GLOBAL_STARTUP:
      desc = "Global STARTUP";
      break;
  }

  return desc;
}

const char *dump_mailbox(int subtype, intptr_t event)
{
  static char buf[256] = { 0 };
  const char *desc = "unknown M";

  switch (subtype)
  {
    case NT_MAILBOX_ADD:
      desc = "ADD";
      break;
    case NT_MAILBOX_REMOVE:
      desc = "REMOVE";
      break;
  }

  struct EventMailbox *ev_m = (struct EventMailbox *) event;
  if (ev_m && ev_m->mailbox)
    snprintf(buf, sizeof(buf), "Mailbox %s - %s", desc, mutt_b2s(ev_m->mailbox->pathbuf));
  return buf;
}

const char *dump_neomutt(int subtype, intptr_t event)
{
  const char *desc = "unknown N";

  // switch (event)
  // {
  // }

  return desc;
}

int notify_dump(struct NotifyCallback *nc)
{
  if (!nc)
    return -1;

  const char *obj = "unknown";
  const char *event = "unknown";

  switch (nc->obj_type)
  {
    case NT_ACCOUNT:
      obj = "ACCOUNT";
      break;
    case NT_CONFIG:
      obj = "CONFIG";
      break;
    case NT_EMAIL:
      obj = "EMAIL";
      break;
    case NT_GLOBAL:
      obj = "GLOBAL";
      break;
    case NT_MAILBOX:
      obj = "MAILBOX";
      break;
    case NT_NEOMUTT:
      obj = "NEOMUTT";
      break;
    case NT_WINDOW:
      obj = "WINDOW";
      break;
  }

  switch (nc->event_type)
  {
    case NT_ACCOUNT:
      event = dump_account(nc->event_subtype, nc->event);
      break;
    case NT_CONFIG:
      event = dump_config2(nc->event_subtype, nc->event);
      break;
    case NT_EMAIL:
      event = dump_email(nc->event_subtype, nc->event);
      break;
    case NT_GLOBAL:
      event = dump_global(nc->event_subtype, nc->event);
      break;
    case NT_MAILBOX:
      event = dump_mailbox(nc->event_subtype, nc->event);
      break;
    case NT_NEOMUTT:
      event = dump_neomutt(nc->event_subtype, nc->event);
      break;
  }

  if (nc->event_type != NT_CONFIG)
    mutt_debug(LL_NOTIFY, "%s: %s\n", obj, event);

  if (nc->event_type == NT_EMAIL)
  {
    process_emails(nc->event_subtype, nc->event);
  }

  return 0;
}
