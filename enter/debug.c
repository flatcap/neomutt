/**
 * @file
 * Enter Debugging
 *
 * @authors
 * Copyright (C) 2022 Richard Russon <rich@flatcap.org>
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @page enter_debug Enter Debugging
 *
 * Lots of debugging of the enter code, conditional on './configure --debug-enter'
 */

#include "config.h"
#include <stdarg.h>
#include <stdio.h>
#include "mutt/lib.h"
#include "state.h"

/**
 * enter_debug - Write to the log file
 * @param level  Logging level, e.g. #LL_DEBUG1
 * @param format Printf format string
 * @param ...    Args for printf
 * @retval num Number of characters printed
 */
int enter_debug(enum LogLevel level, const char *format, ...)
{
  char buf[1024];

  va_list ap;
  va_start(ap, format);
  int len = vsnprintf(buf, sizeof(buf), format, ap);
  va_end(ap);

  mutt_debug(level, buf);

  return len;
}

/**
 * enter_dump_buffer - XXX
 * @param es  State of the Enter buffer
 */
void enter_dump_buffer(const struct EnterState *es)
{
  if (!es)
    return;

  enter_debug(LL_DEBUG1, "buf: %p, len: (%ld c, %ldb), cur: %ld, last: %ld\n", es->wbuf,
              es->wbuflen, es->wbuflen * sizeof(wchar_t), es->curpos, es->lastchar);

  const char *format = "%02x";
  for (size_t i = 0; i <= es->lastchar; i++)
  {
    if (es->wbuf[i] > 0xff)
    {
      format = "%04x";
      break;
    }
  }

  struct Buffer *hex = buf_pool_get();
  buf_addstr(hex, "> ");
  if (es->curpos == 0)
    buf_addstr(hex, "|");
  else
    buf_addstr(hex, " ");
  for (size_t i = 0; i < es->lastchar; i++)
  {
    buf_add_printf(hex, format, es->wbuf[i]);
    if ((i + 1) == es->curpos)
      buf_addstr(hex, "|");
    else
      buf_addstr(hex, " ");
  }
  buf_addstr(hex, "< ");
  buf_add_printf(hex, format, es->wbuf[es->lastchar]);
  enter_debug(LL_DEBUG1, "%s$\n", buf_string(hex));

  buf_pool_release(&hex);
}

/**
 * enter_dump_string - XXX
 */
void enter_dump_string(const struct EnterState *es, const char *label)
{
  if (!es)
    return;

  struct Buffer *str = buf_pool_get();
  mutt_mb_wcstombs(es->wbuf, es->lastchar, str);
  enter_debug(LL_DEBUG1, "%p: >>%s<<\n", es, buf_string(str));
  buf_pool_release(&str);
}
