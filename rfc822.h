/**
 * @file
 * RFC822 Email format routines
 *
 * @authors
 * Copyright (C) 1996-2000 Michael R. Elkins <me@mutt.org>
 * Copyright (C) 2012 Michael R. Elkins <me@mutt.org>
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

#ifndef _MUTT_RFC822_H
#define _MUTT_RFC822_H

#include <stddef.h>
#include <stdbool.h>
#include "email/email.h"
#include "mutt/mutt.h"

int rfc822_write_address(char *buf, size_t buflen, struct Address *addr, int display);
void rfc822_write_address_single(char *buf, size_t buflen, struct Address *addr, int display);

#endif /* _MUTT_RFC822_H */
