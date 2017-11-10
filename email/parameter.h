/**
 * @file
 * Store attributes associated with a MIME part
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

#ifndef _EMAIL_PARAMETER_H
#define _EMAIL_PARAMETER_H

#include "mutt/mutt.h"

/**
 * struct Parameter - Attribute associated with a MIME part
 */
struct Parameter
{
  char *attribute;
  char *value;
  struct Parameter *next;
};

void              mutt_delete_parameter(const char *attribute, struct Parameter **p);
void              mutt_free_parameter(struct Parameter **p);
char *            mutt_get_parameter(const char *s, struct Parameter *p);
struct Parameter *mutt_new_parameter(void);
void              mutt_set_parameter(const char *attribute, const char *value, struct Parameter **p);
int               strict_cmp_parameters(const struct Parameter *p1, const struct Parameter *p2);

#endif /* _EMAIL_PARAMETER_H */
