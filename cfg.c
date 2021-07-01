#include "config.h"
#include <stdint.h>
#include "mutt/lib.h"
#include "config/lib.h"
#include "core/lib.h"
#include "init.h"
#include "menu/type.h"
#include "mutt_thread.h"

const char *NmUrlProtocol = NULL;
const int NmUrlProtocolLen = 0;
bool OptAttachMsg;
void *HomeDir = NULL;

// clang-format off
static const struct Mapping MboxTypeMap[] = {
  { "mbox",    MUTT_MBOX,    },
  { "MMDF",    MUTT_MMDF,    },
  { "MH",      MUTT_MH,      },
  { "Maildir", MUTT_MAILDIR, },
  { NULL,      0,            },
};
// clang-format on

struct EnumDef MboxTypeDef = {
  "mbox_type",
  4,
  (struct Mapping *) &MboxTypeMap,
};

static const struct Mapping UseThreadsMethods[] = {
  // clang-format off
  { "unset",         UT_UNSET },
  { "flat",          UT_FLAT },
  { "threads",       UT_THREADS },
  { "reverse",       UT_REVERSE },
  // aliases
  { "no",            UT_FLAT },
  { "yes",           UT_THREADS },
  { NULL, 0 },
  // clang-format on
};

struct EnumDef UseThreadsTypeDef = {
  "use_threads_type",
  4,
  (struct Mapping *) &UseThreadsMethods,
};

int sort_validator(const struct ConfigSet *cs, const struct ConfigDef *cdef,
                   intptr_t value, struct Buffer *err)
{
  return CSR_SUCCESS;
}

bool config_init_menu(struct ConfigSet *cs)
{
  return true;
}

void nm_edata_free(void **ptr)
{
}

bool nm_query_window_check_timebase(const char *timebase)
{
  return true;
}

enum MenuType menu_get_current_type(void)
{
  return 0;
}

int level_validator(const struct ConfigSet *cs, const struct ConfigDef *cdef,
                    intptr_t value, struct Buffer *err)
{
  return 0;
}

bool pop_auth_is_valid(const char *authenticator)
{
  return true;
}

bool sasl_auth_validator(const char *authenticator)
{
  return true;
}

bool smtp_auth_is_valid(const char *authenticator)
{
  return true;
}

bool store_is_valid_backend(const char *str)
{
  return true;
}

bool imap_auth_is_valid(const char *authenticator)
{
  return true;
}

const struct ComprOps *compress_get_ops(const char *compr)
{
  return NULL;
}

void mutt_pretty_mailbox(char *buf, size_t buflen)
{
}

int main(int argc, char **argv)
{
  struct ConfigSet *cs = cs_new(500);
  NeoMutt = neomutt_new(cs);
  init_config(cs);

  ConfigDumpFlags cdflags = CS_DUMP_NO_FLAGS;
  // cdflags |= CS_DUMP_SHOW_DOCS;
  dump_config(cs, cdflags, stdout);

  cs_free(&cs);
  return 0;
}
