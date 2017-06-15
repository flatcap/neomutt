#include "config.h"
#include <limits.h>
#include <stdlib.h>
#include "config_set.h"
#include "mutt/mutt.h"

bool config_set_init(struct ConfigSet *config)
{
  config->hash = mutt_hash_create(10, 0);
  return true;
}

struct ConfigSet *config_set_new(void)
{
  struct ConfigSet *cs = mutt_mem_calloc(1, sizeof(*cs));
  config_set_init(cs);
  return cs;
}

void config_set_free(struct ConfigSet *config)
{
  mutt_hash_destroy(&config->hash, NULL);
}

void config_set_int(struct ConfigSet *config, const char *name, intptr_t value)
{
  struct HashElem *elem = mutt_hash_find_elem(config->hash, name);
  if (elem)
  {
    elem->data = (void *) value;
    return;
  }

  mutt_hash_insert(config->hash, name, (void *) value);
}

void config_set_string(struct ConfigSet *config, const char *name, const char *value)
{
  struct HashElem *elem = mutt_hash_find_elem(config->hash, name);
  if (elem)
  {
    elem->data = (void *) value;
    return;
  }

  mutt_hash_insert(config->hash, name, (void *) value);
}

int config_get_int(struct ConfigSet *config, const char *name)
{
  struct HashElem *elem = mutt_hash_find_elem(config->hash, name);
  if (elem)
    return (intptr_t) elem->data;

  return INT_MIN;
}

char *config_get_string(struct ConfigSet *config, const char *name)
{
  struct HashElem *elem = mutt_hash_find_elem(config->hash, name);
  if (elem)
    return elem->data;

  return NULL;
}
