#include "config.h"
#include <stdbool.h>
#include "mutt/mutt.h"
#include "email/lib.h"
#include "core/lib.h"

void emaillist_email_add(struct EmailList *el, struct Email *e)
{
  if (!el || !e)
    return;

  struct EmailNode *en = mutt_mem_calloc(1, sizeof(*en));
  en->email = e;
  STAILQ_INSERT_TAIL(el, en, entries);
}

void emaillist_email_del(struct EmailList *el, struct Email *e)
{
  if (!el || !e)
    return;
}

void emaillist_emaillist_add(struct EmailList *el, struct EmailList *el_add)
{
  if (!el || !el_add)
    return;
}

void emaillist_emaillist_del(struct EmailList *el, struct EmailList *el_add)
{
  if (!el || !el_add)
    return;
}

bool mailbox_email_add(struct Mailbox *m, struct Email *e)
{
  if (!m || !e)
    return false;

  // emaillist_email_add(M->EL, E)
  notify_set_parent(e->notify, m->notify);
  // notify_send(M->notify, EventE, NEW) // EventE contains EL
  // struct EventEmail ev_e = { e };
  // notify_send(n->notify, NT_ACCOUNT, NT_ACCOUNT_ADD, IP & ev_a);
  return true;
}

bool mailbox_email_del(struct Mailbox *m, struct Email *e)
{
  if (!m || !e)
    return false;

  // notify_send(E->notify, EventE, DEL) // EventE contains EL(E)
  // emaillist_email_del(M->EL, E)
  //   REMOVE(EL, E)
  return true;
}

bool mailbox_emaillist_add(struct Mailbox *m, struct EmailList *el)
{
  if (!m || !el)
    return false;

  // emaillist_emaillist_add(M->EL, EL)
  //   FOREACH(EL2)
  //     INSERT(EL1, E)
  // FOREACH(EL)
  //   set_parent(E->notify, M->notify)
  // notify_send(M->notify, EventE, NEW) // EventE contains EL
  return true;
}

bool mailbox_emaillist_del(struct Mailbox *m, struct EmailList *el)
{
  if (!m || !el)
    return false;

  // notify_send(M->notify, EventE, DEL) // EventE contains EL, notification propagated
  // FOREACH(EL)
  //   notify_send(E->notify, EventE, DEL) // EventE contains E, notification NOT propagated
  // emaillist_emaillist_del(M->EL, EL)
  //   FOREACH(EL2)
  //     REMOVE(EL1, E)
  return true;
}
