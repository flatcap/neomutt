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

#ifndef MUTT_ENTER_DEBUG_H
#define MUTT_ENTER_DEBUG_H

#include "config.h"
#include "mutt/lib.h"

struct EnterState;

#ifdef USE_DEBUG_ENTER

int  enter_debug      (enum LogLevel level, const char *format, ...);
void enter_dump_buffer(const struct EnterState *es);
void enter_dump_string(const struct EnterState *es, const char *label);

#else

static inline int  enter_debug      (enum LogLevel level, const char *format, ...) { return 0; }
static inline void enter_dump_buffer(const struct EnterState *es) { }
static inline void enter_dump_string(const struct EnterState *es, const char *label) { }

#endif

#endif /* MUTT_ENTER_DEBUG_H */
