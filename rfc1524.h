/**
 * @file
 * RFC1524 Mailcap routines
 *
 * @authors
 * Copyright (C) 1996-2000 Michael R. Elkins <me@mutt.org>
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

#ifndef MUTT_RFC1524_H
#define MUTT_RFC1524_H

#include <stdbool.h>
#include <stddef.h>

struct Body;

/* These Config Variables are only used in rfc1524.c */
extern bool C_MailcapSanitize;

/**
 * struct Rfc1524MailcapEntry - A mailcap entry
 */
struct Rfc1524MailcapEntry
{
  char *command;
  char *testcommand;
  char *composecommand;
  char *composetypecommand;
  char *editcommand;
  char *printcommand;
  char *nametemplate;
  char *conver2;
  bool needsterminal : 1; /**< endwin() and system */
  bool copiousoutput : 1; /**< needs pager, basically */
  bool xneomuttkeep  : 1; /**< do not remove the file on command exit */
};

/**
 * enum MailcapLookup - Mailcap actions
 */
enum MailcapLookup
{
  MUTT_MC_NO_FLAGS = 0, ///< No flags set
  MUTT_MC_EDIT,         ///< Mailcap edit field
  MUTT_MC_COMPOSE,      ///< Mailcap compose field
  MUTT_MC_PRINT,        ///< Mailcap print field
  MUTT_MC_AUTOVIEW,     ///< Mailcap autoview field
};

struct Rfc1524MailcapEntry *rfc1524_new_entry(void);
void rfc1524_free_entry(struct Rfc1524MailcapEntry **entry);
int rfc1524_expand_command(struct Body *a, const char *filename, const char *type, char *command, int clen);
int mutt_rfc1524_expand_filename(const char *nametemplate, const char *oldfile, struct Buffer *newfile);
bool rfc1524_mailcap_lookup(struct Body *a, char *type, struct Rfc1524MailcapEntry *entry, enum MailcapLookup opt);

int mutt_buffer_rfc1524_expand_command(struct Body *a, const char *filename, const char *type, struct Buffer *command);

#endif /* MUTT_RFC1524_H */
