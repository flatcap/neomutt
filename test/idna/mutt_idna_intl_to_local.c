/**
 * @file
 * Test code for mutt_idna_intl_to_local()
 *
 * @authors
 * Copyright (C) 2019 Richard Russon <rich@flatcap.org>
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

#define TEST_NO_MAIN
#include "acutest.h"
#include "config.h"
#include "mutt/mutt.h"
#include "address/lib.h"

void test_mutt_idna_intl_to_local(void)
{
  // char * mutt_idna_intl_to_local(const char *user, const char *domain, int flags);

#ifdef HAVE_LIBIDN
  // {
  //   TEST_CHECK(!mutt_idna_intl_to_local(NULL, "banana", 0));
  // }

  // {
  //   TEST_CHECK(!mutt_idna_intl_to_local("apple", NULL, 0));
  // }

  // {
  //   C_Charset = "utf-8";
  //   char *email = NULL;
  //   TEST_CHECK((email = mutt_idna_intl_to_local("john", "example.com", MI_MAY_BE_IRREVERSIBLE)) != NULL);
  //   TEST_MSG("email: %s\n", email);
  //   FREE(&email);
  // }

  // {
  //   C_Charset = "utf-8";
  //   char *email = NULL;
  //   TEST_CHECK((email = mutt_idna_intl_to_local("josé", "example.com", MI_MAY_BE_IRREVERSIBLE)) != NULL);
  //   TEST_MSG("email: %s\n", email);
  //   FREE(&email);
  // }

  {
    C_Charset = "us-ascii";
    char *email = NULL;
    TEST_CHECK((email = mutt_idna_intl_to_local("苹果", "example.com", MI_MAY_BE_IRREVERSIBLE)) != NULL);
    TEST_MSG("email: %s\n", email);
    FREE(&email);
  }

  // {
  //   C_Charset = "utf-8";
  //   C_IdnDecode = true;
  //   char *email = NULL;
  //   TEST_CHECK((email = mutt_idna_intl_to_local("jim", "💩.la", MI_MAY_BE_IRREVERSIBLE)) != NULL);
  //   TEST_MSG("email: %s\n", email);
  //   FREE(&email);
  // }

  {
    C_Charset = "utf-8";
    C_IdnDecode = true;
    char *email = NULL;
    TEST_CHECK((email = mutt_idna_intl_to_local("Bücher", "xn--!s8h.la", MI_MAY_BE_IRREVERSIBLE)) != NULL);
    TEST_MSG("email: %s\n", email);
    FREE(&email);
  }
#endif
}
