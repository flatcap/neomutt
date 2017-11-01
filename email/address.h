/**
 * @file
 * Representation of an email address
 *
 * @authors
 * Copyright (C) 2017 Richard Russon <rich@flatcap.org>
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

#ifndef _EMAIL_ADDRESS_H
#define _EMAIL_ADDRESS_H

#include <stdbool.h>
#include <stdio.h>

extern const char RFC822Specials[];
extern int RFC822Error;
extern const char *const RFC822Errors[];

#define rfc822_error(x) RFC822Errors[x]

/**
 * struct Address - An email address
 */
struct Address
{
  char *personal; /**< real name of address */
  char *mailbox;  /**< mailbox and host address */
  int group;      /**< group mailbox? */
  struct Address *next;
  bool is_intl : 1;
  bool intl_checked : 1;
};

/**
 * enum AddressError - possible values for RFC822Error
 */
enum AddressError
{
  ERR_MEMORY = 1,
  ERR_MISMATCH_PAREN,
  ERR_MISMATCH_QUOTE,
  ERR_BAD_ROUTE,
  ERR_BAD_ROUTE_ADDR,
  ERR_BAD_ADDR_SPEC
};

bool            addrcmp(struct Address *a, struct Address *b);
bool            addrsrc(struct Address *a, struct Address *lst);
int             has_recips(struct Address *a);
struct Address *mutt_parse_adrlist(struct Address *p, const char *s);
struct Address *rfc822_append(struct Address **a, struct Address *b, bool prune);
void            rfc822_cat(char *buf, size_t buflen, const char *value, const char *specials);
struct Address *rfc822_cpy_adrlist(struct Address *addr, bool prune);
struct Address *rfc822_cpy_adr(struct Address *addr);
void            rfc822_free_address(struct Address **p);
struct Address *rfc822_new_address(void);
struct Address *rfc822_parse_adrlist(struct Address *top, const char *s);
void            rfc822_qualify(struct Address *addr, const char *host);
int             rfc822_remove_from_adrlist(struct Address **a, const char *mailbox);
bool            rfc822_valid_msgid(const char *msgid);
bool            strict_addrcmp(const struct Address *a, const struct Address *b);

#endif /* _EMAIL_ADDRESS_H */
