#include <stdio.h>
#include <stdlib.h>
#include "tailq.h"

struct Address
{
  int payload;
  TAILQ_ENTRY(Address) entries;
};
TAILQ_HEAD(AddressList, Address);

struct Address *address_new(void)
{
  static int payload = 1;

  struct Address *a = calloc(1, sizeof(*a));
  a->payload = payload++;
  return a;
}

void address_free(struct Address **ptr)
{
  if (!ptr || !*ptr)
    return;

  struct Address *a = *ptr;
  free(a);
  *ptr = NULL;
}

void addrlist_remove_first(struct AddressList *al)
{
  if (!al)
    return;

  struct Address *a = TAILQ_FIRST(al);
  TAILQ_REMOVE(al, a, entries);
  address_free(&a);
}

int main()
{
  struct AddressList al = TAILQ_HEAD_INITIALIZER(al);
  struct Address *a = NULL;

  a = address_new();
  TAILQ_INSERT_TAIL(&al, a, entries);
  a = address_new();
  TAILQ_INSERT_TAIL(&al, a, entries);
  a = address_new();
  TAILQ_INSERT_TAIL(&al, a, entries);

  TAILQ_FOREACH(a, &al, entries)
  {
    printf("%d, ", a->payload);
  }
  printf("\n");

  addrlist_remove_first(&al);

  TAILQ_FOREACH(a, &al, entries)
  {
    printf("%d, ", a->payload);
  }
  printf("\n");

  return 0;
}
