/**
 * @file
 * Test code for mutt_addr_remove_xrefs()
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

void test_mutt_addr_remove_xrefs(void)
{
  // struct Address *mutt_addr_remove_xrefs(struct Address *a, struct Address *b);

  {
    struct Address addr = { 0 };
    TEST_CHECK(mutt_addr_remove_xrefs(NULL, &addr) == NULL);
  }

  {
    struct Address addr = { 0 };
    TEST_CHECK(mutt_addr_remove_xrefs(&addr, NULL) == NULL);
  }
}
