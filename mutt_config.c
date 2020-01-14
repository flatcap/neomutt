/**
 * @file
 * Definitions of config variables
 *
 * @authors
 * Copyright (C) 1996-2002,2007,2010,2012-2013,2016 Michael R. Elkins <me@mutt.org>
 * Copyright (C) 2004 g10 Code GmbH
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
 * @page mutt_config Definitions of config variables
 *
 * Definitions of config variables
 */

#include "config.h"
#ifdef _MAKEDOC
#include "doc/makedoc_defs.h"
#else
#include "mutt/lib.h"
#include "config/lib.h"
#include "conn/lib.h"
#include "gui/lib.h"
#include "addrbook.h"
#include "bcache.h"
#include "browser.h"
#include "commands.h"
#include "compose.h"
#include "edit.h"
#include "globals.h"
#include "handler.h"
#include "hdrline.h"
#include "index.h"
#include "init.h"
#include "mailcap.h"
#include "main.h"
#include "mutt_account.h"
#include "mutt_logging.h"
#include "mutt_mailbox.h"
#include "mutt_thread.h"
#include "muttlib.h"
#include "mx.h"
#include "pager.h"
#include "pattern.h"
#include "progress.h"
#include "query.h"
#include "recvattach.h"
#include "recvcmd.h"
#include "remailer.h"
#include "rfc3676.h"
#include "score.h"
#include "send.h"
#include "sendlib.h"
#include "sidebar.h"
#include "smtp.h"
#include "sort.h"
#include "status.h"
#include "imap/lib.h"
#include "maildir/lib.h"
#include "nntp/lib.h"
#include "notmuch/lib.h"
#include "pop/lib.h"
#endif

#ifndef ISPELL
#define ISPELL "ispell"
#endif

/* This option is deprecated */
bool C_IgnoreLinearWhiteSpace = false;

// clang-format off
struct ConfigDef MuttVars[] = {
  /*++*/

  { "abort_noattach", DT_QUAD, &C_AbortNoattach, MUTT_NO, 0, NULL,
    "If set to _yes_, when composing messages containing the regular\n"
    "expression specified by $$abort_noattach_regex and no attachments are\n"
    "given, composition will be aborted. If set to _no_, composing messages\n"
    "as such will never be aborted.\n"
    "\n"
    "Example:\n"
    "```\n"
    "set abort_noattach_regex = \"\\<attach(|ed|ments?)\\>\"\n"
    "```\n"
  },
  /*
  ** .pp
  ** If set to \fIyes\fP, when composing messages containing the regular
  ** expression specified by $$abort_noattach_regex and no attachments are
  ** given, composition will be aborted. If set to \fIno\fP, composing messages
  ** as such will never be aborted.
  ** .pp
  ** Example:
  ** .ts
  ** set abort_noattach_regex = "\\<attach(|ed|ments?)\\>"
  ** .te
  */
  { "abort_noattach_regex", DT_REGEX, &C_AbortNoattachRegex, IP "\\<(attach|attached|attachments?)\\>", 0, NULL,
    "Specifies a regular expression to match against the body of the message, to\n"
    "determine if an attachment was mentioned but mistakenly forgotten.  If it\n"
    "matches, $$abort_noattach will be consulted to determine if message sending\n"
    "will be aborted.\n"
    "\n"
    "Like other regular expressions in NeoMutt, the search is case sensitive\n"
    "if the pattern contains at least one upper case letter, and case\n"
    "insensitive otherwise.\n"
  },
  /*
  ** .pp
  ** Specifies a regular expression to match against the body of the message, to
  ** determine if an attachment was mentioned but mistakenly forgotten.  If it
  ** matches, $$abort_noattach will be consulted to determine if message sending
  ** will be aborted.
  ** .pp
  ** Like other regular expressions in NeoMutt, the search is case sensitive
  ** if the pattern contains at least one upper case letter, and case
  ** insensitive otherwise.
  */
  { "abort_nosubject", DT_QUAD, &C_AbortNosubject, MUTT_ASKYES, 0, NULL,
    "If set to _yes_, when composing messages and no subject is given\n"
    "at the subject prompt, composition will be aborted.  If set to\n"
    "_no_, composing messages with no subject given at the subject\n"
    "prompt will never be aborted.\n"
  },
  /*
  ** .pp
  ** If set to \fIyes\fP, when composing messages and no subject is given
  ** at the subject prompt, composition will be aborted.  If set to
  ** \fIno\fP, composing messages with no subject given at the subject
  ** prompt will never be aborted.
  */
  { "abort_unmodified", DT_QUAD, &C_AbortUnmodified, MUTT_YES, 0, NULL,
    "If set to _yes_, composition will automatically abort after\n"
    "editing the message body if no changes are made to the file (this\n"
    "check only happens after the _first_ edit of the file).  When set\n"
    "to _no_, composition will never be aborted.\n"
  },
  /*
  ** .pp
  ** If set to \fIyes\fP, composition will automatically abort after
  ** editing the message body if no changes are made to the file (this
  ** check only happens after the \fIfirst\fP edit of the file).  When set
  ** to \fIno\fP, composition will never be aborted.
  */
  { "alias_file", DT_STRING|DT_PATH, &C_AliasFile, IP "~/.neomuttrc", 0, NULL,
    "The default file in which to save aliases created by the\n"
    "`$<create-alias>` function. Entries added to this file are\n"
    "encoded in the character set specified by $$config_charset if it\n"
    "is _set_ or the current character set otherwise.\n"
    "\n"
    "**Note:** NeoMutt will not automatically source this file; you must\n"
    "explicitly use the \"$source\" command for it to be executed in case\n"
    "this option points to a dedicated alias file.\n"
    "\n"
    "The default for this option is the currently used neomuttrc file, or\n"
    "\"~/.neomuttrc\" if no user neomuttrc was found.\n"
  },
  /*
  ** .pp
  ** The default file in which to save aliases created by the
  ** \fC$<create-alias>\fP function. Entries added to this file are
  ** encoded in the character set specified by $$config_charset if it
  ** is \fIset\fP or the current character set otherwise.
  ** .pp
  ** \fBNote:\fP NeoMutt will not automatically source this file; you must
  ** explicitly use the "$source" command for it to be executed in case
  ** this option points to a dedicated alias file.
  ** .pp
  ** The default for this option is the currently used neomuttrc file, or
  ** "~/.neomuttrc" if no user neomuttrc was found.
  */
  { "alias_format", DT_STRING|DT_NOT_EMPTY, &C_AliasFormat, IP "%4n %2f %t %-10a   %r", 0, NULL,
    "Specifies the format of the data displayed for the \"$alias\" menu.  The\n"
    "following `printf(3)`-style sequences are available:\n"
    "\n"
    "| .  | Description |\n"
    "|----+----------------------------------------------------------------|\n"
    "| %a | Alias name |\n"
    "| %f | Flags - currently, a \"d\" for an alias marked for deletion |\n"
    "| %n | Index number |\n"
    "| %r | Address which alias expands to |\n"
    "| %t | Character which indicates if the alias is tagged for inclusion |\n"
  },
  /*
  ** .pp
  ** Specifies the format of the data displayed for the "$alias" menu.  The
  ** following \fCprintf(3)\fP-style sequences are available:
  ** .dl
  ** .dt %a .dd Alias name
  ** .dt %f .dd Flags - currently, a "d" for an alias marked for deletion
  ** .dt %n .dd Index number
  ** .dt %r .dd Address which alias expands to
  ** .dt %t .dd Character which indicates if the alias is tagged for inclusion
  ** .de
  */
  { "allow_8bit", DT_BOOL, &C_Allow8bit, true, 0, NULL,
    "Controls whether 8-bit data is converted to 7-bit using either Quoted-\n"
    "Printable or Base64 encoding when sending mail.\n"
  },
  /*
  ** .pp
  ** Controls whether 8-bit data is converted to 7-bit using either Quoted-
  ** Printable or Base64 encoding when sending mail.
  */
  { "allow_ansi", DT_BOOL, &C_AllowAnsi, false, 0, NULL,
    "Controls whether ANSI color codes in messages (and color tags in\n"
    "rich text messages) are to be interpreted.\n"
    "Messages containing these codes are rare, but if this option is _set_,\n"
    "their text will be colored accordingly. Note that this may override\n"
    "your color choices, and even present a security problem, since a\n"
    "message could include a line like\n"
    "```\n"
    "[-- PGP output follows ...\n"
    "```\n"
    "\n"
    "and give it the same color as your attachment color (see also\n"
    "$$crypt_timestamp).\n"
  },
  /*
  ** .pp
  ** Controls whether ANSI color codes in messages (and color tags in
  ** rich text messages) are to be interpreted.
  ** Messages containing these codes are rare, but if this option is \fIset\fP,
  ** their text will be colored accordingly. Note that this may override
  ** your color choices, and even present a security problem, since a
  ** message could include a line like
  ** .ts
  ** [-- PGP output follows ...
  ** .te
  ** .pp
  ** and give it the same color as your attachment color (see also
  ** $$crypt_timestamp).
  */
  { "arrow_cursor", DT_BOOL|R_MENU, &C_ArrowCursor, false, 0, NULL,
    "When _set_, an arrow (\"->\") will be used to indicate the current entry\n"
    "in menus instead of highlighting the whole line.  On slow network or modem\n"
    "links this will make response faster because there is less that has to\n"
    "be redrawn on the screen when moving to the next or previous entries\n"
    "in the menu.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, an arrow ("->") will be used to indicate the current entry
  ** in menus instead of highlighting the whole line.  On slow network or modem
  ** links this will make response faster because there is less that has to
  ** be redrawn on the screen when moving to the next or previous entries
  ** in the menu.
  */
  { "ascii_chars", DT_BOOL|R_INDEX|R_PAGER, &C_AsciiChars, false, 0, NULL,
    "If _set_, NeoMutt will use plain ASCII characters when displaying thread\n"
    "and attachment trees, instead of the default _ACS_ characters.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, NeoMutt will use plain ASCII characters when displaying thread
  ** and attachment trees, instead of the default \fIACS\fP characters.
  */
#ifdef USE_NNTP
  { "ask_follow_up", DT_BOOL, &C_AskFollowUp, false, 0, NULL,
    "If set, NeoMutt will prompt you for follow-up groups before editing\n"
    "the body of an outgoing message.\n"
  },
  /*
  ** .pp
  ** If set, NeoMutt will prompt you for follow-up groups before editing
  ** the body of an outgoing message.
  */
  { "ask_x_comment_to", DT_BOOL, &C_AskXCommentTo, false, 0, NULL,
    "If set, NeoMutt will prompt you for x-comment-to field before editing\n"
    "the body of an outgoing message.\n"
  },
  /*
  ** .pp
  ** If set, NeoMutt will prompt you for x-comment-to field before editing
  ** the body of an outgoing message.
  */
#endif
  { "askbcc", DT_BOOL, &C_Askbcc, false, 0, NULL,
    "If _set_, NeoMutt will prompt you for blind-carbon-copy (Bcc) recipients\n"
    "before editing an outgoing message.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, NeoMutt will prompt you for blind-carbon-copy (Bcc) recipients
  ** before editing an outgoing message.
  */
  { "askcc", DT_BOOL, &C_Askcc, false, 0, NULL,
    "If _set_, NeoMutt will prompt you for carbon-copy (Cc) recipients before\n"
    "editing the body of an outgoing message.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, NeoMutt will prompt you for carbon-copy (Cc) recipients before
  ** editing the body of an outgoing message.
  */
  { "assumed_charset", DT_STRING, &C_AssumedCharset, 0, 0, charset_validator,
    "This variable is a colon-separated list of character encoding\n"
    "schemes for messages without character encoding indication.\n"
    "Header field values and message body content without character encoding\n"
    "indication would be assumed that they are written in one of this list.\n"
    "By default, all the header fields and message body without any charset\n"
    "indication are assumed to be in \"us-ascii\".\n"
    "\n"
    "For example, Japanese users might prefer this:\n"
    "```\n"
    "set assumed_charset=\"iso-2022-jp:euc-jp:shift_jis:utf-8\"\n"
    "```\n"
    "\n"
    "However, only the first content is valid for the message body.\n"
  },
  /*
  ** .pp
  ** This variable is a colon-separated list of character encoding
  ** schemes for messages without character encoding indication.
  ** Header field values and message body content without character encoding
  ** indication would be assumed that they are written in one of this list.
  ** By default, all the header fields and message body without any charset
  ** indication are assumed to be in "us-ascii".
  ** .pp
  ** For example, Japanese users might prefer this:
  ** .ts
  ** set assumed_charset="iso-2022-jp:euc-jp:shift_jis:utf-8"
  ** .te
  ** .pp
  ** However, only the first content is valid for the message body.
  */
  { "attach_charset", DT_STRING, &C_AttachCharset, 0, 0, charset_validator,
    "This variable is a colon-separated list of character encoding\n"
    "schemes for text file attachments. NeoMutt uses this setting to guess\n"
    "which encoding files being attached are encoded in to convert them to\n"
    "a proper character set given in $$send_charset.\n"
    "\n"
    "If _unset_, the value of $$charset will be used instead.\n"
    "For example, the following configuration would work for Japanese\n"
    "text handling:\n"
    "```\n"
    "set attach_charset=\"iso-2022-jp:euc-jp:shift_jis:utf-8\"\n"
    "```\n"
    "\n"
    "Note: for Japanese users, \"iso-2022-*\" must be put at the head\n"
    "of the value as shown above if included.\n"
  },
  /*
  ** .pp
  ** This variable is a colon-separated list of character encoding
  ** schemes for text file attachments. NeoMutt uses this setting to guess
  ** which encoding files being attached are encoded in to convert them to
  ** a proper character set given in $$send_charset.
  ** .pp
  ** If \fIunset\fP, the value of $$charset will be used instead.
  ** For example, the following configuration would work for Japanese
  ** text handling:
  ** .ts
  ** set attach_charset="iso-2022-jp:euc-jp:shift_jis:utf-8"
  ** .te
  ** .pp
  ** Note: for Japanese users, "iso-2022-*" must be put at the head
  ** of the value as shown above if included.
  */
  { "attach_format", DT_STRING|DT_NOT_EMPTY, &C_AttachFormat, IP "%u%D%I %t%4n %T%.40d%> [%.7m/%.10M, %.6e%?C?, %C?, %s] ", 0, NULL,
    "This variable describes the format of the \"attachment\" menu.  The\n"
    "following `printf(3)`-style sequences are understood:\n"
    "\n"
    "| .   | Description |\n"
    "|-----+----------------------------------------------------------------------------------------------------------------------------------|\n"
    "| %C  | Charset |\n"
    "| %c  | Requires charset conversion (\"n\" or \"c\") |\n"
    "| %D  | Deleted flag |\n"
    "| %d  | Description (if none, falls back to %F) |\n"
    "| %e  | MIME content-transfer-encoding |\n"
    "| %f  | Filename |\n"
    "| %F  | Filename in content-disposition header (if none, falls back to %f) |\n"
    "| %I  | Disposition (\"I\" for inline, \"A\" for attachment) |\n"
    "| %m  | Major MIME type |\n"
    "| %M  | MIME subtype |\n"
    "| %n  | Attachment number |\n"
    "| %Q  | \"Q\", if MIME part qualifies for attachment counting |\n"
    "| %s  | Size (see $formatstrings-size) |\n"
    "| %T  | Graphic tree characters |\n"
    "| %t  | Tagged flag |\n"
    "| %u  | Unlink (=to delete) flag |\n"
    "| %X  | Number of qualifying MIME parts in this part and its children (please see the \"$attachments\" section for possible speed effects) |\n"
    "| %>X | Right justify the rest of the string and pad with character \"X\" |\n"
    "| %|X | Pad to the end of the line with character \"X\" |\n"
    "| %*X | Soft-fill with character \"X\" as pad |\n"
    "\n"
    "For an explanation of \"soft-fill\", see the $$index_format documentation.\n"
  },
  /*
  ** .pp
  ** This variable describes the format of the "attachment" menu.  The
  ** following \fCprintf(3)\fP-style sequences are understood:
  ** .dl
  ** .dt %C  .dd Charset
  ** .dt %c  .dd Requires charset conversion ("n" or "c")
  ** .dt %D  .dd Deleted flag
  ** .dt %d  .dd Description (if none, falls back to %F)
  ** .dt %e  .dd MIME content-transfer-encoding
  ** .dt %f  .dd Filename
  ** .dt %F  .dd Filename in content-disposition header (if none, falls back to %f)
  ** .dt %I  .dd Disposition ("I" for inline, "A" for attachment)
  ** .dt %m  .dd Major MIME type
  ** .dt %M  .dd MIME subtype
  ** .dt %n  .dd Attachment number
  ** .dt %Q  .dd "Q", if MIME part qualifies for attachment counting
  ** .dt %s  .dd Size (see $formatstrings-size)
  ** .dt %T  .dd Graphic tree characters
  ** .dt %t  .dd Tagged flag
  ** .dt %u  .dd Unlink (=to delete) flag
  ** .dt %X  .dd Number of qualifying MIME parts in this part and its children
  **             (please see the "$attachments" section for possible speed effects)
  ** .dt %>X .dd Right justify the rest of the string and pad with character "X"
  ** .dt %|X .dd Pad to the end of the line with character "X"
  ** .dt %*X .dd Soft-fill with character "X" as pad
  ** .de
  ** .pp
  ** For an explanation of "soft-fill", see the $$index_format documentation.
  */
  { "attach_save_dir", DT_STRING|DT_PATH, &C_AttachSaveDir, IP "./", 0, NULL,
    "The directory where attachments are saved.\n"
  },
  /*
  ** .pp
  ** The directory where attachments are saved.
  */
  { "attach_save_without_prompting", DT_BOOL, &C_AttachSaveWithoutPrompting, false, 0, NULL,
    "This variable, when set to true, will cause attachments to be saved to\n"
    "the 'attach_save_dir' location without prompting the user for the filename.\n"
    "\n"
  },
  /*
  ** .pp
  ** This variable, when set to true, will cause attachments to be saved to
  ** the 'attach_save_dir' location without prompting the user for the filename.
  ** .pp
  */
  { "attach_sep", DT_STRING, &C_AttachSep, IP "\n", 0, NULL,
    "The separator to add between attachments when operating (saving,\n"
    "printing, piping, etc) on a list of tagged attachments.\n"
  },
  /*
  ** .pp
  ** The separator to add between attachments when operating (saving,
  ** printing, piping, etc) on a list of tagged attachments.
  */
  { "attach_split", DT_BOOL, &C_AttachSplit, true, 0, NULL,
    "If this variable is _unset_, when operating (saving, printing, piping,\n"
    "etc) on a list of tagged attachments, NeoMutt will concatenate the\n"
    "attachments and will operate on them as a single attachment. The\n"
    "$$attach_sep separator is added after each attachment. When _set_,\n"
    "NeoMutt will operate on the attachments one by one.\n"
  },
  /*
  ** .pp
  ** If this variable is \fIunset\fP, when operating (saving, printing, piping,
  ** etc) on a list of tagged attachments, NeoMutt will concatenate the
  ** attachments and will operate on them as a single attachment. The
  ** $$attach_sep separator is added after each attachment. When \fIset\fP,
  ** NeoMutt will operate on the attachments one by one.
  */
  { "attribution", DT_STRING, &C_Attribution, IP "On %d, %n wrote:", 0, NULL,
    "This is the string that will precede a message which has been included\n"
    "in a reply.  For a full listing of defined `printf(3)`-like sequences see\n"
    "the section on $$index_format.\n"
  },
  /*
  ** .pp
  ** This is the string that will precede a message which has been included
  ** in a reply.  For a full listing of defined \fCprintf(3)\fP-like sequences see
  ** the section on $$index_format.
  */
  { "attribution_locale", DT_STRING, &C_AttributionLocale, 0, 0, NULL,
    "The locale used by `strftime(3)` to format dates in the\n"
    "$attribution string.  Legal values are the strings your system\n"
    "accepts for the locale environment variable `$$$LC_TIME`.\n"
    "\n"
    "This variable is to allow the attribution date format to be\n"
    "customized by recipient or folder using hooks.  By default, NeoMutt\n"
    "will use your locale environment, so there is no need to set\n"
    "this except to override that default.\n"
  },
  /*
  ** .pp
  ** The locale used by \fCstrftime(3)\fP to format dates in the
  ** $attribution string.  Legal values are the strings your system
  ** accepts for the locale environment variable \fC$$$LC_TIME\fP.
  ** .pp
  ** This variable is to allow the attribution date format to be
  ** customized by recipient or folder using hooks.  By default, NeoMutt
  ** will use your locale environment, so there is no need to set
  ** this except to override that default.
  */
  { "auto_subscribe", DT_BOOL, &C_AutoSubscribe, false, 0, NULL,
    "When _set_, NeoMutt assumes the presence of a List-Post header\n"
    "means the recipient is subscribed to the list.  Unless the mailing list\n"
    "is in the \"unsubscribe\" or \"unlist\" lists, it will be added\n"
    "to the \"$subscribe\" list.  Parsing and checking these things slows\n"
    "header reading down, so this option is disabled by default.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt assumes the presence of a List-Post header
  ** means the recipient is subscribed to the list.  Unless the mailing list
  ** is in the "unsubscribe" or "unlist" lists, it will be added
  ** to the "$subscribe" list.  Parsing and checking these things slows
  ** header reading down, so this option is disabled by default.
  */
  { "auto_tag", DT_BOOL, &C_AutoTag, false, 0, NULL,
    "When _set_, functions in the _index_ menu which affect a message\n"
    "will be applied to all tagged messages (if there are any).  When\n"
    "unset, you must first use the `<tag-prefix>` function (bound to \";\"\n"
    "by default) to make the next function apply to all tagged messages.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, functions in the \fIindex\fP menu which affect a message
  ** will be applied to all tagged messages (if there are any).  When
  ** unset, you must first use the \fC<tag-prefix>\fP function (bound to ";"
  ** by default) to make the next function apply to all tagged messages.
  */
#ifdef USE_AUTOCRYPT
  { "autocrypt", DT_BOOL, &C_Autocrypt, false, 0, NULL,
    "When _set_, enables autocrypt, which provides\n"
    "passive encryption protection with keys exchanged via headers.\n"
    "See ``$autocryptdoc'' for more details.\n"
    "(Autocrypt only)\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, enables autocrypt, which provides
  ** passive encryption protection with keys exchanged via headers.
  ** See ``$autocryptdoc'' for more details.
  ** (Autocrypt only)
  */
  { "autocrypt_acct_format", DT_STRING|R_MENU, &C_AutocryptAcctFormat, IP "%4n %-30a %20p %10s", 0, NULL,
    "This variable describes the format of the ``autocrypt account'' menu.\n"
    "The following `printf(3)`-style sequences are understood\n"
    "\n"
    "| .  | Description |\n"
    "|----+-------------------------------|\n"
    "| %a | email address |\n"
    "| %k | gpg keyid |\n"
    "| %n | current entry number |\n"
    "| %p | prefer-encrypt flag |\n"
    "| %s | status flag (active/inactive) |\n"
    "\n"
    "(Autocrypt only)\n"
  },
  /*
  ** .pp
  ** This variable describes the format of the ``autocrypt account'' menu.
  ** The following \fCprintf(3)\fP-style sequences are understood
  ** .dl
  ** .dt %a  .dd email address
  ** .dt %k  .dd gpg keyid
  ** .dt %n  .dd current entry number
  ** .dt %p  .dd prefer-encrypt flag
  ** .dt %s  .dd status flag (active/inactive)
  ** .de
  ** .pp
  ** (Autocrypt only)
  */
  { "autocrypt_dir", DT_STRING|DT_PATH, &C_AutocryptDir, IP "~/.mutt/autocrypt", 0, NULL,
    "This variable sets where autocrypt files are stored, including the GPG\n"
    "keyring and sqlite database.  See ``$autocryptdoc'' for more details.\n"
    "(Autocrypt only)\n"
  },
  /*
  ** .pp
  ** This variable sets where autocrypt files are stored, including the GPG
  ** keyring and sqlite database.  See ``$autocryptdoc'' for more details.
  ** (Autocrypt only)
  */
  { "autocrypt_reply", DT_BOOL, &C_AutocryptReply, true, 0, NULL,
    "When _set_, replying to an autocrypt email automatically\n"
    "enables autocrypt in the reply.  You may want to unset this if you're using\n"
    "the same key for autocrypt as normal web-of-trust, so that autocrypt\n"
    "isn't forced on for all encrypted replies.\n"
    "(Autocrypt only)\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, replying to an autocrypt email automatically
  ** enables autocrypt in the reply.  You may want to unset this if you're using
  ** the same key for autocrypt as normal web-of-trust, so that autocrypt
  ** isn't forced on for all encrypted replies.
  ** (Autocrypt only)
  */
#endif
  { "autoedit", DT_BOOL, &C_Autoedit, false, 0, NULL,
    "When _set_ along with $$edit_headers, NeoMutt will skip the initial\n"
    "send-menu (prompting for subject and recipients) and allow you to\n"
    "immediately begin editing the body of your\n"
    "message.  The send-menu may still be accessed once you have finished\n"
    "editing the body of your message.\n"
    "\n"
    "**Note:** when this option is _set_, you can't use send-hooks that depend\n"
    "on the recipients when composing a new (non-reply) message, as the initial\n"
    "list of recipients is empty.\n"
    "\n"
    "Also see $$fast_reply.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP along with $$edit_headers, NeoMutt will skip the initial
  ** send-menu (prompting for subject and recipients) and allow you to
  ** immediately begin editing the body of your
  ** message.  The send-menu may still be accessed once you have finished
  ** editing the body of your message.
  ** .pp
  ** \fBNote:\fP when this option is \fIset\fP, you can't use send-hooks that depend
  ** on the recipients when composing a new (non-reply) message, as the initial
  ** list of recipients is empty.
  ** .pp
  ** Also see $$fast_reply.
  */
  { "beep", DT_BOOL, &C_Beep, true, 0, NULL,
    "When this variable is _set_, NeoMutt will beep when an error occurs.\n"
  },
  /*
  ** .pp
  ** When this variable is \fIset\fP, NeoMutt will beep when an error occurs.
  */
  { "beep_new", DT_BOOL, &C_BeepNew, false, 0, NULL,
    "When this variable is _set_, NeoMutt will beep whenever it prints a message\n"
    "notifying you of new mail.  This is independent of the setting of the\n"
    "$$beep variable.\n"
  },
  /*
  ** .pp
  ** When this variable is \fIset\fP, NeoMutt will beep whenever it prints a message
  ** notifying you of new mail.  This is independent of the setting of the
  ** $$beep variable.
  */
  { "bounce", DT_QUAD, &C_Bounce, MUTT_ASKYES, 0, NULL,
    "Controls whether you will be asked to confirm bouncing messages.\n"
    "If set to _yes_ you don't get asked if you want to bounce a\n"
    "message. Setting this variable to _no_ is not generally useful,\n"
    "and thus not recommended, because you are unable to bounce messages.\n"
  },
  /*
  ** .pp
  ** Controls whether you will be asked to confirm bouncing messages.
  ** If set to \fIyes\fP you don't get asked if you want to bounce a
  ** message. Setting this variable to \fIno\fP is not generally useful,
  ** and thus not recommended, because you are unable to bounce messages.
  */
  { "bounce_delivered", DT_BOOL, &C_BounceDelivered, true, 0, NULL,
    "When this variable is _set_, NeoMutt will include Delivered-To headers when\n"
    "bouncing messages.  Postfix users may wish to _unset_ this variable.\n"
  },
  /*
  ** .pp
  ** When this variable is \fIset\fP, NeoMutt will include Delivered-To headers when
  ** bouncing messages.  Postfix users may wish to \fIunset\fP this variable.
  */
  { "braille_friendly", DT_BOOL, &C_BrailleFriendly, false, 0, NULL,
    "When this variable is _set_, NeoMutt will place the cursor at the beginning\n"
    "of the current line in menus, even when the $$arrow_cursor variable\n"
    "is _unset_, making it easier for blind persons using Braille displays to\n"
    "follow these menus.  The option is _unset_ by default because many\n"
    "visual terminals don't permit making the cursor invisible.\n"
  },
  /*
  ** .pp
  ** When this variable is \fIset\fP, NeoMutt will place the cursor at the beginning
  ** of the current line in menus, even when the $$arrow_cursor variable
  ** is \fIunset\fP, making it easier for blind persons using Braille displays to
  ** follow these menus.  The option is \fIunset\fP by default because many
  ** visual terminals don't permit making the cursor invisible.
  */
  { "browser_abbreviate_mailboxes", DT_BOOL, &C_BrowserAbbreviateMailboxes, true, 0, NULL,
    "When this variable is _set_, NeoMutt will abbreviate mailbox\n"
    "names in the browser mailbox list, using '~' and '='\n"
    "shortcuts.\n"
    "\n"
    "The default `\"alpha\"` setting of $$sort_browser uses\n"
    "locale-based sorting (using `strcoll(3)`), which ignores some\n"
    "punctuation.  This can lead to some situations where the order\n"
    "doesn't make intuitive sense.  In those cases, it may be\n"
    "desirable to _unset_ this variable.\n"
  },
  /*
  ** .pp
  ** When this variable is \fIset\fP, NeoMutt will abbreviate mailbox
  ** names in the browser mailbox list, using '~' and '='
  ** shortcuts.
  ** .pp
  ** The default \fC"alpha"\fP setting of $$sort_browser uses
  ** locale-based sorting (using \fCstrcoll(3)\fP), which ignores some
  ** punctuation.  This can lead to some situations where the order
  ** doesn't make intuitive sense.  In those cases, it may be
  ** desirable to \fIunset\fP this variable.
  */
#ifdef USE_NNTP
  { "catchup_newsgroup", DT_QUAD, &C_CatchupNewsgroup, MUTT_ASKYES, 0, NULL,
    "If this variable is _set_, NeoMutt will mark all articles in newsgroup\n"
    "as read when you quit the newsgroup (catchup newsgroup).\n"
  },
  /*
  ** .pp
  ** If this variable is \fIset\fP, NeoMutt will mark all articles in newsgroup
  ** as read when you quit the newsgroup (catchup newsgroup).
  */
#endif
#ifdef USE_SSL
  { "certificate_file", DT_STRING|DT_PATH, &C_CertificateFile, IP "~/.mutt_certificates", 0, NULL,
    "This variable specifies the file where the certificates you trust\n"
    "are saved. When an unknown certificate is encountered, you are asked\n"
    "if you accept it or not. If you accept it, the certificate can also\n"
    "be saved in this file and further connections are automatically\n"
    "accepted.\n"
    "\n"
    "You can also manually add CA certificates in this file. Any server\n"
    "certificate that is signed with one of these CA certificates is\n"
    "also automatically accepted.\n"
    "\n"
    "Example:\n"
    "```\n"
    "set certificate_file=~/.neomutt/certificates\n"
    "```\n"
  },
  /*
  ** .pp
  ** This variable specifies the file where the certificates you trust
  ** are saved. When an unknown certificate is encountered, you are asked
  ** if you accept it or not. If you accept it, the certificate can also
  ** be saved in this file and further connections are automatically
  ** accepted.
  ** .pp
  ** You can also manually add CA certificates in this file. Any server
  ** certificate that is signed with one of these CA certificates is
  ** also automatically accepted.
  ** .pp
  ** Example:
  ** .ts
  ** set certificate_file=~/.neomutt/certificates
  ** .te
  */
#endif
  { "change_folder_next", DT_BOOL, &C_ChangeFolderNext, false, 0, NULL,
    "When this variable is _set_, the `<change-folder>` function\n"
    "mailbox suggestion will start at the next folder in your \"$mailboxes\"\n"
    "list, instead of starting at the first folder in the list.\n"
  },
  /*
  ** .pp
  ** When this variable is \fIset\fP, the \fC<change-folder>\fP function
  ** mailbox suggestion will start at the next folder in your "$mailboxes"
  ** list, instead of starting at the first folder in the list.
  */
  { "charset", DT_STRING|DT_NOT_EMPTY, &C_Charset, 0, 0, charset_validator,
    "Character set your terminal uses to display and enter textual data.\n"
    "It is also the fallback for $$send_charset.\n"
    "\n"
    "Upon startup NeoMutt tries to derive this value from environment variables\n"
    "such as `$$$LC_CTYPE` or `$$$LANG`.\n"
    "\n"
    "**Note:** It should only be set in case NeoMutt isn't able to determine the\n"
    "character set used correctly.\n"
  },
  /*
  ** .pp
  ** Character set your terminal uses to display and enter textual data.
  ** It is also the fallback for $$send_charset.
  ** .pp
  ** Upon startup NeoMutt tries to derive this value from environment variables
  ** such as \fC$$$LC_CTYPE\fP or \fC$$$LANG\fP.
  ** .pp
  ** \fBNote:\fP It should only be set in case NeoMutt isn't able to determine the
  ** character set used correctly.
  */
  { "check_mbox_size", DT_BOOL, &C_CheckMboxSize, false, 0, NULL,
    "When this variable is _set_, NeoMutt will use file size attribute instead of\n"
    "access time when checking for new mail in mbox and mmdf folders.\n"
    "\n"
    "This variable is _unset_ by default and should only be enabled when\n"
    "new mail detection for these folder types is unreliable or doesn't work.\n"
    "\n"
    "Note that enabling this variable should happen before any \"$mailboxes\"\n"
    "directives occur in configuration files regarding mbox or mmdf folders\n"
    "because NeoMutt needs to determine the initial new mail status of such a\n"
    "mailbox by performing a fast mailbox scan when it is defined.\n"
    "Afterwards the new mail status is tracked by file size changes.\n"
  },
  /*
  ** .pp
  ** When this variable is \fIset\fP, NeoMutt will use file size attribute instead of
  ** access time when checking for new mail in mbox and mmdf folders.
  ** .pp
  ** This variable is \fIunset\fP by default and should only be enabled when
  ** new mail detection for these folder types is unreliable or doesn't work.
  ** .pp
  ** Note that enabling this variable should happen before any "$mailboxes"
  ** directives occur in configuration files regarding mbox or mmdf folders
  ** because NeoMutt needs to determine the initial new mail status of such a
  ** mailbox by performing a fast mailbox scan when it is defined.
  ** Afterwards the new mail status is tracked by file size changes.
  */
  { "check_new", DT_BOOL, &C_CheckNew, true, 0, NULL,
    "**Note:** this option only affects _maildir_ and _MH_ style\n"
    "mailboxes.\n"
    "\n"
    "When _set_, NeoMutt will check for new mail delivered while the\n"
    "mailbox is open.  Especially with MH mailboxes, this operation can\n"
    "take quite some time since it involves scanning the directory and\n"
    "checking each file to see if it has already been looked at.  If\n"
    "this variable is _unset_, no check for new mail is performed\n"
    "while the mailbox is open.\n"
  },
  /*
  ** .pp
  ** \fBNote:\fP this option only affects \fImaildir\fP and \fIMH\fP style
  ** mailboxes.
  ** .pp
  ** When \fIset\fP, NeoMutt will check for new mail delivered while the
  ** mailbox is open.  Especially with MH mailboxes, this operation can
  ** take quite some time since it involves scanning the directory and
  ** checking each file to see if it has already been looked at.  If
  ** this variable is \fIunset\fP, no check for new mail is performed
  ** while the mailbox is open.
  */
  { "collapse_all", DT_BOOL, &C_CollapseAll, false, 0, NULL,
    "When _set_, NeoMutt will collapse all threads when entering a folder.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will collapse all threads when entering a folder.
  */
  { "collapse_flagged", DT_BOOL, &C_CollapseFlagged, true, 0, NULL,
    "When _unset_, NeoMutt will not collapse a thread if it contains any\n"
    "flagged messages.\n"
  },
  /*
  ** .pp
  ** When \fIunset\fP, NeoMutt will not collapse a thread if it contains any
  ** flagged messages.
  */
  { "collapse_unread", DT_BOOL, &C_CollapseUnread, true, 0, NULL,
    "When _unset_, NeoMutt will not collapse a thread if it contains any\n"
    "unread messages.\n"
  },
  /*
  ** .pp
  ** When \fIunset\fP, NeoMutt will not collapse a thread if it contains any
  ** unread messages.
  */
  { "compose_format", DT_STRING|R_MENU, &C_ComposeFormat, IP "-- NeoMutt: Compose  [Approx. msg size: %l   Atts: %a]%>-", 0, NULL,
    "Controls the format of the status line displayed in the \"compose\"\n"
    "menu.  This string is similar to $$status_format, but has its own\n"
    "set of `printf(3)`-like sequences:\n"
    "\n"
    "| .  | Description |\n"
    "|----+------------------------------------------------------------------------------|\n"
    "| %a | Total number of attachments |\n"
    "| %h | Local hostname |\n"
    "| %l | Approximate size (in bytes) of the current message (see $formatstrings-size) |\n"
    "| %v | NeoMutt version string |\n"
    "\n"
    "See the text describing the $$status_format option for more\n"
    "information on how to set $$compose_format.\n"
  },
  /*
  ** .pp
  ** Controls the format of the status line displayed in the "compose"
  ** menu.  This string is similar to $$status_format, but has its own
  ** set of \fCprintf(3)\fP-like sequences:
  ** .dl
  ** .dt %a .dd Total number of attachments
  ** .dt %h .dd Local hostname
  ** .dt %l .dd Approximate size (in bytes) of the current message (see $formatstrings-size)
  ** .dt %v .dd NeoMutt version string
  ** .de
  ** .pp
  ** See the text describing the $$status_format option for more
  ** information on how to set $$compose_format.
  */
  { "config_charset", DT_STRING, &C_ConfigCharset, 0, 0, charset_validator,
    "When defined, NeoMutt will recode commands in rc files from this\n"
    "encoding to the current character set as specified by $$charset\n"
    "and aliases written to $$alias_file from the current character set.\n"
    "\n"
    "Please note that if setting $$charset it must be done before\n"
    "setting $$config_charset.\n"
    "\n"
    "Recoding should be avoided as it may render unconvertable\n"
    "characters as question marks which can lead to undesired\n"
    "side effects (for example in regular expressions).\n"
  },
  /*
  ** .pp
  ** When defined, NeoMutt will recode commands in rc files from this
  ** encoding to the current character set as specified by $$charset
  ** and aliases written to $$alias_file from the current character set.
  ** .pp
  ** Please note that if setting $$charset it must be done before
  ** setting $$config_charset.
  ** .pp
  ** Recoding should be avoided as it may render unconvertable
  ** characters as question marks which can lead to undesired
  ** side effects (for example in regular expressions).
  */
  { "confirmappend", DT_BOOL, &C_Confirmappend, true, 0, NULL,
    "When _set_, NeoMutt will prompt for confirmation when appending messages to\n"
    "an existing mailbox.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will prompt for confirmation when appending messages to
  ** an existing mailbox.
  */
  { "confirmcreate", DT_BOOL, &C_Confirmcreate, true, 0, NULL,
    "When _set_, NeoMutt will prompt for confirmation when saving messages to a\n"
    "mailbox which does not yet exist before creating it.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will prompt for confirmation when saving messages to a
  ** mailbox which does not yet exist before creating it.
  */
  { "connect_timeout", DT_NUMBER, &C_ConnectTimeout, 30, 0, NULL,
    "Causes NeoMutt to timeout a network connection (for IMAP, POP or SMTP) after this\n"
    "many seconds if the connection is not able to be established.  A negative\n"
    "value causes NeoMutt to wait indefinitely for the connection attempt to succeed.\n"
  },
  /*
  ** .pp
  ** Causes NeoMutt to timeout a network connection (for IMAP, POP or SMTP) after this
  ** many seconds if the connection is not able to be established.  A negative
  ** value causes NeoMutt to wait indefinitely for the connection attempt to succeed.
  */
  { "content_type", DT_STRING, &C_ContentType, IP "text/plain", 0, NULL,
    "Sets the default Content-Type for the body of newly composed messages.\n"
  },
  /*
  ** .pp
  ** Sets the default Content-Type for the body of newly composed messages.
  */
  { "copy", DT_QUAD, &C_Copy, MUTT_YES, 0, NULL,
    "This variable controls whether or not copies of your outgoing messages\n"
    "will be saved for later references.  Also see $$record,\n"
    "$$save_name, $$force_name and \"$fcc-hook\".\n"
  },
  /*
  ** .pp
  ** This variable controls whether or not copies of your outgoing messages
  ** will be saved for later references.  Also see $$record,
  ** $$save_name, $$force_name and "$fcc-hook".
  */
  { "crypt_autoencrypt", DT_BOOL, &C_CryptAutoencrypt, false, 0, NULL,
    "Setting this variable will cause NeoMutt to always attempt to PGP\n"
    "encrypt outgoing messages.  This is probably only useful in\n"
    "connection to the \"$send-hook\" command.  It can be overridden\n"
    "by use of the pgp menu, when encryption is not required or\n"
    "signing is requested as well.  If $$smime_is_default is _set_,\n"
    "then OpenSSL is used instead to create S/MIME messages and\n"
    "settings can be overridden by use of the smime menu instead.\n"
    "(Crypto only)\n"
  },
  /*
  ** .pp
  ** Setting this variable will cause NeoMutt to always attempt to PGP
  ** encrypt outgoing messages.  This is probably only useful in
  ** connection to the "$send-hook" command.  It can be overridden
  ** by use of the pgp menu, when encryption is not required or
  ** signing is requested as well.  If $$smime_is_default is \fIset\fP,
  ** then OpenSSL is used instead to create S/MIME messages and
  ** settings can be overridden by use of the smime menu instead.
  ** (Crypto only)
  */
  { "crypt_autopgp", DT_BOOL, &C_CryptAutopgp, true, 0, NULL,
    "This variable controls whether or not NeoMutt may automatically enable\n"
    "PGP encryption/signing for messages.  See also $$crypt_autoencrypt,\n"
    "$$crypt_replyencrypt,\n"
    "$$crypt_autosign, $$crypt_replysign and $$smime_is_default.\n"
  },
  /*
  ** .pp
  ** This variable controls whether or not NeoMutt may automatically enable
  ** PGP encryption/signing for messages.  See also $$crypt_autoencrypt,
  ** $$crypt_replyencrypt,
  ** $$crypt_autosign, $$crypt_replysign and $$smime_is_default.
  */
  { "crypt_autosign", DT_BOOL, &C_CryptAutosign, false, 0, NULL,
    "Setting this variable will cause NeoMutt to always attempt to\n"
    "cryptographically sign outgoing messages.  This can be overridden\n"
    "by use of the pgp menu, when signing is not required or\n"
    "encryption is requested as well. If $$smime_is_default is _set_,\n"
    "then OpenSSL is used instead to create S/MIME messages and settings can\n"
    "be overridden by use of the smime menu instead of the pgp menu.\n"
    "(Crypto only)\n"
  },
  /*
  ** .pp
  ** Setting this variable will cause NeoMutt to always attempt to
  ** cryptographically sign outgoing messages.  This can be overridden
  ** by use of the pgp menu, when signing is not required or
  ** encryption is requested as well. If $$smime_is_default is \fIset\fP,
  ** then OpenSSL is used instead to create S/MIME messages and settings can
  ** be overridden by use of the smime menu instead of the pgp menu.
  ** (Crypto only)
  */
  { "crypt_autosmime", DT_BOOL, &C_CryptAutosmime, true, 0, NULL,
    "This variable controls whether or not NeoMutt may automatically enable\n"
    "S/MIME encryption/signing for messages. See also $$crypt_autoencrypt,\n"
    "$$crypt_replyencrypt,\n"
    "$$crypt_autosign, $$crypt_replysign and $$smime_is_default.\n"
  },
  /*
  ** .pp
  ** This variable controls whether or not NeoMutt may automatically enable
  ** S/MIME encryption/signing for messages. See also $$crypt_autoencrypt,
  ** $$crypt_replyencrypt,
  ** $$crypt_autosign, $$crypt_replysign and $$smime_is_default.
  */
  { "crypt_chars", DT_MBTABLE|R_INDEX|R_PAGER, &C_CryptChars, IP "SPsK ", 0, NULL,
    "Controls the characters used in cryptography flags.\n"
    "\n"
    "| Character | Default | Description |\n"
    "|-----------+---------+-----------------------------------------------------------------|\n"
    "| 1         | S       | The mail is signed, and the signature is successfully verified. |\n"
    "| 2         | P       | The mail is PGP encrypted. |\n"
    "| 3         | s       | The mail is signed. |\n"
    "| 4         | K       | The mail contains a PGP public key. |\n"
    "| 5         | <space> | The mail has no crypto info. |\n"
  },
  /*
  ** .pp
  ** Controls the characters used in cryptography flags.
  ** .dl
  ** .dt \fBCharacter\fP .dd \fBDefault\fP .dd \fBDescription\fP
  ** .dt 1 .dd S .dd The mail is signed, and the signature is successfully verified.
  ** .dt 2 .dd P .dd The mail is PGP encrypted.
  ** .dt 3 .dd s .dd The mail is signed.
  ** .dt 4 .dd K .dd The mail contains a PGP public key.
  ** .dt 5 .dd <space> .dd The mail has no crypto info.
  ** .de
  */
  { "crypt_confirmhook", DT_BOOL, &C_CryptConfirmhook, true, 0, NULL,
    "If set, then you will be prompted for confirmation of keys when using\n"
    "the _crypt-hook_ command.  If unset, no such confirmation prompt will\n"
    "be presented.  This is generally considered unsafe, especially where\n"
    "typos are concerned.\n"
  },
  /*
  ** .pp
  ** If set, then you will be prompted for confirmation of keys when using
  ** the \fIcrypt-hook\fP command.  If unset, no such confirmation prompt will
  ** be presented.  This is generally considered unsafe, especially where
  ** typos are concerned.
  */
  { "crypt_opportunistic_encrypt", DT_BOOL, &C_CryptOpportunisticEncrypt, false, 0, NULL,
    "Setting this variable will cause NeoMutt to automatically enable and\n"
    "disable encryption, based on whether all message recipient keys\n"
    "can be located by NeoMutt.\n"
    "\n"
    "When this option is enabled, NeoMutt will enable/disable encryption\n"
    "each time the TO, CC, and BCC lists are edited.  If\n"
    "$$edit_headers is set, NeoMutt will also do so each time the message\n"
    "is edited.\n"
    "\n"
    "While this is set, encryption can't be manually enabled/disabled.\n"
    "The pgp or smime menus provide a selection to temporarily disable\n"
    "this option for the current message.\n"
    "\n"
    "If $$crypt_autoencrypt or $$crypt_replyencrypt enable encryption for\n"
    "a message, this option will be disabled for that message.  It can\n"
    "be manually re-enabled in the pgp or smime menus.\n"
    "(Crypto only)\n"
  },
  /*
  ** .pp
  ** Setting this variable will cause NeoMutt to automatically enable and
  ** disable encryption, based on whether all message recipient keys
  ** can be located by NeoMutt.
  ** .pp
  ** When this option is enabled, NeoMutt will enable/disable encryption
  ** each time the TO, CC, and BCC lists are edited.  If
  ** $$edit_headers is set, NeoMutt will also do so each time the message
  ** is edited.
  ** .pp
  ** While this is set, encryption can't be manually enabled/disabled.
  ** The pgp or smime menus provide a selection to temporarily disable
  ** this option for the current message.
  ** .pp
  ** If $$crypt_autoencrypt or $$crypt_replyencrypt enable encryption for
  ** a message, this option will be disabled for that message.  It can
  ** be manually re-enabled in the pgp or smime menus.
  ** (Crypto only)
  */
  { "crypt_opportunistic_encrypt_strong_keys", DT_BOOL, &C_CryptOpportunisticEncryptStrongKeys, false, 0, NULL,
    "When set, this modifies the behavior of $$crypt_opportunistic_encrypt\n"
    "to only search for \"strong keys\", that is, keys with full validity\n"
    "according to the web-of-trust algorithm.  A key with marginal or no\n"
    "validity will not enable opportunistic encryption.\n"
    "\n"
    "For S/MIME, the behavior depends on the backend.  Classic S/MIME will\n"
    "filter for certificates with the 't'(trusted) flag in the .index file.\n"
    "The GPGME backend will use the same filters as with OpenPGP, and depends\n"
    "on GPGME's logic for assigning the GPGME_VALIDITY_FULL and\n"
    "GPGME_VALIDITY_ULTIMATE validity flag.\n"
  },
  /*
  ** .pp
  ** When set, this modifies the behavior of $$crypt_opportunistic_encrypt
  ** to only search for "strong keys", that is, keys with full validity
  ** according to the web-of-trust algorithm.  A key with marginal or no
  ** validity will not enable opportunistic encryption.
  ** .pp
  ** For S/MIME, the behavior depends on the backend.  Classic S/MIME will
  ** filter for certificates with the 't'(trusted) flag in the .index file.
  ** The GPGME backend will use the same filters as with OpenPGP, and depends
  ** on GPGME's logic for assigning the GPGME_VALIDITY_FULL and
  ** GPGME_VALIDITY_ULTIMATE validity flag.
  */
  { "crypt_protected_headers_read", DT_BOOL, &C_CryptProtectedHeadersRead, true, 0, NULL,
    "When set, NeoMutt will display protected headers (\"Memory Hole\") in the pager,\n"
    "and will update the index and header cache with revised headers.\n"
    "**\n"
    "Protected headers are stored inside the encrypted or signed part of an\n"
    "an email, to prevent disclosure or tampering.\n"
    "For more information see https://github.com/autocrypt/memoryhole.\n"
    "Currently NeoMutt only supports the Subject header.\n"
    "\n"
    "Encrypted messages using protected headers often substitute the exposed\n"
    "Subject header with a dummy value (see $$crypt_protected_headers_subject).\n"
    "NeoMutt will update its concept of the correct subject **after** the\n"
    "message is opened, i.e. via the `<display-message>` function.\n"
    "If you reply to a message before opening it, NeoMutt will end up using\n"
    "the dummy Subject header, so be sure to open such a message first.\n"
    "(Crypto only)\n"
  },
  /*
  ** .pp
  ** When set, NeoMutt will display protected headers ("Memory Hole") in the pager,
  ** and will update the index and header cache with revised headers.
  **
  ** Protected headers are stored inside the encrypted or signed part of an
  ** an email, to prevent disclosure or tampering.
  ** For more information see https://github.com/autocrypt/memoryhole.
  ** Currently NeoMutt only supports the Subject header.
  ** .pp
  ** Encrypted messages using protected headers often substitute the exposed
  ** Subject header with a dummy value (see $$crypt_protected_headers_subject).
  ** NeoMutt will update its concept of the correct subject \fBafter\fP the
  ** message is opened, i.e. via the \fC<display-message>\fP function.
  ** If you reply to a message before opening it, NeoMutt will end up using
  ** the dummy Subject header, so be sure to open such a message first.
  ** (Crypto only)
  */
  { "crypt_protected_headers_save", DT_BOOL, &C_CryptProtectedHeadersSave, false, 0, NULL,
    "When $$crypt_protected_headers_read is set, and a message with a\n"
    "protected Subject is opened, NeoMutt will save the updated Subject\n"
    "into the header cache by default.  This allows searching/limiting\n"
    "based on the protected Subject header if the mailbox is\n"
    "re-opened, without having to re-open the message each time.\n"
    "However, for mbox/mh mailbox types, or if header caching is not\n"
    "set up, you would need to re-open the message each time the\n"
    "mailbox was reopened before you could see or search/limit on the\n"
    "protected subject again.\n"
    "\n"
    "When this variable is set, NeoMutt additionally saves the protected\n"
    "Subject back **in the clear-text message headers**.  This\n"
    "provides better usability, but with the tradeoff of reduced\n"
    "security.  The protected Subject header, which may have\n"
    "previously been encrypted, is now stored in clear-text in the\n"
    "message headers.  Copying the message elsewhere, via NeoMutt or\n"
    "external tools, could expose this previously encrypted data.\n"
    "Please make sure you understand the consequences of this before\n"
    "you enable this variable.\n"
    "(Crypto only)\n"
  },
  /*
  ** .pp
  ** When $$crypt_protected_headers_read is set, and a message with a
  ** protected Subject is opened, NeoMutt will save the updated Subject
  ** into the header cache by default.  This allows searching/limiting
  ** based on the protected Subject header if the mailbox is
  ** re-opened, without having to re-open the message each time.
  ** However, for mbox/mh mailbox types, or if header caching is not
  ** set up, you would need to re-open the message each time the
  ** mailbox was reopened before you could see or search/limit on the
  ** protected subject again.
  ** .pp
  ** When this variable is set, NeoMutt additionally saves the protected
  ** Subject back \fBin the clear-text message headers\fP.  This
  ** provides better usability, but with the tradeoff of reduced
  ** security.  The protected Subject header, which may have
  ** previously been encrypted, is now stored in clear-text in the
  ** message headers.  Copying the message elsewhere, via NeoMutt or
  ** external tools, could expose this previously encrypted data.
  ** Please make sure you understand the consequences of this before
  ** you enable this variable.
  ** (Crypto only)
  */
  { "crypt_protected_headers_subject", DT_STRING, &C_CryptProtectedHeadersSubject, IP "Encrypted subject", 0, NULL,
    "When $$crypt_protected_headers_write is set, and the message is marked\n"
    "for encryption, this will be substituted into the Subject field in the\n"
    "message headers.\n"
    "**\n"
    "To prevent a subject from being substituted, unset this variable, or set it\n"
    "to the empty string.\n"
    "(Crypto only)\n"
  },
  /*
  ** .pp
  ** When $$crypt_protected_headers_write is set, and the message is marked
  ** for encryption, this will be substituted into the Subject field in the
  ** message headers.
  **
  ** To prevent a subject from being substituted, unset this variable, or set it
  ** to the empty string.
  ** (Crypto only)
  */
  { "crypt_protected_headers_write", DT_BOOL, &C_CryptProtectedHeadersWrite, false, 0, NULL,
    "When set, NeoMutt will generate protected headers (\"Memory Hole\") for\n"
    "signed and encrypted emails.\n"
    "**\n"
    "Protected headers are stored inside the encrypted or signed part of an\n"
    "an email, to prevent disclosure or tampering.\n"
    "For more information see https://github.com/autocrypt/memoryhole.\n"
    "**\n"
    "Currently NeoMutt only supports the Subject header.\n"
    "(Crypto only)\n"
  },
  /*
  ** .pp
  ** When set, NeoMutt will generate protected headers ("Memory Hole") for
  ** signed and encrypted emails.
  **
  ** Protected headers are stored inside the encrypted or signed part of an
  ** an email, to prevent disclosure or tampering.
  ** For more information see https://github.com/autocrypt/memoryhole.
  **
  ** Currently NeoMutt only supports the Subject header.
  ** (Crypto only)
  */
  { "crypt_replyencrypt", DT_BOOL, &C_CryptReplyencrypt, true, 0, NULL,
    "If _set_, automatically PGP or OpenSSL encrypt replies to messages which are\n"
    "encrypted.\n"
    "(Crypto only)\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, automatically PGP or OpenSSL encrypt replies to messages which are
  ** encrypted.
  ** (Crypto only)
  */
  { "crypt_replysign", DT_BOOL, &C_CryptReplysign, false, 0, NULL,
    "If _set_, automatically PGP or OpenSSL sign replies to messages which are\n"
    "signed.\n"
    "\n"
    "**Note:** this does not work on messages that are encrypted\n"
    "_and_ signed!\n"
    "(Crypto only)\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, automatically PGP or OpenSSL sign replies to messages which are
  ** signed.
  ** .pp
  ** \fBNote:\fP this does not work on messages that are encrypted
  ** \fIand\fP signed!
  ** (Crypto only)
  */
  { "crypt_replysignencrypted", DT_BOOL, &C_CryptReplysignencrypted, false, 0, NULL,
    "If _set_, automatically PGP or OpenSSL sign replies to messages\n"
    "which are encrypted. This makes sense in combination with\n"
    "$$crypt_replyencrypt, because it allows you to sign all\n"
    "messages which are automatically encrypted.  This works around\n"
    "the problem noted in $$crypt_replysign, that NeoMutt is not able\n"
    "to find out whether an encrypted message is also signed.\n"
    "(Crypto only)\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, automatically PGP or OpenSSL sign replies to messages
  ** which are encrypted. This makes sense in combination with
  ** $$crypt_replyencrypt, because it allows you to sign all
  ** messages which are automatically encrypted.  This works around
  ** the problem noted in $$crypt_replysign, that NeoMutt is not able
  ** to find out whether an encrypted message is also signed.
  ** (Crypto only)
  */
  { "crypt_timestamp", DT_BOOL, &C_CryptTimestamp, true, 0, NULL,
    "If _set_, NeoMutt will include a time stamp in the lines surrounding\n"
    "PGP or S/MIME output, so spoofing such lines is more difficult.\n"
    "If you are using colors to mark these lines, and rely on these,\n"
    "you may _unset_ this setting.\n"
    "(Crypto only)\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, NeoMutt will include a time stamp in the lines surrounding
  ** PGP or S/MIME output, so spoofing such lines is more difficult.
  ** If you are using colors to mark these lines, and rely on these,
  ** you may \fIunset\fP this setting.
  ** (Crypto only)
  */
  { "crypt_use_gpgme", DT_BOOL, &C_CryptUseGpgme, false, 0, NULL,
    "This variable controls the use of the GPGME-enabled crypto backends.\n"
    "If it is _set_ and NeoMutt was built with gpgme support, the gpgme code for\n"
    "S/MIME and PGP will be used instead of the classic code.  Note that\n"
    "you need to set this option in .neomuttrc; it won't have any effect when\n"
    "used interactively.\n"
    "\n"
    "Note that the GPGME backend does not support creating old-style inline\n"
    "(traditional) PGP encrypted or signed messages (see $$pgp_autoinline).\n"
  },
  /*
  ** .pp
  ** This variable controls the use of the GPGME-enabled crypto backends.
  ** If it is \fIset\fP and NeoMutt was built with gpgme support, the gpgme code for
  ** S/MIME and PGP will be used instead of the classic code.  Note that
  ** you need to set this option in .neomuttrc; it won't have any effect when
  ** used interactively.
  ** .pp
  ** Note that the GPGME backend does not support creating old-style inline
  ** (traditional) PGP encrypted or signed messages (see $$pgp_autoinline).
  */
  { "crypt_use_pka", DT_BOOL, &C_CryptUsePka, false, 0, NULL,
    "Controls whether NeoMutt uses PKA\n"
    "(see http://www.g10code.de/docs/pka-intro.de.pdf) during signature\n"
    "verification (only supported by the GPGME backend).\n"
  },
  /*
  ** .pp
  ** Controls whether NeoMutt uses PKA
  ** (see http://www.g10code.de/docs/pka-intro.de.pdf) during signature
  ** verification (only supported by the GPGME backend).
  */
  { "crypt_verify_sig", DT_QUAD, &C_CryptVerifySig, MUTT_YES, 0, NULL,
    "If \"_yes_\", always attempt to verify PGP or S/MIME signatures.\n"
    "If \"_ask-*_\", ask whether or not to verify the signature.\n"
    "If \"_no_\", never attempt to verify cryptographic signatures.\n"
    "(Crypto only)\n"
  },
  /*
  ** .pp
  ** If \fI"yes"\fP, always attempt to verify PGP or S/MIME signatures.
  ** If \fI"ask-*"\fP, ask whether or not to verify the signature.
  ** If \fI"no"\fP, never attempt to verify cryptographic signatures.
  ** (Crypto only)
  */
  { "date_format", DT_STRING|DT_NOT_EMPTY|R_MENU, &C_DateFormat, IP "!%a, %b %d, %Y at %I:%M:%S%p %Z", 0, NULL,
    "This variable controls the format of the date printed by the \"%d\"\n"
    "sequence in $$index_format.  This is passed to the `strftime(3)`\n"
    "function to process the date, see the man page for the proper syntax.\n"
    "\n"
    "Unless the first character in the string is a bang (\"!\"), the month\n"
    "and week day names are expanded according to the locale.\n"
    "If the first character in the string is a\n"
    "bang, the bang is discarded, and the month and week day names in the\n"
    "rest of the string are expanded in the _C_ locale (that is in US\n"
    "English).\n"
  },
  /*
  ** .pp
  ** This variable controls the format of the date printed by the "%d"
  ** sequence in $$index_format.  This is passed to the \fCstrftime(3)\fP
  ** function to process the date, see the man page for the proper syntax.
  ** .pp
  ** Unless the first character in the string is a bang ("!"), the month
  ** and week day names are expanded according to the locale.
  ** If the first character in the string is a
  ** bang, the bang is discarded, and the month and week day names in the
  ** rest of the string are expanded in the \fIC\fP locale (that is in US
  ** English).
  */
  { "debug_file", DT_STRING|DT_PATH, &C_DebugFile, IP "~/.neomuttdebug", 0, NULL,
    "Debug logging is controlled by the variables `$$debug_file` and `$$debug_level`.\n"
    "`$$debug_file` specifies the root of the filename.  NeoMutt will add \"0\" to the end.\n"
    "Each time NeoMutt is run with logging enabled, the log files are rotated.\n"
    "A maximum of five log files are kept, numbered 0 (most recent) to 4 (oldest).\n"
    "\n"
    "This option can be enabled on the command line, \"neomutt -l mylog\"\n"
    "\n"
    "See also: `$$debug_level`\n"
  },
  /*
  ** .pp
  ** Debug logging is controlled by the variables \fC$$debug_file\fP and \fC$$debug_level\fP.
  ** \fC$$debug_file\fP specifies the root of the filename.  NeoMutt will add "0" to the end.
  ** Each time NeoMutt is run with logging enabled, the log files are rotated.
  ** A maximum of five log files are kept, numbered 0 (most recent) to 4 (oldest).
  ** .pp
  ** This option can be enabled on the command line, "neomutt -l mylog"
  ** .pp
  ** See also: \fC$$debug_level\fP
  */
  { "debug_level", DT_NUMBER, &C_DebugLevel, 0, 0, level_validator,
    "Debug logging is controlled by the variables `$$debug_file` and `$$debug_level`.\n"
    "\n"
    "The debug level controls how much information is saved to the log file.\n"
    "If you have a problem with NeoMutt, then enabling logging may help find the cause.\n"
    "Levels 1-3 will usually provide enough information for writing a bug report.\n"
    "Levels 4,5 will be extremely verbose.\n"
    "\n"
    "Warning: Logging at high levels may save private information to the file.\n"
    "\n"
    "This option can be enabled on the command line, \"neomutt -d 2\"\n"
    "\n"
    "See also: `$$debug_file`\n"
  },
  /*
  ** .pp
  ** Debug logging is controlled by the variables \fC$$debug_file\fP and \fC$$debug_level\fP.
  ** .pp
  ** The debug level controls how much information is saved to the log file.
  ** If you have a problem with NeoMutt, then enabling logging may help find the cause.
  ** Levels 1-3 will usually provide enough information for writing a bug report.
  ** Levels 4,5 will be extremely verbose.
  ** .pp
  ** Warning: Logging at high levels may save private information to the file.
  ** .pp
  ** This option can be enabled on the command line, "neomutt -d 2"
  ** .pp
  ** See also: \fC$$debug_file\fP
  */
  { "default_hook", DT_STRING, &C_DefaultHook, IP "~f %s !~P | (~P ~C %s)", 0, NULL,
    "This variable controls how \"$message-hook\", \"$reply-hook\", \"$send-hook\",\n"
    "\"$send2-hook\", \"$save-hook\", and \"$fcc-hook\" will\n"
    "be interpreted if they are specified with only a simple regex,\n"
    "instead of a matching pattern.  The hooks are expanded when they are\n"
    "declared, so a hook will be interpreted according to the value of this\n"
    "variable at the time the hook is declared.\n"
    "\n"
    "The default value matches\n"
    "if the message is either from a user matching the regular expression\n"
    "given, or if it is from you (if the from address matches\n"
    "\"$alternates\") and is to or cc'ed to a user matching the given\n"
    "regular expression.\n"
  },
  /*
  ** .pp
  ** This variable controls how "$message-hook", "$reply-hook", "$send-hook",
  ** "$send2-hook", "$save-hook", and "$fcc-hook" will
  ** be interpreted if they are specified with only a simple regex,
  ** instead of a matching pattern.  The hooks are expanded when they are
  ** declared, so a hook will be interpreted according to the value of this
  ** variable at the time the hook is declared.
  ** .pp
  ** The default value matches
  ** if the message is either from a user matching the regular expression
  ** given, or if it is from you (if the from address matches
  ** "$alternates") and is to or cc'ed to a user matching the given
  ** regular expression.
  */
  { "delete", DT_QUAD, &C_Delete, MUTT_ASKYES, 0, NULL,
    "Controls whether or not messages are really deleted when closing or\n"
    "synchronizing a mailbox.  If set to _yes_, messages marked for\n"
    "deleting will automatically be purged without prompting.  If set to\n"
    "_no_, messages marked for deletion will be kept in the mailbox.\n"
  },
  /*
  ** .pp
  ** Controls whether or not messages are really deleted when closing or
  ** synchronizing a mailbox.  If set to \fIyes\fP, messages marked for
  ** deleting will automatically be purged without prompting.  If set to
  ** \fIno\fP, messages marked for deletion will be kept in the mailbox.
  */
  { "delete_untag", DT_BOOL, &C_DeleteUntag, true, 0, NULL,
    "If this option is _set_, NeoMutt will untag messages when marking them\n"
    "for deletion.  This applies when you either explicitly delete a message,\n"
    "or when you save it to another folder.\n"
  },
  /*
  ** .pp
  ** If this option is \fIset\fP, NeoMutt will untag messages when marking them
  ** for deletion.  This applies when you either explicitly delete a message,
  ** or when you save it to another folder.
  */
  { "digest_collapse", DT_BOOL, &C_DigestCollapse, true, 0, NULL,
    "If this option is _set_, NeoMutt's received-attachments menu will not show the subparts of\n"
    "individual messages in a multipart/digest.  To see these subparts, press \"v\" on that menu.\n"
  },
  /*
  ** .pp
  ** If this option is \fIset\fP, NeoMutt's received-attachments menu will not show the subparts of
  ** individual messages in a multipart/digest.  To see these subparts, press "v" on that menu.
  */
  { "display_filter", DT_STRING|DT_COMMAND|R_PAGER, &C_DisplayFilter, 0, 0, NULL,
    "When set, specifies a command used to filter messages.  When a message\n"
    "is viewed it is passed as standard input to $$display_filter, and the\n"
    "filtered message is read from the standard output.\n"
    "\n"
    "When preparing the message, NeoMutt inserts some escape sequences into the\n"
    "text.  They are of the form: `<esc>]9;XXX<bel>` where \"XXX\" is a random\n"
    "64-bit number.\n"
    "\n"
    "If these escape sequences interfere with your filter, they can be removed\n"
    "using a tool like `ansifilter` or `sed 's/^\x1b]9;[0-9]\\+\x7//'`\n"
    "\n"
    "If they are removed, then PGP and MIME headers will no longer be coloured.\n"
    "This can be fixed by adding this to your config:\n"
    "`color body magenta default '^\\[-- .* --\\]$$$'`.\n"
  },
  /*
  ** .pp
  ** When set, specifies a command used to filter messages.  When a message
  ** is viewed it is passed as standard input to $$display_filter, and the
  ** filtered message is read from the standard output.
  ** .pp
  ** When preparing the message, NeoMutt inserts some escape sequences into the
  ** text.  They are of the form: \fC<esc>]9;XXX<bel>\fP where "XXX" is a random
  ** 64-bit number.
  ** .pp
  ** If these escape sequences interfere with your filter, they can be removed
  ** using a tool like \fCansifilter\fP or \fCsed 's/^\x1b]9;[0-9]\+\x7//'\fP
  ** .pp
  ** If they are removed, then PGP and MIME headers will no longer be coloured.
  ** This can be fixed by adding this to your config:
  ** \fCcolor body magenta default '^\[-- .* --\]$$$'\fP.
  */
  { "dsn_notify", DT_STRING, &C_DsnNotify, 0, 0, NULL,
    "This variable sets the request for when notification is returned.  The\n"
    "string consists of a comma separated list (no spaces!) of one or more\n"
    "of the following: _never_, to never request notification,\n"
    "_failure_, to request notification on transmission failure,\n"
    "_delay_, to be notified of message delays, _success_, to be\n"
    "notified of successful transmission.\n"
    "\n"
    "Example:\n"
    "```\n"
    "set dsn_notify=\"failure,delay\"\n"
    "```\n"
    "\n"
    "**Note:** when using $$sendmail for delivery, you should not enable\n"
    "this unless you are either using Sendmail 8.8.x or greater or a MTA\n"
    "providing a `sendmail(1)`-compatible interface supporting the `-N` option\n"
    "for DSN. For SMTP delivery, DSN support is auto-detected so that it\n"
    "depends on the server whether DSN will be used or not.\n"
  },
  /*
  ** .pp
  ** This variable sets the request for when notification is returned.  The
  ** string consists of a comma separated list (no spaces!) of one or more
  ** of the following: \fInever\fP, to never request notification,
  ** \fIfailure\fP, to request notification on transmission failure,
  ** \fIdelay\fP, to be notified of message delays, \fIsuccess\fP, to be
  ** notified of successful transmission.
  ** .pp
  ** Example:
  ** .ts
  ** set dsn_notify="failure,delay"
  ** .te
  ** .pp
  ** \fBNote:\fP when using $$sendmail for delivery, you should not enable
  ** this unless you are either using Sendmail 8.8.x or greater or a MTA
  ** providing a \fCsendmail(1)\fP-compatible interface supporting the \fC-N\fP option
  ** for DSN. For SMTP delivery, DSN support is auto-detected so that it
  ** depends on the server whether DSN will be used or not.
  */
  { "dsn_return", DT_STRING, &C_DsnReturn, 0, 0, NULL,
    "This variable controls how much of your message is returned in DSN\n"
    "messages.  It may be set to either _hdrs_ to return just the\n"
    "message header, or _full_ to return the full message.\n"
    "\n"
    "Example:\n"
    "```\n"
    "set dsn_return=hdrs\n"
    "```\n"
    "\n"
    "**Note:** when using $$sendmail for delivery, you should not enable\n"
    "this unless you are either using Sendmail 8.8.x or greater or a MTA\n"
    "providing a `sendmail(1)`-compatible interface supporting the `-R` option\n"
    "for DSN. For SMTP delivery, DSN support is auto-detected so that it\n"
    "depends on the server whether DSN will be used or not.\n"
  },
  /*
  ** .pp
  ** This variable controls how much of your message is returned in DSN
  ** messages.  It may be set to either \fIhdrs\fP to return just the
  ** message header, or \fIfull\fP to return the full message.
  ** .pp
  ** Example:
  ** .ts
  ** set dsn_return=hdrs
  ** .te
  ** .pp
  ** \fBNote:\fP when using $$sendmail for delivery, you should not enable
  ** this unless you are either using Sendmail 8.8.x or greater or a MTA
  ** providing a \fCsendmail(1)\fP-compatible interface supporting the \fC-R\fP option
  ** for DSN. For SMTP delivery, DSN support is auto-detected so that it
  ** depends on the server whether DSN will be used or not.
  */
  { "duplicate_threads", DT_BOOL|R_RESORT|R_RESORT_INIT|R_INDEX, &C_DuplicateThreads, true, 0, pager_validator,
    "This variable controls whether NeoMutt, when $$sort is set to _threads_, threads\n"
    "messages with the same Message-Id together.  If it is _set_, it will indicate\n"
    "that it thinks they are duplicates of each other with an equals sign\n"
    "in the thread tree.\n"
  },
  /*
  ** .pp
  ** This variable controls whether NeoMutt, when $$sort is set to \fIthreads\fP, threads
  ** messages with the same Message-Id together.  If it is \fIset\fP, it will indicate
  ** that it thinks they are duplicates of each other with an equals sign
  ** in the thread tree.
  */
  { "edit_headers", DT_BOOL, &C_EditHeaders, false, 0, NULL,
    "This option allows you to edit the header of your outgoing messages\n"
    "along with the body of your message.\n"
    "\n"
    "Although the compose menu may have localized header labels, the\n"
    "labels passed to your editor will be standard RFC2822 headers,\n"
    "(e.g. To:, Cc:, Subject:).  Headers added in your editor must\n"
    "also be RFC2822 headers, or one of the pseudo headers listed in\n"
    "\"$edit-header\".  NeoMutt will not understand localized header\n"
    "labels, just as it would not when parsing an actual email.\n"
    "\n"
    "**Note** that changes made to the References: and Date: headers are\n"
    "ignored for interoperability reasons.\n"
  },
  /*
  ** .pp
  ** This option allows you to edit the header of your outgoing messages
  ** along with the body of your message.
  ** .pp
  ** Although the compose menu may have localized header labels, the
  ** labels passed to your editor will be standard RFC2822 headers,
  ** (e.g. To:, Cc:, Subject:).  Headers added in your editor must
  ** also be RFC2822 headers, or one of the pseudo headers listed in
  ** "$edit-header".  NeoMutt will not understand localized header
  ** labels, just as it would not when parsing an actual email.
  ** .pp
  ** \fBNote\fP that changes made to the References: and Date: headers are
  ** ignored for interoperability reasons.
  */
  { "editor", DT_STRING|DT_COMMAND, &C_Editor, IP "vi", 0, NULL,
    "This variable specifies which editor is used by NeoMutt.\n"
    "It defaults to the value of the `$$$VISUAL`, or `$$$EDITOR`, environment\n"
    "variable, or to the string \"vi\" if neither of those are set.\n"
    "\n"
    "The `$$editor` string may contain a _%s_ escape, which will be replaced by the name\n"
    "of the file to be edited.  If the _%s_ escape does not appear in `$$editor`, a\n"
    "space and the name to be edited are appended.\n"
    "\n"
    "The resulting string is then executed by running\n"
    "```\n"
    "sh -c 'string'\n"
    "```\n"
    "\n"
    "where _string_ is the expansion of `$$editor` described above.\n"
  },
  /*
  ** .pp
  ** This variable specifies which editor is used by NeoMutt.
  ** It defaults to the value of the \fC$$$VISUAL\fP, or \fC$$$EDITOR\fP, environment
  ** variable, or to the string "vi" if neither of those are set.
  ** .pp
  ** The \fC$$editor\fP string may contain a \fI%s\fP escape, which will be replaced by the name
  ** of the file to be edited.  If the \fI%s\fP escape does not appear in \fC$$editor\fP, a
  ** space and the name to be edited are appended.
  ** .pp
  ** The resulting string is then executed by running
  ** .ts
  ** sh -c 'string'
  ** .te
  ** .pp
  ** where \fIstring\fP is the expansion of \fC$$editor\fP described above.
  */
  { "empty_subject", DT_STRING, &C_EmptySubject, IP "Re: your mail", 0, NULL,
    "This variable specifies the subject to be used when replying to an email\n"
    "with an empty subject.  It defaults to \"Re: your mail\".\n"
  },
  /*
  ** .pp
  ** This variable specifies the subject to be used when replying to an email
  ** with an empty subject.  It defaults to "Re: your mail".
  */
  { "encode_from", DT_BOOL, &C_EncodeFrom, false, 0, NULL,
    "When _set_, NeoMutt will quoted-printable encode messages when\n"
    "they contain the string \"From \" (note the trailing space) in the beginning of a line.\n"
    "This is useful to avoid the tampering certain mail delivery and transport\n"
    "agents tend to do with messages (in order to prevent tools from\n"
    "misinterpreting the line as a mbox message separator).\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will quoted-printable encode messages when
  ** they contain the string "From " (note the trailing space) in the beginning of a line.
  ** This is useful to avoid the tampering certain mail delivery and transport
  ** agents tend to do with messages (in order to prevent tools from
  ** misinterpreting the line as a mbox message separator).
  */
#ifdef USE_SSL_OPENSSL
  { "entropy_file", DT_STRING|DT_PATH, &C_EntropyFile, 0, 0, NULL,
    "The file which includes random data that is used to initialize SSL\n"
    "library functions.\n"
  },
  /*
  ** .pp
  ** The file which includes random data that is used to initialize SSL
  ** library functions.
  */
#endif
  { "envelope_from_address", DT_ADDRESS, &C_EnvelopeFromAddress, 0, 0, NULL,
    "Manually sets the _envelope_ sender for outgoing messages.\n"
    "This value is ignored if $$use_envelope_from is _unset_.\n"
  },
  /*
  ** .pp
  ** Manually sets the \fIenvelope\fP sender for outgoing messages.
  ** This value is ignored if $$use_envelope_from is \fIunset\fP.
  */
  { "escape", DT_STRING, &C_Escape, IP "~", 0, NULL,
    "Escape character to use for functions in the built-in editor.\n"
  },
  /*
  ** .pp
  ** Escape character to use for functions in the built-in editor.
  */
  { "external_search_command", DT_STRING|DT_COMMAND, &C_ExternalSearchCommand, 0, 0, NULL,
    "If set, contains the name of the external program used by \"~I\" patterns.\n"
    "This will usually be a wrapper script around mairix, mu, or similar\n"
    "indexers other than notmuch (for which there is optional special support).\n"
    "\n"
    "Here is an example how it works.  Let's assume $$external_search_command\n"
    "is set to \"mairix_filter\", and mairix_filter is a script which\n"
    "runs the old but well loved mairix indexer with the arguments\n"
    "given to mairix_filter, in the \"raw\" mode of mairix, producing\n"
    "on the standard output a list of Message-IDs, one per line.\n"
    "\n"
    "If possible, it also filters down the results coming from mairix\n"
    "such that only messages in the current folder remain.  It can do\n"
    "this because it gets a hidden first argument which is the path\n"
    "to the folder.\n"
    "(This can be the type of clean and simple script called a _one-liner_.)\n"
    "\n"
    "Now if NeoMutt gets a limit or tag command followed by the pattern\n"
    "\"~I '-t s:bleeping='\", mairix_filter runs mairix with the\n"
    "arguments from inside the quotes (the quotes are needed because\n"
    "of the space after \"-t\"), mairix finds all messages with\n"
    "\"bleeping\" in the Subject plus all messages sharing threads\n"
    "with these and outputs their file names, and mairix_filter\n"
    "translates the file names into Message-IDs.  Finally, NeoMutt\n"
    "reads the Message-IDs and targets the matching messages with the\n"
    "command given to it.\n"
    "\n"
    "You, the user, still have to rewrite the mairix_filter script to\n"
    "match the behavior of your indexer, but this should help users\n"
    "of indexers other than notmuch to integrate them cleanly with NeoMutt.\n"
  },
  /*
  ** .pp
  ** If set, contains the name of the external program used by "~I" patterns.
  ** This will usually be a wrapper script around mairix, mu, or similar
  ** indexers other than notmuch (for which there is optional special support).
  ** .pp
  ** Here is an example how it works.  Let's assume $$external_search_command
  ** is set to "mairix_filter", and mairix_filter is a script which
  ** runs the old but well loved mairix indexer with the arguments
  ** given to mairix_filter, in the "raw" mode of mairix, producing
  ** on the standard output a list of Message-IDs, one per line.
  ** .pp
  ** If possible, it also filters down the results coming from mairix
  ** such that only messages in the current folder remain.  It can do
  ** this because it gets a hidden first argument which is the path
  ** to the folder.
  ** (This can be the type of clean and simple script called a \fIone-liner\fP.)
  ** .pp
  ** Now if NeoMutt gets a limit or tag command followed by the pattern
  ** "~I '-t s:bleeping='", mairix_filter runs mairix with the
  ** arguments from inside the quotes (the quotes are needed because
  ** of the space after "-t"), mairix finds all messages with
  ** "bleeping" in the Subject plus all messages sharing threads
  ** with these and outputs their file names, and mairix_filter
  ** translates the file names into Message-IDs.  Finally, NeoMutt
  ** reads the Message-IDs and targets the matching messages with the
  ** command given to it.
  ** .pp
  ** You, the user, still have to rewrite the mairix_filter script to
  ** match the behavior of your indexer, but this should help users
  ** of indexers other than notmuch to integrate them cleanly with NeoMutt.
  */
  { "fast_reply", DT_BOOL, &C_FastReply, false, 0, NULL,
    "When _set_, the initial prompt for recipients and subject are skipped\n"
    "when replying to messages, and the initial prompt for subject is\n"
    "skipped when forwarding messages.\n"
    "\n"
    "**Note:** this variable has no effect when the $$autoedit\n"
    "variable is _set_.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, the initial prompt for recipients and subject are skipped
  ** when replying to messages, and the initial prompt for subject is
  ** skipped when forwarding messages.
  ** .pp
  ** \fBNote:\fP this variable has no effect when the $$autoedit
  ** variable is \fIset\fP.
  */
  { "fcc_attach", DT_QUAD, &C_FccAttach, MUTT_YES, 0, NULL,
    "This variable controls whether or not attachments on outgoing messages\n"
    "are saved along with the main body of your message.\n"
  },
  /*
  ** .pp
  ** This variable controls whether or not attachments on outgoing messages
  ** are saved along with the main body of your message.
  */
  { "fcc_before_send", DT_BOOL, &C_FccBeforeSend, false, 0, NULL,
    "When this variable is _set_, FCCs will occur before sending\n"
    "the message.  Before sending, the message cannot be manipulated,\n"
    "so it will be stored the exact same as sent:\n"
    "$$fcc_attach and $$fcc_clear will be ignored (using their default\n"
    "values).\n"
    "\n"
    "When _unset_, the default, FCCs will occur after sending.\n"
    "Variables $$fcc_attach and $$fcc_clear will be respected, allowing\n"
    "it to be stored without attachments or encryption/signing if\n"
    "desired.\n"
  },
  /*
  ** .pp
  ** When this variable is \fIset\fP, FCCs will occur before sending
  ** the message.  Before sending, the message cannot be manipulated,
  ** so it will be stored the exact same as sent:
  ** $$fcc_attach and $$fcc_clear will be ignored (using their default
  ** values).
  ** .pp
  ** When \fIunset\fP, the default, FCCs will occur after sending.
  ** Variables $$fcc_attach and $$fcc_clear will be respected, allowing
  ** it to be stored without attachments or encryption/signing if
  ** desired.
  */
  { "fcc_clear", DT_BOOL, &C_FccClear, false, 0, NULL,
    "When this variable is _set_, FCCs will be stored unencrypted and\n"
    "unsigned, even when the actual message is encrypted and/or\n"
    "signed.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** When this variable is \fIset\fP, FCCs will be stored unencrypted and
  ** unsigned, even when the actual message is encrypted and/or
  ** signed.
  ** (PGP only)
  */
  { "flag_chars", DT_MBTABLE|R_INDEX|R_PAGER, &C_FlagChars, IP "*!DdrONon- ", 0, NULL,
    "Controls the characters used in several flags.\n"
    "\n"
    "| Character | Default | Description |\n"
    "|-----------+---------+-----------------------------------------------|\n"
    "| 1         | *       | The mail is tagged. |\n"
    "| 2         | !       | The mail is flagged as important. |\n"
    "| 3         | D       | The mail is marked for deletion. |\n"
    "| 4         | d       | The mail has attachments marked for deletion. |\n"
    "| 5         | r       | The mail has been replied to. |\n"
    "| 6         | O       | The mail is Old (Unread but seen). |\n"
    "| 7         | N       | The mail is New (Unread but not seen). |\n"
    "| 8         | o       | The mail thread is Old (Unread but seen). |\n"
    "| 9         | n       | The mail thread is New (Unread but not seen). |\n"
    "| 10        | -       | The mail is read - %S expando. |\n"
    "| 11        | <space> | The mail is read - %Z expando. |\n"
  },
  /*
  ** .pp
  ** Controls the characters used in several flags.
  ** .dl
  ** .dt \fBCharacter\fP .dd \fBDefault\fP .dd \fBDescription\fP
  ** .dt 1 .dd * .dd The mail is tagged.
  ** .dt 2 .dd ! .dd The mail is flagged as important.
  ** .dt 3 .dd D .dd The mail is marked for deletion.
  ** .dt 4 .dd d .dd The mail has attachments marked for deletion.
  ** .dt 5 .dd r .dd The mail has been replied to.
  ** .dt 6 .dd O .dd The mail is Old (Unread but seen).
  ** .dt 7 .dd N .dd The mail is New (Unread but not seen).
  ** .dt 8 .dd o .dd The mail thread is Old (Unread but seen).
  ** .dt 9 .dd n .dd The mail thread is New (Unread but not seen).
  ** .dt 10 .dd - .dd The mail is read - %S expando.
  ** .dt 11 .dd <space> .dd The mail is read - %Z expando.
  ** .de
  */
  { "flag_safe", DT_BOOL, &C_FlagSafe, false, 0, NULL,
    "If set, flagged messages can't be deleted.\n"
  },
  /*
  ** .pp
  ** If set, flagged messages can't be deleted.
  */
  { "folder", DT_STRING|DT_PATH|DT_MAILBOX, &C_Folder, IP "~/Mail", 0, NULL,
    "Specifies the default location of your mailboxes.  A \"+\" or \"=\" at the\n"
    "beginning of a pathname will be expanded to the value of this\n"
    "variable.  Note that if you change this variable (from the default)\n"
    "value you need to make sure that the assignment occurs _before_\n"
    "you use \"+\" or \"=\" for any other variables since expansion takes place\n"
    "when handling the \"$mailboxes\" command.\n"
  },
  /*
  ** .pp
  ** Specifies the default location of your mailboxes.  A "+" or "=" at the
  ** beginning of a pathname will be expanded to the value of this
  ** variable.  Note that if you change this variable (from the default)
  ** value you need to make sure that the assignment occurs \fIbefore\fP
  ** you use "+" or "=" for any other variables since expansion takes place
  ** when handling the "$mailboxes" command.
  */
  { "folder_format", DT_STRING|DT_NOT_EMPTY|R_MENU, &C_FolderFormat, IP "%2C %t %N %F %2l %-8.8u %-8.8g %8s %d %i", 0, NULL,
    "This variable allows you to customize the file browser display to your\n"
    "personal taste.  This string is similar to $$index_format, but has\n"
    "its own set of `printf(3)`-like sequences:\n"
    "\n"
    "| .   | Description |\n"
    "|-----+--------------------------------------------------------------------------------------------------|\n"
    "| %C  | Current file number |\n"
    "| %d  | Date/time folder was last modified |\n"
    "| %D  | Date/time folder was last modified using $$date_format. |\n"
    "| %f  | Filename (\"/\" is appended to directory names, \"@\" to symbolic links and \"*\" to executable files) |\n"
    "| %F  | File permissions |\n"
    "| %g  | Group name (or numeric gid, if missing) |\n"
    "| %i  | Description of the folder |\n"
    "| %l  | Number of hard links |\n"
    "| %m  | Number of messages in the mailbox * |\n"
    "| %n  | Number of unread messages in the mailbox * |\n"
    "| %N  | \"N\" if mailbox has new mail, blank otherwise |\n"
    "| %s  | Size in bytes (see $formatstrings-size) |\n"
    "| %t  | \"*\" if the file is tagged, blank otherwise |\n"
    "| %u  | Owner name (or numeric uid, if missing) |\n"
    "| %>X | Right justify the rest of the string and pad with character \"X\" |\n"
    "| %|X | Pad to the end of the line with character \"X\" |\n"
    "| %*X | Soft-fill with character \"X\" as pad |\n"
    "\n"
    "For an explanation of \"soft-fill\", see the $$index_format documentation.\n"
    "\n"
    "* = can be optionally printed if nonzero\n"
    "\n"
    "%m, %n, and %N only work for monitored mailboxes.\n"
    "%m requires $$mail_check_stats to be set.\n"
    "%n requires $$mail_check_stats to be set (except for IMAP mailboxes).\n"
  },
  /*
  ** .pp
  ** This variable allows you to customize the file browser display to your
  ** personal taste.  This string is similar to $$index_format, but has
  ** its own set of \fCprintf(3)\fP-like sequences:
  ** .dl
  ** .dt %C  .dd   .dd Current file number
  ** .dt %d  .dd   .dd Date/time folder was last modified
  ** .dt %D  .dd   .dd Date/time folder was last modified using $$date_format.
  ** .dt %f  .dd   .dd Filename ("/" is appended to directory names,
  **                   "@" to symbolic links and "*" to executable files)
  ** .dt %F  .dd   .dd File permissions
  ** .dt %g  .dd   .dd Group name (or numeric gid, if missing)
  ** .dt %i  .dd   .dd Description of the folder
  ** .dt %l  .dd   .dd Number of hard links
  ** .dt %m  .dd * .dd Number of messages in the mailbox
  ** .dt %n  .dd * .dd Number of unread messages in the mailbox
  ** .dt %N  .dd   .dd "N" if mailbox has new mail, blank otherwise
  ** .dt %s  .dd   .dd Size in bytes (see $formatstrings-size)
  ** .dt %t  .dd   .dd "*" if the file is tagged, blank otherwise
  ** .dt %u  .dd   .dd Owner name (or numeric uid, if missing)
  ** .dt %>X .dd   .dd Right justify the rest of the string and pad with character "X"
  ** .dt %|X .dd   .dd Pad to the end of the line with character "X"
  ** .dt %*X .dd   .dd Soft-fill with character "X" as pad
  ** .de
  ** .pp
  ** For an explanation of "soft-fill", see the $$index_format documentation.
  ** .pp
  ** * = can be optionally printed if nonzero
  ** .pp
  ** %m, %n, and %N only work for monitored mailboxes.
  ** %m requires $$mail_check_stats to be set.
  ** %n requires $$mail_check_stats to be set (except for IMAP mailboxes).
  */
  { "followup_to", DT_BOOL, &C_FollowupTo, true, 0, NULL,
    "Controls whether or not the \"Mail-Followup-To:\" header field is\n"
    "generated when sending mail.  When _set_, NeoMutt will generate this\n"
    "field when you are replying to a known mailing list, specified with\n"
    "the \"$subscribe\" or \"$lists\" commands.\n"
    "\n"
    "This field has two purposes.  First, preventing you from\n"
    "receiving duplicate copies of replies to messages which you send\n"
    "to mailing lists, and second, ensuring that you do get a reply\n"
    "separately for any messages sent to known lists to which you are\n"
    "not subscribed.\n"
    "\n"
    "The header will contain only the list's address\n"
    "for subscribed lists, and both the list address and your own\n"
    "email address for unsubscribed lists.  Without this header, a\n"
    "group reply to your message sent to a subscribed list will be\n"
    "sent to both the list and your address, resulting in two copies\n"
    "of the same email for you.\n"
  },
  /*
  ** .pp
  ** Controls whether or not the "Mail-Followup-To:" header field is
  ** generated when sending mail.  When \fIset\fP, NeoMutt will generate this
  ** field when you are replying to a known mailing list, specified with
  ** the "$subscribe" or "$lists" commands.
  ** .pp
  ** This field has two purposes.  First, preventing you from
  ** receiving duplicate copies of replies to messages which you send
  ** to mailing lists, and second, ensuring that you do get a reply
  ** separately for any messages sent to known lists to which you are
  ** not subscribed.
  ** .pp
  ** The header will contain only the list's address
  ** for subscribed lists, and both the list address and your own
  ** email address for unsubscribed lists.  Without this header, a
  ** group reply to your message sent to a subscribed list will be
  ** sent to both the list and your address, resulting in two copies
  ** of the same email for you.
  */
#ifdef USE_NNTP
  { "followup_to_poster", DT_QUAD, &C_FollowupToPoster, MUTT_ASKYES, 0, NULL,
    "If this variable is _set_ and the keyword \"poster\" is present in\n"
    "_Followup-To_ header, follow-up to newsgroup function is not\n"
    "permitted.  The message will be mailed to the submitter of the\n"
    "message via mail.\n"
  },
  /*
  ** .pp
  ** If this variable is \fIset\fP and the keyword "poster" is present in
  ** \fIFollowup-To\fP header, follow-up to newsgroup function is not
  ** permitted.  The message will be mailed to the submitter of the
  ** message via mail.
  */
#endif
  { "force_name", DT_BOOL, &C_ForceName, false, 0, NULL,
    "This variable is similar to $$save_name, except that NeoMutt will\n"
    "store a copy of your outgoing message by the username of the address\n"
    "you are sending to even if that mailbox does not exist.\n"
    "\n"
    "Also see the $$record variable.\n"
  },
  /*
  ** .pp
  ** This variable is similar to $$save_name, except that NeoMutt will
  ** store a copy of your outgoing message by the username of the address
  ** you are sending to even if that mailbox does not exist.
  ** .pp
  ** Also see the $$record variable.
  */
  { "forward_attachments", DT_QUAD, &C_ForwardAttachments, MUTT_ASKYES, 0, NULL,
    "When forwarding inline (i.e. $$mime_forward _unset_ or\n"
    "answered with ``no'' and $$forward_decode _set_), attachments\n"
    "which cannot be decoded in a reasonable manner will be attached\n"
    "to the newly composed message if this quadoption is _set_ or\n"
    "answered with ``yes''.\n"
  },
  /*
  ** .pp
  ** When forwarding inline (i.e. $$mime_forward \fIunset\fP or
  ** answered with ``no'' and $$forward_decode \fIset\fP), attachments
  ** which cannot be decoded in a reasonable manner will be attached
  ** to the newly composed message if this quadoption is \fIset\fP or
  ** answered with ``yes''.
  */
  { "forward_attribution_intro", DT_STRING, &C_ForwardAttributionIntro, IP "----- Forwarded message from %f -----", 0, NULL,
    "This is the string that will precede a message which has been forwarded\n"
    "in the main body of a message (when $$mime_forward is unset).\n"
    "For a full listing of defined `printf(3)`-like sequences see\n"
    "the section on $$index_format.  See also $$attribution_locale.\n"
  },
  /*
  ** .pp
  ** This is the string that will precede a message which has been forwarded
  ** in the main body of a message (when $$mime_forward is unset).
  ** For a full listing of defined \fCprintf(3)\fP-like sequences see
  ** the section on $$index_format.  See also $$attribution_locale.
  */
  { "forward_attribution_trailer", DT_STRING, &C_ForwardAttributionTrailer, IP "----- End forwarded message -----", 0, NULL,
    "This is the string that will follow a message which has been forwarded\n"
    "in the main body of a message (when $$mime_forward is unset).\n"
    "For a full listing of defined `printf(3)`-like sequences see\n"
    "the section on $$index_format.  See also $$attribution_locale.\n"
  },
  /*
  ** .pp
  ** This is the string that will follow a message which has been forwarded
  ** in the main body of a message (when $$mime_forward is unset).
  ** For a full listing of defined \fCprintf(3)\fP-like sequences see
  ** the section on $$index_format.  See also $$attribution_locale.
  */
  { "forward_decode", DT_BOOL, &C_ForwardDecode, true, 0, NULL,
    "Controls the decoding of complex MIME messages into `text/plain` when\n"
    "forwarding a message.  The message header is also RFC2047 decoded.\n"
    "This variable is only used, if $$mime_forward is _unset_,\n"
    "otherwise $$mime_forward_decode is used instead.\n"
  },
  /*
  ** .pp
  ** Controls the decoding of complex MIME messages into \fCtext/plain\fP when
  ** forwarding a message.  The message header is also RFC2047 decoded.
  ** This variable is only used, if $$mime_forward is \fIunset\fP,
  ** otherwise $$mime_forward_decode is used instead.
  */
  { "forward_decrypt", DT_BOOL, &C_ForwardDecrypt, true, 0, NULL,
    "Controls the handling of encrypted messages when forwarding a message.\n"
    "When _set_, the outer layer of encryption is stripped off.  This\n"
    "variable is only used if $$mime_forward is _set_ and\n"
    "$$mime_forward_decode is _unset_.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** Controls the handling of encrypted messages when forwarding a message.
  ** When \fIset\fP, the outer layer of encryption is stripped off.  This
  ** variable is only used if $$mime_forward is \fIset\fP and
  ** $$mime_forward_decode is \fIunset\fP.
  ** (PGP only)
  */
  { "forward_edit", DT_QUAD, &C_ForwardEdit, MUTT_YES, 0, NULL,
    "This quadoption controls whether or not the user is automatically\n"
    "placed in the editor when forwarding messages.  For those who always want\n"
    "to forward with no modification, use a setting of \"no\".\n"
  },
  /*
  ** .pp
  ** This quadoption controls whether or not the user is automatically
  ** placed in the editor when forwarding messages.  For those who always want
  ** to forward with no modification, use a setting of "no".
  */
  { "forward_format", DT_STRING|DT_NOT_EMPTY, &C_ForwardFormat, IP "[%a: %s]", 0, NULL,
    "This variable controls the default subject when forwarding a message.\n"
    "It uses the same format sequences as the $$index_format variable.\n"
  },
  /*
  ** .pp
  ** This variable controls the default subject when forwarding a message.
  ** It uses the same format sequences as the $$index_format variable.
  */
  { "forward_quote", DT_BOOL, &C_ForwardQuote, false, 0, NULL,
    "When _set_, forwarded messages included in the main body of the\n"
    "message (when $$mime_forward is _unset_) will be quoted using\n"
    "$$indent_string.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, forwarded messages included in the main body of the
  ** message (when $$mime_forward is \fIunset\fP) will be quoted using
  ** $$indent_string.
  */
  { "forward_references", DT_BOOL, &C_ForwardReferences, false, 0, NULL,
    "When _set_, forwarded messages set the \"In-Reply-To:\" and\n"
    "\"References:\" headers in the same way as normal replies would. Hence the\n"
    "forwarded message becomes part of the original thread instead of starting\n"
    "a new one.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, forwarded messages set the "In-Reply-To:" and
  ** "References:" headers in the same way as normal replies would. Hence the
  ** forwarded message becomes part of the original thread instead of starting
  ** a new one.
  */
  { "from", DT_ADDRESS, &C_From, 0, 0, NULL,
    "When _set_, this variable contains a default \"from\" address.  It\n"
    "can be overridden using \"$my_hdr\" (including from a \"$send-hook\") and\n"
    "$$reverse_name.  This variable is ignored if $$use_from is _unset_.\n"
    "\n"
    "If not specified, then it may be read from the environment variable `$$$EMAIL`.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, this variable contains a default "from" address.  It
  ** can be overridden using "$my_hdr" (including from a "$send-hook") and
  ** $$reverse_name.  This variable is ignored if $$use_from is \fIunset\fP.
  ** .pp
  ** If not specified, then it may be read from the environment variable \fC$$$EMAIL\fP.
  */
  { "from_chars", DT_MBTABLE|R_INDEX|R_PAGER, &C_FromChars, 0, 0, NULL,
    "Controls the character used to prefix the %F and %L fields in the\n"
    "index.\n"
    "\n"
    "| Character | Description |\n"
    "|-----------+---------------------------------------------------------------------------------------------|\n"
    "| 1         | Mail is written by you and has a To address, or has a known mailing list in the To address. |\n"
    "| 2         | Mail is written by you and has a Cc address, or has a known mailing list in the Cc address. |\n"
    "| 3         | Mail is written by you and has a Bcc address. |\n"
    "| 4         | All remaining cases. |\n"
    "\n"
    "If this is empty or unset (default), the traditional long \"To \",\n"
    "\"Cc \" and \"Bcc \" prefixes are used.  If set but too short to\n"
    "include a character for a particular case, a single space will be\n"
    "prepended to the field.  To prevent any prefix at all from being\n"
    "added in a particular case, use the special value CR (aka ^M)\n"
    "for the corresponding character.\n"
    "\n"
    "This slightly odd interface is necessitated by NeoMutt's handling of\n"
    "string variables; one can't tell a variable that is unset from one\n"
    "that is set to the empty string.\n"
  },
  /*
  ** .pp
  ** Controls the character used to prefix the %F and %L fields in the
  ** index.
  ** .dl
  ** .dt \fBCharacter\fP .dd \fBDescription\fP
  ** .dt 1 .dd Mail is written by you and has a To address, or has a known mailing list in the To address.
  ** .dt 2 .dd Mail is written by you and has a Cc address, or has a known mailing list in the Cc address.
  ** .dt 3 .dd Mail is written by you and has a Bcc address.
  ** .dt 4 .dd All remaining cases.
  ** .de
  ** .pp
  ** If this is empty or unset (default), the traditional long "To ",
  ** "Cc " and "Bcc " prefixes are used.  If set but too short to
  ** include a character for a particular case, a single space will be
  ** prepended to the field.  To prevent any prefix at all from being
  ** added in a particular case, use the special value CR (aka ^M)
  ** for the corresponding character.
  ** .pp
  ** This slightly odd interface is necessitated by NeoMutt's handling of
  ** string variables; one can't tell a variable that is unset from one
  ** that is set to the empty string.
  */
  { "gecos_mask", DT_REGEX, &C_GecosMask, IP "^[^,]*", 0, NULL,
    "A regular expression used by NeoMutt to parse the GECOS field of a password\n"
    "entry when expanding the alias.  The default value\n"
    "will return the string up to the first \",\" encountered.\n"
    "If the GECOS field contains a string like \"lastname, firstname\" then you\n"
    "should set it to \"`.*`\".\n"
    "\n"
    "This can be useful if you see the following behavior: you address an e-mail\n"
    "to user ID \"stevef\" whose full name is \"Steve Franklin\".  If NeoMutt expands\n"
    "\"stevef\" to '\"Franklin\" stevef@foo.bar' then you should set the $$gecos_mask to\n"
    "a regular expression that will match the whole name so NeoMutt will expand\n"
    "\"Franklin\" to \"Franklin, Steve\".\n"
  },
  /*
  ** .pp
  ** A regular expression used by NeoMutt to parse the GECOS field of a password
  ** entry when expanding the alias.  The default value
  ** will return the string up to the first "," encountered.
  ** If the GECOS field contains a string like "lastname, firstname" then you
  ** should set it to "\fC.*\fP".
  ** .pp
  ** This can be useful if you see the following behavior: you address an e-mail
  ** to user ID "stevef" whose full name is "Steve Franklin".  If NeoMutt expands
  ** "stevef" to '"Franklin" stevef@foo.bar' then you should set the $$gecos_mask to
  ** a regular expression that will match the whole name so NeoMutt will expand
  ** "Franklin" to "Franklin, Steve".
  */
#ifdef USE_NNTP
  { "group_index_format", DT_STRING|DT_NOT_EMPTY|R_INDEX|R_PAGER, &C_GroupIndexFormat, IP "%4C %M%N %5s  %-45.45f %d", 0, NULL,
    "This variable allows you to customize the newsgroup browser display to\n"
    "your personal taste.  This string is similar to \"$index_format\", but\n"
    "has its own set of printf()-like sequences:\n"
    "\n"
    "| .   | Description |\n"
    "|-----+--------------------------------------------------------------------|\n"
    "| %C  | Current newsgroup number |\n"
    "| %d  | Description of newsgroup (becomes from server) |\n"
    "| %f  | Newsgroup name |\n"
    "| %M  | - if newsgroup not allowed for direct post (moderated for example) |\n"
    "| %N  | N if newsgroup is new, u if unsubscribed, blank otherwise |\n"
    "| %n  | Number of new articles in newsgroup |\n"
    "| %s  | Number of unread articles in newsgroup |\n"
    "| %>X | Right justify the rest of the string and pad with character \"X\" |\n"
    "| %|X | Pad to the end of the line with character \"X\" |\n"
  },
  /*
  ** .pp
  ** This variable allows you to customize the newsgroup browser display to
  ** your personal taste.  This string is similar to "$index_format", but
  ** has its own set of printf()-like sequences:
  ** .dl
  ** .dt %C  .dd Current newsgroup number
  ** .dt %d  .dd Description of newsgroup (becomes from server)
  ** .dt %f  .dd Newsgroup name
  ** .dt %M  .dd - if newsgroup not allowed for direct post (moderated for example)
  ** .dt %N  .dd N if newsgroup is new, u if unsubscribed, blank otherwise
  ** .dt %n  .dd Number of new articles in newsgroup
  ** .dt %s  .dd Number of unread articles in newsgroup
  ** .dt %>X .dd Right justify the rest of the string and pad with character "X"
  ** .dt %|X .dd Pad to the end of the line with character "X"
  ** .de
  */
#endif
  { "hdrs", DT_BOOL, &C_Hdrs, true, 0, NULL,
    "When _unset_, the header fields normally added by the \"$my_hdr\"\n"
    "command are not created.  This variable _must_ be unset before\n"
    "composing a new message or replying in order to take effect.  If _set_,\n"
    "the user defined header fields are added to every new message.\n"
  },
  /*
  ** .pp
  ** When \fIunset\fP, the header fields normally added by the "$my_hdr"
  ** command are not created.  This variable \fImust\fP be unset before
  ** composing a new message or replying in order to take effect.  If \fIset\fP,
  ** the user defined header fields are added to every new message.
  */
  { "header", DT_BOOL, &C_Header, false, 0, NULL,
    "When _set_, this variable causes NeoMutt to include the header\n"
    "of the message you are replying to into the edit buffer.\n"
    "The $$weed setting applies.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, this variable causes NeoMutt to include the header
  ** of the message you are replying to into the edit buffer.
  ** The $$weed setting applies.
  */
#ifdef USE_HCACHE
  { "header_cache", DT_STRING|DT_PATH, &C_HeaderCache, 0, 0, NULL,
    "This variable points to the header cache database. If the path points to\n"
    "an existing directory, NeoMutt will create a dedicated header cache\n"
    "database per folder. Otherwise, the path points to a regular file, which\n"
    "will be created as needed and used as a shared global header cache for\n"
    "all folders.\n"
    "By default it is _unset_ so no header caching will be used.\n"
    "\n"
    "Header caching can greatly improve speed when opening POP, IMAP\n"
    "MH or Maildir folders, see \"$caching\" for details.\n"
  },
  /*
  ** .pp
  ** This variable points to the header cache database. If the path points to
  ** an existing directory, NeoMutt will create a dedicated header cache
  ** database per folder. Otherwise, the path points to a regular file, which
  ** will be created as needed and used as a shared global header cache for
  ** all folders.
  ** By default it is \fIunset\fP so no header caching will be used.
  ** .pp
  ** Header caching can greatly improve speed when opening POP, IMAP
  ** MH or Maildir folders, see "$caching" for details.
  */
  { "header_cache_backend", DT_STRING, &C_HeaderCacheBackend, 0, 0, hcache_validator,
    "This variable specifies the header cache backend.\n"
  },
  /*
  ** .pp
  ** This variable specifies the header cache backend.
  */
#if defined(HAVE_QDBM) || defined(HAVE_TC) || defined(HAVE_KC)
  { "header_cache_compress", DT_BOOL, &C_HeaderCacheCompress, true, 0, NULL,
    "When NeoMutt is compiled with qdbm, tokyocabinet or kyotocabinet\n"
    "as header cache backend, this option determines whether the\n"
    "database will be compressed. Compression results in database\n"
    "files roughly being one fifth of the usual diskspace, but the\n"
    "decompression can result in a slower opening of cached folder(s)\n"
    "which in general is still much faster than opening non header\n"
    "cached folders.\n"
  },
  /*
  ** .pp
  ** When NeoMutt is compiled with qdbm, tokyocabinet or kyotocabinet
  ** as header cache backend, this option determines whether the
  ** database will be compressed. Compression results in database
  ** files roughly being one fifth of the usual diskspace, but the
  ** decompression can result in a slower opening of cached folder(s)
  ** which in general is still much faster than opening non header
  ** cached folders.
  */
#endif /* HAVE_QDBM */
#if defined(HAVE_GDBM) || defined(HAVE_BDB)
  { "header_cache_pagesize", DT_LONG|DT_NOT_NEGATIVE, &C_HeaderCachePagesize, 16384, 0, NULL,
    "When NeoMutt is compiled with either gdbm or bdb4 as the header cache backend,\n"
    "this option changes the database page size.  Too large or too small\n"
    "values can waste space, memory, or CPU time. The default should be more\n"
    "or less optimal for most use cases.\n"
  },
  /*
  ** .pp
  ** When NeoMutt is compiled with either gdbm or bdb4 as the header cache backend,
  ** this option changes the database page size.  Too large or too small
  ** values can waste space, memory, or CPU time. The default should be more
  ** or less optimal for most use cases.
  */
#endif /* HAVE_GDBM || HAVE_BDB */
#endif /* USE_HCACHE */
  { "header_color_partial", DT_BOOL|R_PAGER_FLOW, &C_HeaderColorPartial, false, 0, NULL,
    "When _set_, color header regexes behave like color body regexes:\n"
    "color is applied to the exact text matched by the regex.  When\n"
    "_unset_, color is applied to the entire header.\n"
    "\n"
    "One use of this option might be to apply color to just the header labels.\n"
    "\n"
    "See \"$color\" for more details.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, color header regexes behave like color body regexes:
  ** color is applied to the exact text matched by the regex.  When
  ** \fIunset\fP, color is applied to the entire header.
  ** .pp
  ** One use of this option might be to apply color to just the header labels.
  ** .pp
  ** See "$color" for more details.
  */
  { "help", DT_BOOL|R_REFLOW, &C_Help, true, 0, NULL,
    "When _set_, help lines describing the bindings for the major functions\n"
    "provided by each menu are displayed on the first line of the screen.\n"
    "\n"
    "**Note:** The binding will not be displayed correctly if the\n"
    "function is bound to a sequence rather than a single keystroke.  Also,\n"
    "the help line may not be updated if a binding is changed while NeoMutt is\n"
    "running.  Since this variable is primarily aimed at new users, neither\n"
    "of these should present a major problem.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, help lines describing the bindings for the major functions
  ** provided by each menu are displayed on the first line of the screen.
  ** .pp
  ** \fBNote:\fP The binding will not be displayed correctly if the
  ** function is bound to a sequence rather than a single keystroke.  Also,
  ** the help line may not be updated if a binding is changed while NeoMutt is
  ** running.  Since this variable is primarily aimed at new users, neither
  ** of these should present a major problem.
  */
  { "hidden_host", DT_BOOL, &C_HiddenHost, false, 0, NULL,
    "When _set_, NeoMutt will skip the host name part of $$hostname variable\n"
    "when adding the domain part to addresses.  This variable does not\n"
    "affect the generation of Message-IDs, and it will not lead to the\n"
    "cut-off of first-level domains.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will skip the host name part of $$hostname variable
  ** when adding the domain part to addresses.  This variable does not
  ** affect the generation of Message-IDs, and it will not lead to the
  ** cut-off of first-level domains.
  */
  { "hidden_tags", DT_SLIST|SLIST_SEP_COMMA, &C_HiddenTags, IP "unread,draft,flagged,passed,replied,attachment,signed,encrypted", 0, NULL,
    "This variable specifies private notmuch/imap tags which should not be printed\n"
    "on screen.\n"
  },
  /*
  ** .pp
  ** This variable specifies private notmuch/imap tags which should not be printed
  ** on screen.
  */
  { "hide_limited", DT_BOOL|R_TREE|R_INDEX, &C_HideLimited, false, 0, NULL,
    "When _set_, NeoMutt will not show the presence of messages that are hidden\n"
    "by limiting, in the thread tree.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will not show the presence of messages that are hidden
  ** by limiting, in the thread tree.
  */
  { "hide_missing", DT_BOOL|R_TREE|R_INDEX, &C_HideMissing, true, 0, NULL,
    "When _set_, NeoMutt will not show the presence of missing messages in the\n"
    "thread tree.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will not show the presence of missing messages in the
  ** thread tree.
  */
  { "hide_thread_subject", DT_BOOL|R_TREE|R_INDEX, &C_HideThreadSubject, true, 0, NULL,
    "When _set_, NeoMutt will not show the subject of messages in the thread\n"
    "tree that have the same subject as their parent or closest previously\n"
    "displayed sibling.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will not show the subject of messages in the thread
  ** tree that have the same subject as their parent or closest previously
  ** displayed sibling.
  */
  { "hide_top_limited", DT_BOOL|R_TREE|R_INDEX, &C_HideTopLimited, false, 0, NULL,
    "When _set_, NeoMutt will not show the presence of messages that are hidden\n"
    "by limiting, at the top of threads in the thread tree.  Note that when\n"
    "$$hide_limited is _set_, this option will have no effect.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will not show the presence of messages that are hidden
  ** by limiting, at the top of threads in the thread tree.  Note that when
  ** $$hide_limited is \fIset\fP, this option will have no effect.
  */
  { "hide_top_missing", DT_BOOL|R_TREE|R_INDEX, &C_HideTopMissing, true, 0, NULL,
    "When _set_, NeoMutt will not show the presence of missing messages at the\n"
    "top of threads in the thread tree.  Note that when $$hide_missing is\n"
    "_set_, this option will have no effect.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will not show the presence of missing messages at the
  ** top of threads in the thread tree.  Note that when $$hide_missing is
  ** \fIset\fP, this option will have no effect.
  */
  { "history", DT_NUMBER|DT_NOT_NEGATIVE, &C_History, 10, 0, NULL,
    "This variable controls the size (in number of strings remembered) of\n"
    "the string history buffer per category. The buffer is cleared each time the\n"
    "variable is set.\n"
  },
  /*
  ** .pp
  ** This variable controls the size (in number of strings remembered) of
  ** the string history buffer per category. The buffer is cleared each time the
  ** variable is set.
  */
  { "history_file", DT_STRING|DT_PATH, &C_HistoryFile, IP "~/.mutthistory", 0, NULL,
    "The file in which NeoMutt will save its history.\n"
    "\n"
    "Also see $$save_history.\n"
  },
  /*
  ** .pp
  ** The file in which NeoMutt will save its history.
  ** .pp
  ** Also see $$save_history.
  */
  { "history_remove_dups", DT_BOOL, &C_HistoryRemoveDups, false, 0, NULL,
    "When _set_, all of the string history will be scanned for duplicates\n"
    "when a new entry is added.  Duplicate entries in the $$history_file will\n"
    "also be removed when it is periodically compacted.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, all of the string history will be scanned for duplicates
  ** when a new entry is added.  Duplicate entries in the $$history_file will
  ** also be removed when it is periodically compacted.
  */
  { "honor_disposition", DT_BOOL, &C_HonorDisposition, false, 0, NULL,
    "When _set_, NeoMutt will not display attachments with a\n"
    "disposition of \"attachment\" inline even if it could\n"
    "render the part to plain text. These MIME parts can only\n"
    "be viewed from the attachment menu.\n"
    "\n"
    "If _unset_, NeoMutt will render all MIME parts it can\n"
    "properly transform to plain text.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will not display attachments with a
  ** disposition of "attachment" inline even if it could
  ** render the part to plain text. These MIME parts can only
  ** be viewed from the attachment menu.
  ** .pp
  ** If \fIunset\fP, NeoMutt will render all MIME parts it can
  ** properly transform to plain text.
  */
  { "honor_followup_to", DT_QUAD, &C_HonorFollowupTo, MUTT_YES, 0, NULL,
    "This variable controls whether or not a Mail-Followup-To header is\n"
    "honored when group-replying to a message.\n"
  },
  /*
  ** .pp
  ** This variable controls whether or not a Mail-Followup-To header is
  ** honored when group-replying to a message.
  */
  { "hostname", DT_STRING, &C_Hostname, 0, 0, NULL,
    "Specifies the fully-qualified hostname of the system NeoMutt is running on\n"
    "containing the host's name and the DNS domain it belongs to. It is used\n"
    "as the domain part (after \"@\") for local email addresses as well as\n"
    "Message-Id headers.\n"
    "\n"
    "If not specified in a config file, then NeoMutt will try to determine the hostname itself.\n"
    "\n"
    "Optionally, NeoMutt can be compiled with a fixed domain name.\n"
    "\n"
    "Also see $$use_domain and $$hidden_host.\n"
  },
  /*
  ** .pp
  ** Specifies the fully-qualified hostname of the system NeoMutt is running on
  ** containing the host's name and the DNS domain it belongs to. It is used
  ** as the domain part (after "@") for local email addresses as well as
  ** Message-Id headers.
  ** .pp
  ** If not specified in a config file, then NeoMutt will try to determine the hostname itself.
  ** .pp
  ** Optionally, NeoMutt can be compiled with a fixed domain name.
  ** .pp
  ** Also see $$use_domain and $$hidden_host.
  */
#ifdef HAVE_LIBIDN
  { "idn_decode", DT_BOOL|R_MENU, &C_IdnDecode, true, 0, NULL,
    "When _set_, NeoMutt will show you international domain names decoded.\n"
    "Note: You can use IDNs for addresses even if this is _unset_.\n"
    "This variable only affects decoding. (IDN only)\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will show you international domain names decoded.
  ** Note: You can use IDNs for addresses even if this is \fIunset\fP.
  ** This variable only affects decoding. (IDN only)
  */
  { "idn_encode", DT_BOOL|R_MENU, &C_IdnEncode, true, 0, NULL,
    "When _set_, NeoMutt will encode international domain names using\n"
    "IDN.  Unset this if your SMTP server can handle newer (RFC6531)\n"
    "UTF-8 encoded domains. (IDN only)\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will encode international domain names using
  ** IDN.  Unset this if your SMTP server can handle newer (RFC6531)
  ** UTF-8 encoded domains. (IDN only)
  */
#endif /* HAVE_LIBIDN */
  { "ignore_list_reply_to", DT_BOOL, &C_IgnoreListReplyTo, false, 0, NULL,
    "Affects the behavior of the `<reply>` function when replying to\n"
    "messages from mailing lists (as defined by the \"$subscribe\" or\n"
    "\"$lists\" commands).  When _set_, if the \"Reply-To:\" field is\n"
    "set to the same value as the \"To:\" field, NeoMutt assumes that the\n"
    "\"Reply-To:\" field was set by the mailing list to automate responses\n"
    "to the list, and will ignore this field.  To direct a response to the\n"
    "mailing list when this option is _set_, use the `$<list-reply>`\n"
    "function; `<group-reply>` will reply to both the sender and the\n"
    "list.\n"
  },
  /*
  ** .pp
  ** Affects the behavior of the \fC<reply>\fP function when replying to
  ** messages from mailing lists (as defined by the "$subscribe" or
  ** "$lists" commands).  When \fIset\fP, if the "Reply-To:" field is
  ** set to the same value as the "To:" field, NeoMutt assumes that the
  ** "Reply-To:" field was set by the mailing list to automate responses
  ** to the list, and will ignore this field.  To direct a response to the
  ** mailing list when this option is \fIset\fP, use the \fC$<list-reply>\fP
  ** function; \fC<group-reply>\fP will reply to both the sender and the
  ** list.
  */
#ifdef USE_IMAP
  { "imap_authenticators", DT_SLIST|SLIST_SEP_COLON, &C_ImapAuthenticators, 0, 0, NULL,
    "This is a colon-delimited list of authentication methods NeoMutt may\n"
    "attempt to use to log in to an IMAP server, in the order NeoMutt should\n"
    "try them.  Authentication methods are either \"login\" or the right\n"
    "side of an IMAP \"AUTH=xxx\" capability string, e.g. \"digest-md5\", \"gssapi\"\n"
    "or \"cram-md5\". This option is case-insensitive. If it's\n"
    "_unset_ (the default) NeoMutt will try all available methods,\n"
    "in order from most-secure to least-secure.\n"
    "\n"
    "Example:\n"
    "```\n"
    "set imap_authenticators=\"gssapi:cram-md5:login\"\n"
    "```\n"
    "\n"
    "**Note:** NeoMutt will only fall back to other authentication methods if\n"
    "the previous methods are unavailable. If a method is available but\n"
    "authentication fails, NeoMutt will not connect to the IMAP server.\n"
  },
  /*
  ** .pp
  ** This is a colon-delimited list of authentication methods NeoMutt may
  ** attempt to use to log in to an IMAP server, in the order NeoMutt should
  ** try them.  Authentication methods are either "login" or the right
  ** side of an IMAP "AUTH=xxx" capability string, e.g. "digest-md5", "gssapi"
  ** or "cram-md5". This option is case-insensitive. If it's
  ** \fIunset\fP (the default) NeoMutt will try all available methods,
  ** in order from most-secure to least-secure.
  ** .pp
  ** Example:
  ** .ts
  ** set imap_authenticators="gssapi:cram-md5:login"
  ** .te
  ** .pp
  ** \fBNote:\fP NeoMutt will only fall back to other authentication methods if
  ** the previous methods are unavailable. If a method is available but
  ** authentication fails, NeoMutt will not connect to the IMAP server.
  */
  { "imap_check_subscribed", DT_BOOL, &C_ImapCheckSubscribed, false, 0, NULL,
    "When _set_, NeoMutt will fetch the set of subscribed folders from\n"
    "your server whenever a mailbox is **selected**, and add them to the set\n"
    "of mailboxes it polls for new mail just as if you had issued individual\n"
    "\"$mailboxes\" commands.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will fetch the set of subscribed folders from
  ** your server whenever a mailbox is \fBselected\fP, and add them to the set
  ** of mailboxes it polls for new mail just as if you had issued individual
  ** "$mailboxes" commands.
  */
  { "imap_condstore", DT_BOOL, &C_ImapCondstore, false, 0, NULL,
    "When _set_, NeoMutt will use the CONDSTORE extension (RFC 7162)\n"
    "if advertised by the server.  NeoMutt's current implementation is basic,\n"
    "used only for initial message fetching and flag updates.\n"
    "\n"
    "For some IMAP servers, enabling this will slightly speed up\n"
    "downloading initial messages.  Unfortunately, Gmail is not one\n"
    "those, and displays worse performance when enabled.  Your\n"
    "mileage may vary.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will use the CONDSTORE extension (RFC7162)
  ** if advertised by the server.  NeoMutt's current implementation is basic,
  ** used only for initial message fetching and flag updates.
  ** .pp
  ** For some IMAP servers, enabling this will slightly speed up
  ** downloading initial messages.  Unfortunately, Gmail is not one
  ** those, and displays worse performance when enabled.  Your
  ** mileage may vary.
  */
#ifdef USE_ZLIB
  { "imap_deflate", DT_BOOL, &C_ImapDeflate, true, 0, NULL,
    "When _set_, mutt will use the COMPRESS=DEFLATE extension (RFC4978)\n"
    "if advertised by the server.\n"
    "\n"
    "In general a good compression efficiency can be achieved, which\n"
    "speeds up reading large mailboxes also on fairly good connections.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, mutt will use the COMPRESS=DEFLATE extension (RFC4978)
  ** if advertised by the server.
  ** .pp
  ** In general a good compression efficiency can be achieved, which
  ** speeds up reading large mailboxes also on fairly good connections.
  */
#endif
  { "imap_delim_chars", DT_STRING, &C_ImapDelimChars, IP "/.", 0, NULL,
    "This contains the list of characters which you would like to treat\n"
    "as folder separators for displaying IMAP paths. In particular it\n"
    "helps in using the \"=\" shortcut for your _folder_ variable.\n"
  },
  /*
  ** .pp
  ** This contains the list of characters which you would like to treat
  ** as folder separators for displaying IMAP paths. In particular it
  ** helps in using the "=" shortcut for your \fIfolder\fP variable.
  */
  { "imap_fetch_chunk_size", DT_LONG|DT_NOT_NEGATIVE, &C_ImapFetchChunkSize, 0, 0, NULL,
    "When set to a value greater than 0, new headers will be downloaded\n"
    "in sets of this size.  If you have a very large mailbox, this might\n"
    "prevent a timeout and disconnect when opening the mailbox, by sending\n"
    "a FETCH per set of this size instead of a single FETCH for all new\n"
    "headers.\n"
  },
  /*
  ** .pp
  ** When set to a value greater than 0, new headers will be downloaded
  ** in sets of this size.  If you have a very large mailbox, this might
  ** prevent a timeout and disconnect when opening the mailbox, by sending
  ** a FETCH per set of this size instead of a single FETCH for all new
  ** headers.
  */
  { "imap_headers", DT_STRING|R_INDEX, &C_ImapHeaders, 0, 0, NULL,
    "NeoMutt requests these header fields in addition to the default headers\n"
    "(\"Date:\", \"From:\", \"Sender:\", \"Subject:\", \"To:\", \"Cc:\", \"Message-Id:\",\n"
    "\"References:\", \"Content-Type:\", \"Content-Description:\", \"In-Reply-To:\",\n"
    "\"Reply-To:\", \"Lines:\", \"List-Post:\", \"X-Label:\") from IMAP\n"
    "servers before displaying the index menu. You may want to add more\n"
    "headers for spam detection.\n"
    "\n"
    "**Note:** This is a space separated list, items should be uppercase\n"
    "and not contain the colon, e.g. \"X-BOGOSITY X-SPAM-STATUS\" for the\n"
    "\"X-Bogosity:\" and \"X-Spam-Status:\" header fields.\n"
  },
  /*
  ** .pp
  ** NeoMutt requests these header fields in addition to the default headers
  ** ("Date:", "From:", "Sender:", "Subject:", "To:", "Cc:", "Message-Id:",
  ** "References:", "Content-Type:", "Content-Description:", "In-Reply-To:",
  ** "Reply-To:", "Lines:", "List-Post:", "X-Label:") from IMAP
  ** servers before displaying the index menu. You may want to add more
  ** headers for spam detection.
  ** .pp
  ** \fBNote:\fP This is a space separated list, items should be uppercase
  ** and not contain the colon, e.g. "X-BOGOSITY X-SPAM-STATUS" for the
  ** "X-Bogosity:" and "X-Spam-Status:" header fields.
  */
  { "imap_idle", DT_BOOL, &C_ImapIdle, false, 0, NULL,
    "When _set_, NeoMutt will attempt to use the IMAP IDLE extension\n"
    "to check for new mail in the current mailbox. Some servers\n"
    "(dovecot was the inspiration for this option) react badly\n"
    "to NeoMutt's implementation. If your connection seems to freeze\n"
    "up periodically, try unsetting this.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will attempt to use the IMAP IDLE extension
  ** to check for new mail in the current mailbox. Some servers
  ** (dovecot was the inspiration for this option) react badly
  ** to NeoMutt's implementation. If your connection seems to freeze
  ** up periodically, try unsetting this.
  */
  { "imap_keepalive", DT_NUMBER|DT_NOT_NEGATIVE, &C_ImapKeepalive, 300, 0, NULL,
    "This variable specifies the maximum amount of time in seconds that NeoMutt\n"
    "will wait before polling open IMAP connections, to prevent the server\n"
    "from closing them before NeoMutt has finished with them. The default is\n"
    "well within the RFC-specified minimum amount of time (30 minutes) before\n"
    "a server is allowed to do this, but in practice the RFC does get\n"
    "violated every now and then. Reduce this number if you find yourself\n"
    "getting disconnected from your IMAP server due to inactivity.\n"
  },
  /*
  ** .pp
  ** This variable specifies the maximum amount of time in seconds that NeoMutt
  ** will wait before polling open IMAP connections, to prevent the server
  ** from closing them before NeoMutt has finished with them. The default is
  ** well within the RFC-specified minimum amount of time (30 minutes) before
  ** a server is allowed to do this, but in practice the RFC does get
  ** violated every now and then. Reduce this number if you find yourself
  ** getting disconnected from your IMAP server due to inactivity.
  */
  { "imap_list_subscribed", DT_BOOL, &C_ImapListSubscribed, false, 0, NULL,
    "This variable configures whether IMAP folder browsing will look for\n"
    "only subscribed folders or all folders.  This can be toggled in the\n"
    "IMAP browser with the `<toggle-subscribed>` function.\n"
  },
  /*
  ** .pp
  ** This variable configures whether IMAP folder browsing will look for
  ** only subscribed folders or all folders.  This can be toggled in the
  ** IMAP browser with the \fC<toggle-subscribed>\fP function.
  */
  { "imap_login", DT_STRING|DT_SENSITIVE, &C_ImapLogin, 0, 0, NULL,
    "Your login name on the IMAP server.\n"
    "\n"
    "This variable defaults to the value of $$imap_user.\n"
  },
  /*
  ** .pp
  ** Your login name on the IMAP server.
  ** .pp
  ** This variable defaults to the value of $$imap_user.
  */
  { "imap_oauth_refresh_command", DT_STRING|DT_COMMAND|DT_SENSITIVE, &C_ImapOauthRefreshCommand, 0, 0, NULL,
    "The command to run to generate an OAUTH refresh token for\n"
    "authorizing your connection to your IMAP server.  This command will be\n"
    "run on every connection attempt that uses the OAUTHBEARER authentication\n"
    "mechanism.  See \"$oauth\" for details.\n"
  },
  /*
  ** .pp
  ** The command to run to generate an OAUTH refresh token for
  ** authorizing your connection to your IMAP server.  This command will be
  ** run on every connection attempt that uses the OAUTHBEARER authentication
  ** mechanism.  See "$oauth" for details.
  */
  { "imap_pass", DT_STRING|DT_SENSITIVE, &C_ImapPass, 0, 0, NULL,
    "Specifies the password for your IMAP account.  If _unset_, NeoMutt will\n"
    "prompt you for your password when you invoke the `<imap-fetch-mail>` function\n"
    "or try to open an IMAP folder.\n"
    "\n"
    "**Warning**: you should only use this option when you are on a\n"
    "fairly secure machine, because the superuser can read your neomuttrc even\n"
    "if you are the only one who can read the file.\n"
  },
  /*
  ** .pp
  ** Specifies the password for your IMAP account.  If \fIunset\fP, NeoMutt will
  ** prompt you for your password when you invoke the \fC<imap-fetch-mail>\fP function
  ** or try to open an IMAP folder.
  ** .pp
  ** \fBWarning\fP: you should only use this option when you are on a
  ** fairly secure machine, because the superuser can read your neomuttrc even
  ** if you are the only one who can read the file.
  */
  { "imap_passive", DT_BOOL, &C_ImapPassive, true, 0, NULL,
    "When _set_, NeoMutt will not open new IMAP connections to check for new\n"
    "mail.  NeoMutt will only check for new mail over existing IMAP\n"
    "connections.  This is useful if you don't want to be prompted for\n"
    "user/password pairs on NeoMutt invocation, or if opening the connection\n"
    "is slow.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will not open new IMAP connections to check for new
  ** mail.  NeoMutt will only check for new mail over existing IMAP
  ** connections.  This is useful if you don't want to be prompted for
  ** user/password pairs on NeoMutt invocation, or if opening the connection
  ** is slow.
  */
  { "imap_peek", DT_BOOL, &C_ImapPeek, true, 0, NULL,
    "When _set_, NeoMutt will avoid implicitly marking your mail as read whenever\n"
    "you fetch a message from the server. This is generally a good thing,\n"
    "but can make closing an IMAP folder somewhat slower. This option\n"
    "exists to appease speed freaks.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will avoid implicitly marking your mail as read whenever
  ** you fetch a message from the server. This is generally a good thing,
  ** but can make closing an IMAP folder somewhat slower. This option
  ** exists to appease speed freaks.
  */
  { "imap_pipeline_depth", DT_NUMBER|DT_NOT_NEGATIVE, &C_ImapPipelineDepth, 15, 0, NULL,
    "Controls the number of IMAP commands that may be queued up before they\n"
    "are sent to the server. A deeper pipeline reduces the amount of time\n"
    "NeoMutt must wait for the server, and can make IMAP servers feel much\n"
    "more responsive. But not all servers correctly handle pipelined commands,\n"
    "so if you have problems you might want to try setting this variable to 0.\n"
    "\n"
    "**Note:** Changes to this variable have no effect on open connections.\n"
  },
  /*
  ** .pp
  ** Controls the number of IMAP commands that may be queued up before they
  ** are sent to the server. A deeper pipeline reduces the amount of time
  ** NeoMutt must wait for the server, and can make IMAP servers feel much
  ** more responsive. But not all servers correctly handle pipelined commands,
  ** so if you have problems you might want to try setting this variable to 0.
  ** .pp
  ** \fBNote:\fP Changes to this variable have no effect on open connections.
  */
  { "imap_poll_timeout", DT_NUMBER|DT_NOT_NEGATIVE, &C_ImapPollTimeout, 15, 0, NULL,
    "This variable specifies the maximum amount of time in seconds\n"
    "that NeoMutt will wait for a response when polling IMAP connections\n"
    "for new mail, before timing out and closing the connection.  Set\n"
    "to 0 to disable timing out.\n"
  },
  /*
  ** .pp
  ** This variable specifies the maximum amount of time in seconds
  ** that NeoMutt will wait for a response when polling IMAP connections
  ** for new mail, before timing out and closing the connection.  Set
  ** to 0 to disable timing out.
  */
  { "imap_qresync", DT_BOOL, &C_ImapQresync, false, 0, NULL,
    "When _set_, NeoMutt will use the QRESYNC extension (RFC 7162)\n"
    "if advertised by the server.  NeoMutt's current implementation is basic,\n"
    "used only for initial message fetching and flag updates.\n"
    "\n"
    "Note: this feature is currently experimental.  If you experience\n"
    "strange behavior, such as duplicate or missing messages please\n"
    "file a bug report to let us know.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will use the QRESYNC extension (RFC7162)
  ** if advertised by the server.  NeoMutt's current implementation is basic,
  ** used only for initial message fetching and flag updates.
  ** .pp
  ** Note: this feature is currently experimental.  If you experience
  ** strange behavior, such as duplicate or missing messages please
  ** file a bug report to let us know.
  */
  { "imap_rfc5161", DT_BOOL, &C_ImapRfc5161, true, 0, NULL,
    "When _set_, NeoMutt will use the IMAP ENABLE extension (RFC 5161) to\n"
    "select CAPABILITIES. Some servers (notably Coremail System IMap Server) do\n"
    "not properly respond to ENABLE commands, which might cause NeoMutt to hang.\n"
    "If your connection seems to freeze at login, try unsetting this. See also\n"
    "https://github.com/neomutt/neomutt/issues/1689\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will use the IMAP ENABLE extension (RFC5161) to
  ** select CAPABILITIES. Some servers (notably Coremail System IMap Server) do
  ** not properly respond to ENABLE commands, which might cause NeoMutt to hang.
  ** If your connection seems to freeze at login, try unsetting this. See also
  ** https://github.com/neomutt/neomutt/issues/1689
  */
  { "imap_servernoise", DT_BOOL, &C_ImapServernoise, true, 0, NULL,
    "When _set_, NeoMutt will display warning messages from the IMAP\n"
    "server as error messages. Since these messages are often\n"
    "harmless, or generated due to configuration problems on the\n"
    "server which are out of the users' hands, you may wish to suppress\n"
    "them at some point.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will display warning messages from the IMAP
  ** server as error messages. Since these messages are often
  ** harmless, or generated due to configuration problems on the
  ** server which are out of the users' hands, you may wish to suppress
  ** them at some point.
  */
  { "imap_user", DT_STRING|DT_SENSITIVE, &C_ImapUser, 0, 0, NULL,
    "The name of the user whose mail you intend to access on the IMAP\n"
    "server.\n"
    "\n"
    "This variable defaults to your user name on the local machine.\n"
  },
  /*
  ** .pp
  ** The name of the user whose mail you intend to access on the IMAP
  ** server.
  ** .pp
  ** This variable defaults to your user name on the local machine.
  */
#endif
  { "implicit_autoview", DT_BOOL, &C_ImplicitAutoview, false, 0, NULL,
    "If set to \"yes\", NeoMutt will look for a mailcap entry with the\n"
    "\"`copiousoutput`\" flag set for _every_ MIME attachment it doesn't have\n"
    "an internal viewer defined for.  If such an entry is found, NeoMutt will\n"
    "use the viewer defined in that entry to convert the body part to text\n"
    "form.\n"
  },
  /*
  ** .pp
  ** If set to "yes", NeoMutt will look for a mailcap entry with the
  ** "\fCcopiousoutput\fP" flag set for \fIevery\fP MIME attachment it doesn't have
  ** an internal viewer defined for.  If such an entry is found, NeoMutt will
  ** use the viewer defined in that entry to convert the body part to text
  ** form.
  */
  { "include", DT_QUAD, &C_Include, MUTT_ASKYES, 0, NULL,
    "Controls whether or not a copy of the message(s) you are replying to\n"
    "is included in your reply.\n"
  },
  /*
  ** .pp
  ** Controls whether or not a copy of the message(s) you are replying to
  ** is included in your reply.
  */
  { "include_encrypted", DT_BOOL, &C_IncludeEncrypted, false, 0, NULL,
    "Controls whether or not NeoMutt includes separately encrypted attachment\n"
    "contents when replying.\n"
    "\n"
    "This variable was added to prevent accidental exposure of encrypted\n"
    "contents when replying to an attacker.  If a previously encrypted message\n"
    "were attached by the attacker, they could trick an unwary recipient into\n"
    "decrypting and including the message in their reply.\n"
  },
  /*
  ** .pp
  ** Controls whether or not NeoMutt includes separately encrypted attachment
  ** contents when replying.
  ** .pp
  ** This variable was added to prevent accidental exposure of encrypted
  ** contents when replying to an attacker.  If a previously encrypted message
  ** were attached by the attacker, they could trick an unwary recipient into
  ** decrypting and including the message in their reply.
  */
  { "include_onlyfirst", DT_BOOL, &C_IncludeOnlyfirst, false, 0, NULL,
    "Controls whether or not NeoMutt includes only the first attachment\n"
    "of the message you are replying.\n"
  },
  /*
  ** .pp
  ** Controls whether or not NeoMutt includes only the first attachment
  ** of the message you are replying.
  */
  { "indent_string", DT_STRING, &C_IndentString, IP "> ", 0, NULL,
    "Specifies the string to prepend to each line of text quoted in a\n"
    "message to which you are replying.  You are strongly encouraged not to\n"
    "change this value, as it tends to agitate the more fanatical netizens.\n"
    "\n"
    "The value of this option is ignored if $$text_flowed is set, because\n"
    "the quoting mechanism is strictly defined for format=flowed.\n"
    "\n"
    "This option is a format string, please see the description of\n"
    "$$index_format for supported `printf(3)`-style sequences.\n"
  },
  /*
  ** .pp
  ** Specifies the string to prepend to each line of text quoted in a
  ** message to which you are replying.  You are strongly encouraged not to
  ** change this value, as it tends to agitate the more fanatical netizens.
  ** .pp
  ** The value of this option is ignored if $$text_flowed is set, because
  ** the quoting mechanism is strictly defined for format=flowed.
  ** .pp
  ** This option is a format string, please see the description of
  ** $$index_format for supported \fCprintf(3)\fP-style sequences.
  */
  { "index_format", DT_STRING|DT_NOT_EMPTY|R_INDEX|R_PAGER, &C_IndexFormat, IP "%4C %Z %{%b %d} %-15.15L (%?l?%4l&%4c?) %s", 0, NULL,
    "This variable allows you to customize the message index display to\n"
    "your personal taste.\n"
    "\n"
    "\"Format strings\" are similar to the strings used in the C\n"
    "function `printf(3)` to format output (see the man page for more details).\n"
    "For an explanation of the %? construct, see the $status_format description.\n"
    "The following sequences are defined in NeoMutt:\n"
    "\n"
    "| .       | Description |\n"
    "|---------+-------------|\n"
    "| %a      | Address of the author |\n"
    "| %A      | Reply-to address (if present; otherwise: address of author) |\n"
    "| %b      | Filename of the original message folder (think mailbox) |\n"
    "| %B      | The list to which the letter was sent, or else the folder name (%b). |\n"
    "| %C      | Current message number |\n"
    "| %c      | Number of characters (bytes) in the body of the message (see $formatstrings-size) |\n"
    "| %cr     | Number of characters (bytes) in the raw message, including the header (see $formatstrings-size) |\n"
    "| %D      | Date and time of message using $date_format and local timezone |\n"
    "| %d      | Date and time of message using $date_format and sender's timezone |\n"
    "| %e      | Current message number in thread |\n"
    "| %E      | Number of messages in current thread |\n"
    "| %F      | Author name, or recipient name if the message is from you |\n"
    "| %Fp     | Like %F, but plain. No contextual formatting is applied to recipient name |\n"
    "| %f      | Sender (address + real name), either From: or Return-Path: |\n"
    "| %g      | Newsgroup name (if compiled with NNTP support) |\n"
    "| %g      | Message tags (e.g. notmuch tags/imap flags) |\n"
    "| %Gx     | Individual message tag (e.g. notmuch tags/imap flags) |\n"
    "| %H      | Spam attribute(s) of this message |\n"
    "| %I      | Initials of author |\n"
    "| %i      | Message-id of the current message |\n"
    "| %J      | Message tags (if present, tree unfolded, and != parent's tags) |\n"
    "| %K      | The list to which the letter was sent (if any; otherwise: empty) |\n"
    "| %L      | If an address in the \"To:\" or \"Cc:\" header field matches an address Defined by the users \"$subscribe\" command, this displays \"To <list-name>\", otherwise the same as %F |\n"
    "| %l      | number of lines in the unprocessed message (may not work with maildir, mh, and IMAP folders) |\n"
    "| %M      | Number of hidden messages if the thread is collapsed |\n"
    "| %m      | Total number of message in the mailbox |\n"
    "| %N      | Message score |\n"
    "| %n      | Author's real name (or address if missing) |\n"
    "| %O      | Original save folder where NeoMutt would formerly have Stashed the message: list name or recipient name If not sent to a list |\n"
    "| %P      | Progress indicator for the built-in pager (how much of the file has been displayed) |\n"
    "| %q      | Newsgroup name (if compiled with NNTP support) |\n"
    "| %R      | Comma separated list of \"Cc:\" recipients |\n"
    "| %r      | Comma separated list of \"To:\" recipients |\n"
    "| %S      | Single character status of the message (\"N\"/\"O\"/\"D\"/\"d\"/\"!\"/\"r\"/\"*\") |\n"
    "| %s      | Subject of the message |\n"
    "| %T      | The appropriate character from the $$to_chars string |\n"
    "| %t      | \"To:\" field (recipients) |\n"
    "| %u      | User (login) name of the author |\n"
    "| %v      | First name of the author, or the recipient if the message is from you |\n"
    "| %W      | Name of organization of author (\"Organization:\" field) |\n"
    "| %x      | \"X-Comment-To:\" field (if present and compiled with NNTP support) |\n"
    "| %X      | Number of MIME attachments (please see the \"$attachments\" section for possible speed effects) |\n"
    "| %Y      | \"X-Label:\" field, if present, and _(1)_ not at part of a thread tree, _(2)_ at the top of a thread, or _(3)_ \"X-Label:\" is different from Preceding message's \"X-Label:\" |\n"
    "| %y      | \"X-Label:\" field, if present |\n"
    "| %Z      | A three character set of message status flags.  The first character is new/read/replied flags (\"n\"/\"o\"/\"r\"/\"O\"/\"N\").  The second is deleted or encryption flags (\"D\"/\"d\"/\"S\"/\"P\"/\"s\"/\"K\").  The third is either tagged/flagged (\"*\"/\"!\"), or one of the characters Listed in $$to_chars. |\n"
    "| %zc     | Message crypto flags |\n"
    "| %zs     | Message status flags |\n"
    "| %zt     | Message tag flags |\n"
    "| %@name@ | insert and evaluate format-string from the matching ``$index-format-hook'' command\n"
    "| %{fmt}  | the date and time of the message is converted to sender's 0, NULL, time zone, and \"fmt\" is expanded by the library function `strftime(3)`; a leading bang disables locales |\n"
    "| %[fmt]  | the date and time of the message is converted to the local time zone, and \"fmt\" is expanded by the library function `strftime(3)`; a leading bang disables locales |\n"
    "| %(fmt)  | the local date and time when the message was received. \"fmt\" is expanded by the library function `strftime(3)`; a leading bang disables locales |\n"
    "| %>X     | right justify the rest of the string and pad with character \"X\" |\n"
    "| %|X     | pad to the end of the line with character \"X\" |\n"
    "| %*X     | soft-fill with character \"X\" as pad |\n"
    "\n"
    "Date format expressions can be constructed based on relative dates. Using\n"
    "the date formatting operators along with nested conditionals, the date\n"
    "format can be modified based on how old a message is.  See the section on\n"
    "\"Conditional Dates\" for an explanation and examples\n"
    "\n"
    "Note that for mbox/mmdf, ``%l'' applies to the unprocessed message, and\n"
    "for maildir/mh, the value comes from the ``Lines:'' header field when\n"
    "present (the meaning is normally the same). Thus the value depends on\n"
    "the encodings used in the different parts of the message and has little\n"
    "meaning in practice.\n"
    "\n"
    "\"Soft-fill\" deserves some explanation: Normal right-justification\n"
    "will print everything to the left of the \"%>\", displaying padding and\n"
    "whatever lies to the right only if there's room. By contrast,\n"
    "soft-fill gives priority to the right-hand side, guaranteeing space\n"
    "to display it and showing padding only if there's still room. If\n"
    "necessary, soft-fill will eat text leftwards to make room for\n"
    "rightward text.\n"
    "\n"
    "Note that these expandos are supported in\n"
    "``$save-hook'', ``$fcc-hook'' and ``$fcc-save-hook'', too.\n"
  },
  /*
  ** .pp
  ** This variable allows you to customize the message index display to
  ** your personal taste.
  ** .pp
  ** "Format strings" are similar to the strings used in the C
  ** function \fCprintf(3)\fP to format output (see the man page for more details).
  ** For an explanation of the %? construct, see the $status_format description.
  ** The following sequences are defined in NeoMutt:
  ** .dl
  ** .dt %a .dd Address of the author
  ** .dt %A .dd Reply-to address (if present; otherwise: address of author)
  ** .dt %b .dd Filename of the original message folder (think mailbox)
  ** .dt %B .dd The list to which the letter was sent, or else the folder name (%b).
  ** .dt %C .dd Current message number
  ** .dt %c .dd Number of characters (bytes) in the body of the message (see $formatstrings-size)
  ** .dt %cr .dd Number of characters (bytes) in the raw message, including the header (see $formatstrings-size)
  ** .dt %D .dd Date and time of message using $date_format and local timezone
  ** .dt %d .dd Date and time of message using $date_format and sender's timezone
  ** .dt %e .dd Current message number in thread
  ** .dt %E .dd Number of messages in current thread
  ** .dt %F .dd Author name, or recipient name if the message is from you
  ** .dt %Fp .dd Like %F, but plain. No contextual formatting is applied to recipient name
  ** .dt %f .dd Sender (address + real name), either From: or Return-Path:
  ** .dt %g .dd Newsgroup name (if compiled with NNTP support)
  ** .dt %g .dd Message tags (e.g. notmuch tags/imap flags)
  ** .dt %Gx .dd Individual message tag (e.g. notmuch tags/imap flags)
  ** .dt %H .dd Spam attribute(s) of this message
  ** .dt %I .dd Initials of author
  ** .dt %i .dd Message-id of the current message
  ** .dt %J .dd Message tags (if present, tree unfolded, and != parent's tags)
  ** .dt %K .dd The list to which the letter was sent (if any; otherwise: empty)
  ** .dt %L .dd If an address in the "To:" or "Cc:" header field matches an address
  **            Defined by the users "$subscribe" command, this displays
  **            "To <list-name>", otherwise the same as %F
  ** .dt %l .dd number of lines in the unprocessed message (may not work with
  **            maildir, mh, and IMAP folders)
  ** .dt %M .dd Number of hidden messages if the thread is collapsed
  ** .dt %m .dd Total number of message in the mailbox
  ** .dt %N .dd Message score
  ** .dt %n .dd Author's real name (or address if missing)
  ** .dt %O .dd Original save folder where NeoMutt would formerly have
  **            Stashed the message: list name or recipient name
  **            If not sent to a list
  ** .dt %P .dd Progress indicator for the built-in pager (how much of the file has been displayed)
  ** .dt %q .dd Newsgroup name (if compiled with NNTP support)
  ** .dt %R .dd Comma separated list of "Cc:" recipients
  ** .dt %r .dd Comma separated list of "To:" recipients
  ** .dt %S .dd Single character status of the message ("N"/"O"/"D"/"d"/"!"/"r"/"\(as")
  ** .dt %s .dd Subject of the message
  ** .dt %T .dd The appropriate character from the $$to_chars string
  ** .dt %t .dd "To:" field (recipients)
  ** .dt %u .dd User (login) name of the author
  ** .dt %v .dd First name of the author, or the recipient if the message is from you
  ** .dt %W .dd Name of organization of author ("Organization:" field)
  ** .dt %x .dd "X-Comment-To:" field (if present and compiled with NNTP support)
  ** .dt %X .dd Number of MIME attachments
  **            (please see the "$attachments" section for possible speed effects)
  ** .dt %Y .dd "X-Label:" field, if present, and \fI(1)\fP not at part of a thread tree,
  **            \fI(2)\fP at the top of a thread, or \fI(3)\fP "X-Label:" is different from
  **            Preceding message's "X-Label:"
  ** .dt %y .dd "X-Label:" field, if present
  ** .dt %Z .dd A three character set of message status flags.
  **            The first character is new/read/replied flags ("n"/"o"/"r"/"O"/"N").
  **            The second is deleted or encryption flags ("D"/"d"/"S"/"P"/"s"/"K").
  **            The third is either tagged/flagged ("\(as"/"!"), or one of the characters
  **            Listed in $$to_chars.
  ** .dt %zc .dd Message crypto flags
  ** .dt %zs .dd Message status flags
  ** .dt %zt .dd Message tag flags
  ** .dt %@name@ .dd insert and evaluate format-string from the matching
  **                 ``$index-format-hook'' command
  ** .dt %{fmt} .dd the date and time of the message is converted to sender's 0, NULL,
  **                time zone, and "fmt" is expanded by the library function
  **                \fCstrftime(3)\fP; a leading bang disables locales
  ** .dt %[fmt] .dd the date and time of the message is converted to the local
  **                time zone, and "fmt" is expanded by the library function
  **                \fCstrftime(3)\fP; a leading bang disables locales
  ** .dt %(fmt) .dd the local date and time when the message was received.
  **                "fmt" is expanded by the library function \fCstrftime(3)\fP;
  **                a leading bang disables locales
  ** .dt %>X    .dd right justify the rest of the string and pad with character "X"
  ** .dt %|X    .dd pad to the end of the line with character "X"
  ** .dt %*X    .dd soft-fill with character "X" as pad
  ** .de
  ** .pp
  ** Date format expressions can be constructed based on relative dates. Using
  ** the date formatting operators along with nested conditionals, the date
  ** format can be modified based on how old a message is.  See the section on
  ** "Conditional Dates" for an explanation and examples
  ** .pp
  ** Note that for mbox/mmdf, ``%l'' applies to the unprocessed message, and
  ** for maildir/mh, the value comes from the ``Lines:'' header field when
  ** present (the meaning is normally the same). Thus the value depends on
  ** the encodings used in the different parts of the message and has little
  ** meaning in practice.
  ** .pp
  ** "Soft-fill" deserves some explanation: Normal right-justification
  ** will print everything to the left of the "%>", displaying padding and
  ** whatever lies to the right only if there's room. By contrast,
  ** soft-fill gives priority to the right-hand side, guaranteeing space
  ** to display it and showing padding only if there's still room. If
  ** necessary, soft-fill will eat text leftwards to make room for
  ** rightward text.
  ** .pp
  ** Note that these expandos are supported in
  ** ``$save-hook'', ``$fcc-hook'' and ``$fcc-save-hook'', too.
  */
#ifdef USE_NNTP
  { "inews", DT_STRING|DT_COMMAND, &C_Inews, 0, 0, NULL,
    "If set, specifies the program and arguments used to deliver news posted\n"
    "by NeoMutt.  Otherwise, NeoMutt posts article using current connection to\n"
    "news server.  The following printf-style sequence is understood:\n"
    "\n"
    "| .  | Description |\n"
    "|----+-------------------|\n"
    "| %a | account url |\n"
    "| %p | port |\n"
    "| %P | port if specified |\n"
    "| %s | news server name |\n"
    "| %S | url schema |\n"
    "| %u | username |\n"
    "\n"
    "Example:\n"
    "```\n"
    "set inews=\"/usr/local/bin/inews -hS\"\n"
    "```\n"
  },
  /*
  ** .pp
  ** If set, specifies the program and arguments used to deliver news posted
  ** by NeoMutt.  Otherwise, NeoMutt posts article using current connection to
  ** news server.  The following printf-style sequence is understood:
  ** .dl
  ** .dt %a .dd account url
  ** .dt %p .dd port
  ** .dt %P .dd port if specified
  ** .dt %s .dd news server name
  ** .dt %S .dd url schema
  ** .dt %u .dd username
  ** .de
  ** .pp
  ** Example:
  ** .ts
  ** set inews="/usr/local/bin/inews -hS"
  ** .te
  */
#endif
  { "ispell", DT_STRING|DT_COMMAND, &C_Ispell, IP ISPELL, 0, NULL,
    "How to invoke ispell (GNU's spell-checking software).\n"
  },
  /*
  ** .pp
  ** How to invoke ispell (GNU's spell-checking software).
  */
  { "keep_flagged", DT_BOOL, &C_KeepFlagged, false, 0, NULL,
    "If _set_, read messages marked as flagged will not be moved\n"
    "from your spool mailbox to your $$mbox mailbox, or as a result of\n"
    "a \"$mbox-hook\" command.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, read messages marked as flagged will not be moved
  ** from your spool mailbox to your $$mbox mailbox, or as a result of
  ** a "$mbox-hook" command.
  */
  { "mail_check", DT_NUMBER|DT_NOT_NEGATIVE, &C_MailCheck, 5, 0, NULL,
    "This variable configures how often (in seconds) NeoMutt should look for\n"
    "new mail. Also see the $$timeout variable.\n"
  },
  /*
  ** .pp
  ** This variable configures how often (in seconds) NeoMutt should look for
  ** new mail. Also see the $$timeout variable.
  */
  { "mail_check_recent", DT_BOOL, &C_MailCheckRecent, true, 0, NULL,
    "When _set_, NeoMutt will only notify you about new mail that has been received\n"
    "since the last time you opened the mailbox.  When _unset_, NeoMutt will notify you\n"
    "if any new mail exists in the mailbox, regardless of whether you have visited it\n"
    "recently.\n"
    "\n"
    "When _$$mark_old_ is set, NeoMutt does not consider the mailbox to contain new\n"
    "mail if only old messages exist.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will only notify you about new mail that has been received
  ** since the last time you opened the mailbox.  When \fIunset\fP, NeoMutt will notify you
  ** if any new mail exists in the mailbox, regardless of whether you have visited it
  ** recently.
  ** .pp
  ** When \fI$$mark_old\fP is set, NeoMutt does not consider the mailbox to contain new
  ** mail if only old messages exist.
  */
  { "mail_check_stats", DT_BOOL, &C_MailCheckStats, false, 0, NULL,
    "When _set_, NeoMutt will periodically calculate message\n"
    "statistics of a mailbox while polling for new mail.  It will\n"
    "check for unread, flagged, and total message counts.  Because\n"
    "this operation is more performance intensive, it defaults to\n"
    "_unset_, and has a separate option, $$mail_check_stats_interval, to\n"
    "control how often to update these counts.\n"
    "\n"
    "Message statistics can also be explicitly calculated by invoking the\n"
    "`<check-stats>` function.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will periodically calculate message
  ** statistics of a mailbox while polling for new mail.  It will
  ** check for unread, flagged, and total message counts.  Because
  ** this operation is more performance intensive, it defaults to
  ** \fIunset\fP, and has a separate option, $$mail_check_stats_interval, to
  ** control how often to update these counts.
  ** .pp
  ** Message statistics can also be explicitly calculated by invoking the
  ** \fC<check-stats>\fP function.
  */
  { "mail_check_stats_interval", DT_NUMBER|DT_NOT_NEGATIVE, &C_MailCheckStatsInterval, 60, 0, NULL,
    "When $$mail_check_stats is _set_, this variable configures\n"
    "how often (in seconds) NeoMutt will update message counts.\n"
  },
  /*
  ** .pp
  ** When $$mail_check_stats is \fIset\fP, this variable configures
  ** how often (in seconds) NeoMutt will update message counts.
  */
  { "mailcap_path", DT_SLIST|SLIST_SEP_COLON, &C_MailcapPath, IP "~/.mailcap:" PKGDATADIR "/mailcap:" SYSCONFDIR "/mailcap:/etc/mailcap:/usr/etc/mailcap:/usr/local/etc/mailcap", 0, NULL,
    "This variable specifies which files to consult when attempting to\n"
    "display MIME bodies not directly supported by NeoMutt.  The default value\n"
    "is generated during startup: see the ``$mailcap'' section of the manual.\n"
    "\n"
    "$$mailcap_path is overridden by the environment variable `$$$MAILCAPS`.\n"
    "\n"
    "The default search path is from RFC1524.\n"
  },
  /*
  ** .pp
  ** This variable specifies which files to consult when attempting to
  ** display MIME bodies not directly supported by NeoMutt.  The default value
  ** is generated during startup: see the ``$mailcap'' section of the manual.
  ** .pp
  ** $$mailcap_path is overridden by the environment variable \fC$$$MAILCAPS\fP.
  ** .pp
  ** The default search path is from RFC1524.
  */
  { "mailcap_sanitize", DT_BOOL, &C_MailcapSanitize, true, 0, NULL,
    "If _set_, NeoMutt will restrict possible characters in mailcap % expandos\n"
    "to a well-defined set of safe characters.  This is the safe setting,\n"
    "but we are not sure it doesn't break some more advanced MIME stuff.\n"
    "\n"
    "**DON'T CHANGE THIS SETTING UNLESS YOU ARE REALLY SURE WHAT YOU ARE\n"
    "DOING!**\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, NeoMutt will restrict possible characters in mailcap % expandos
  ** to a well-defined set of safe characters.  This is the safe setting,
  ** but we are not sure it doesn't break some more advanced MIME stuff.
  ** .pp
  ** \fBDON'T CHANGE THIS SETTING UNLESS YOU ARE REALLY SURE WHAT YOU ARE
  ** DOING!\fP
  */
  { "maildir_check_cur", DT_BOOL, &C_MaildirCheckCur, false, 0, NULL,
    "If _set_, NeoMutt will poll both the new and cur directories of\n"
    "a maildir folder for new messages.  This might be useful if other\n"
    "programs interacting with the folder (e.g. dovecot) are moving new\n"
    "messages to the cur directory.  Note that setting this option may\n"
    "slow down polling for new messages in large folders, since NeoMutt has\n"
    "to scan all cur messages.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, NeoMutt will poll both the new and cur directories of
  ** a maildir folder for new messages.  This might be useful if other
  ** programs interacting with the folder (e.g. dovecot) are moving new
  ** messages to the cur directory.  Note that setting this option may
  ** slow down polling for new messages in large folders, since NeoMutt has
  ** to scan all cur messages.
  */
#ifdef USE_HCACHE
  { "maildir_header_cache_verify", DT_BOOL, &C_MaildirHeaderCacheVerify, true, 0, NULL,
    "Check for Maildir unaware programs other than NeoMutt having modified maildir\n"
    "files when the header cache is in use.  This incurs one `stat(2)` per\n"
    "message every time the folder is opened (which can be very slow for NFS\n"
    "folders).\n"
  },
  /*
  ** .pp
  ** Check for Maildir unaware programs other than NeoMutt having modified maildir
  ** files when the header cache is in use.  This incurs one \fCstat(2)\fP per
  ** message every time the folder is opened (which can be very slow for NFS
  ** folders).
  */
#endif
  { "maildir_trash", DT_BOOL, &C_MaildirTrash, false, 0, NULL,
    "If _set_, messages marked as deleted will be saved with the maildir\n"
    "trashed flag instead of unlinked.  **Note:** this only applies\n"
    "to maildir-style mailboxes.  Setting it will have no effect on other\n"
    "mailbox types.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, messages marked as deleted will be saved with the maildir
  ** trashed flag instead of unlinked.  \fBNote:\fP this only applies
  ** to maildir-style mailboxes.  Setting it will have no effect on other
  ** mailbox types.
  */
  { "mark_macro_prefix", DT_STRING, &C_MarkMacroPrefix, IP "'", 0, NULL,
    "Prefix for macros created using mark-message.  A new macro\n"
    "automatically generated with `<mark-message>a` will be composed\n"
    "from this prefix and the letter _a_.\n"
  },
  /*
  ** .pp
  ** Prefix for macros created using mark-message.  A new macro
  ** automatically generated with \fI<mark-message>a\fP will be composed
  ** from this prefix and the letter \fIa\fP.
  */
  { "mark_old", DT_BOOL|R_INDEX|R_PAGER, &C_MarkOld, true, 0, NULL,
    "Controls whether or not NeoMutt marks _new_ **unread**\n"
    "messages as _old_ if you exit a mailbox without reading them.\n"
    "With this option _set_, the next time you start NeoMutt, the messages\n"
    "will show up with an \"O\" next to them in the index menu,\n"
    "indicating that they are old.\n"
  },
  /*
  ** .pp
  ** Controls whether or not NeoMutt marks \fInew\fP \fBunread\fP
  ** messages as \fIold\fP if you exit a mailbox without reading them.
  ** With this option \fIset\fP, the next time you start NeoMutt, the messages
  ** will show up with an "O" next to them in the index menu,
  ** indicating that they are old.
  */
  { "markers", DT_BOOL|R_PAGER_FLOW, &C_Markers, true, 0, NULL,
    "Controls the display of wrapped lines in the internal pager. If set, a\n"
    "\"+\" marker is displayed at the beginning of wrapped lines.\n"
    "\n"
    "Also see the $$smart_wrap variable.\n"
  },
  /*
  ** .pp
  ** Controls the display of wrapped lines in the internal pager. If set, a
  ** "+" marker is displayed at the beginning of wrapped lines.
  ** .pp
  ** Also see the $$smart_wrap variable.
  */
  { "mask", DT_REGEX|DT_REGEX_MATCH_CASE|DT_REGEX_ALLOW_NOT|DT_REGEX_NOSUB, &C_Mask, IP "!^\\.[^.]", 0, NULL,
    "A regular expression used in the file browser, optionally preceded by\n"
    "the _not_ operator \"!\".  Only files whose names match this mask\n"
    "will be shown. The match is always case-sensitive.\n"
  },
  /*
  ** .pp
  ** A regular expression used in the file browser, optionally preceded by
  ** the \fInot\fP operator "!".  Only files whose names match this mask
  ** will be shown. The match is always case-sensitive.
  */
  { "mbox", DT_STRING|DT_PATH|DT_MAILBOX|R_INDEX|R_PAGER, &C_Mbox, IP "~/mbox", 0, NULL,
    "This specifies the folder into which read mail in your $$spoolfile\n"
    "folder will be appended.\n"
    "\n"
    "Also see the $$move variable.\n"
  },
  /*
  ** .pp
  ** This specifies the folder into which read mail in your $$spoolfile
  ** folder will be appended.
  ** .pp
  ** Also see the $$move variable.
  */
  { "mbox_type", DT_ENUM, &C_MboxType, MUTT_MBOX, IP &MagicDef, NULL,
    "The default mailbox type used when creating new folders. May be any of\n"
    "\"mbox\", \"MMDF\", \"MH\" or \"Maildir\".\n"
    "\n"
    "This can also be set using the `-m` command-line option.\n"
  },
  /*
  ** .pp
  ** The default mailbox type used when creating new folders. May be any of
  ** "mbox", "MMDF", "MH" or "Maildir".
  ** .pp
  ** This can also be set using the \fC-m\fP command-line option.
  */
  { "menu_context", DT_NUMBER|DT_NOT_NEGATIVE, &C_MenuContext, 0, 0, NULL,
    "This variable controls the number of lines of context that are given\n"
    "when scrolling through menus. (Similar to $$pager_context.)\n"
  },
  /*
  ** .pp
  ** This variable controls the number of lines of context that are given
  ** when scrolling through menus. (Similar to $$pager_context.)
  */
  { "menu_move_off", DT_BOOL, &C_MenuMoveOff, true, 0, NULL,
    "When _unset_, the bottom entry of menus will never scroll up past\n"
    "the bottom of the screen, unless there are less entries than lines.\n"
    "When _set_, the bottom entry may move off the bottom.\n"
  },
  /*
  ** .pp
  ** When \fIunset\fP, the bottom entry of menus will never scroll up past
  ** the bottom of the screen, unless there are less entries than lines.
  ** When \fIset\fP, the bottom entry may move off the bottom.
  */
  { "menu_scroll", DT_BOOL, &C_MenuScroll, false, 0, NULL,
    "When _set_, menus will be scrolled up or down one line when you\n"
    "attempt to move across a screen boundary.  If _unset_, the screen\n"
    "is cleared and the next or previous page of the menu is displayed\n"
    "(useful for slow links to avoid many redraws).\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, menus will be scrolled up or down one line when you
  ** attempt to move across a screen boundary.  If \fIunset\fP, the screen
  ** is cleared and the next or previous page of the menu is displayed
  ** (useful for slow links to avoid many redraws).
  */
#if defined(USE_IMAP) || defined(USE_POP)
  { "message_cache_clean", DT_BOOL, &C_MessageCacheClean, false, 0, NULL,
    "If _set_, NeoMutt will clean out obsolete entries from the message cache when\n"
    "the mailbox is synchronized. You probably only want to set it\n"
    "every once in a while, since it can be a little slow\n"
    "(especially for large folders).\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, NeoMutt will clean out obsolete entries from the message cache when
  ** the mailbox is synchronized. You probably only want to set it
  ** every once in a while, since it can be a little slow
  ** (especially for large folders).
  */
  { "message_cachedir", DT_STRING|DT_PATH, &C_MessageCachedir, 0, 0, NULL,
    "Set this to a directory and NeoMutt will cache copies of messages from\n"
    "your IMAP and POP servers here. You are free to remove entries at any\n"
    "time.\n"
    "\n"
    "When setting this variable to a directory, NeoMutt needs to fetch every\n"
    "remote message only once and can perform regular expression searches\n"
    "as fast as for local folders.\n"
    "\n"
    "Also see the $$message_cache_clean variable.\n"
  },
  /*
  ** .pp
  ** Set this to a directory and NeoMutt will cache copies of messages from
  ** your IMAP and POP servers here. You are free to remove entries at any
  ** time.
  ** .pp
  ** When setting this variable to a directory, NeoMutt needs to fetch every
  ** remote message only once and can perform regular expression searches
  ** as fast as for local folders.
  ** .pp
  ** Also see the $$message_cache_clean variable.
  */
#endif
  { "message_format", DT_STRING|DT_NOT_EMPTY, &C_MessageFormat, IP "%s", 0, NULL,
    "This is the string displayed in the \"attachment\" menu for\n"
    "attachments of type `message/rfc822`.  For a full listing of defined\n"
    "`printf(3)`-like sequences see the section on $$index_format.\n"
  },
  /*
  ** .pp
  ** This is the string displayed in the "attachment" menu for
  ** attachments of type \fCmessage/rfc822\fP.  For a full listing of defined
  ** \fCprintf(3)\fP-like sequences see the section on $$index_format.
  */
  { "meta_key", DT_BOOL, &C_MetaKey, false, 0, NULL,
    "If _set_, forces NeoMutt to interpret keystrokes with the high bit (bit 8)\n"
    "set as if the user had pressed the Esc key and whatever key remains\n"
    "after having the high bit removed.  For example, if the key pressed\n"
    "has an ASCII value of `0xf8`, then this is treated as if the user had\n"
    "pressed Esc then \"x\".  This is because the result of removing the\n"
    "high bit from `0xf8` is `0x78`, which is the ASCII character\n"
    "\"x\".\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, forces NeoMutt to interpret keystrokes with the high bit (bit 8)
  ** set as if the user had pressed the Esc key and whatever key remains
  ** after having the high bit removed.  For example, if the key pressed
  ** has an ASCII value of \fC0xf8\fP, then this is treated as if the user had
  ** pressed Esc then "x".  This is because the result of removing the
  ** high bit from \fC0xf8\fP is \fC0x78\fP, which is the ASCII character
  ** "x".
  */
  { "metoo", DT_BOOL, &C_Metoo, false, 0, NULL,
    "If _unset_, NeoMutt will remove your address (see the \"$alternates\"\n"
    "command) from the list of recipients when replying to a message.\n"
  },
  /*
  ** .pp
  ** If \fIunset\fP, NeoMutt will remove your address (see the "$alternates"
  ** command) from the list of recipients when replying to a message.
  */
  { "mh_purge", DT_BOOL, &C_MhPurge, false, 0, NULL,
    "When _unset_, NeoMutt will mimic mh's behavior and rename deleted messages\n"
    "to `,<old file name>` in mh folders instead of really deleting\n"
    "them. This leaves the message on disk but makes programs reading the folder\n"
    "ignore it. If the variable is _set_, the message files will simply be\n"
    "deleted.\n"
    "\n"
    "This option is similar to $$maildir_trash for Maildir folders.\n"
  },
  /*
  ** .pp
  ** When \fIunset\fP, NeoMutt will mimic mh's behavior and rename deleted messages
  ** to \fI,<old file name>\fP in mh folders instead of really deleting
  ** them. This leaves the message on disk but makes programs reading the folder
  ** ignore it. If the variable is \fIset\fP, the message files will simply be
  ** deleted.
  ** .pp
  ** This option is similar to $$maildir_trash for Maildir folders.
  */
  { "mh_seq_flagged", DT_STRING, &C_MhSeqFlagged, IP "flagged", 0, NULL,
    "The name of the MH sequence used for flagged messages.\n"
  },
  /*
  ** .pp
  ** The name of the MH sequence used for flagged messages.
  */
  { "mh_seq_replied", DT_STRING, &C_MhSeqReplied, IP "replied", 0, NULL,
    "The name of the MH sequence used to tag replied messages.\n"
  },
  /*
  ** .pp
  ** The name of the MH sequence used to tag replied messages.
  */
  { "mh_seq_unseen", DT_STRING, &C_MhSeqUnseen, IP "unseen", 0, NULL,
    "The name of the MH sequence used for unseen messages.\n"
  },
  /*
  ** .pp
  ** The name of the MH sequence used for unseen messages.
  */
  { "mime_forward", DT_QUAD, &C_MimeForward, MUTT_NO, 0, NULL,
    "When _set_, the message you are forwarding will be attached as a\n"
    "separate `message/rfc822` MIME part instead of included in the main body of the\n"
    "message.  This is useful for forwarding MIME messages so the receiver\n"
    "can properly view the message as it was delivered to you. If you like\n"
    "to switch between MIME and not MIME from mail to mail, set this\n"
    "variable to \"ask-no\" or \"ask-yes\".\n"
    "\n"
    "Also see $$forward_decode and $$mime_forward_decode.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, the message you are forwarding will be attached as a
  ** separate \fCmessage/rfc822\fP MIME part instead of included in the main body of the
  ** message.  This is useful for forwarding MIME messages so the receiver
  ** can properly view the message as it was delivered to you. If you like
  ** to switch between MIME and not MIME from mail to mail, set this
  ** variable to "ask-no" or "ask-yes".
  ** .pp
  ** Also see $$forward_decode and $$mime_forward_decode.
  */
  { "mime_forward_decode", DT_BOOL, &C_MimeForwardDecode, false, 0, NULL,
    "Controls the decoding of complex MIME messages into `text/plain` when\n"
    "forwarding a message while $$mime_forward is _set_. Otherwise\n"
    "$$forward_decode is used instead.\n"
  },
  /*
  ** .pp
  ** Controls the decoding of complex MIME messages into \fCtext/plain\fP when
  ** forwarding a message while $$mime_forward is \fIset\fP. Otherwise
  ** $$forward_decode is used instead.
  */
  { "mime_forward_rest", DT_QUAD, &C_MimeForwardRest, MUTT_YES, 0, NULL,
    "When forwarding multiple attachments of a MIME message from the attachment\n"
    "menu, attachments which can't be decoded in a reasonable manner will\n"
    "be attached to the newly composed message if this option is _set_.\n"
  },
  /*
  ** .pp
  ** When forwarding multiple attachments of a MIME message from the attachment
  ** menu, attachments which can't be decoded in a reasonable manner will
  ** be attached to the newly composed message if this option is \fIset\fP.
  */
#ifdef USE_NNTP
  { "mime_subject", DT_BOOL, &C_MimeSubject, true, 0, NULL,
    "If _unset_, 8-bit \"subject:\" line in article header will not be\n"
    "encoded according to RFC2047 to base64.  This is useful when message\n"
    "is Usenet article, because MIME for news is nonstandard feature.\n"
  },
  /*
  ** .pp
  ** If \fIunset\fP, 8-bit "subject:" line in article header will not be
  ** encoded according to RFC2047 to base64.  This is useful when message
  ** is Usenet article, because MIME for news is nonstandard feature.
  */
#endif
  { "mime_type_query_command", DT_STRING|DT_COMMAND, &C_MimeTypeQueryCommand, 0, 0, NULL,
    "This specifies a command to run, to determine the mime type of a\n"
    "new attachment when composing a message.  Unless\n"
    "$$mime_type_query_first is set, this will only be run if the\n"
    "attachment's extension is not found in the mime.types file.\n"
    "\n"
    "The string may contain a \"%s\", which will be substituted with the\n"
    "attachment filename.  NeoMutt will add quotes around the string substituted\n"
    "for \"%s\" automatically according to shell quoting rules, so you should\n"
    "avoid adding your own.  If no \"%s\" is found in the string, NeoMutt will\n"
    "append the attachment filename to the end of the string.\n"
    "\n"
    "The command should output a single line containing the\n"
    "attachment's mime type.\n"
    "\n"
    "Suggested values are \"xdg-mime query filetype\" or\n"
    "\"file -bi\".\n"
  },
  /*
  ** .pp
  ** This specifies a command to run, to determine the mime type of a
  ** new attachment when composing a message.  Unless
  ** $$mime_type_query_first is set, this will only be run if the
  ** attachment's extension is not found in the mime.types file.
  ** .pp
  ** The string may contain a "%s", which will be substituted with the
  ** attachment filename.  NeoMutt will add quotes around the string substituted
  ** for "%s" automatically according to shell quoting rules, so you should
  ** avoid adding your own.  If no "%s" is found in the string, NeoMutt will
  ** append the attachment filename to the end of the string.
  ** .pp
  ** The command should output a single line containing the
  ** attachment's mime type.
  ** .pp
  ** Suggested values are "xdg-mime query filetype" or
  ** "file -bi".
  */
  { "mime_type_query_first", DT_BOOL, &C_MimeTypeQueryFirst, false, 0, NULL,
    "When _set_, the $$mime_type_query_command will be run before the\n"
    "mime.types lookup.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, the $$mime_type_query_command will be run before the
  ** mime.types lookup.
  */
#ifdef MIXMASTER
  { "mix_entry_format", DT_STRING|DT_NOT_EMPTY, &C_MixEntryFormat, IP "%4n %c %-16s %a", 0, NULL,
    "This variable describes the format of a remailer line on the mixmaster\n"
    "chain selection screen.  The following `printf(3)`-like sequences are\n"
    "supported:\n"
    "\n"
    "| .  | Description |\n"
    "|----+--------------------------------|\n"
    "| %a | The remailer's e-mail address |\n"
    "| %c | Remailer capabilities |\n"
    "| %n | The running number on the menu |\n"
    "| %s | The remailer's short name |\n"
  },
  /*
  ** .pp
  ** This variable describes the format of a remailer line on the mixmaster
  ** chain selection screen.  The following \fCprintf(3)\fP-like sequences are
  ** supported:
  ** .dl
  ** .dt %a .dd The remailer's e-mail address
  ** .dt %c .dd Remailer capabilities
  ** .dt %n .dd The running number on the menu
  ** .dt %s .dd The remailer's short name
  ** .de
  */
  { "mixmaster", DT_STRING|DT_COMMAND, &C_Mixmaster, IP MIXMASTER, 0, NULL,
    "This variable contains the path to the Mixmaster binary on your\n"
    "system.  It is used with various sets of parameters to gather the\n"
    "list of known remailers, and to finally send a message through the\n"
    "mixmaster chain.\n"
  },
  /*
  ** .pp
  ** This variable contains the path to the Mixmaster binary on your
  ** system.  It is used with various sets of parameters to gather the
  ** list of known remailers, and to finally send a message through the
  ** mixmaster chain.
  */
#endif
  { "move", DT_QUAD, &C_Move, MUTT_NO, 0, NULL,
    "Controls whether or not NeoMutt will move read messages\n"
    "from your spool mailbox to your $$mbox mailbox, or as a result of\n"
    "a \"$mbox-hook\" command.\n"
  },
  /*
  ** .pp
  ** Controls whether or not NeoMutt will move read messages
  ** from your spool mailbox to your $$mbox mailbox, or as a result of
  ** a "$mbox-hook" command.
  */
  { "narrow_tree", DT_BOOL|R_TREE|R_INDEX, &C_NarrowTree, false, 0, NULL,
    "This variable, when _set_, makes the thread tree narrower, allowing\n"
    "deeper threads to fit on the screen.\n"
  },
  /*
  ** .pp
  ** This variable, when \fIset\fP, makes the thread tree narrower, allowing
  ** deeper threads to fit on the screen.
  */
#ifdef USE_SOCKET
  { "net_inc", DT_NUMBER|DT_NOT_NEGATIVE, &C_NetInc, 10, 0, NULL,
    "Operations that expect to transfer a large amount of data over the\n"
    "network will update their progress every $$net_inc kilobytes.\n"
    "If set to 0, no progress messages will be displayed.\n"
    "\n"
    "See also $$read_inc, $$write_inc and $$net_inc.\n"
  },
  /*
  ** .pp
  ** Operations that expect to transfer a large amount of data over the
  ** network will update their progress every $$net_inc kilobytes.
  ** If set to 0, no progress messages will be displayed.
  ** .pp
  ** See also $$read_inc, $$write_inc and $$net_inc.
  */
#endif
  { "new_mail_command", DT_STRING|DT_COMMAND, &C_NewMailCommand, 0, 0, NULL,
    "If _set_, NeoMutt will call this command after a new message is received.\n"
    "See the $$status_format documentation for the values that can be formatted\n"
    "into this command.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, NeoMutt will call this command after a new message is received.
  ** See the $$status_format documentation for the values that can be formatted
  ** into this command.
  */
#ifdef USE_NNTP
  { "news_cache_dir", DT_STRING|DT_PATH, &C_NewsCacheDir, IP "~/.neomutt", 0, NULL,
    "This variable pointing to directory where NeoMutt will save cached news\n"
    "articles and headers in. If _unset_, articles and headers will not be\n"
    "saved at all and will be reloaded from the server each time.\n"
  },
  /*
  ** .pp
  ** This variable pointing to directory where NeoMutt will save cached news
  ** articles and headers in. If \fIunset\fP, articles and headers will not be
  ** saved at all and will be reloaded from the server each time.
  */
  { "news_server", DT_STRING, &C_NewsServer, 0, 0, NULL,
    "This variable specifies domain name or address of NNTP server.\n"
    "\n"
    "You can also specify username and an alternative port for each news server,\n"
    "e.g. `[[s]news://][username[:password]@]server[:port]`\n"
    "\n"
    "This option can also be set using the command line option \"-g\", the\n"
    "environment variable `$$$NNTPSERVER`, or putting the server name in the\n"
    "file \"/etc/nntpserver\".\n"
  },
  /*
  ** .pp
  ** This variable specifies domain name or address of NNTP server.
  ** .pp
  ** You can also specify username and an alternative port for each news server,
  ** e.g. \fC[[s]news://][username[:password]@]server[:port]\fP
  ** .pp
  ** This option can also be set using the command line option "-g", the
  ** environment variable \fC$$$NNTPSERVER\fP, or putting the server name in the
  ** file "/etc/nntpserver".
  */
  { "newsgroups_charset", DT_STRING, &C_NewsgroupsCharset, IP "utf-8", 0, charset_validator,
    "Character set of newsgroups descriptions.\n"
  },
  /*
  ** .pp
  ** Character set of newsgroups descriptions.
  */
  { "newsrc", DT_STRING|DT_PATH, &C_Newsrc, IP "~/.newsrc", 0, NULL,
    "The file, containing info about subscribed newsgroups - names and\n"
    "indexes of read articles.  The following printf-style sequence\n"
    "is understood:\n"
    "\n"
    "| Expando | Description       | Example |\n"
    "|---------+-------------------+-----------------------|\n"
    "| %a      | Account url       | `news:news.gmane.org` |\n"
    "| %p      | Port              | `119` |\n"
    "| %P      | Port if specified | `10119` |\n"
    "| %s      | News server name  | `news.gmane.org` |\n"
    "| %S      | Url schema        | `news` |\n"
    "| %u      | Username          | `username` |\n"
  },
  /*
  ** .pp
  ** The file, containing info about subscribed newsgroups - names and
  ** indexes of read articles.  The following printf-style sequence
  ** is understood:
  ** .dl
  ** .dt \fBExpando\fP .dd \fBDescription\fP .dd \fBExample\fP
  ** .dt %a .dd Account url       .dd \fCnews:news.gmane.org\fP
  ** .dt %p .dd Port              .dd \fC119\fP
  ** .dt %P .dd Port if specified .dd \fC10119\fP
  ** .dt %s .dd News server name  .dd \fCnews.gmane.org\fP
  ** .dt %S .dd Url schema        .dd \fCnews\fP
  ** .dt %u .dd Username          .dd \fCusername\fP
  ** .de
  */
#endif
#ifdef USE_NOTMUCH
  { "nm_db_limit", DT_NUMBER|DT_NOT_NEGATIVE, &C_NmDbLimit, 0, 0, NULL,
    "This variable specifies the default limit used in notmuch queries.\n"
  },
  /*
  ** .pp
  ** This variable specifies the default limit used in notmuch queries.
  */
  { "nm_default_uri", DT_STRING, &C_NmDefaultUri, 0, 0, NULL,
    "This variable specifies the default Notmuch database in format\n"
    "notmuch://<absolute path>.\n"
  },
  /*
  ** .pp
  ** This variable specifies the default Notmuch database in format
  ** notmuch://<absolute path>.
  */
  { "nm_exclude_tags", DT_STRING, &C_NmExcludeTags, 0, 0, NULL,
    "The messages tagged with these tags are excluded and not loaded\n"
    "from notmuch DB to NeoMutt unless specified explicitly.\n"
  },
  /*
  ** .pp
  ** The messages tagged with these tags are excluded and not loaded
  ** from notmuch DB to NeoMutt unless specified explicitly.
  */
  { "nm_flagged_tag", DT_STRING, &C_NmFlaggedTag, IP "flagged", 0, NULL,
    "This variable specifies notmuch tag which is used for flagged messages. The\n"
    "variable is used to count flagged messages in DB and set the flagged flag when\n"
    "modifying tags. All other NeoMutt commands use standard (e.g. maildir) flags.\n"
  },
  /*
  ** .pp
  ** This variable specifies notmuch tag which is used for flagged messages. The
  ** variable is used to count flagged messages in DB and set the flagged flag when
  ** modifying tags. All other NeoMutt commands use standard (e.g. maildir) flags.
  */
  { "nm_open_timeout", DT_NUMBER|DT_NOT_NEGATIVE, &C_NmOpenTimeout, 5, 0, NULL,
    "This variable specifies the timeout for database open in seconds.\n"
  },
  /*
  ** .pp
  ** This variable specifies the timeout for database open in seconds.
  */
  { "nm_query_type", DT_STRING, &C_NmQueryType, IP "messages", 0, NULL,
    "This variable specifies the default query type (threads or messages) used in notmuch queries.\n"
  },
  /*
  ** .pp
  ** This variable specifies the default query type (threads or messages) used in notmuch queries.
  */
  { "nm_query_window_current_position", DT_NUMBER, &C_NmQueryWindowCurrentPosition, 0, 0, NULL,
    "This variable contains the position of the current search for window based vfolder.\n"
  },
  /*
  ** .pp
  ** This variable contains the position of the current search for window based vfolder.
  */
  { "nm_query_window_current_search", DT_STRING, &C_NmQueryWindowCurrentSearch, 0, 0, NULL,
    "This variable contains the currently setup notmuch search for window based vfolder.\n"
  },
  /*
  ** .pp
  ** This variable contains the currently setup notmuch search for window based vfolder.
  */
  { "nm_query_window_duration", DT_NUMBER|DT_NOT_NEGATIVE, &C_NmQueryWindowDuration, 0, 0, NULL,
    "This variable sets the time duration of a windowed notmuch query.\n"
    "Accepted values all non negative integers. A value of 0 disables the feature.\n"
  },
  /*
  ** .pp
  ** This variable sets the time duration of a windowed notmuch query.
  ** Accepted values all non negative integers. A value of 0 disables the feature.
  */
  { "nm_query_window_timebase", DT_STRING, &C_NmQueryWindowTimebase, IP "week", 0, NULL,
    "This variable sets the time base of a windowed notmuch query.\n"
    "Accepted values are 'minute', 'hour', 'day', 'week', 'month', 'year'\n"
  },
  /*
  ** .pp
  ** This variable sets the time base of a windowed notmuch query.
  ** Accepted values are 'minute', 'hour', 'day', 'week', 'month', 'year'
  */
  { "nm_record", DT_BOOL, &C_NmRecord, false, 0, NULL,
    "This variable specifies if the NeoMutt record should indexed by notmuch.\n"
  },
  /*
  ** .pp
  ** This variable specifies if the NeoMutt record should indexed by notmuch.
  */
  { "nm_record_tags", DT_STRING, &C_NmRecordTags, 0, 0, NULL,
    "This variable specifies the default tags applied to messages stored to the NeoMutt record.\n"
    "When set to 0 this variable disable the window feature.\n"
  },
  /*
  ** .pp
  ** This variable specifies the default tags applied to messages stored to the NeoMutt record.
  ** When set to 0 this variable disable the window feature.
  */
  { "nm_replied_tag", DT_STRING, &C_NmRepliedTag, IP "replied", 0, NULL,
    "This variable specifies notmuch tag which is used for replied messages. The\n"
    "variable is used to set the replied flag when modifiying tags. All other NeoMutt\n"
    "commands use standard (e.g. maildir) flags.\n"
  },
  /*
  ** .pp
  ** This variable specifies notmuch tag which is used for replied messages. The
  ** variable is used to set the replied flag when modifiying tags. All other NeoMutt
  ** commands use standard (e.g. maildir) flags.
  */
  { "nm_unread_tag", DT_STRING, &C_NmUnreadTag, IP "unread", 0, NULL,
    "This variable specifies notmuch tag which is used for unread messages. The\n"
    "variable is used to count unread messages in DB and set the unread flag when\n"
    "modifiying tags. All other NeoMutt commands use standard (e.g. maildir) flags.\n"
  },
  /*
  ** .pp
  ** This variable specifies notmuch tag which is used for unread messages. The
  ** variable is used to count unread messages in DB and set the unread flag when
  ** modifiying tags. All other NeoMutt commands use standard (e.g. maildir) flags.
  */
#endif
#ifdef USE_NNTP
  { "nntp_authenticators", DT_STRING, &C_NntpAuthenticators, 0, 0, NULL,
    "This is a colon-delimited list of authentication methods NeoMutt may\n"
    "attempt to use to log in to a news server, in the order NeoMutt should\n"
    "try them.  Authentication methods are either \"user\" or any\n"
    "SASL mechanism, e.g. \"digest-md5\", \"gssapi\" or \"cram-md5\".\n"
    "This option is case-insensitive.  If it's _unset_ (the default)\n"
    "NeoMutt will try all available methods, in order from most-secure to\n"
    "least-secure.\n"
    "\n"
    "Example:\n"
    "```\n"
    "set nntp_authenticators=\"digest-md5:user\"\n"
    "```\n"
    "\n"
    "**Note:** NeoMutt will only fall back to other authentication methods if\n"
    "the previous methods are unavailable. If a method is available but\n"
    "authentication fails, NeoMutt will not connect to the IMAP server.\n"
  },
  /*
  ** .pp
  ** This is a colon-delimited list of authentication methods NeoMutt may
  ** attempt to use to log in to a news server, in the order NeoMutt should
  ** try them.  Authentication methods are either "user" or any
  ** SASL mechanism, e.g. "digest-md5", "gssapi" or "cram-md5".
  ** This option is case-insensitive.  If it's \fIunset\fP (the default)
  ** NeoMutt will try all available methods, in order from most-secure to
  ** least-secure.
  ** .pp
  ** Example:
  ** .ts
  ** set nntp_authenticators="digest-md5:user"
  ** .te
  ** .pp
  ** \fBNote:\fP NeoMutt will only fall back to other authentication methods if
  ** the previous methods are unavailable. If a method is available but
  ** authentication fails, NeoMutt will not connect to the IMAP server.
  */
  { "nntp_context", DT_NUMBER|DT_NOT_NEGATIVE, &C_NntpContext, 1000, 0, NULL,
    "This variable defines number of articles which will be in index when\n"
    "newsgroup entered.  If active newsgroup have more articles than this\n"
    "number, oldest articles will be ignored.  Also controls how many\n"
    "articles headers will be saved in cache when you quit newsgroup.\n"
  },
  /*
  ** .pp
  ** This variable defines number of articles which will be in index when
  ** newsgroup entered.  If active newsgroup have more articles than this
  ** number, oldest articles will be ignored.  Also controls how many
  ** articles headers will be saved in cache when you quit newsgroup.
  */
  { "nntp_listgroup", DT_BOOL, &C_NntpListgroup, true, 0, NULL,
    "This variable controls whether or not existence of each article is\n"
    "checked when newsgroup is entered.\n"
  },
  /*
  ** .pp
  ** This variable controls whether or not existence of each article is
  ** checked when newsgroup is entered.
  */
  { "nntp_load_description", DT_BOOL, &C_NntpLoadDescription, true, 0, NULL,
    "This variable controls whether or not descriptions for each newsgroup\n"
    "must be loaded when newsgroup is added to list (first time list\n"
    "loading or new newsgroup adding).\n"
  },
  /*
  ** .pp
  ** This variable controls whether or not descriptions for each newsgroup
  ** must be loaded when newsgroup is added to list (first time list
  ** loading or new newsgroup adding).
  */
  { "nntp_pass", DT_STRING|DT_SENSITIVE, &C_NntpPass, 0, 0, NULL,
    "Your password for NNTP account.\n"
  },
  /*
  ** .pp
  ** Your password for NNTP account.
  */
  { "nntp_poll", DT_NUMBER|DT_NOT_NEGATIVE, &C_NntpPoll, 60, 0, NULL,
    "The time in seconds until any operations on newsgroup except post new\n"
    "article will cause recheck for new news.  If set to 0, NeoMutt will\n"
    "recheck newsgroup on each operation in index (stepping, read article,\n"
    "etc.).\n"
  },
  /*
  ** .pp
  ** The time in seconds until any operations on newsgroup except post new
  ** article will cause recheck for new news.  If set to 0, NeoMutt will
  ** recheck newsgroup on each operation in index (stepping, read article,
  ** etc.).
  */
  { "nntp_user", DT_STRING|DT_SENSITIVE, &C_NntpUser, 0, 0, NULL,
    "Your login name on the NNTP server.  If _unset_ and NNTP server requires\n"
    "authentication, NeoMutt will prompt you for your account name when you\n"
    "connect to news server.\n"
  },
  /*
  ** .pp
  ** Your login name on the NNTP server.  If \fIunset\fP and NNTP server requires
  ** authentication, NeoMutt will prompt you for your account name when you
  ** connect to news server.
  */
#endif
  { "pager", DT_STRING|DT_COMMAND, &C_Pager, IP "builtin", 0, NULL,
    "This variable specifies which pager you would like to use to view\n"
    "messages. The value \"builtin\" means to use the built-in pager, otherwise this\n"
    "variable should specify the pathname of the external pager you would\n"
    "like to use.\n"
    "\n"
    "Using an external pager may have some disadvantages: Additional\n"
    "keystrokes are necessary because you can't call NeoMutt functions\n"
    "directly from the pager, and screen resizes cause lines longer than\n"
    "the screen width to be badly formatted in the help menu.\n"
  },
  /*
  ** .pp
  ** This variable specifies which pager you would like to use to view
  ** messages. The value "builtin" means to use the built-in pager, otherwise this
  ** variable should specify the pathname of the external pager you would
  ** like to use.
  ** .pp
  ** Using an external pager may have some disadvantages: Additional
  ** keystrokes are necessary because you can't call NeoMutt functions
  ** directly from the pager, and screen resizes cause lines longer than
  ** the screen width to be badly formatted in the help menu.
  */
  { "pager_context", DT_NUMBER|DT_NOT_NEGATIVE, &C_PagerContext, 0, 0, NULL,
    "This variable controls the number of lines of context that are given\n"
    "when displaying the next or previous page in the internal pager.  By\n"
    "default, NeoMutt will display the line after the last one on the screen\n"
    "at the top of the next page (0 lines of context).\n"
    "\n"
    "This variable also specifies the amount of context given for search\n"
    "results. If positive, this many lines will be given before a match,\n"
    "if 0, the match will be top-aligned.\n"
  },
  /*
  ** .pp
  ** This variable controls the number of lines of context that are given
  ** when displaying the next or previous page in the internal pager.  By
  ** default, NeoMutt will display the line after the last one on the screen
  ** at the top of the next page (0 lines of context).
  ** .pp
  ** This variable also specifies the amount of context given for search
  ** results. If positive, this many lines will be given before a match,
  ** if 0, the match will be top-aligned.
  */
  { "pager_format", DT_STRING|R_PAGER, &C_PagerFormat, IP "-%Z- %C/%m: %-20.20n   %s%*  -- (%P)", 0, NULL,
    "This variable controls the format of the one-line message \"status\"\n"
    "displayed before each message in either the internal or an external\n"
    "pager.  The valid sequences are listed in the $$index_format\n"
    "section.\n"
  },
  /*
  ** .pp
  ** This variable controls the format of the one-line message "status"
  ** displayed before each message in either the internal or an external
  ** pager.  The valid sequences are listed in the $$index_format
  ** section.
  */
  { "pager_index_lines", DT_NUMBER|DT_NOT_NEGATIVE|R_PAGER|R_REFLOW, &C_PagerIndexLines, 0, 0, NULL,
    "Determines the number of lines of a mini-index which is shown when in\n"
    "the pager.  The current message, unless near the top or bottom of the\n"
    "folder, will be roughly one third of the way down this mini-index,\n"
    "giving the reader the context of a few messages before and after the\n"
    "message.  This is useful, for example, to determine how many messages\n"
    "remain to be read in the current thread.  One of the lines is reserved\n"
    "for the status bar from the index, so a setting of 6\n"
    "will only show 5 lines of the actual index.  A value of 0 results in\n"
    "no index being shown.  If the number of messages in the current folder\n"
    "is less than $$pager_index_lines, then the index will only use as\n"
    "many lines as it needs.\n"
  },
  /*
  ** .pp
  ** Determines the number of lines of a mini-index which is shown when in
  ** the pager.  The current message, unless near the top or bottom of the
  ** folder, will be roughly one third of the way down this mini-index,
  ** giving the reader the context of a few messages before and after the
  ** message.  This is useful, for example, to determine how many messages
  ** remain to be read in the current thread.  One of the lines is reserved
  ** for the status bar from the index, so a setting of 6
  ** will only show 5 lines of the actual index.  A value of 0 results in
  ** no index being shown.  If the number of messages in the current folder
  ** is less than $$pager_index_lines, then the index will only use as
  ** many lines as it needs.
  */
  { "pager_stop", DT_BOOL, &C_PagerStop, false, 0, NULL,
    "When _set_, the internal-pager will **not** move to the next message\n"
    "when you are at the end of a message and invoke the `<next-page>`\n"
    "function.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, the internal-pager will \fBnot\fP move to the next message
  ** when you are at the end of a message and invoke the \fC<next-page>\fP
  ** function.
  */
  { "pgp_auto_decode", DT_BOOL, &C_PgpAutoDecode, false, 0, NULL,
    "If _set_, NeoMutt will automatically attempt to decrypt traditional PGP\n"
    "messages whenever the user performs an operation which ordinarily would\n"
    "result in the contents of the message being operated on.  For example,\n"
    "if the user displays a pgp-traditional message which has not been manually\n"
    "checked with the `$<check-traditional-pgp>` function, NeoMutt will automatically\n"
    "check the message for traditional pgp.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, NeoMutt will automatically attempt to decrypt traditional PGP
  ** messages whenever the user performs an operation which ordinarily would
  ** result in the contents of the message being operated on.  For example,
  ** if the user displays a pgp-traditional message which has not been manually
  ** checked with the \fC$<check-traditional-pgp>\fP function, NeoMutt will automatically
  ** check the message for traditional pgp.
  */
  { "pgp_autoinline", DT_BOOL, &C_PgpAutoinline, false, 0, NULL,
    "This option controls whether NeoMutt generates old-style inline\n"
    "(traditional) PGP encrypted or signed messages under certain\n"
    "circumstances.  This can be overridden by use of the pgp menu,\n"
    "when inline is not required.  The GPGME backend does not support\n"
    "this option.\n"
    "\n"
    "Note that NeoMutt might automatically use PGP/MIME for messages\n"
    "which consist of more than a single MIME part.  NeoMutt can be\n"
    "configured to ask before sending PGP/MIME messages when inline\n"
    "(traditional) would not work.\n"
    "\n"
    "Also see the $$pgp_mime_auto variable.\n"
    "\n"
    "Also note that using the old-style PGP message format is **strongly**\n"
    "**deprecated**.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** This option controls whether NeoMutt generates old-style inline
  ** (traditional) PGP encrypted or signed messages under certain
  ** circumstances.  This can be overridden by use of the pgp menu,
  ** when inline is not required.  The GPGME backend does not support
  ** this option.
  ** .pp
  ** Note that NeoMutt might automatically use PGP/MIME for messages
  ** which consist of more than a single MIME part.  NeoMutt can be
  ** configured to ask before sending PGP/MIME messages when inline
  ** (traditional) would not work.
  ** .pp
  ** Also see the $$pgp_mime_auto variable.
  ** .pp
  ** Also note that using the old-style PGP message format is \fBstrongly\fP
  ** \fBdeprecated\fP.
  ** (PGP only)
  */
#ifdef CRYPT_BACKEND_CLASSIC_PGP
  { "pgp_check_exit", DT_BOOL, &C_PgpCheckExit, true, 0, NULL,
    "If _set_, NeoMutt will check the exit code of the PGP subprocess when\n"
    "signing or encrypting.  A non-zero exit code means that the\n"
    "subprocess failed.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, NeoMutt will check the exit code of the PGP subprocess when
  ** signing or encrypting.  A non-zero exit code means that the
  ** subprocess failed.
  ** (PGP only)
  */
  { "pgp_check_gpg_decrypt_status_fd", DT_BOOL, &C_PgpCheckGpgDecryptStatusFd, true, 0, NULL,
    "If _set_, NeoMutt will check the status file descriptor output\n"
    "of $$pgp_decrypt_command and $$pgp_decode_command for GnuPG status codes\n"
    "indicating successful decryption.  This will check for the presence of\n"
    "DECRYPTION_OKAY, absence of DECRYPTION_FAILED, and that all\n"
    "PLAINTEXT occurs between the BEGIN_DECRYPTION and END_DECRYPTION\n"
    "status codes.\n"
    "\n"
    "If _unset_, NeoMutt will instead match the status fd output\n"
    "against $$pgp_decryption_okay.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, NeoMutt will check the status file descriptor output
  ** of $$pgp_decrypt_command and $$pgp_decode_command for GnuPG status codes
  ** indicating successful decryption.  This will check for the presence of
  ** DECRYPTION_OKAY, absence of DECRYPTION_FAILED, and that all
  ** PLAINTEXT occurs between the BEGIN_DECRYPTION and END_DECRYPTION
  ** status codes.
  ** .pp
  ** If \fIunset\fP, NeoMutt will instead match the status fd output
  ** against $$pgp_decryption_okay.
  ** (PGP only)
  */
  { "pgp_clearsign_command", DT_STRING|DT_COMMAND, &C_PgpClearsignCommand, 0, 0, NULL,
    "This format is used to create an old-style \"clearsigned\" PGP\n"
    "message.  Note that the use of this format is **strongly**\n"
    "**deprecated**.\n"
    "\n"
    "This is a format string, see the $$pgp_decode_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "Note that in this case, %r expands to the search string, which is a list of\n"
    "one or more quoted values such as email address, name, or keyid.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** This format is used to create an old-style "clearsigned" PGP
  ** message.  Note that the use of this format is \fBstrongly\fP
  ** \fBdeprecated\fP.
  ** .pp
  ** This is a format string, see the $$pgp_decode_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** Note that in this case, %r expands to the search string, which is a list of
  ** one or more quoted values such as email address, name, or keyid.
  ** (PGP only)
  */
  { "pgp_decode_command", DT_STRING|DT_COMMAND, &C_PgpDecodeCommand, 0, 0, NULL,
    "This format strings specifies a command which is used to decode\n"
    "application/pgp attachments.\n"
    "\n"
    "The PGP command formats have their own set of `printf(3)`-like sequences:\n"
    "\n"
    "| .  | Description |\n"
    "|----+--------------------------------------------------------------------------------------------------------------------------------|\n"
    "| %p | Expands to PGPPASSFD=0 when a pass phrase is needed, to an empty string otherwise. Note: This may be used with a %? construct. |\n"
    "| %f | Expands to the name of a file containing a message. |\n"
    "| %s | Expands to the name of a file containing the signature part of a `multipart/signed` attachment when verifying it. |\n"
    "| %a | The value of $$pgp_sign_as if set, otherwise the value of $$pgp_default_key. |\n"
    "| %r | One or more key IDs (or fingerprints if available). |\n"
    "\n"
    "For examples on how to configure these formats for the various versions\n"
    "of PGP which are floating around, see the pgp and gpg sample configuration files in\n"
    "the `samples/` subdirectory which has been installed on your system\n"
    "alongside the documentation.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** This format strings specifies a command which is used to decode
  ** application/pgp attachments.
  ** .pp
  ** The PGP command formats have their own set of \fCprintf(3)\fP-like sequences:
  ** .dl
  ** .dt %p .dd Expands to PGPPASSFD=0 when a pass phrase is needed, to an empty
  **            string otherwise. Note: This may be used with a %? construct.
  ** .dt %f .dd Expands to the name of a file containing a message.
  ** .dt %s .dd Expands to the name of a file containing the signature part
  ** .          of a \fCmultipart/signed\fP attachment when verifying it.
  ** .dt %a .dd The value of $$pgp_sign_as if set, otherwise the value
  **            of $$pgp_default_key.
  ** .dt %r .dd One or more key IDs (or fingerprints if available).
  ** .de
  ** .pp
  ** For examples on how to configure these formats for the various versions
  ** of PGP which are floating around, see the pgp and gpg sample configuration files in
  ** the \fCsamples/\fP subdirectory which has been installed on your system
  ** alongside the documentation.
  ** (PGP only)
  */
  { "pgp_decrypt_command", DT_STRING|DT_COMMAND, &C_PgpDecryptCommand, 0, 0, NULL,
    "This command is used to decrypt a PGP encrypted message.\n"
    "\n"
    "This is a format string, see the $$pgp_decode_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "(PGP only)\n"
    "\n"
    "Note: When decrypting messages using `gpg`, a pinentry program needs to\n"
    "be invoked unless the password is cached within `gpg-agent`.\n"
    "Currently, the `pinentry-tty` program (usually distributed with\n"
    "`gpg`) isn't suitable for being invoked by NeoMutt.  You are encouraged\n"
    "to use a different pinentry-program when running NeoMutt in order to avoid\n"
    "problems.\n"
    "\n"
    "See also: https://github.com/neomutt/neomutt/issues/1014\n"
  },
  /*
  ** .pp
  ** This command is used to decrypt a PGP encrypted message.
  ** .pp
  ** This is a format string, see the $$pgp_decode_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** (PGP only)
  ** .pp
  ** Note: When decrypting messages using \fCgpg\fP, a pinentry program needs to
  ** be invoked unless the password is cached within \fCgpg-agent\fP.
  ** Currently, the \fCpinentry-tty\fP program (usually distributed with
  ** \fCgpg\fP) isn't suitable for being invoked by NeoMutt.  You are encouraged
  ** to use a different pinentry-program when running NeoMutt in order to avoid
  ** problems.
  ** .pp
  ** See also: https://github.com/neomutt/neomutt/issues/1014
  */
  { "pgp_decryption_okay", DT_REGEX, &C_PgpDecryptionOkay, 0, 0, NULL,
    "If you assign text to this variable, then an encrypted PGP\n"
    "message is only considered successfully decrypted if the output\n"
    "from $$pgp_decrypt_command contains the text.  This is used to\n"
    "protect against a spoofed encrypted message, with multipart/encrypted\n"
    "headers but containing a block that is not actually encrypted.\n"
    "(e.g. simply signed and ascii armored text).\n"
    "\n"
    "Note that if $$pgp_check_gpg_decrypt_status_fd is set, this variable\n"
    "is ignored.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** If you assign text to this variable, then an encrypted PGP
  ** message is only considered successfully decrypted if the output
  ** from $$pgp_decrypt_command contains the text.  This is used to
  ** protect against a spoofed encrypted message, with multipart/encrypted
  ** headers but containing a block that is not actually encrypted.
  ** (e.g. simply signed and ascii armored text).
  ** .pp
  ** Note that if $$pgp_check_gpg_decrypt_status_fd is set, this variable
  ** is ignored.
  ** (PGP only)
  */
#endif
  { "pgp_default_key", DT_STRING, &C_PgpDefaultKey, 0, 0, NULL,
    "This is the default key-pair to use for PGP operations.  It will be\n"
    "used for encryption (see $$postpone_encrypt and $$pgp_self_encrypt).\n"
    "\n"
    "It will also be used for signing unless $$pgp_sign_as is set.\n"
    "\n"
    "The (now deprecated) _pgp_self_encrypt_as_ is an alias for this\n"
    "variable, and should no longer be used.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** This is the default key-pair to use for PGP operations.  It will be
  ** used for encryption (see $$postpone_encrypt and $$pgp_self_encrypt).
  ** .pp
  ** It will also be used for signing unless $$pgp_sign_as is set.
  ** .pp
  ** The (now deprecated) \fIpgp_self_encrypt_as\fP is an alias for this
  ** variable, and should no longer be used.
  ** (PGP only)
  */
#ifdef CRYPT_BACKEND_CLASSIC_PGP
  { "pgp_encrypt_only_command", DT_STRING|DT_COMMAND, &C_PgpEncryptOnlyCommand, 0, 0, NULL,
    "This command is used to encrypt a body part without signing it.\n"
    "\n"
    "This is a format string, see the $$pgp_decode_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "Note that in this case, %r expands to the search string, which is a list of\n"
    "one or more quoted values such as email address, name, or keyid.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** This command is used to encrypt a body part without signing it.
  ** .pp
  ** This is a format string, see the $$pgp_decode_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** Note that in this case, %r expands to the search string, which is a list of
  ** one or more quoted values such as email address, name, or keyid.
  ** (PGP only)
  */
  { "pgp_encrypt_sign_command", DT_STRING|DT_COMMAND, &C_PgpEncryptSignCommand, 0, 0, NULL,
    "This command is used to both sign and encrypt a body part.\n"
    "\n"
    "This is a format string, see the $$pgp_decode_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** This command is used to both sign and encrypt a body part.
  ** .pp
  ** This is a format string, see the $$pgp_decode_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** (PGP only)
  */
#endif
  { "pgp_entry_format", DT_STRING|DT_NOT_EMPTY, &C_PgpEntryFormat, IP "%4n %t%f %4l/0x%k %-4a %2c %u", 0, NULL,
    "This variable allows you to customize the PGP key selection menu to\n"
    "your personal taste. This string is similar to $$index_format, but\n"
    "has its own set of `printf(3)`-like sequences:\n"
    "\n"
    "| .      | Description |\n"
    "|--------+----------------------------------------------------------|\n"
    "| %a     | Algorithm |\n"
    "| %c     | Capabilities |\n"
    "| %f     | Flags |\n"
    "| %k     | Key id |\n"
    "| %l     | Key length |\n"
    "| %n     | Number |\n"
    "| %p     | Protocol |\n"
    "| %t     | Trust/validity of the key-uid association |\n"
    "| %u     | User id |\n"
    "| %[<s>] | Date of the key where <s> is an `strftime(3)` expression |\n"
    "\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** This variable allows you to customize the PGP key selection menu to
  ** your personal taste. This string is similar to $$index_format, but
  ** has its own set of \fCprintf(3)\fP-like sequences:
  ** .dl
  ** .dt %a     .dd Algorithm
  ** .dt %c     .dd Capabilities
  ** .dt %f     .dd Flags
  ** .dt %k     .dd Key id
  ** .dt %l     .dd Key length
  ** .dt %n     .dd Number
  ** .dt %p     .dd Protocol
  ** .dt %t     .dd Trust/validity of the key-uid association
  ** .dt %u     .dd User id
  ** .dt %[<s>] .dd Date of the key where <s> is an \fCstrftime(3)\fP expression
  ** .de
  ** .pp
  ** (PGP only)
  */
#ifdef CRYPT_BACKEND_CLASSIC_PGP
  { "pgp_export_command", DT_STRING|DT_COMMAND, &C_PgpExportCommand, 0, 0, NULL,
    "This command is used to export a public key from the user's\n"
    "key ring.\n"
    "\n"
    "This is a format string, see the $$pgp_decode_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** This command is used to export a public key from the user's
  ** key ring.
  ** .pp
  ** This is a format string, see the $$pgp_decode_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** (PGP only)
  */
  { "pgp_getkeys_command", DT_STRING|DT_COMMAND, &C_PgpGetkeysCommand, 0, 0, NULL,
    "This command is invoked whenever NeoMutt needs to fetch the public key associated with\n"
    "an email address.  Of the sequences supported by $$pgp_decode_command, %r is\n"
    "the only `printf(3)`-like sequence used with this format.  Note that\n"
    "in this case, %r expands to the email address, not the public key ID (the key ID is\n"
    "unknown, which is why NeoMutt is invoking this command).\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** This command is invoked whenever NeoMutt needs to fetch the public key associated with
  ** an email address.  Of the sequences supported by $$pgp_decode_command, %r is
  ** the only \fCprintf(3)\fP-like sequence used with this format.  Note that
  ** in this case, %r expands to the email address, not the public key ID (the key ID is
  ** unknown, which is why NeoMutt is invoking this command).
  ** (PGP only)
  */
  { "pgp_good_sign", DT_REGEX, &C_PgpGoodSign, 0, 0, NULL,
    "If you assign a text to this variable, then a PGP signature is only\n"
    "considered verified if the output from $$pgp_verify_command contains\n"
    "the text. Use this variable if the exit code from the command is 0\n"
    "even for bad signatures.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** If you assign a text to this variable, then a PGP signature is only
  ** considered verified if the output from $$pgp_verify_command contains
  ** the text. Use this variable if the exit code from the command is 0
  ** even for bad signatures.
  ** (PGP only)
  */
#endif
  { "pgp_ignore_subkeys", DT_BOOL, &C_PgpIgnoreSubkeys, true, 0, NULL,
    "Setting this variable will cause NeoMutt to ignore OpenPGP subkeys. Instead,\n"
    "the principal key will inherit the subkeys' capabilities.  _Unset_ this\n"
    "if you want to play interesting key selection games.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** Setting this variable will cause NeoMutt to ignore OpenPGP subkeys. Instead,
  ** the principal key will inherit the subkeys' capabilities.  \fIUnset\fP this
  ** if you want to play interesting key selection games.
  ** (PGP only)
  */
#ifdef CRYPT_BACKEND_CLASSIC_PGP
  { "pgp_import_command", DT_STRING|DT_COMMAND, &C_PgpImportCommand, 0, 0, NULL,
    "This command is used to import a key from a message into\n"
    "the user's public key ring.\n"
    "\n"
    "This is a format string, see the $$pgp_decode_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** This command is used to import a key from a message into
  ** the user's public key ring.
  ** .pp
  ** This is a format string, see the $$pgp_decode_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** (PGP only)
  */
  { "pgp_list_pubring_command", DT_STRING|DT_COMMAND, &C_PgpListPubringCommand, 0, 0, NULL,
    "This command is used to list the public key ring's contents.  The\n"
    "output format must be analogous to the one used by\n"
    "```\n"
    "gpg --list-keys --with-colons --with-fingerprint\n"
    "```\n"
    "\n"
    "Note: gpg's `fixed-list-mode` option should not be used.  It\n"
    "produces a different date format which may result in NeoMutt showing\n"
    "incorrect key generation dates.\n"
    "\n"
    "This is a format string, see the $$pgp_decode_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** This command is used to list the public key ring's contents.  The
  ** output format must be analogous to the one used by
  ** .ts
  ** gpg --list-keys --with-colons --with-fingerprint
  ** .te
  ** .pp
  ** Note: gpg's \fCfixed-list-mode\fP option should not be used.  It
  ** produces a different date format which may result in NeoMutt showing
  ** incorrect key generation dates.
  ** .pp
  ** This is a format string, see the $$pgp_decode_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** (PGP only)
  */
  { "pgp_list_secring_command", DT_STRING|DT_COMMAND, &C_PgpListSecringCommand, 0, 0, NULL,
    "This command is used to list the secret key ring's contents.  The\n"
    "output format must be analogous to the one used by:\n"
    "```\n"
    "gpg --list-keys --with-colons --with-fingerprint\n"
    "```\n"
    "\n"
    "Note: gpg's `fixed-list-mode` option should not be used.  It\n"
    "produces a different date format which may result in NeoMutt showing\n"
    "incorrect key generation dates.\n"
    "\n"
    "This is a format string, see the $$pgp_decode_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** This command is used to list the secret key ring's contents.  The
  ** output format must be analogous to the one used by:
  ** .ts
  ** gpg --list-keys --with-colons --with-fingerprint
  ** .te
  ** .pp
  ** Note: gpg's \fCfixed-list-mode\fP option should not be used.  It
  ** produces a different date format which may result in NeoMutt showing
  ** incorrect key generation dates.
  ** .pp
  ** This is a format string, see the $$pgp_decode_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** (PGP only)
  */
#endif
  { "pgp_long_ids", DT_BOOL, &C_PgpLongIds, true, 0, NULL,
    "If _set_, use 64 bit PGP key IDs, if _unset_ use the normal 32 bit key IDs.\n"
    "NOTE: Internally, NeoMutt has transitioned to using fingerprints (or long key IDs\n"
    "as a fallback).  This option now only controls the display of key IDs\n"
    "in the key selection menu and a few other places.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, use 64 bit PGP key IDs, if \fIunset\fP use the normal 32 bit key IDs.
  ** NOTE: Internally, NeoMutt has transitioned to using fingerprints (or long key IDs
  ** as a fallback).  This option now only controls the display of key IDs
  ** in the key selection menu and a few other places.
  ** (PGP only)
  */
  { "pgp_mime_auto", DT_QUAD, &C_PgpMimeAuto, MUTT_ASKYES, 0, NULL,
    "This option controls whether NeoMutt will prompt you for\n"
    "automatically sending a (signed/encrypted) message using\n"
    "PGP/MIME when inline (traditional) fails (for any reason).\n"
    "\n"
    "Also note that using the old-style PGP message format is **strongly**\n"
    "**deprecated**.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** This option controls whether NeoMutt will prompt you for
  ** automatically sending a (signed/encrypted) message using
  ** PGP/MIME when inline (traditional) fails (for any reason).
  ** .pp
  ** Also note that using the old-style PGP message format is \fBstrongly\fP
  ** \fBdeprecated\fP.
  ** (PGP only)
  */
  { "pgp_replyinline", DT_BOOL, &C_PgpReplyinline, false, 0, NULL,
    "Setting this variable will cause NeoMutt to always attempt to\n"
    "create an inline (traditional) message when replying to a\n"
    "message which is PGP encrypted/signed inline.  This can be\n"
    "overridden by use of the pgp menu, when inline is not\n"
    "required.  This option does not automatically detect if the\n"
    "(replied-to) message is inline; instead it relies on NeoMutt\n"
    "internals for previously checked/flagged messages.\n"
    "\n"
    "Note that NeoMutt might automatically use PGP/MIME for messages\n"
    "which consist of more than a single MIME part.  NeoMutt can be\n"
    "configured to ask before sending PGP/MIME messages when inline\n"
    "(traditional) would not work.\n"
    "\n"
    "Also see the $$pgp_mime_auto variable.\n"
    "\n"
    "Also note that using the old-style PGP message format is **strongly**\n"
    "**deprecated**.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** Setting this variable will cause NeoMutt to always attempt to
  ** create an inline (traditional) message when replying to a
  ** message which is PGP encrypted/signed inline.  This can be
  ** overridden by use of the pgp menu, when inline is not
  ** required.  This option does not automatically detect if the
  ** (replied-to) message is inline; instead it relies on NeoMutt
  ** internals for previously checked/flagged messages.
  ** .pp
  ** Note that NeoMutt might automatically use PGP/MIME for messages
  ** which consist of more than a single MIME part.  NeoMutt can be
  ** configured to ask before sending PGP/MIME messages when inline
  ** (traditional) would not work.
  ** .pp
  ** Also see the $$pgp_mime_auto variable.
  ** .pp
  ** Also note that using the old-style PGP message format is \fBstrongly\fP
  ** \fBdeprecated\fP.
  ** (PGP only)
  */
  { "pgp_retainable_sigs", DT_BOOL, &C_PgpRetainableSigs, false, 0, NULL,
    "If _set_, signed and encrypted messages will consist of nested\n"
    "`multipart/signed` and `multipart/encrypted` body parts.\n"
    "\n"
    "This is useful for applications like encrypted and signed mailing\n"
    "lists, where the outer layer (`multipart/encrypted`) can be easily\n"
    "removed, while the inner `multipart/signed` part is retained.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, signed and encrypted messages will consist of nested
  ** \fCmultipart/signed\fP and \fCmultipart/encrypted\fP body parts.
  ** .pp
  ** This is useful for applications like encrypted and signed mailing
  ** lists, where the outer layer (\fCmultipart/encrypted\fP) can be easily
  ** removed, while the inner \fCmultipart/signed\fP part is retained.
  ** (PGP only)
  */
  { "pgp_self_encrypt", DT_BOOL, &C_PgpSelfEncrypt, true, 0, NULL,
    "When _set_, PGP encrypted messages will also be encrypted\n"
    "using the key in $$pgp_default_key.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, PGP encrypted messages will also be encrypted
  ** using the key in $$pgp_default_key.
  ** (PGP only)
  */
  { "pgp_show_unusable", DT_BOOL, &C_PgpShowUnusable, true, 0, NULL,
    "If _set_, NeoMutt will display non-usable keys on the PGP key selection\n"
    "menu.  This includes keys which have been revoked, have expired, or\n"
    "have been marked as \"disabled\" by the user.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, NeoMutt will display non-usable keys on the PGP key selection
  ** menu.  This includes keys which have been revoked, have expired, or
  ** have been marked as "disabled" by the user.
  ** (PGP only)
  */
  { "pgp_sign_as", DT_STRING, &C_PgpSignAs, 0, 0, NULL,
    "If you have a different key pair to use for signing, you should\n"
    "set this to the signing key.  Most people will only need to set\n"
    "$$pgp_default_key.  It is recommended that you use the keyid form\n"
    "to specify your key (e.g. `0x00112233`).\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** If you have a different key pair to use for signing, you should
  ** set this to the signing key.  Most people will only need to set
  ** $$pgp_default_key.  It is recommended that you use the keyid form
  ** to specify your key (e.g. \fC0x00112233\fP).
  ** (PGP only)
  */
#ifdef CRYPT_BACKEND_CLASSIC_PGP
  { "pgp_sign_command", DT_STRING|DT_COMMAND, &C_PgpSignCommand, 0, 0, NULL,
    "This command is used to create the detached PGP signature for a\n"
    "`multipart/signed` PGP/MIME body part.\n"
    "\n"
    "This is a format string, see the $$pgp_decode_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** This command is used to create the detached PGP signature for a
  ** \fCmultipart/signed\fP PGP/MIME body part.
  ** .pp
  ** This is a format string, see the $$pgp_decode_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** (PGP only)
  */
#endif
  { "pgp_sort_keys", DT_SORT|DT_SORT_KEYS, &C_PgpSortKeys, SORT_ADDRESS, 0, NULL,
    "Specifies how the entries in the pgp menu are sorted. The\n"
    "following are legal values:\n"
    "\n"
    "| .       | Description |\n"
    "|---------+--------------------------------|\n"
    "| address | sort alphabetically by user id |\n"
    "| keyid   | sort alphabetically by key id |\n"
    "| date    | sort by key creation date |\n"
    "| trust   | sort by the trust of the key |\n"
    "\n"
    "If you prefer reverse order of the above values, prefix it with\n"
    "\"reverse-\".\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** Specifies how the entries in the pgp menu are sorted. The
  ** following are legal values:
  ** .dl
  ** .dt address .dd sort alphabetically by user id
  ** .dt keyid   .dd sort alphabetically by key id
  ** .dt date    .dd sort by key creation date
  ** .dt trust   .dd sort by the trust of the key
  ** .de
  ** .pp
  ** If you prefer reverse order of the above values, prefix it with
  ** "reverse-".
  ** (PGP only)
  */
  { "pgp_strict_enc", DT_BOOL, &C_PgpStrictEnc, true, 0, NULL,
    "If _set_, NeoMutt will automatically encode PGP/MIME signed messages as\n"
    "quoted-printable.  Please note that unsetting this variable may\n"
    "lead to problems with non-verifyable PGP signatures, so only change\n"
    "this if you know what you are doing.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, NeoMutt will automatically encode PGP/MIME signed messages as
  ** quoted-printable.  Please note that unsetting this variable may
  ** lead to problems with non-verifyable PGP signatures, so only change
  ** this if you know what you are doing.
  ** (PGP only)
  */
#ifdef CRYPT_BACKEND_CLASSIC_PGP
  { "pgp_timeout", DT_LONG|DT_NOT_NEGATIVE, &C_PgpTimeout, 300, 0, NULL,
    "The number of seconds after which a cached passphrase will expire if\n"
    "not used.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** The number of seconds after which a cached passphrase will expire if
  ** not used.
  ** (PGP only)
  */
  { "pgp_use_gpg_agent", DT_BOOL, &C_PgpUseGpgAgent, true, 0, NULL,
    "If _set_, NeoMutt expects a `gpg-agent(1)` process will handle\n"
    "private key passphrase prompts.  If _unset_, NeoMutt will prompt\n"
    "for the passphrase and pass it via stdin to the pgp command.\n"
    "\n"
    "Note that as of version 2.1, GnuPG automatically spawns an agent\n"
    "and requires the agent be used for passphrase management.  Since\n"
    "that version is increasingly prevalent, this variable now\n"
    "defaults _set_.\n"
    "\n"
    "NeoMutt works with a GUI or curses pinentry program.  A TTY pinentry\n"
    "should not be used.\n"
    "\n"
    "If you are using an older version of GnuPG without an agent running,\n"
    "or another encryption program without an agent, you will need to\n"
    "_unset_ this variable.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, NeoMutt expects a \fCgpg-agent(1)\fP process will handle
  ** private key passphrase prompts.  If \fIunset\fP, NeoMutt will prompt
  ** for the passphrase and pass it via stdin to the pgp command.
  ** .pp
  ** Note that as of version 2.1, GnuPG automatically spawns an agent
  ** and requires the agent be used for passphrase management.  Since
  ** that version is increasingly prevalent, this variable now
  ** defaults \fIset\fP.
  ** .pp
  ** NeoMutt works with a GUI or curses pinentry program.  A TTY pinentry
  ** should not be used.
  ** .pp
  ** If you are using an older version of GnuPG without an agent running,
  ** or another encryption program without an agent, you will need to
  ** \fIunset\fP this variable.
  ** (PGP only)
  */
  { "pgp_verify_command", DT_STRING|DT_COMMAND, &C_PgpVerifyCommand, 0, 0, NULL,
    "This command is used to verify PGP signatures.\n"
    "\n"
    "This is a format string, see the $$pgp_decode_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** This command is used to verify PGP signatures.
  ** .pp
  ** This is a format string, see the $$pgp_decode_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** (PGP only)
  */
  { "pgp_verify_key_command", DT_STRING|DT_COMMAND, &C_PgpVerifyKeyCommand, 0, 0, NULL,
    "This command is used to verify key information from the key selection\n"
    "menu.\n"
    "\n"
    "This is a format string, see the $$pgp_decode_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "(PGP only)\n"
  },
  /*
  ** .pp
  ** This command is used to verify key information from the key selection
  ** menu.
  ** .pp
  ** This is a format string, see the $$pgp_decode_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** (PGP only)
  */
#endif
  { "pipe_decode", DT_BOOL, &C_PipeDecode, false, 0, NULL,
    "Used in connection with the `<pipe-message>` command.  When _unset_,\n"
    "NeoMutt will pipe the messages without any preprocessing. When _set_, NeoMutt\n"
    "will weed headers and will attempt to decode the messages\n"
    "first.\n"
  },
  /*
  ** .pp
  ** Used in connection with the \fC<pipe-message>\fP command.  When \fIunset\fP,
  ** NeoMutt will pipe the messages without any preprocessing. When \fIset\fP, NeoMutt
  ** will weed headers and will attempt to decode the messages
  ** first.
  */
  { "pipe_sep", DT_STRING, &C_PipeSep, IP "\n", 0, NULL,
    "The separator to add between messages when piping a list of tagged\n"
    "messages to an external Unix command.\n"
  },
  /*
  ** .pp
  ** The separator to add between messages when piping a list of tagged
  ** messages to an external Unix command.
  */
  { "pipe_split", DT_BOOL, &C_PipeSplit, false, 0, NULL,
    "Used in connection with the `<pipe-message>` function following\n"
    "`<tag-prefix>`.  If this variable is _unset_, when piping a list of\n"
    "tagged messages NeoMutt will concatenate the messages and will pipe them\n"
    "all concatenated.  When _set_, NeoMutt will pipe the messages one by one.\n"
    "In both cases the messages are piped in the current sorted order,\n"
    "and the $$pipe_sep separator is added after each message.\n"
  },
  /*
  ** .pp
  ** Used in connection with the \fC<pipe-message>\fP function following
  ** \fC<tag-prefix>\fP.  If this variable is \fIunset\fP, when piping a list of
  ** tagged messages NeoMutt will concatenate the messages and will pipe them
  ** all concatenated.  When \fIset\fP, NeoMutt will pipe the messages one by one.
  ** In both cases the messages are piped in the current sorted order,
  ** and the $$pipe_sep separator is added after each message.
  */
#ifdef USE_POP
  { "pop_auth_try_all", DT_BOOL, &C_PopAuthTryAll, true, 0, NULL,
    "If _set_, NeoMutt will try all available authentication methods.\n"
    "When _unset_, NeoMutt will only fall back to other authentication\n"
    "methods if the previous methods are unavailable. If a method is\n"
    "available but authentication fails, NeoMutt will not connect to the POP server.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, NeoMutt will try all available authentication methods.
  ** When \fIunset\fP, NeoMutt will only fall back to other authentication
  ** methods if the previous methods are unavailable. If a method is
  ** available but authentication fails, NeoMutt will not connect to the POP server.
  */
  { "pop_authenticators", DT_SLIST|SLIST_SEP_COLON, &C_PopAuthenticators, 0, 0, NULL,
    "This is a colon-delimited list of authentication methods NeoMutt may\n"
    "attempt to use to log in to an POP server, in the order NeoMutt should\n"
    "try them.  Authentication methods are either \"user\", \"apop\" or any\n"
    "SASL mechanism, e.g. \"digest-md5\", \"gssapi\" or \"cram-md5\".\n"
    "This option is case-insensitive. If this option is _unset_\n"
    "(the default) NeoMutt will try all available methods, in order from\n"
    "most-secure to least-secure.\n"
    "\n"
    "Example:\n"
    "```\n"
    "set pop_authenticators=\"digest-md5:apop:user\"\n"
    "```\n"
  },
  /*
  ** .pp
  ** This is a colon-delimited list of authentication methods NeoMutt may
  ** attempt to use to log in to an POP server, in the order NeoMutt should
  ** try them.  Authentication methods are either "user", "apop" or any
  ** SASL mechanism, e.g. "digest-md5", "gssapi" or "cram-md5".
  ** This option is case-insensitive. If this option is \fIunset\fP
  ** (the default) NeoMutt will try all available methods, in order from
  ** most-secure to least-secure.
  ** .pp
  ** Example:
  ** .ts
  ** set pop_authenticators="digest-md5:apop:user"
  ** .te
  */
  { "pop_checkinterval", DT_NUMBER|DT_NOT_NEGATIVE, &C_PopCheckinterval, 60, 0, NULL,
    "This variable configures how often (in seconds) NeoMutt should look for\n"
    "new mail in the currently selected mailbox if it is a POP mailbox.\n"
  },
  /*
  ** .pp
  ** This variable configures how often (in seconds) NeoMutt should look for
  ** new mail in the currently selected mailbox if it is a POP mailbox.
  */
  { "pop_delete", DT_QUAD, &C_PopDelete, MUTT_ASKNO, 0, NULL,
    "If _set_, NeoMutt will delete successfully downloaded messages from the POP\n"
    "server when using the `$<fetch-mail>` function.  When _unset_, NeoMutt will\n"
    "download messages but also leave them on the POP server.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, NeoMutt will delete successfully downloaded messages from the POP
  ** server when using the \fC$<fetch-mail>\fP function.  When \fIunset\fP, NeoMutt will
  ** download messages but also leave them on the POP server.
  */
  { "pop_host", DT_STRING, &C_PopHost, 0, 0, NULL,
    "The name of your POP server for the `$<fetch-mail>` function.  You\n"
    "can also specify an alternative port, username and password, i.e.:\n"
    "```\n"
    "[pop[s]://][username[:password]@]popserver[:port]\n"
    "```\n"
    "\n"
    "where \"[...]\" denotes an optional part.\n"
  },
  /*
  ** .pp
  ** The name of your POP server for the \fC$<fetch-mail>\fP function.  You
  ** can also specify an alternative port, username and password, i.e.:
  ** .ts
  ** [pop[s]://][username[:password]@]popserver[:port]
  ** .te
  ** .pp
  ** where "[...]" denotes an optional part.
  */
  { "pop_last", DT_BOOL, &C_PopLast, false, 0, NULL,
    "If this variable is _set_, NeoMutt will try to use the \"`LAST`\" POP command\n"
    "for retrieving only unread messages from the POP server when using\n"
    "the `$<fetch-mail>` function.\n"
  },
  /*
  ** .pp
  ** If this variable is \fIset\fP, NeoMutt will try to use the "\fCLAST\fP" POP command
  ** for retrieving only unread messages from the POP server when using
  ** the \fC$<fetch-mail>\fP function.
  */
  { "pop_oauth_refresh_command", DT_STRING|DT_COMMAND|DT_SENSITIVE, &C_PopOauthRefreshCommand, 0, 0, NULL,
    "The command to run to generate an OAUTH refresh token for\n"
    "authorizing your connection to your POP server.  This command will be\n"
    "run on every connection attempt that uses the OAUTHBEARER authentication\n"
    "mechanism.  See \"$oauth\" for details.\n"
  },
  /*
  ** .pp
  ** The command to run to generate an OAUTH refresh token for
  ** authorizing your connection to your POP server.  This command will be
  ** run on every connection attempt that uses the OAUTHBEARER authentication
  ** mechanism.  See "$oauth" for details.
  */
  { "pop_pass", DT_STRING|DT_SENSITIVE, &C_PopPass, 0, 0, NULL,
    "Specifies the password for your POP account.  If _unset_, NeoMutt will\n"
    "prompt you for your password when you open a POP mailbox.\n"
    "\n"
    "**Warning**: you should only use this option when you are on a\n"
    "fairly secure machine, because the superuser can read your neomuttrc\n"
    "even if you are the only one who can read the file.\n"
  },
  /*
  ** .pp
  ** Specifies the password for your POP account.  If \fIunset\fP, NeoMutt will
  ** prompt you for your password when you open a POP mailbox.
  ** .pp
  ** \fBWarning\fP: you should only use this option when you are on a
  ** fairly secure machine, because the superuser can read your neomuttrc
  ** even if you are the only one who can read the file.
  */
  { "pop_reconnect", DT_QUAD, &C_PopReconnect, MUTT_ASKYES, 0, NULL,
    "Controls whether or not NeoMutt will try to reconnect to the POP server if\n"
    "the connection is lost.\n"
  },
  /*
  ** .pp
  ** Controls whether or not NeoMutt will try to reconnect to the POP server if
  ** the connection is lost.
  */
  { "pop_user", DT_STRING|DT_SENSITIVE, &C_PopUser, 0, 0, NULL,
    "Your login name on the POP server.\n"
    "\n"
    "This variable defaults to your user name on the local machine.\n"
  },
  /*
  ** .pp
  ** Your login name on the POP server.
  ** .pp
  ** This variable defaults to your user name on the local machine.
  */
#endif /* USE_POP */
  { "post_indent_string", DT_STRING, &C_PostIndentString, 0, 0, NULL,
    "Similar to the $$attribution variable, NeoMutt will append this\n"
    "string after the inclusion of a message which is being replied to.\n"
  },
  /*
  ** .pp
  ** Similar to the $$attribution variable, NeoMutt will append this
  ** string after the inclusion of a message which is being replied to.
  */
#ifdef USE_NNTP
  { "post_moderated", DT_QUAD, &C_PostModerated, MUTT_ASKYES, 0, NULL,
    "If set to _yes_, NeoMutt will post article to newsgroup that have\n"
    "not permissions to posting (e.g. moderated).  **Note:** if news server\n"
    "does not support posting to that newsgroup or totally read-only, that\n"
    "posting will not have an effect.\n"
  },
  /*
  ** .pp
  ** If set to \fIyes\fP, NeoMutt will post article to newsgroup that have
  ** not permissions to posting (e.g. moderated).  \fBNote:\fP if news server
  ** does not support posting to that newsgroup or totally read-only, that
  ** posting will not have an effect.
  */
#endif
  { "postpone", DT_QUAD, &C_Postpone, MUTT_ASKYES, 0, NULL,
    "Controls whether or not messages are saved in the $$postponed\n"
    "mailbox when you elect not to send immediately. If set to\n"
    "_ask-yes_ or _ask-no_, you will be prompted with \"Save\n"
    "(postpone) draft message?\" when quitting from the \"compose\"\n"
    "screen.\n"
    "\n"
    "Also see the $$recall variable.\n"
  },
  /*
  ** .pp
  ** Controls whether or not messages are saved in the $$postponed
  ** mailbox when you elect not to send immediately. If set to
  ** \fIask-yes\fP or \fIask-no\fP, you will be prompted with "Save
  ** (postpone) draft message?" when quitting from the "compose"
  ** screen.
  ** .pp
  ** Also see the $$recall variable.
  */
  { "postpone_encrypt", DT_BOOL, &C_PostponeEncrypt, false, 0, NULL,
    "When _set_, postponed messages that are marked for encryption will be\n"
    "self-encrypted.  NeoMutt will first try to encrypt using the value specified\n"
    "in $$pgp_default_key or $$smime_default_key.  If those are not\n"
    "set, it will try the deprecated $$postpone_encrypt_as.\n"
    "(Crypto only)\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, postponed messages that are marked for encryption will be
  ** self-encrypted.  NeoMutt will first try to encrypt using the value specified
  ** in $$pgp_default_key or $$smime_default_key.  If those are not
  ** set, it will try the deprecated $$postpone_encrypt_as.
  ** (Crypto only)
  */
  { "postpone_encrypt_as", DT_STRING, &C_PostponeEncryptAs, 0, 0, NULL,
    "This is a deprecated fall-back variable for $$postpone_encrypt.\n"
    "Please use $$pgp_default_key or $$smime_default_key.\n"
    "(Crypto only)\n"
  },
  /*
  ** .pp
  ** This is a deprecated fall-back variable for $$postpone_encrypt.
  ** Please use $$pgp_default_key or $$smime_default_key.
  ** (Crypto only)
  */
  { "postponed", DT_STRING|DT_PATH|DT_MAILBOX|R_INDEX, &C_Postponed, IP "~/postponed", 0, NULL,
    "NeoMutt allows you to indefinitely \"$postpone sending a message\" which\n"
    "you are editing.  When you choose to postpone a message, NeoMutt saves it\n"
    "in the mailbox specified by this variable.\n"
    "\n"
    "Also see the $$postpone variable.\n"
  },
  /*
  ** .pp
  ** NeoMutt allows you to indefinitely "$postpone sending a message" which
  ** you are editing.  When you choose to postpone a message, NeoMutt saves it
  ** in the mailbox specified by this variable.
  ** .pp
  ** Also see the $$postpone variable.
  */
#ifdef USE_SOCKET
  { "preconnect", DT_STRING, &C_Preconnect, 0, 0, NULL,
    "If _set_, a shell command to be executed if NeoMutt fails to establish\n"
    "a connection to the server. This is useful for setting up secure\n"
    "connections, e.g. with `ssh(1)`. If the command returns a  nonzero\n"
    "status, NeoMutt gives up opening the server. Example:\n"
    "```\n"
    "set preconnect=\"ssh -f -q -L 1234:mailhost.net:143 mailhost.net \\\n"
    "sleep 20 < /dev/null > /dev/null\"\n"
    "```\n"
    "\n"
    "Mailbox \"foo\" on \"mailhost.net\" can now be reached\n"
    "as \"{localhost:1234}foo\". 0, NULL,\n"
    "\n"
    "Note: For this example to work, you must be able to log in to the\n"
    "remote machine without having to enter a password.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, a shell command to be executed if NeoMutt fails to establish
  ** a connection to the server. This is useful for setting up secure
  ** connections, e.g. with \fCssh(1)\fP. If the command returns a  nonzero
  ** status, NeoMutt gives up opening the server. Example:
  ** .ts
  ** set preconnect="ssh -f -q -L 1234:mailhost.net:143 mailhost.net \(rs
  ** sleep 20 < /dev/null > /dev/null"
  ** .te
  ** .pp
  ** Mailbox "foo" on "mailhost.net" can now be reached
  ** as "{localhost:1234}foo". 0, NULL,
  ** .pp
  ** Note: For this example to work, you must be able to log in to the
  ** remote machine without having to enter a password.
  */
#endif /* USE_SOCKET */
  { "preferred_languages", DT_SLIST|SLIST_SEP_COMMA, &C_PreferredLanguages, 0, 0, NULL,
    "RFC8255 : user preferred languages to be search in parts and display\n"
    "Ex. : set preferred_languages=\"en,fr,de\"\n"
  },
  /*
  ** .pp
  ** RFC8255 : user preferred languages to be search in parts and display
  ** Ex. : set preferred_languages="en,fr,de"
  */
  { "print", DT_QUAD, &C_Print, MUTT_ASKNO, 0, NULL,
    "Controls whether or not NeoMutt really prints messages.\n"
    "This is set to \"ask-no\" by default, because some people\n"
    "accidentally hit \"p\" often.\n"
  },
  /*
  ** .pp
  ** Controls whether or not NeoMutt really prints messages.
  ** This is set to "ask-no" by default, because some people
  ** accidentally hit "p" often.
  */
  { "print_command", DT_STRING|DT_COMMAND, &C_PrintCommand, IP "lpr", 0, NULL,
    "This specifies the command pipe that should be used to print messages.\n"
  },
  /*
  ** .pp
  ** This specifies the command pipe that should be used to print messages.
  */
  { "print_decode", DT_BOOL, &C_PrintDecode, true, 0, NULL,
    "Used in connection with the `<print-message>` command.  If this\n"
    "option is _set_, the message is decoded before it is passed to the\n"
    "external command specified by $$print_command.  If this option\n"
    "is _unset_, no processing will be applied to the message when\n"
    "printing it.  The latter setting may be useful if you are using\n"
    "some advanced printer filter which is able to properly format\n"
    "e-mail messages for printing.\n"
  },
  /*
  ** .pp
  ** Used in connection with the \fC<print-message>\fP command.  If this
  ** option is \fIset\fP, the message is decoded before it is passed to the
  ** external command specified by $$print_command.  If this option
  ** is \fIunset\fP, no processing will be applied to the message when
  ** printing it.  The latter setting may be useful if you are using
  ** some advanced printer filter which is able to properly format
  ** e-mail messages for printing.
  */
  { "print_split", DT_BOOL, &C_PrintSplit, false, 0, NULL,
    "Used in connection with the `<print-message>` command.  If this option\n"
    "is _set_, the command specified by $$print_command is executed once for\n"
    "each message which is to be printed.  If this option is _unset_,\n"
    "the command specified by $$print_command is executed only once, and\n"
    "all the messages are concatenated, with a form feed as the message\n"
    "separator.\n"
    "\n"
    "Those who use the `enscript`(1) program's mail-printing mode will\n"
    "most likely want to _set_ this option.\n"
  },
  /*
  ** .pp
  ** Used in connection with the \fC<print-message>\fP command.  If this option
  ** is \fIset\fP, the command specified by $$print_command is executed once for
  ** each message which is to be printed.  If this option is \fIunset\fP,
  ** the command specified by $$print_command is executed only once, and
  ** all the messages are concatenated, with a form feed as the message
  ** separator.
  ** .pp
  ** Those who use the \fCenscript\fP(1) program's mail-printing mode will
  ** most likely want to \fIset\fP this option.
  */
  { "prompt_after", DT_BOOL, &C_PromptAfter, true, 0, NULL,
    "If you use an _external_ $$pager, setting this variable will\n"
    "cause NeoMutt to prompt you for a command when the pager exits rather\n"
    "than returning to the index menu.  If _unset_, NeoMutt will return to the\n"
    "index menu when the external pager exits.\n"
  },
  /*
  ** .pp
  ** If you use an \fIexternal\fP $$pager, setting this variable will
  ** cause NeoMutt to prompt you for a command when the pager exits rather
  ** than returning to the index menu.  If \fIunset\fP, NeoMutt will return to the
  ** index menu when the external pager exits.
  */
  { "query_command", DT_STRING|DT_COMMAND, &C_QueryCommand, 0, 0, NULL,
    "This specifies the command NeoMutt will use to make external address\n"
    "queries.  The string may contain a \"%s\", which will be substituted\n"
    "with the query string the user types.  NeoMutt will add quotes around the\n"
    "string substituted for \"%s\" automatically according to shell quoting\n"
    "rules, so you should avoid adding your own.  If no \"%s\" is found in\n"
    "the string, NeoMutt will append the user's query to the end of the string.\n"
    "See \"$query\" for more information.\n"
  },
  /*
  ** .pp
  ** This specifies the command NeoMutt will use to make external address
  ** queries.  The string may contain a "%s", which will be substituted
  ** with the query string the user types.  NeoMutt will add quotes around the
  ** string substituted for "%s" automatically according to shell quoting
  ** rules, so you should avoid adding your own.  If no "%s" is found in
  ** the string, NeoMutt will append the user's query to the end of the string.
  ** See "$query" for more information.
  */
  { "query_format", DT_STRING|DT_NOT_EMPTY, &C_QueryFormat, IP "%4c %t %-25.25a %-25.25n %?e?(%e)?", 0, NULL,
    "This variable describes the format of the \"query\" menu. The\n"
    "following `printf(3)`-style sequences are understood:\n"
    "\n"
    "| .   | Description |\n"
    "|-----+-------------------------------------------------------|\n"
    "| %a  | Destination address |\n"
    "| %c  | Current entry number |\n"
    "| %e  | Extra information * |\n"
    "| %n  | Destination name |\n"
    "| %t  | \"*\" if current entry is tagged, a space otherwise |\n"
    "| %>X | Right justify the rest of the string and pad with \"X\" |\n"
    "| %|X | Pad to the end of the line with \"X\" |\n"
    "| %*X | Soft-fill with character \"X\" as pad |\n"
    "\n"
    "For an explanation of \"soft-fill\", see the $$index_format documentation.\n"
    "\n"
    "* = can be optionally printed if nonzero, see the $$status_format documentation.\n"
  },
  /*
  ** .pp
  ** This variable describes the format of the "query" menu. The
  ** following \fCprintf(3)\fP-style sequences are understood:
  ** .dl
  ** .dt %a  .dd   .dd Destination address
  ** .dt %c  .dd   .dd Current entry number
  ** .dt %e  .dd * .dd Extra information
  ** .dt %n  .dd   .dd Destination name
  ** .dt %t  .dd   .dd "*" if current entry is tagged, a space otherwise
  ** .dt %>X .dd   .dd Right justify the rest of the string and pad with "X"
  ** .dt %|X .dd   .dd Pad to the end of the line with "X"
  ** .dt %*X .dd   .dd Soft-fill with character "X" as pad
  ** .de
  ** .pp
  ** For an explanation of "soft-fill", see the $$index_format documentation.
  ** .pp
  ** * = can be optionally printed if nonzero, see the $$status_format documentation.
  */
  { "quit", DT_QUAD, &C_Quit, MUTT_YES, 0, NULL,
    "This variable controls whether \"quit\" and \"exit\" actually quit\n"
    "from NeoMutt.  If this option is _set_, they do quit, if it is _unset_, they\n"
    "have no effect, and if it is set to _ask-yes_ or _ask-no_, you are\n"
    "prompted for confirmation when you try to quit.\n"
  },
  /*
  ** .pp
  ** This variable controls whether "quit" and "exit" actually quit
  ** from NeoMutt.  If this option is \fIset\fP, they do quit, if it is \fIunset\fP, they
  ** have no effect, and if it is set to \fIask-yes\fP or \fIask-no\fP, you are
  ** prompted for confirmation when you try to quit.
  */
  { "quote_regex", DT_REGEX|R_PAGER, &C_QuoteRegex, IP "^([ \t]*[|>:}#])+", 0, NULL,
    "A regular expression used in the internal pager to determine quoted\n"
    "sections of text in the body of a message. Quoted text may be filtered\n"
    "out using the `<toggle-quoted>` command, or colored according to the\n"
    "\"color quoted\" family of directives.\n"
    "\n"
    "Higher levels of quoting may be colored differently (\"color quoted1\",\n"
    "\"color quoted2\", etc.). The quoting level is determined by removing\n"
    "the last character from the matched text and recursively reapplying\n"
    "the regular expression until it fails to produce a match.\n"
    "\n"
    "Match detection may be overridden by the $$smileys regular expression.\n"
  },
  /*
  ** .pp
  ** A regular expression used in the internal pager to determine quoted
  ** sections of text in the body of a message. Quoted text may be filtered
  ** out using the \fC<toggle-quoted>\fP command, or colored according to the
  ** "color quoted" family of directives.
  ** .pp
  ** Higher levels of quoting may be colored differently ("color quoted1",
  ** "color quoted2", etc.). The quoting level is determined by removing
  ** the last character from the matched text and recursively reapplying
  ** the regular expression until it fails to produce a match.
  ** .pp
  ** Match detection may be overridden by the $$smileys regular expression.
  */
  { "toggle_quoted_show_levels", DT_NUMBER|DT_NOT_NEGATIVE, &C_ToggleQuotedShowLevels, 0, 0, NULL,
    "Quoted text may be filtered out using the `<toggle-quoted>` command.\n"
    "If set to a number greater than 0, then the `<toggle-quoted>`\n"
    "command will only filter out quote levels above this number.\n"
  },
  /*
  ** .pp
  ** Quoted text may be filtered out using the \fC<toggle-quoted>\fP command.
  ** If set to a number greater than 0, then the \fC<toggle-quoted>\fP
  ** command will only filter out quote levels above this number.
  */
  { "read_inc", DT_NUMBER|DT_NOT_NEGATIVE, &C_ReadInc, 10, 0, NULL,
    "If set to a value greater than 0, NeoMutt will display which message it\n"
    "is currently on when reading a mailbox or when performing search actions\n"
    "such as search and limit. The message is printed after\n"
    "this many messages have been read or searched (e.g., if set to 25, NeoMutt will\n"
    "print a message when it is at message 25, and then again when it gets\n"
    "to message 50).  This variable is meant to indicate progress when\n"
    "reading or searching large mailboxes which may take some time.\n"
    "When set to 0, only a single message will appear before the reading\n"
    "the mailbox.\n"
    "\n"
    "Also see the $$write_inc, $$net_inc and $$time_inc variables and the\n"
    "\"$tuning\" section of the manual for performance considerations.\n"
  },
  /*
  ** .pp
  ** If set to a value greater than 0, NeoMutt will display which message it
  ** is currently on when reading a mailbox or when performing search actions
  ** such as search and limit. The message is printed after
  ** this many messages have been read or searched (e.g., if set to 25, NeoMutt will
  ** print a message when it is at message 25, and then again when it gets
  ** to message 50).  This variable is meant to indicate progress when
  ** reading or searching large mailboxes which may take some time.
  ** When set to 0, only a single message will appear before the reading
  ** the mailbox.
  ** .pp
  ** Also see the $$write_inc, $$net_inc and $$time_inc variables and the
  ** "$tuning" section of the manual for performance considerations.
  */
  { "read_only", DT_BOOL, &C_ReadOnly, false, 0, NULL,
    "If _set_, all folders are opened in read-only mode.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, all folders are opened in read-only mode.
  */
  { "realname", DT_STRING|R_INDEX|R_PAGER, &C_Realname, 0, 0, NULL,
    "This variable specifies what \"real\" or \"personal\" name should be used\n"
    "when sending messages.\n"
    "\n"
    "If not specified, then the user's \"real name\" will be read from `/etc/passwd`.\n"
    "This option will not be used, if \"$$from\" is set.\n"
  },
  /*
  ** .pp
  ** This variable specifies what "real" or "personal" name should be used
  ** when sending messages.
  ** .pp
  ** If not specified, then the user's "real name" will be read from \fC/etc/passwd\fP.
  ** This option will not be used, if "$$from" is set.
  */
  { "recall", DT_QUAD, &C_Recall, MUTT_ASKYES, 0, NULL,
    "Controls whether or not NeoMutt recalls postponed messages\n"
    "when composing a new message.\n"
    "\n"
    "Setting this variable to _yes_ is not generally useful, and thus not\n"
    "recommended.  Note that the `<recall-message>` function can be used\n"
    "to manually recall postponed messages.\n"
    "\n"
    "Also see $$postponed variable.\n"
  },
  /*
  ** .pp
  ** Controls whether or not NeoMutt recalls postponed messages
  ** when composing a new message.
  ** .pp
  ** Setting this variable to \fIyes\fP is not generally useful, and thus not
  ** recommended.  Note that the \fC<recall-message>\fP function can be used
  ** to manually recall postponed messages.
  ** .pp
  ** Also see $$postponed variable.
  */
  { "record", DT_STRING|DT_PATH|DT_MAILBOX, &C_Record, IP "~/sent", 0, NULL,
    "This specifies the file into which your outgoing messages should be\n"
    "appended.  (This is meant as the primary method for saving a copy of\n"
    "your messages, but another way to do this is using the \"$my_hdr\"\n"
    "command to create a \"Bcc:\" field with your email address in it.)\n"
    "\n"
    "The value of _$$record_ is overridden by the $$force_name and\n"
    "$$save_name variables, and the \"$fcc-hook\" command.  Also see $$copy\n"
    "and $$write_bcc.\n"
  },
  /*
  ** .pp
  ** This specifies the file into which your outgoing messages should be
  ** appended.  (This is meant as the primary method for saving a copy of
  ** your messages, but another way to do this is using the "$my_hdr"
  ** command to create a "Bcc:" field with your email address in it.)
  ** .pp
  ** The value of \fI$$record\fP is overridden by the $$force_name and
  ** $$save_name variables, and the "$fcc-hook" command.  Also see $$copy
  ** and $$write_bcc.
  */
  { "reflow_space_quotes", DT_BOOL, &C_ReflowSpaceQuotes, true, 0, NULL,
    "This option controls how quotes from format=flowed messages are displayed\n"
    "in the pager and when replying (with $$text_flowed _unset_).\n"
    "When set, this option adds spaces after each level of quote marks, turning\n"
    "\">>>foo\" into \"> > > foo\".\n"
    "\n"
    "**Note:** If $$reflow_text is _unset_, this option has no effect.\n"
    "Also, this option does not affect replies when $$text_flowed is _set_.\n"
  },
  /*
  ** .pp
  ** This option controls how quotes from format=flowed messages are displayed
  ** in the pager and when replying (with $$text_flowed \fIunset\fP).
  ** When set, this option adds spaces after each level of quote marks, turning
  ** ">>>foo" into "> > > foo".
  ** .pp
  ** \fBNote:\fP If $$reflow_text is \fIunset\fP, this option has no effect.
  ** Also, this option does not affect replies when $$text_flowed is \fIset\fP.
  */
  { "reflow_text", DT_BOOL, &C_ReflowText, true, 0, NULL,
    "When _set_, NeoMutt will reformat paragraphs in text/plain\n"
    "parts marked format=flowed.  If _unset_, NeoMutt will display paragraphs\n"
    "unaltered from how they appear in the message body.  See RFC3676 for\n"
    "details on the _format=flowed_ format.\n"
    "\n"
    "Also see $$reflow_wrap, and $$wrap.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will reformat paragraphs in text/plain
  ** parts marked format=flowed.  If \fIunset\fP, NeoMutt will display paragraphs
  ** unaltered from how they appear in the message body.  See RFC3676 for
  ** details on the \fIformat=flowed\fP format.
  ** .pp
  ** Also see $$reflow_wrap, and $$wrap.
  */
  { "reflow_wrap", DT_NUMBER, &C_ReflowWrap, 78, 0, NULL,
    "This variable controls the maximum paragraph width when reformatting text/plain\n"
    "parts when $$reflow_text is _set_.  When the value is 0, paragraphs will\n"
    "be wrapped at the terminal's right margin.  A positive value sets the\n"
    "paragraph width relative to the left margin.  A negative value set the\n"
    "paragraph width relative to the right margin.\n"
    "\n"
    "Also see $$wrap.\n"
  },
  /*
  ** .pp
  ** This variable controls the maximum paragraph width when reformatting text/plain
  ** parts when $$reflow_text is \fIset\fP.  When the value is 0, paragraphs will
  ** be wrapped at the terminal's right margin.  A positive value sets the
  ** paragraph width relative to the left margin.  A negative value set the
  ** paragraph width relative to the right margin.
  ** .pp
  ** Also see $$wrap.
  */
  { "reply_regex", DT_REGEX|R_INDEX|R_RESORT, &C_ReplyRegex, IP "^((re|aw|sv)(\\[[0-9]+\\])*:[ \t]*)*", 0, reply_validator,
    "A regular expression used to recognize reply messages when threading\n"
    "and replying. The default value corresponds to the English \"Re:\", the\n"
    "German \"Aw:\" and the Swedish \"Sv:\".\n"
  },
  /*
  ** .pp
  ** A regular expression used to recognize reply messages when threading
  ** and replying. The default value corresponds to the English "Re:", the
  ** German "Aw:" and the Swedish "Sv:".
  */
  { "reply_self", DT_BOOL, &C_ReplySelf, false, 0, NULL,
    "If _unset_ and you are replying to a message sent by you, NeoMutt will\n"
    "assume that you want to reply to the recipients of that message rather\n"
    "than to yourself.\n"
    "\n"
    "Also see the \"$alternates\" command.\n"
  },
  /*
  ** .pp
  ** If \fIunset\fP and you are replying to a message sent by you, NeoMutt will
  ** assume that you want to reply to the recipients of that message rather
  ** than to yourself.
  ** .pp
  ** Also see the "$alternates" command.
  */
  { "reply_to", DT_QUAD, &C_ReplyTo, MUTT_ASKYES, 0, NULL,
    "If _set_, when replying to a message, NeoMutt will use the address listed\n"
    "in the Reply-to: header as the recipient of the reply.  If _unset_,\n"
    "it will use the address in the From: header field instead.  This\n"
    "option is useful for reading a mailing list that sets the Reply-To:\n"
    "header field to the list address and you want to send a private\n"
    "message to the author of a message.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, when replying to a message, NeoMutt will use the address listed
  ** in the Reply-to: header as the recipient of the reply.  If \fIunset\fP,
  ** it will use the address in the From: header field instead.  This
  ** option is useful for reading a mailing list that sets the Reply-To:
  ** header field to the list address and you want to send a private
  ** message to the author of a message.
  */
  { "reply_with_xorig", DT_BOOL, &C_ReplyWithXorig, false, 0, NULL,
    "This variable provides a toggle. When active, the From: header will be\n"
    "extracted from the current mail's 'X-Original-To:' header. This setting\n"
    "does not have precedence over \"$reverse_realname\".\n"
    "\n"
    "Assuming 'fast_reply' is disabled, this option will prompt the user with a\n"
    "prefilled From: header.\n"
  },
  /*
  ** .pp
  ** This variable provides a toggle. When active, the From: header will be
  ** extracted from the current mail's 'X-Original-To:' header. This setting
  ** does not have precedence over "$reverse_realname".
  ** .pp
  ** Assuming 'fast_reply' is disabled, this option will prompt the user with a
  ** prefilled From: header.
  */
  { "resolve", DT_BOOL, &C_Resolve, true, 0, NULL,
    "When _set_, the cursor will be automatically advanced to the next\n"
    "(possibly undeleted) message whenever a command that modifies the\n"
    "current message is executed.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, the cursor will be automatically advanced to the next
  ** (possibly undeleted) message whenever a command that modifies the
  ** current message is executed.
  */
  { "resume_draft_files", DT_BOOL, &C_ResumeDraftFiles, false, 0, NULL,
    "If _set_, draft files (specified by `-H` on the command\n"
    "line) are processed similarly to when resuming a postponed\n"
    "message.  Recipients are not prompted for; send-hooks are not\n"
    "evaluated; no alias expansion takes place; user-defined headers\n"
    "and signatures are not added to the message.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, draft files (specified by \fC-H\fP on the command
  ** line) are processed similarly to when resuming a postponed
  ** message.  Recipients are not prompted for; send-hooks are not
  ** evaluated; no alias expansion takes place; user-defined headers
  ** and signatures are not added to the message.
  */
  { "resume_edited_draft_files", DT_BOOL, &C_ResumeEditedDraftFiles, true, 0, NULL,
    "If _set_, draft files previously edited (via `-E -H` on\n"
    "the command line) will have $$resume_draft_files automatically\n"
    "set when they are used as a draft file again.\n"
    "\n"
    "The first time a draft file is saved, NeoMutt will add a header,\n"
    "X-Mutt-Resume-Draft to the saved file.  The next time the draft\n"
    "file is read in, if NeoMutt sees the header, it will set\n"
    "$$resume_draft_files.\n"
    "\n"
    "This option is designed to prevent multiple signatures,\n"
    "user-defined headers, and other processing effects from being\n"
    "made multiple times to the draft file.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, draft files previously edited (via \fC-E -H\fP on
  ** the command line) will have $$resume_draft_files automatically
  ** set when they are used as a draft file again.
  ** .pp
  ** The first time a draft file is saved, NeoMutt will add a header,
  ** X-Mutt-Resume-Draft to the saved file.  The next time the draft
  ** file is read in, if NeoMutt sees the header, it will set
  ** $$resume_draft_files.
  ** .pp
  ** This option is designed to prevent multiple signatures,
  ** user-defined headers, and other processing effects from being
  ** made multiple times to the draft file.
  */
  { "reverse_alias", DT_BOOL|R_INDEX|R_PAGER, &C_ReverseAlias, false, 0, NULL,
    "This variable controls whether or not NeoMutt will display the \"personal\"\n"
    "name from your aliases in the index menu if it finds an alias that\n"
    "matches the message's sender.  For example, if you have the following\n"
    "alias:\n"
    "```\n"
    "alias juser abd30425@somewhere.net (Joe User)\n"
    "```\n"
    "\n"
    "and then you receive mail which contains the following header:\n"
    "```\n"
    "From: abd30425@somewhere.net\n"
    "```\n"
    "\n"
    "It would be displayed in the index menu as \"Joe User\" instead of\n"
    "\"abd30425@somewhere.net.\"  This is useful when the person's e-mail\n"
    "address is not human friendly.\n"
  },
  /*
  ** .pp
  ** This variable controls whether or not NeoMutt will display the "personal"
  ** name from your aliases in the index menu if it finds an alias that
  ** matches the message's sender.  For example, if you have the following
  ** alias:
  ** .ts
  ** alias juser abd30425@somewhere.net (Joe User)
  ** .te
  ** .pp
  ** and then you receive mail which contains the following header:
  ** .ts
  ** From: abd30425@somewhere.net
  ** .te
  ** .pp
  ** It would be displayed in the index menu as "Joe User" instead of
  ** "abd30425@somewhere.net."  This is useful when the person's e-mail
  ** address is not human friendly.
  */
  { "reverse_name", DT_BOOL|R_INDEX|R_PAGER, &C_ReverseName, false, 0, NULL,
    "It may sometimes arrive that you receive mail to a certain machine,\n"
    "move the messages to another machine, and reply to some the messages\n"
    "from there.  If this variable is _set_, the default _From:_ line of\n"
    "the reply messages is built using the address where you received the\n"
    "messages you are replying to **if** that address matches your\n"
    "\"$alternates\".  If the variable is _unset_, or the address that would be\n"
    "used doesn't match your \"$alternates\", the _From:_ line will use\n"
    "your address on the current machine.\n"
    "\n"
    "Also see the \"$alternates\" command and $$reverse_realname.\n"
  },
  /*
  ** .pp
  ** It may sometimes arrive that you receive mail to a certain machine,
  ** move the messages to another machine, and reply to some the messages
  ** from there.  If this variable is \fIset\fP, the default \fIFrom:\fP line of
  ** the reply messages is built using the address where you received the
  ** messages you are replying to \fBif\fP that address matches your
  ** "$alternates".  If the variable is \fIunset\fP, or the address that would be
  ** used doesn't match your "$alternates", the \fIFrom:\fP line will use
  ** your address on the current machine.
  ** .pp
  ** Also see the "$alternates" command and $$reverse_realname.
  */
  { "reverse_realname", DT_BOOL|R_INDEX|R_PAGER, &C_ReverseRealname, true, 0, NULL,
    "This variable fine-tunes the behavior of the $$reverse_name feature.\n"
    "\n"
    "When it is _unset_, NeoMutt will remove the real name part of a\n"
    "matching address.  This allows the use of the email address\n"
    "without having to also use what the sender put in the real name\n"
    "field.\n"
    "\n"
    "When it is _set_, NeoMutt will use the matching address as-is.\n"
    "\n"
    "In either case, a missing real name will be filled in afterwards\n"
    "using the value of $$realname.\n"
  },
  /*
  ** .pp
  ** This variable fine-tunes the behavior of the $$reverse_name feature.
  ** .pp
  ** When it is \fIunset\fP, NeoMutt will remove the real name part of a
  ** matching address.  This allows the use of the email address
  ** without having to also use what the sender put in the real name
  ** field.
  ** .pp
  ** When it is \fIset\fP, NeoMutt will use the matching address as-is.
  ** .pp
  ** In either case, a missing real name will be filled in afterwards
  ** using the value of $$realname.
  */
  { "rfc2047_parameters", DT_BOOL, &C_Rfc2047Parameters, false, 0, NULL,
    "When this variable is _set_, NeoMutt will decode RFC2047-encoded MIME\n"
    "parameters. You want to set this variable when NeoMutt suggests you\n"
    "to save attachments to files named like:\n"
    "```\n"
    "=?iso-8859-1?Q?file=5F=E4=5F991116=2Ezip?=\n"
    "```\n"
    "\n"
    "When this variable is _set_ interactively, the change won't be\n"
    "active until you change folders.\n"
    "\n"
    "Note that this use of RFC2047's encoding is explicitly\n"
    "prohibited by the standard, but nevertheless encountered in the\n"
    "wild.\n"
    "\n"
    "Also note that setting this parameter will _not_ have the effect\n"
    "that NeoMutt _generates_ this kind of encoding.  Instead, NeoMutt will\n"
    "unconditionally use the encoding specified in RFC2231.\n"
  },
  /*
  ** .pp
  ** When this variable is \fIset\fP, NeoMutt will decode RFC2047-encoded MIME
  ** parameters. You want to set this variable when NeoMutt suggests you
  ** to save attachments to files named like:
  ** .ts
  ** =?iso-8859-1?Q?file=5F=E4=5F991116=2Ezip?=
  ** .te
  ** .pp
  ** When this variable is \fIset\fP interactively, the change won't be
  ** active until you change folders.
  ** .pp
  ** Note that this use of RFC2047's encoding is explicitly
  ** prohibited by the standard, but nevertheless encountered in the
  ** wild.
  ** .pp
  ** Also note that setting this parameter will \fInot\fP have the effect
  ** that NeoMutt \fIgenerates\fP this kind of encoding.  Instead, NeoMutt will
  ** unconditionally use the encoding specified in RFC2231.
  */
  { "save_address", DT_BOOL, &C_SaveAddress, false, 0, NULL,
    "If _set_, NeoMutt will take the sender's full address when choosing a\n"
    "default folder for saving a mail. If $$save_name or $$force_name\n"
    "is _set_ too, the selection of the Fcc folder will be changed as well.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, NeoMutt will take the sender's full address when choosing a
  ** default folder for saving a mail. If $$save_name or $$force_name
  ** is \fIset\fP too, the selection of the Fcc folder will be changed as well.
  */
  { "save_empty", DT_BOOL, &C_SaveEmpty, true, 0, NULL,
    "When _unset_, mailboxes which contain no saved messages will be removed\n"
    "when closed (the exception is $$spoolfile which is never removed).\n"
    "If _set_, mailboxes are never removed.\n"
    "\n"
    "**Note:** This only applies to mbox and MMDF folders, NeoMutt does not\n"
    "delete MH and Maildir directories.\n"
  },
  /*
  ** .pp
  ** When \fIunset\fP, mailboxes which contain no saved messages will be removed
  ** when closed (the exception is $$spoolfile which is never removed).
  ** If \fIset\fP, mailboxes are never removed.
  ** .pp
  ** \fBNote:\fP This only applies to mbox and MMDF folders, NeoMutt does not
  ** delete MH and Maildir directories.
  */
  { "save_history", DT_NUMBER|DT_NOT_NEGATIVE, &C_SaveHistory, 0, 0, NULL,
    "This variable controls the size of the history (per category) saved in the\n"
    "$$history_file file.\n"
  },
  /*
  ** .pp
  ** This variable controls the size of the history (per category) saved in the
  ** $$history_file file.
  */
  { "save_name", DT_BOOL, &C_SaveName, false, 0, NULL,
    "This variable controls how copies of outgoing messages are saved.\n"
    "When _set_, a check is made to see if a mailbox specified by the\n"
    "recipient address exists (this is done by searching for a mailbox in\n"
    "the $$folder directory with the _username_ part of the\n"
    "recipient address).  If the mailbox exists, the outgoing message will\n"
    "be saved to that mailbox, otherwise the message is saved to the\n"
    "$$record mailbox.\n"
    "\n"
    "Also see the $$force_name variable.\n"
  },
  /*
  ** .pp
  ** This variable controls how copies of outgoing messages are saved.
  ** When \fIset\fP, a check is made to see if a mailbox specified by the
  ** recipient address exists (this is done by searching for a mailbox in
  ** the $$folder directory with the \fIusername\fP part of the
  ** recipient address).  If the mailbox exists, the outgoing message will
  ** be saved to that mailbox, otherwise the message is saved to the
  ** $$record mailbox.
  ** .pp
  ** Also see the $$force_name variable.
  */
#ifdef USE_NNTP
  { "save_unsubscribed", DT_BOOL, &C_SaveUnsubscribed, false, 0, NULL,
    "When _set_, info about unsubscribed newsgroups will be saved into\n"
    "\"newsrc\" file and into cache.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, info about unsubscribed newsgroups will be saved into
  ** "newsrc" file and into cache.
  */
#endif
  { "score", DT_BOOL, &C_Score, true, 0, NULL,
    "When this variable is _unset_, scoring is turned off.  This can\n"
    "be useful to selectively disable scoring for certain folders when the\n"
    "$$score_threshold_delete variable and related are used.\n"
  },
  /*
  ** .pp
  ** When this variable is \fIunset\fP, scoring is turned off.  This can
  ** be useful to selectively disable scoring for certain folders when the
  ** $$score_threshold_delete variable and related are used.
  */
  { "score_threshold_delete", DT_NUMBER, &C_ScoreThresholdDelete, -1, 0, NULL,
    "Messages which have been assigned a score equal to or lower than the value\n"
    "of this variable are automatically marked for deletion by NeoMutt.  Since\n"
    "NeoMutt scores are always greater than or equal to zero, the default setting\n"
    "of this variable will never mark a message for deletion.\n"
  },
  /*
  ** .pp
  ** Messages which have been assigned a score equal to or lower than the value
  ** of this variable are automatically marked for deletion by NeoMutt.  Since
  ** NeoMutt scores are always greater than or equal to zero, the default setting
  ** of this variable will never mark a message for deletion.
  */
  { "score_threshold_flag", DT_NUMBER, &C_ScoreThresholdFlag, 9999, 0, NULL,
    "Messages which have been assigned a score greater than or equal to this\n"
    "variable's value are automatically marked \"flagged\".\n"
  },
  /*
  ** .pp
  ** Messages which have been assigned a score greater than or equal to this
  ** variable's value are automatically marked "flagged".
  */
  { "score_threshold_read", DT_NUMBER, &C_ScoreThresholdRead, -1, 0, NULL,
    "Messages which have been assigned a score equal to or lower than the value\n"
    "of this variable are automatically marked as read by NeoMutt.  Since\n"
    "NeoMutt scores are always greater than or equal to zero, the default setting\n"
    "of this variable will never mark a message read.\n"
  },
  /*
  ** .pp
  ** Messages which have been assigned a score equal to or lower than the value
  ** of this variable are automatically marked as read by NeoMutt.  Since
  ** NeoMutt scores are always greater than or equal to zero, the default setting
  ** of this variable will never mark a message read.
  */
  { "search_context", DT_NUMBER|DT_NOT_NEGATIVE, &C_SearchContext, 0, 0, NULL,
    "For the pager, this variable specifies the number of lines shown\n"
    "before search results. By default, search results will be top-aligned.\n"
  },
  /*
  ** .pp
  ** For the pager, this variable specifies the number of lines shown
  ** before search results. By default, search results will be top-aligned.
  */
  { "send_charset", DT_STRING, &C_SendCharset, IP "us-ascii:iso-8859-1:utf-8", 0, charset_validator,
    "A colon-delimited list of character sets for outgoing messages. NeoMutt will use the\n"
    "first character set into which the text can be converted exactly.\n"
    "If your $$charset is not \"iso-8859-1\" and recipients may not\n"
    "understand \"UTF-8\", it is advisable to include in the list an\n"
    "appropriate widely used standard character set (such as\n"
    "\"iso-8859-2\", \"koi8-r\" or \"iso-2022-jp\") either instead of or after\n"
    "\"iso-8859-1\".\n"
    "\n"
    "In case the text can't be converted into one of these exactly,\n"
    "NeoMutt uses $$charset as a fallback.\n"
  },
  /*
  ** .pp
  ** A colon-delimited list of character sets for outgoing messages. NeoMutt will use the
  ** first character set into which the text can be converted exactly.
  ** If your $$charset is not "iso-8859-1" and recipients may not
  ** understand "UTF-8", it is advisable to include in the list an
  ** appropriate widely used standard character set (such as
  ** "iso-8859-2", "koi8-r" or "iso-2022-jp") either instead of or after
  ** "iso-8859-1".
  ** .pp
  ** In case the text can't be converted into one of these exactly,
  ** NeoMutt uses $$charset as a fallback.
  */
  { "sendmail", DT_STRING|DT_COMMAND, &C_Sendmail, IP SENDMAIL " -oem -oi", 0, NULL,
    "Specifies the program and arguments used to deliver mail sent by NeoMutt.\n"
    "NeoMutt expects that the specified program interprets additional\n"
    "arguments as recipient addresses.  NeoMutt appends all recipients after\n"
    "adding a `--` delimiter (if not already present).  Additional\n"
    "flags, such as for $$use_8bitmime, $$use_envelope_from,\n"
    "$$dsn_notify, or $$dsn_return will be added before the delimiter.\n"
    "\n"
    "**See also:** $$write_bcc.\n"
  },
  /*
  ** .pp
  ** Specifies the program and arguments used to deliver mail sent by NeoMutt.
  ** NeoMutt expects that the specified program interprets additional
  ** arguments as recipient addresses.  NeoMutt appends all recipients after
  ** adding a \fC--\fP delimiter (if not already present).  Additional
  ** flags, such as for $$use_8bitmime, $$use_envelope_from,
  ** $$dsn_notify, or $$dsn_return will be added before the delimiter.
  ** .pp
  ** \fBSee also:\fP $$write_bcc.
  */
  { "sendmail_wait", DT_NUMBER, &C_SendmailWait, 0, 0, NULL,
    "Specifies the number of seconds to wait for the $$sendmail process\n"
    "to finish before giving up and putting delivery in the background.\n"
    "\n"
    "NeoMutt interprets the value of this variable as follows:\n"
    "\n"
    "| .  | Description |\n"
    "|----+--------------------------------------------------------------------|\n"
    "| >0 | number of seconds to wait for sendmail to finish before continuing |\n"
    "| 0  | wait forever for sendmail to finish |\n"
    "| <0 | always put sendmail in the background without waiting |\n"
    "\n"
    "Note that if you specify a value other than 0, the output of the child\n"
    "process will be put in a temporary file.  If there is some error, you\n"
    "will be informed as to where to find the output.\n"
  },
  /*
  ** .pp
  ** Specifies the number of seconds to wait for the $$sendmail process
  ** to finish before giving up and putting delivery in the background.
  ** .pp
  ** NeoMutt interprets the value of this variable as follows:
  ** .dl
  ** .dt >0 .dd number of seconds to wait for sendmail to finish before continuing
  ** .dt 0  .dd wait forever for sendmail to finish
  ** .dt <0 .dd always put sendmail in the background without waiting
  ** .de
  ** .pp
  ** Note that if you specify a value other than 0, the output of the child
  ** process will be put in a temporary file.  If there is some error, you
  ** will be informed as to where to find the output.
  */
  { "shell", DT_STRING|DT_COMMAND, &C_Shell, IP "/bin/sh", 0, NULL,
    "Command to use when spawning a subshell.\n"
    "If not specified, then the user's login shell from `/etc/passwd` is used.\n"
  },
  /*
  ** .pp
  ** Command to use when spawning a subshell.
  ** If not specified, then the user's login shell from \fC/etc/passwd\fP is used.
  */
  { "show_multipart_alternative", DT_STRING, &C_ShowMultipartAlternative, 0, 0, multipart_validator,
    "When _set_ to `info`, the multipart/alternative information is shown.\n"
    "When _set_ to `inline`, all of the alternatives are displayed.\n"
    "When not set, the default behavior is to show only the chosen alternative.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP to \fCinfo\fP, the multipart/alternative information is shown.
  ** When \fIset\fP to \fCinline\fP, all of the alternatives are displayed.
  ** When not set, the default behavior is to show only the chosen alternative.
  */
#ifdef USE_NNTP
  { "show_new_news", DT_BOOL, &C_ShowNewNews, true, 0, NULL,
    "If _set_, news server will be asked for new newsgroups on entering\n"
    "the browser.  Otherwise, it will be done only once for a news server.\n"
    "Also controls whether or not number of new articles of subscribed\n"
    "newsgroups will be then checked.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, news server will be asked for new newsgroups on entering
  ** the browser.  Otherwise, it will be done only once for a news server.
  ** Also controls whether or not number of new articles of subscribed
  ** newsgroups will be then checked.
  */
  { "show_only_unread", DT_BOOL, &C_ShowOnlyUnread, false, 0, NULL,
    "If _set_, only subscribed newsgroups that contain unread articles\n"
    "will be displayed in browser.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, only subscribed newsgroups that contain unread articles
  ** will be displayed in browser.
  */
#endif
#ifdef USE_SIDEBAR
  { "sidebar_component_depth", DT_NUMBER|R_SIDEBAR, &C_SidebarComponentDepth, 0, 0, NULL,
    "By default the sidebar will show the mailbox's path, relative to the\n"
    "$$folder variable. This specifies the number of parent directories to hide\n"
    "from display in the sidebar. For example: If a maildir is normally\n"
    "displayed in the sidebar as dir1/dir2/dir3/maildir, setting\n"
    "`sidebar_component_depth=2` will display it as dir3/maildir, having\n"
    "truncated the 2 highest directories.\n"
    "\n"
    "**See also:** $$sidebar_short_path\n"
  },
  /*
  ** .pp
  ** By default the sidebar will show the mailbox's path, relative to the
  ** $$folder variable. This specifies the number of parent directories to hide
  ** from display in the sidebar. For example: If a maildir is normally
  ** displayed in the sidebar as dir1/dir2/dir3/maildir, setting
  ** \fCsidebar_component_depth=2\fP will display it as dir3/maildir, having
  ** truncated the 2 highest directories.
  ** .pp
  ** \fBSee also:\fP $$sidebar_short_path
  */
  { "sidebar_delim_chars", DT_STRING|R_SIDEBAR, &C_SidebarDelimChars, IP "/.", 0, NULL,
    "This contains the list of characters which you would like to treat\n"
    "as folder separators for displaying paths in the sidebar.\n"
    "\n"
    "Local mail is often arranged in directories: 'dir1/dir2/mailbox'.\n"
    "```\n"
    "set sidebar_delim_chars='/'\n"
    "```\n"
    "\n"
    "IMAP mailboxes are often named: 'folder1.folder2.mailbox'.\n"
    "```\n"
    "set sidebar_delim_chars='.'\n"
    "```\n"
    "\n"
    "**See also:** $$sidebar_short_path, $$sidebar_folder_indent, $$sidebar_indent_string.\n"
  },
  /*
  ** .pp
  ** This contains the list of characters which you would like to treat
  ** as folder separators for displaying paths in the sidebar.
  ** .pp
  ** Local mail is often arranged in directories: 'dir1/dir2/mailbox'.
  ** .ts
  ** set sidebar_delim_chars='/'
  ** .te
  ** .pp
  ** IMAP mailboxes are often named: 'folder1.folder2.mailbox'.
  ** .ts
  ** set sidebar_delim_chars='.'
  ** .te
  ** .pp
  ** \fBSee also:\fP $$sidebar_short_path, $$sidebar_folder_indent, $$sidebar_indent_string.
  */
  { "sidebar_divider_char", DT_STRING|R_SIDEBAR, &C_SidebarDividerChar, 0, 0, NULL,
    "This specifies the characters to be drawn between the sidebar (when\n"
    "visible) and the other NeoMutt panels. ASCII and Unicode line-drawing\n"
    "characters are supported.\n"
  },
  /*
  ** .pp
  ** This specifies the characters to be drawn between the sidebar (when
  ** visible) and the other NeoMutt panels. ASCII and Unicode line-drawing
  ** characters are supported.
  */
  { "sidebar_folder_indent", DT_BOOL|R_SIDEBAR, &C_SidebarFolderIndent, false, 0, NULL,
    "Set this to indent mailboxes in the sidebar.\n"
    "\n"
    "**See also:** $$sidebar_short_path, $$sidebar_indent_string, $$sidebar_delim_chars.\n"
  },
  /*
  ** .pp
  ** Set this to indent mailboxes in the sidebar.
  ** .pp
  ** \fBSee also:\fP $$sidebar_short_path, $$sidebar_indent_string, $$sidebar_delim_chars.
  */
  { "sidebar_format", DT_STRING|DT_NOT_EMPTY|R_SIDEBAR, &C_SidebarFormat, IP "%B%*  %n", 0, NULL,
    "This variable allows you to customize the sidebar display. This string is\n"
    "similar to $$index_format, but has its own set of `printf(3)`-like\n"
    "sequences:\n"
    "\n"
    "| .   | Description |\n"
    "|-----+---------------------------------------------------------------------------------------------------------------------------|\n"
    "| %B  | Name of the mailbox |\n"
    "| %D  | Description of the mailbox |\n"
    "| %S  | * Size of mailbox (total number of messages) |\n"
    "| %N  | * Number of unread messages in the mailbox |\n"
    "| %n  | N if mailbox has new mail, blank otherwise |\n"
    "| %F  | * Number of Flagged messages in the mailbox |\n"
    "| %!  | \"!\" : one flagged message; \"!!\" : two flagged messages; \"n!\" : n flagged messages (for n > 2).  Otherwise prints nothing. |\n"
    "| %d  | * @ Number of deleted messages |\n"
    "| %L  | * @ Number of messages after limiting |\n"
    "| %t  | * @ Number of tagged messages |\n"
    "| %>X | right justify the rest of the string and pad with \"X\" |\n"
    "| %|X | pad to the end of the line with \"X\" |\n"
    "| %*X | soft-fill with character \"X\" as pad |\n"
    "\n"
    "* = Can be optionally printed if nonzero\n"
    "@ = Only applicable to the current folder\n"
    "\n"
    "In order to use %S, %N, %F, and %!, $$mail_check_stats must\n"
    "be _set_.  When thus set, a suggested value for this option is\n"
    "\"%B%?F? [%F]?%* %?N?%N/?%S\".\n"
  },
  /*
  ** .pp
  ** This variable allows you to customize the sidebar display. This string is
  ** similar to $$index_format, but has its own set of \fCprintf(3)\fP-like
  ** sequences:
  ** .dl
  ** .dt %B .dd     .dd Name of the mailbox
  ** .dt %d .dd * @ .dd Number of deleted messages in the mailbox
  ** .dt %D .dd     .dd Descriptive name of the mailbox
  ** .dt %F .dd *   .dd Number of flagged messages in the mailbox
  ** .dt %L .dd * @ .dd Number of messages after limiting
  ** .dt %n .dd     .dd 'N' if mailbox has new mail, ' ' (space) otherwise
  ** .dt %N .dd *   .dd Number of unread messages in the mailbox (seen or unseen)
  ** .dt %o .dd *   .dd Number of old messages in the mailbox (unread, seen)
  ** .dt %r .dd *   .dd Number of read messages in the mailbox (read, seen)
  ** .dt %S .dd *   .dd Size of mailbox (total number of messages)
  ** .dt %t .dd * @ .dd Number of tagged messages in the mailbox
  ** .dt %Z .dd *   .dd Number of new messages in the mailbox (unread, unseen)
  ** .dt %! .dd     .dd "!" : one flagged message;
  **                    "!!" : two flagged messages;
  **                    "n!" : n flagged messages (for n > 2).
  **                    Otherwise prints nothing.
  ** .dt %>X .dd .dd Right justify the rest of the string and pad with "X"
  ** .dt %|X .dd .dd Pad to the end of the line with "X"
  ** .dt %*X .dd .dd Soft-fill with character "X" as pad
  ** .de
  ** .pp
  ** * = Can be optionally printed if nonzero
  ** .pp
  ** @ = Only applicable to the current folder
  ** .pp
  ** In order to use %S, %N, %F, and %!, $$mail_check_stats must
  ** be \fIset\fP.  When thus set, a suggested value for this option is
  ** "%B%?F? [%F]?%* %?N?%N/?%S".
  */
  { "sidebar_indent_string", DT_STRING|R_SIDEBAR, &C_SidebarIndentString, IP "  ", 0, NULL,
    "This specifies the string that is used to indent mailboxes in the sidebar.\n"
    "It defaults to two spaces.\n"
    "\n"
    "**See also:** $$sidebar_short_path, $$sidebar_folder_indent, $$sidebar_delim_chars.\n"
  },
  /*
  ** .pp
  ** This specifies the string that is used to indent mailboxes in the sidebar.
  ** It defaults to two spaces.
  ** .pp
  ** \fBSee also:\fP $$sidebar_short_path, $$sidebar_folder_indent, $$sidebar_delim_chars.
  */
  { "sidebar_new_mail_only", DT_BOOL|R_SIDEBAR, &C_SidebarNewMailOnly, false, 0, NULL,
    "When set, the sidebar will only display mailboxes containing new, or\n"
    "flagged, mail.\n"
    "\n"
    "**See also:** $$sidebar_whitelist, $$sidebar_non_empty_mailbox_only.\n"
  },
  /*
  ** .pp
  ** When set, the sidebar will only display mailboxes containing new, or
  ** flagged, mail.
  ** .pp
  ** \fBSee also:\fP $$sidebar_whitelist, $$sidebar_non_empty_mailbox_only.
  */
  { "sidebar_next_new_wrap", DT_BOOL, &C_SidebarNextNewWrap, false, 0, NULL,
    "When set, the `<sidebar-next-new>` command will not stop and the end of\n"
    "the list of mailboxes, but wrap around to the beginning. The\n"
    "`<sidebar-prev-new>` command is similarly affected, wrapping around to\n"
    "the end of the list.\n"
  },
  /*
  ** .pp
  ** When set, the \fC<sidebar-next-new>\fP command will not stop and the end of
  ** the list of mailboxes, but wrap around to the beginning. The
  ** \fC<sidebar-prev-new>\fP command is similarly affected, wrapping around to
  ** the end of the list.
  */
  { "sidebar_non_empty_mailbox_only", DT_BOOL|R_SIDEBAR, &C_SidebarNonEmptyMailboxOnly, false, 0, NULL,
    "When set, the sidebar will only display mailboxes that contain one or more mails.\n"
    "\n"
    "**See also:** $$sidebar_new_mail_only, $$sidebar_whitelist.\n"
  },
  /*
  ** .pp
  ** When set, the sidebar will only display mailboxes that contain one or more mails.
  ** .pp
  ** \fBSee also:\fP $$sidebar_new_mail_only, $$sidebar_whitelist.
  */
  { "sidebar_on_right", DT_BOOL|R_INDEX|R_PAGER|R_REFLOW, &C_SidebarOnRight, false, 0, NULL,
    "When set, the sidebar will appear on the right-hand side of the screen.\n"
  },
  /*
  ** .pp
  ** When set, the sidebar will appear on the right-hand side of the screen.
  */
  { "sidebar_short_path", DT_BOOL|R_SIDEBAR, &C_SidebarShortPath, false, 0, NULL,
    "By default the sidebar will show the mailbox's path, relative to the\n"
    "$$folder variable. Setting `sidebar_shortpath=yes` will shorten the\n"
    "names relative to the previous name. Here's an example:\n"
    "\n"
    "| shortpath=no   | shortpath=yes | shortpath=yes, folderindent=yes, indentstr=\"..\" |\n"
    "|----------------+---------------+-------------------------------------------------|\n"
    "| `fruit`        | `fruit`       | `fruit` |\n"
    "| `fruit.apple`  | `apple`       | `..apple` |\n"
    "| `fruit.banana` | `banana`      | `..banana` |\n"
    "| `fruit.cherry` | `cherry`      | `..cherry` |\n"
    "\n"
    "**See also:** $$sidebar_delim_chars, $$sidebar_folder_indent,\n"
    "$$sidebar_indent_string, $$sidebar_component_depth.\n"
  },
  /*
  ** .pp
  ** By default the sidebar will show the mailbox's path, relative to the
  ** $$folder variable. Setting \fCsidebar_shortpath=yes\fP will shorten the
  ** names relative to the previous name. Here's an example:
  ** .dl
  ** .dt \fBshortpath=no\fP .dd \fBshortpath=yes\fP .dd \fBshortpath=yes, folderindent=yes, indentstr=".."\fP
  ** .dt \fCfruit\fP        .dd \fCfruit\fP         .dd \fCfruit\fP
  ** .dt \fCfruit.apple\fP  .dd \fCapple\fP         .dd \fC..apple\fP
  ** .dt \fCfruit.banana\fP .dd \fCbanana\fP        .dd \fC..banana\fP
  ** .dt \fCfruit.cherry\fP .dd \fCcherry\fP        .dd \fC..cherry\fP
  ** .de
  ** .pp
  ** \fBSee also:\fP $$sidebar_delim_chars, $$sidebar_folder_indent,
  ** $$sidebar_indent_string, $$sidebar_component_depth.
  */
  { "sidebar_sort_method", DT_SORT|DT_SORT_SIDEBAR|R_SIDEBAR, &C_SidebarSortMethod, SORT_ORDER, 0, NULL,
    "Specifies how to sort entries in the file browser.  By default, the\n"
    "entries are sorted alphabetically.  Valid values:\n"
    "\n"
    "- alpha (alphabetically)\n"
    "- count (all message count)\n"
    "- flagged (flagged message count)\n"
    "- name (alphabetically)\n"
    "- new (unread message count)\n"
    "- path (alphabetically)\n"
    "- unread (unread message count)\n"
    "- unsorted\n"
    "\n"
    "You may optionally use the \"reverse-\" prefix to specify reverse sorting\n"
    "order (example: \"`set sort_browser=reverse-date`\").\n"
  },
  /*
  ** .pp
  ** Specifies how to sort entries in the file browser.  By default, the
  ** entries are sorted alphabetically.  Valid values:
  ** .il
  ** .dd alpha (alphabetically)
  ** .dd count (all message count)
  ** .dd flagged (flagged message count)
  ** .dd name (alphabetically)
  ** .dd new (unread message count)
  ** .dd path (alphabetically)
  ** .dd unread (unread message count)
  ** .dd unsorted
  ** .ie
  ** .pp
  ** You may optionally use the "reverse-" prefix to specify reverse sorting
  ** order (example: "\fCset sort_browser=reverse-date\fP").
  */
  { "sidebar_visible", DT_BOOL|R_REFLOW, &C_SidebarVisible, false, 0, NULL,
    "This specifies whether or not to show sidebar. The sidebar shows a list of\n"
    "all your mailboxes.\n"
    "\n"
    "**See also:** $$sidebar_format, $$sidebar_width\n"
  },
  /*
  ** .pp
  ** This specifies whether or not to show sidebar. The sidebar shows a list of
  ** all your mailboxes.
  ** .pp
  ** \fBSee also:\fP $$sidebar_format, $$sidebar_width
  */
  { "sidebar_width", DT_NUMBER|DT_NOT_NEGATIVE|R_REFLOW, &C_SidebarWidth, 30, 0, NULL,
    "This controls the width of the sidebar.  It is measured in screen columns.\n"
    "For example: sidebar_width=20 could display 20 ASCII characters, or 10\n"
    "Chinese characters.\n"
  },
  /*
  ** .pp
  ** This controls the width of the sidebar.  It is measured in screen columns.
  ** For example: sidebar_width=20 could display 20 ASCII characters, or 10
  ** Chinese characters.
  */
#endif
  { "sig_dashes", DT_BOOL, &C_SigDashes, true, 0, NULL,
    "If _set_, a line containing \"-- \" (note the trailing space) will be inserted before your\n"
    "$$signature.  It is **strongly** recommended that you not _unset_\n"
    "this variable unless your signature contains just your name.  The\n"
    "reason for this is because many software packages use \"-- \n\" to\n"
    "detect your signature.  For example, NeoMutt has the ability to highlight\n"
    "the signature in a different color in the built-in pager.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, a line containing "-- " (note the trailing space) will be inserted before your
  ** $$signature.  It is \fBstrongly\fP recommended that you not \fIunset\fP
  ** this variable unless your signature contains just your name.  The
  ** reason for this is because many software packages use "-- \n" to
  ** detect your signature.  For example, NeoMutt has the ability to highlight
  ** the signature in a different color in the built-in pager.
  */
  { "sig_on_top", DT_BOOL, &C_SigOnTop, false, 0, NULL,
    "If _set_, the signature will be included before any quoted or forwarded\n"
    "text.  It is **strongly** recommended that you do not set this variable\n"
    "unless you really know what you are doing, and are prepared to take\n"
    "some heat from netiquette guardians.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, the signature will be included before any quoted or forwarded
  ** text.  It is \fBstrongly\fP recommended that you do not set this variable
  ** unless you really know what you are doing, and are prepared to take
  ** some heat from netiquette guardians.
  */
  { "signature", DT_STRING|DT_PATH, &C_Signature, IP "~/.signature", 0, NULL,
    "Specifies the filename of your signature, which is appended to all\n"
    "outgoing messages.   If the filename ends with a pipe (\"|\"), it is\n"
    "assumed that filename is a shell command and input should be read from\n"
    "its standard output.\n"
  },
  /*
  ** .pp
  ** Specifies the filename of your signature, which is appended to all
  ** outgoing messages.   If the filename ends with a pipe ("|"), it is
  ** assumed that filename is a shell command and input should be read from
  ** its standard output.
  */
  { "simple_search", DT_STRING, &C_SimpleSearch, IP "~f %s | ~s %s", 0, NULL,
    "Specifies how NeoMutt should expand a simple search into a real search\n"
    "pattern.  A simple search is one that does not contain any of the \"~\" pattern\n"
    "operators.  See \"$patterns\" for more information on search patterns.\n"
    "\n"
    "For example, if you simply type \"joe\" at a search or limit prompt, NeoMutt\n"
    "will automatically expand it to the value specified by this variable by\n"
    "replacing \"%s\" with the supplied string.\n"
    "For the default value, \"joe\" would be expanded to: \"~f joe | ~s joe\".\n"
  },
  /*
  ** .pp
  ** Specifies how NeoMutt should expand a simple search into a real search
  ** pattern.  A simple search is one that does not contain any of the "~" pattern
  ** operators.  See "$patterns" for more information on search patterns.
  ** .pp
  ** For example, if you simply type "joe" at a search or limit prompt, NeoMutt
  ** will automatically expand it to the value specified by this variable by
  ** replacing "%s" with the supplied string.
  ** For the default value, "joe" would be expanded to: "~f joe | ~s joe".
  */
  { "size_show_bytes", DT_BOOL|R_MENU, &C_SizeShowBytes, false, 0, NULL,
    "If _set_, message sizes will display bytes for values less than\n"
    "1 kilobyte.  See $formatstrings-size.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, message sizes will display bytes for values less than
  ** 1 kilobyte.  See $formatstrings-size.
  */
  { "size_show_fractions", DT_BOOL|R_MENU, &C_SizeShowFractions, true, 0, NULL,
    "If _set_, message sizes will be displayed with a single decimal value\n"
    "for sizes from 0 to 10 kilobytes and 1 to 10 megabytes.\n"
    "See $formatstrings-size.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, message sizes will be displayed with a single decimal value
  ** for sizes from 0 to 10 kilobytes and 1 to 10 megabytes.
  ** See $formatstrings-size.
  */
  { "size_show_mb", DT_BOOL|R_MENU, &C_SizeShowMb, true, 0, NULL,
    "If _set_, message sizes will display megabytes for values greater than\n"
    "or equal to 1 megabyte.  See $formatstrings-size.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, message sizes will display megabytes for values greater than
  ** or equal to 1 megabyte.  See $formatstrings-size.
  */
  { "size_units_on_left", DT_BOOL|R_MENU, &C_SizeUnitsOnLeft, false, 0, NULL,
    "If _set_, message sizes units will be displayed to the left of the number.\n"
    "See $formatstrings-size.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, message sizes units will be displayed to the left of the number.
  ** See $formatstrings-size.
  */
  { "skip_quoted_offset", DT_NUMBER|DT_NOT_NEGATIVE, &C_SkipQuotedOffset, 0, 0, NULL,
    "Lines of quoted text that are displayed before the unquoted text after\n"
    "\"skip to quoted\" command (S)\n"
  },
  /*
  ** .pp
  ** Lines of quoted text that are displayed before the unquoted text after
  ** "skip to quoted" command (S)
  */
  { "sleep_time", DT_NUMBER|DT_NOT_NEGATIVE, &C_SleepTime, 1, 0, NULL,
    "Specifies time, in seconds, to pause while displaying certain informational\n"
    "messages, while moving from folder to folder and after expunging\n"
    "messages from the current folder.  The default is to pause one second, so\n"
    "a value of zero for this option suppresses the pause.\n"
  },
  /*
  ** .pp
  ** Specifies time, in seconds, to pause while displaying certain informational
  ** messages, while moving from folder to folder and after expunging
  ** messages from the current folder.  The default is to pause one second, so
  ** a value of zero for this option suppresses the pause.
  */
  { "smart_wrap", DT_BOOL|R_PAGER_FLOW, &C_SmartWrap, true, 0, NULL,
    "Controls the display of lines longer than the screen width in the\n"
    "internal pager. If _set_, long lines are wrapped at a word boundary.  If\n"
    "_unset_, lines are simply wrapped at the screen edge. Also see the\n"
    "$$markers variable.\n"
  },
  /*
  ** .pp
  ** Controls the display of lines longer than the screen width in the
  ** internal pager. If \fIset\fP, long lines are wrapped at a word boundary.  If
  ** \fIunset\fP, lines are simply wrapped at the screen edge. Also see the
  ** $$markers variable.
  */
  { "smileys", DT_REGEX|R_PAGER, &C_Smileys, IP "(>From )|(:[-^]?[][)(><}{|/DP])", 0, NULL,
    "The _pager_ uses this variable to catch some common false\n"
    "positives of $$quote_regex, most notably smileys and not consider\n"
    "a line quoted text if it also matches $$smileys. This mostly\n"
    "happens at the beginning of a line.\n"
  },
  /*
  ** .pp
  ** The \fIpager\fP uses this variable to catch some common false
  ** positives of $$quote_regex, most notably smileys and not consider
  ** a line quoted text if it also matches $$smileys. This mostly
  ** happens at the beginning of a line.
  */
#ifdef CRYPT_BACKEND_CLASSIC_SMIME
  { "smime_ask_cert_label", DT_BOOL, &C_SmimeAskCertLabel, true, 0, NULL,
    "This flag controls whether you want to be asked to enter a label\n"
    "for a certificate about to be added to the database or not. It is\n"
    "_set_ by default.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** This flag controls whether you want to be asked to enter a label
  ** for a certificate about to be added to the database or not. It is
  ** \fIset\fP by default.
  ** (S/MIME only)
  */
  { "smime_ca_location", DT_STRING|DT_PATH, &C_SmimeCaLocation, 0, 0, NULL,
    "This variable contains the name of either a directory, or a file which\n"
    "contains trusted certificates for use with OpenSSL.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** This variable contains the name of either a directory, or a file which
  ** contains trusted certificates for use with OpenSSL.
  ** (S/MIME only)
  */
  { "smime_certificates", DT_STRING|DT_PATH, &C_SmimeCertificates, 0, 0, NULL,
    "Since for S/MIME there is no pubring/secring as with PGP, NeoMutt has to handle\n"
    "storage and retrieval of keys by itself. This is very basic right\n"
    "now, and keys and certificates are stored in two different\n"
    "directories, both named as the hash-value retrieved from\n"
    "OpenSSL. There is an index file which contains mailbox-address\n"
    "keyid pairs, and which can be manually edited. This option points to\n"
    "the location of the certificates.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** Since for S/MIME there is no pubring/secring as with PGP, NeoMutt has to handle
  ** storage and retrieval of keys by itself. This is very basic right
  ** now, and keys and certificates are stored in two different
  ** directories, both named as the hash-value retrieved from
  ** OpenSSL. There is an index file which contains mailbox-address
  ** keyid pairs, and which can be manually edited. This option points to
  ** the location of the certificates.
  ** (S/MIME only)
  */
  { "smime_decrypt_command", DT_STRING|DT_COMMAND, &C_SmimeDecryptCommand, 0, 0, NULL,
    "This format string specifies a command which is used to decrypt\n"
    "`application/x-pkcs7-mime` attachments.\n"
    "\n"
    "The OpenSSL command formats have their own set of `printf(3)`-like sequences\n"
    "similar to PGP's:\n"
    "\n"
    "| .  | Description |\n"
    "|----+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n"
    "| %f | Expands to the name of a file containing a message. |\n"
    "| %s | Expands to the name of a file containing the signature part of a `multipart/signed` attachment when verifying it. |\n"
    "| %k | The key-pair specified with $$smime_default_key |\n"
    "| %i | Intermediate certificates |\n"
    "| %c | One or more certificate IDs. |\n"
    "| %a | The algorithm used for encryption. |\n"
    "| %d | The message digest algorithm specified with $$smime_sign_digest_alg. |\n"
    "| %C | CA location:  Depending on whether $$smime_ca_location points to a directory or file, this expands to \"-CApath $$smime_ca_location\" or \"-CAfile $$smime_ca_location\". |\n"
    "\n"
    "For examples on how to configure these formats, see the `smime.rc` in\n"
    "the `samples/` subdirectory which has been installed on your system\n"
    "alongside the documentation.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** This format string specifies a command which is used to decrypt
  ** \fCapplication/x-pkcs7-mime\fP attachments.
  ** .pp
  ** The OpenSSL command formats have their own set of \fCprintf(3)\fP-like sequences
  ** similar to PGP's:
  ** .dl
  ** .dt %f .dd Expands to the name of a file containing a message.
  ** .dt %s .dd Expands to the name of a file containing the signature part
  ** .          of a \fCmultipart/signed\fP attachment when verifying it.
  ** .dt %k .dd The key-pair specified with $$smime_default_key
  ** .dt %i .dd Intermediate certificates
  ** .dt %c .dd One or more certificate IDs.
  ** .dt %a .dd The algorithm used for encryption.
  ** .dt %d .dd The message digest algorithm specified with $$smime_sign_digest_alg.
  ** .dt %C .dd CA location:  Depending on whether $$smime_ca_location
  ** .          points to a directory or file, this expands to
  ** .          "-CApath $$smime_ca_location" or "-CAfile $$smime_ca_location".
  ** .de
  ** .pp
  ** For examples on how to configure these formats, see the \fCsmime.rc\fP in
  ** the \fCsamples/\fP subdirectory which has been installed on your system
  ** alongside the documentation.
  ** (S/MIME only)
  */
  { "smime_decrypt_use_default_key", DT_BOOL, &C_SmimeDecryptUseDefaultKey, true, 0, NULL,
    "If _set_ (default) this tells NeoMutt to use the default key for decryption. Otherwise,\n"
    "if managing multiple certificate-key-pairs, NeoMutt will try to use the mailbox-address\n"
    "to determine the key to use. It will ask you to supply a key, if it can't find one.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** If \fIset\fP (default) this tells NeoMutt to use the default key for decryption. Otherwise,
  ** if managing multiple certificate-key-pairs, NeoMutt will try to use the mailbox-address
  ** to determine the key to use. It will ask you to supply a key, if it can't find one.
  ** (S/MIME only)
  */
#endif
  { "smime_default_key", DT_STRING, &C_SmimeDefaultKey, 0, 0, NULL,
    "This is the default key-pair to use for S/MIME operations, and must be\n"
    "set to the keyid (the hash-value that OpenSSL generates) to work properly.\n"
    "\n"
    "It will be used for encryption (see $$postpone_encrypt and\n"
    "$$smime_self_encrypt).\n"
    "\n"
    "It will be used for decryption unless $$smime_decrypt_use_default_key\n"
    "is _unset_.\n"
    "\n"
    "It will also be used for signing unless $$smime_sign_as is set.\n"
    "\n"
    "The (now deprecated) _smime_self_encrypt_as_ is an alias for this\n"
    "variable, and should no longer be used.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** This is the default key-pair to use for S/MIME operations, and must be
  ** set to the keyid (the hash-value that OpenSSL generates) to work properly.
  ** .pp
  ** It will be used for encryption (see $$postpone_encrypt and
  ** $$smime_self_encrypt).
  ** .pp
  ** It will be used for decryption unless $$smime_decrypt_use_default_key
  ** is \fIunset\fP.
  ** .pp
  ** It will also be used for signing unless $$smime_sign_as is set.
  ** .pp
  ** The (now deprecated) \fIsmime_self_encrypt_as\fP is an alias for this
  ** variable, and should no longer be used.
  ** (S/MIME only)
  */
#ifdef CRYPT_BACKEND_CLASSIC_SMIME
  { "smime_encrypt_command", DT_STRING|DT_COMMAND, &C_SmimeEncryptCommand, 0, 0, NULL,
    "This command is used to create encrypted S/MIME messages.\n"
    "\n"
    "This is a format string, see the $$smime_decrypt_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "(S/MIME only)\n"
    "*/\n"
    "/*\n"
    "\n"
    "Encrypt the message to $$smime_default_key too.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** This command is used to create encrypted S/MIME messages.
  ** .pp
  ** This is a format string, see the $$smime_decrypt_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** (S/MIME only)
  */
  /*
  ** .pp
  ** Encrypt the message to $$smime_default_key too.
  ** (S/MIME only)
  */
#endif
  { "smime_encrypt_with", DT_STRING, &C_SmimeEncryptWith, IP "aes256", 0, NULL,
    "This sets the algorithm that should be used for encryption.\n"
    "Valid choices are \"aes128\", \"aes192\", \"aes256\", \"des\", \"des3\", \"rc2-40\", \"rc2-64\", \"rc2-128\".\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** This sets the algorithm that should be used for encryption.
  ** Valid choices are "aes128", "aes192", "aes256", "des", "des3", "rc2-40", "rc2-64", "rc2-128".
  ** (S/MIME only)
  */
#ifdef CRYPT_BACKEND_CLASSIC_SMIME
  { "smime_get_cert_command", DT_STRING|DT_COMMAND, &C_SmimeGetCertCommand, 0, 0, NULL,
    "This command is used to extract X509 certificates from a PKCS7 structure.\n"
    "\n"
    "This is a format string, see the $$smime_decrypt_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** This command is used to extract X509 certificates from a PKCS7 structure.
  ** .pp
  ** This is a format string, see the $$smime_decrypt_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** (S/MIME only)
  */
  { "smime_get_cert_email_command", DT_STRING|DT_COMMAND, &C_SmimeGetCertEmailCommand, 0, 0, NULL,
    "This command is used to extract the mail address(es) used for storing\n"
    "X509 certificates, and for verification purposes (to check whether the\n"
    "certificate was issued for the sender's mailbox).\n"
    "\n"
    "This is a format string, see the $$smime_decrypt_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** This command is used to extract the mail address(es) used for storing
  ** X509 certificates, and for verification purposes (to check whether the
  ** certificate was issued for the sender's mailbox).
  ** .pp
  ** This is a format string, see the $$smime_decrypt_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** (S/MIME only)
  */
  { "smime_get_signer_cert_command", DT_STRING|DT_COMMAND, &C_SmimeGetSignerCertCommand, 0, 0, NULL,
    "This command is used to extract only the signers X509 certificate from a S/MIME\n"
    "signature, so that the certificate's owner may get compared to the\n"
    "email's \"From:\" field.\n"
    "\n"
    "This is a format string, see the $$smime_decrypt_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** This command is used to extract only the signers X509 certificate from a S/MIME
  ** signature, so that the certificate's owner may get compared to the
  ** email's "From:" field.
  ** .pp
  ** This is a format string, see the $$smime_decrypt_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** (S/MIME only)
  */
  { "smime_import_cert_command", DT_STRING|DT_COMMAND, &C_SmimeImportCertCommand, 0, 0, NULL,
    "This command is used to import a certificate via smime_keys.\n"
    "\n"
    "This is a format string, see the $$smime_decrypt_command command for\n"
    "possible `printf(3)`-like sequences.  NOTE: %c and %k will default\n"
    "to $$smime_sign_as if set, otherwise $$smime_default_key.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** This command is used to import a certificate via smime_keys.
  ** .pp
  ** This is a format string, see the $$smime_decrypt_command command for
  ** possible \fCprintf(3)\fP-like sequences.  NOTE: %c and %k will default
  ** to $$smime_sign_as if set, otherwise $$smime_default_key.
  ** (S/MIME only)
  */
#endif
  { "smime_is_default", DT_BOOL, &C_SmimeIsDefault, false, 0, NULL,
    "The default behavior of NeoMutt is to use PGP on all auto-sign/encryption\n"
    "operations. To override and to use OpenSSL instead this must be _set_.\n"
    "However, this has no effect while replying, since NeoMutt will automatically\n"
    "select the same application that was used to sign/encrypt the original\n"
    "message.  (Note that this variable can be overridden by unsetting $$crypt_autosmime.)\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** The default behavior of NeoMutt is to use PGP on all auto-sign/encryption
  ** operations. To override and to use OpenSSL instead this must be \fIset\fP.
  ** However, this has no effect while replying, since NeoMutt will automatically
  ** select the same application that was used to sign/encrypt the original
  ** message.  (Note that this variable can be overridden by unsetting $$crypt_autosmime.)
  ** (S/MIME only)
  */
#ifdef CRYPT_BACKEND_CLASSIC_SMIME
  { "smime_keys", DT_STRING|DT_PATH, &C_SmimeKeys, 0, 0, NULL,
    "Since for S/MIME there is no pubring/secring as with PGP, NeoMutt has to handle\n"
    "storage and retrieval of keys/certs by itself. This is very basic right now,\n"
    "and stores keys and certificates in two different directories, both\n"
    "named as the hash-value retrieved from OpenSSL. There is an index file\n"
    "which contains mailbox-address keyid pair, and which can be manually\n"
    "edited. This option points to the location of the private keys.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** Since for S/MIME there is no pubring/secring as with PGP, NeoMutt has to handle
  ** storage and retrieval of keys/certs by itself. This is very basic right now,
  ** and stores keys and certificates in two different directories, both
  ** named as the hash-value retrieved from OpenSSL. There is an index file
  ** which contains mailbox-address keyid pair, and which can be manually
  ** edited. This option points to the location of the private keys.
  ** (S/MIME only)
  */
  { "smime_pk7out_command", DT_STRING|DT_COMMAND, &C_SmimePk7outCommand, 0, 0, NULL,
    "This command is used to extract PKCS7 structures of S/MIME signatures,\n"
    "in order to extract the public X509 certificate(s).\n"
    "\n"
    "This is a format string, see the $$smime_decrypt_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** This command is used to extract PKCS7 structures of S/MIME signatures,
  ** in order to extract the public X509 certificate(s).
  ** .pp
  ** This is a format string, see the $$smime_decrypt_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** (S/MIME only)
  */
#endif
  { "smime_self_encrypt", DT_BOOL, &C_SmimeSelfEncrypt, true, 0, NULL,
    "When _set_, S/MIME encrypted messages will also be encrypted\n"
    "using the certificate in $$smime_default_key.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, S/MIME encrypted messages will also be encrypted
  ** using the certificate in $$smime_default_key.
  ** (S/MIME only)
  */
  { "smime_sign_as", DT_STRING, &C_SmimeSignAs, 0, 0, NULL,
    "If you have a separate key to use for signing, you should set this\n"
    "to the signing key. Most people will only need to set $$smime_default_key.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** If you have a separate key to use for signing, you should set this
  ** to the signing key. Most people will only need to set $$smime_default_key.
  ** (S/MIME only)
  */
#ifdef CRYPT_BACKEND_CLASSIC_SMIME
  { "smime_sign_command", DT_STRING|DT_COMMAND, &C_SmimeSignCommand, 0, 0, NULL,
    "This command is used to created S/MIME signatures of type\n"
    "`multipart/signed`, which can be read by all mail clients.\n"
    "\n"
    "This is a format string, see the $$smime_decrypt_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** This command is used to created S/MIME signatures of type
  ** \fCmultipart/signed\fP, which can be read by all mail clients.
  ** .pp
  ** This is a format string, see the $$smime_decrypt_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** (S/MIME only)
  */
  { "smime_sign_digest_alg", DT_STRING, &C_SmimeSignDigestAlg, IP "sha256", 0, NULL,
    "This sets the algorithm that should be used for the signature message digest.\n"
    "Valid choices are \"md5\", \"sha1\", \"sha224\", \"sha256\", \"sha384\", \"sha512\".\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** This sets the algorithm that should be used for the signature message digest.
  ** Valid choices are "md5", "sha1", "sha224", "sha256", "sha384", "sha512".
  ** (S/MIME only)
  */
  { "smime_timeout", DT_NUMBER|DT_NOT_NEGATIVE, &C_SmimeTimeout, 300, 0, NULL,
    "The number of seconds after which a cached passphrase will expire if\n"
    "not used.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** The number of seconds after which a cached passphrase will expire if
  ** not used.
  ** (S/MIME only)
  */
  { "smime_verify_command", DT_STRING|DT_COMMAND, &C_SmimeVerifyCommand, 0, 0, NULL,
    "This command is used to verify S/MIME signatures of type `multipart/signed`.\n"
    "\n"
    "This is a format string, see the $$smime_decrypt_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** This command is used to verify S/MIME signatures of type \fCmultipart/signed\fP.
  ** .pp
  ** This is a format string, see the $$smime_decrypt_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** (S/MIME only)
  */
  { "smime_verify_opaque_command", DT_STRING|DT_COMMAND, &C_SmimeVerifyOpaqueCommand, 0, 0, NULL,
    "This command is used to verify S/MIME signatures of type\n"
    "`application/x-pkcs7-mime`.\n"
    "\n"
    "This is a format string, see the $$smime_decrypt_command command for\n"
    "possible `printf(3)`-like sequences.\n"
    "(S/MIME only)\n"
  },
  /*
  ** .pp
  ** This command is used to verify S/MIME signatures of type
  ** \fCapplication/x-pkcs7-mime\fP.
  ** .pp
  ** This is a format string, see the $$smime_decrypt_command command for
  ** possible \fCprintf(3)\fP-like sequences.
  ** (S/MIME only)
  */
#endif
#ifdef USE_SMTP
  { "smtp_authenticators", DT_SLIST|SLIST_SEP_COLON, &C_SmtpAuthenticators, 0, 0, NULL,
    "This is a colon-delimited list of authentication methods NeoMutt may\n"
    "attempt to use to log in to an SMTP server, in the order NeoMutt should\n"
    "try them.  Authentication methods are any SASL mechanism, e.g. \"plain\",\n"
    "\"digest-md5\", \"gssapi\" or \"cram-md5\".\n"
    "This option is case-insensitive. If it is \"unset\"\n"
    "(the default) NeoMutt will try all available methods, in order from\n"
    "most-secure to least-secure. Support for the \"plain\" mechanism is\n"
    "bundled; other mechanisms are provided by an external SASL library (look\n"
    "for +USE_SASL in the output of neomutt -v).\n"
    "\n"
    "Example:\n"
    "```\n"
    "set smtp_authenticators=\"digest-md5:cram-md5\"\n"
    "```\n"
  },
  /*
  ** .pp
  ** This is a colon-delimited list of authentication methods NeoMutt may
  ** attempt to use to log in to an SMTP server, in the order NeoMutt should
  ** try them.  Authentication methods are any SASL mechanism, e.g. "plain",
  ** "digest-md5", "gssapi" or "cram-md5".
  ** This option is case-insensitive. If it is "unset"
  ** (the default) NeoMutt will try all available methods, in order from
  ** most-secure to least-secure. Support for the "plain" mechanism is
  ** bundled; other mechanisms are provided by an external SASL library (look
  ** for +USE_SASL in the output of neomutt -v).
  ** .pp
  ** Example:
  ** .ts
  ** set smtp_authenticators="digest-md5:cram-md5"
  ** .te
  */
  { "smtp_oauth_refresh_command", DT_STRING|DT_COMMAND|DT_SENSITIVE, &C_SmtpOauthRefreshCommand, 0, 0, NULL,
    "The command to run to generate an OAUTH refresh token for\n"
    "authorizing your connection to your SMTP server.  This command will be\n"
    "run on every connection attempt that uses the OAUTHBEARER authentication\n"
    "mechanism.  See \"$oauth\" for details.\n"
  },
  /*
  ** .pp
  ** The command to run to generate an OAUTH refresh token for
  ** authorizing your connection to your SMTP server.  This command will be
  ** run on every connection attempt that uses the OAUTHBEARER authentication
  ** mechanism.  See "$oauth" for details.
  */
  { "smtp_pass", DT_STRING|DT_SENSITIVE, &C_SmtpPass, 0, 0, NULL,
    "Specifies the password for your SMTP account.  If _unset_, NeoMutt will\n"
    "prompt you for your password when you first send mail via SMTP.\n"
    "See $$smtp_url to configure NeoMutt to send mail via SMTP.\n"
    "\n"
    "**Warning**: you should only use this option when you are on a\n"
    "fairly secure machine, because the superuser can read your neomuttrc even\n"
    "if you are the only one who can read the file.\n"
  },
  /*
  ** .pp
  ** Specifies the password for your SMTP account.  If \fIunset\fP, NeoMutt will
  ** prompt you for your password when you first send mail via SMTP.
  ** See $$smtp_url to configure NeoMutt to send mail via SMTP.
  ** .pp
  ** \fBWarning\fP: you should only use this option when you are on a
  ** fairly secure machine, because the superuser can read your neomuttrc even
  ** if you are the only one who can read the file.
  */
  { "smtp_user", DT_STRING|DT_SENSITIVE, &C_SmtpUser, 0, 0, NULL,
    "The username for the SMTP server.\n"
    "\n"
    "This variable defaults to your user name on the local machine.\n"
  },
  /*
  ** .pp
  ** The username for the SMTP server.
  ** .pp
  ** This variable defaults to your user name on the local machine.
  */
  { "smtp_url", DT_STRING|DT_SENSITIVE, &C_SmtpUrl, 0, 0, NULL,
    "Defines the SMTP smarthost where sent messages should relayed for\n"
    "delivery. This should take the form of an SMTP URL, e.g.:\n"
    "```\n"
    "smtp[s]://[user[:pass]@]host[:port]\n"
    "```\n"
    "\n"
    "where \"[...]\" denotes an optional part.\n"
    "Setting this variable overrides the value of the $$sendmail\n"
    "variable.\n"
    "\n"
    "Also see $$write_bcc.\n"
  },
  /*
  ** .pp
  ** Defines the SMTP smarthost where sent messages should relayed for
  ** delivery. This should take the form of an SMTP URL, e.g.:
  ** .ts
  ** smtp[s]://[user[:pass]@]host[:port]
  ** .te
  ** .pp
  ** where "[...]" denotes an optional part.
  ** Setting this variable overrides the value of the $$sendmail
  ** variable.
  ** .pp
  ** Also see $$write_bcc.
  */
#endif /* USE_SMTP */
  { "sort", DT_SORT|R_INDEX|R_RESORT, &C_Sort, SORT_DATE, 0, pager_validator,
    "Specifies how to sort messages in the \"index\" menu.  Valid values\n"
    "are:\n"
    "\n"
    "- date or date-sent\n"
    "- date-received\n"
    "- from\n"
    "- mailbox-order (unsorted)\n"
    "- score\n"
    "- size\n"
    "- spam\n"
    "- subject\n"
    "- threads\n"
    "- to\n"
    "\n"
    "You may optionally use the \"reverse-\" prefix to specify reverse sorting\n"
    "order.\n"
    "\n"
    "Example:\n"
    "```\n"
    "set sort=reverse-date-sent\n"
    "```\n"
  },
  /*
  ** .pp
  ** Specifies how to sort messages in the "index" menu.  Valid values
  ** are:
  ** .il
  ** .dd date or date-sent
  ** .dd date-received
  ** .dd from
  ** .dd mailbox-order (unsorted)
  ** .dd score
  ** .dd size
  ** .dd spam
  ** .dd subject
  ** .dd threads
  ** .dd to
  ** .ie
  ** .pp
  ** You may optionally use the "reverse-" prefix to specify reverse sorting
  ** order.
  ** .pp
  ** Example:
  ** .ts
  ** set sort=reverse-date-sent
  ** .te
  */
  { "sort_alias", DT_SORT|DT_SORT_ALIAS, &C_SortAlias, SORT_ALIAS, 0, NULL,
    "Specifies how the entries in the \"alias\" menu are sorted.  The\n"
    "following are legal values:\n"
    "\n"
    "- address (sort alphabetically by email address)\n"
    "- alias (sort alphabetically by alias name)\n"
    "- unsorted (leave in order specified in .neomuttrc)\n"
  },
  /*
  ** .pp
  ** Specifies how the entries in the "alias" menu are sorted.  The
  ** following are legal values:
  ** .il
  ** .dd address (sort alphabetically by email address)
  ** .dd alias (sort alphabetically by alias name)
  ** .dd unsorted (leave in order specified in .neomuttrc)
  ** .ie
  */
  { "sort_aux", DT_SORT|DT_SORT_AUX|R_INDEX|R_RESORT|R_RESORT_SUB, &C_SortAux, SORT_DATE, 0, NULL,
    "This provides a secondary sort for messages in the \"index\" menu, used\n"
    "when the $$sort value is equal for two messages.\n"
    "\n"
    "When sorting by threads, this variable controls how threads are sorted\n"
    "in relation to other threads, and how the branches of the thread trees\n"
    "are sorted.  This can be set to any value that $$sort can, except\n"
    "\"threads\" (in that case, NeoMutt will just use \"date-sent\").  You can also\n"
    "specify the \"last-\" prefix in addition to the \"reverse-\" prefix, but \"last-\"\n"
    "must come after \"reverse-\".  The \"last-\" prefix causes messages to be\n"
    "sorted against its siblings by which has the last descendant, using\n"
    "the rest of $$sort_aux as an ordering.  For instance,\n"
    "```\n"
    "set sort_aux=last-date-received\n"
    "```\n"
    "\n"
    "would mean that if a new message is received in a\n"
    "thread, that thread becomes the last one displayed (or the first, if\n"
    "you have \"`set sort=reverse-threads`\".)\n"
    "\n"
    "Note: For reversed-threads $$sort\n"
    "order, $$sort_aux is reversed again (which is not the right thing to do,\n"
    "but kept to not break any existing configuration setting).\n"
  },
  /*
  ** .pp
  ** This provides a secondary sort for messages in the "index" menu, used
  ** when the $$sort value is equal for two messages.
  ** .pp
  ** When sorting by threads, this variable controls how threads are sorted
  ** in relation to other threads, and how the branches of the thread trees
  ** are sorted.  This can be set to any value that $$sort can, except
  ** "threads" (in that case, NeoMutt will just use "date-sent").  You can also
  ** specify the "last-" prefix in addition to the "reverse-" prefix, but "last-"
  ** must come after "reverse-".  The "last-" prefix causes messages to be
  ** sorted against its siblings by which has the last descendant, using
  ** the rest of $$sort_aux as an ordering.  For instance,
  ** .ts
  ** set sort_aux=last-date-received
  ** .te
  ** .pp
  ** would mean that if a new message is received in a
  ** thread, that thread becomes the last one displayed (or the first, if
  ** you have "\fCset sort=reverse-threads\fP".)
  ** .pp
  ** Note: For reversed-threads $$sort
  ** order, $$sort_aux is reversed again (which is not the right thing to do,
  ** but kept to not break any existing configuration setting).
  */
  { "sort_browser", DT_SORT|DT_SORT_BROWSER, &C_SortBrowser, SORT_ALPHA, 0, NULL,
    "Specifies how to sort entries in the file browser.  By default, the\n"
    "entries are sorted alphabetically.  Valid values:\n"
    "\n"
    "- alpha (alphabetically)\n"
    "- count (all message count)\n"
    "- date\n"
    "- desc (description)\n"
    "- new (new message count)\n"
    "- size\n"
    "- unsorted\n"
    "\n"
    "You may optionally use the \"reverse-\" prefix to specify reverse sorting\n"
    "order (example: \"`set sort_browser=reverse-date`\").\n"
  },
  /*
  ** .pp
  ** Specifies how to sort entries in the file browser.  By default, the
  ** entries are sorted alphabetically.  Valid values:
  ** .il
  ** .dd alpha (alphabetically)
  ** .dd count (all message count)
  ** .dd date
  ** .dd desc (description)
  ** .dd new (new message count)
  ** .dd size
  ** .dd unsorted
  ** .ie
  ** .pp
  ** You may optionally use the "reverse-" prefix to specify reverse sorting
  ** order (example: "\fCset sort_browser=reverse-date\fP").
  */
  { "sort_re", DT_BOOL|R_INDEX|R_RESORT|R_RESORT_INIT, &C_SortRe, true, 0, pager_validator,
    "This variable is only useful when sorting by mailboxes in sidebar. By default,\n"
    "entries are unsorted.  Valid values:\n"
    "\n"
    "- count (all message count)\n"
    "- desc  (virtual mailbox description)\n"
    "- new (new message count)\n"
    "- path\n"
    "- unsorted\n"
  },
  /*
  ** .pp
  ** This variable is only useful when sorting by mailboxes in sidebar. By default,
  ** entries are unsorted.  Valid values:
  ** .il
  ** .dd count (all message count)
  ** .dd desc  (virtual mailbox description)
  ** .dd new (new message count)
  ** .dd path
  ** .dd unsorted
  ** .ie
  */
  { "spam_separator", DT_STRING, &C_SpamSeparator, IP ",", 0, NULL,
    "This variable controls what happens when multiple spam headers\n"
    "are matched: if _unset_, each successive header will overwrite any\n"
    "previous matches value for the spam label. If _set_, each successive\n"
    "match will append to the previous, using this variable's value as a\n"
    "separator.\n"
  },
  /*
  ** .pp
  ** This variable controls what happens when multiple spam headers
  ** are matched: if \fIunset\fP, each successive header will overwrite any
  ** previous matches value for the spam label. If \fIset\fP, each successive
  ** match will append to the previous, using this variable's value as a
  ** separator.
  */
  { "spoolfile", DT_STRING|DT_PATH|DT_MAILBOX, &C_Spoolfile, 0, 0, NULL,
    "If your spool mailbox is in a non-default place where NeoMutt can't find\n"
    "it, you can specify its location with this variable. The description from\n"
    "\"named-mailboxes\" or \"virtual-mailboxes\" may be used for the spoolfile.\n"
    "\n"
    "If not specified, then the environment variables `$$$MAIL` and\n"
    "`$$$MAILDIR` will be checked.\n"
  },
  /*
  ** .pp
  ** If your spool mailbox is in a non-default place where NeoMutt can't find
  ** it, you can specify its location with this variable. The description from
  ** "named-mailboxes" or "virtual-mailboxes" may be used for the spoolfile.
  ** .pp
  ** If not specified, then the environment variables \fC$$$MAIL\fP and
  ** \fC$$$MAILDIR\fP will be checked.
  */
#ifdef USE_SSL
#ifdef USE_SSL_GNUTLS
  { "ssl_ca_certificates_file", DT_STRING|DT_PATH, &C_SslCaCertificatesFile, 0, 0, NULL,
    "This variable specifies a file containing trusted CA certificates.\n"
    "Any server certificate that is signed with one of these CA\n"
    "certificates is also automatically accepted. (GnuTLS only)\n"
    "\n"
    "Example:\n"
    "```\n"
    "set ssl_ca_certificates_file=/etc/ssl/certs/ca-certificates.crt\n"
    "```\n"
  },
  /*
  ** .pp
  ** This variable specifies a file containing trusted CA certificates.
  ** Any server certificate that is signed with one of these CA
  ** certificates is also automatically accepted. (GnuTLS only)
  ** .pp
  ** Example:
  ** .ts
  ** set ssl_ca_certificates_file=/etc/ssl/certs/ca-certificates.crt
  ** .te
  */
#endif /* USE_SSL_GNUTLS */
  { "ssl_ciphers", DT_STRING, &C_SslCiphers, 0, 0, NULL,
    "Contains a colon-separated list of ciphers to use when using SSL.\n"
    "For OpenSSL, see ciphers(1) for the syntax of the string.\n"
    "\n"
    "For GnuTLS, this option will be used in place of \"NORMAL\" at the\n"
    "start of the priority string.  See gnutls_priority_init(3) for the\n"
    "syntax and more details. (Note: GnuTLS version 2.1.7 or higher is\n"
    "required.)\n"
  },
  /*
  ** .pp
  ** Contains a colon-separated list of ciphers to use when using SSL.
  ** For OpenSSL, see ciphers(1) for the syntax of the string.
  ** .pp
  ** For GnuTLS, this option will be used in place of "NORMAL" at the
  ** start of the priority string.  See gnutls_priority_init(3) for the
  ** syntax and more details. (Note: GnuTLS version 2.1.7 or higher is
  ** required.)
  */
  { "ssl_client_cert", DT_STRING|DT_PATH, &C_SslClientCert, 0, 0, NULL,
    "The file containing a client certificate and its associated private\n"
    "key.\n"
  },
  /*
  ** .pp
  ** The file containing a client certificate and its associated private
  ** key.
  */
  { "ssl_force_tls", DT_BOOL, &C_SslForceTls, true, 0, NULL,
    "If this variable is _set_, NeoMutt will require that all connections\n"
    "to remote servers be encrypted. Furthermore it will attempt to\n"
    "negotiate TLS even if the server does not advertise the capability,\n"
    "since it would otherwise have to abort the connection anyway. This\n"
    "option supersedes $$ssl_starttls.\n"
  },
  /*
  ** .pp
  ** If this variable is \fIset\fP, NeoMutt will require that all connections
  ** to remote servers be encrypted. Furthermore it will attempt to
  ** negotiate TLS even if the server does not advertise the capability,
  ** since it would otherwise have to abort the connection anyway. This
  ** option supersedes $$ssl_starttls.
  */
#ifdef USE_SSL_GNUTLS
  { "ssl_min_dh_prime_bits", DT_NUMBER|DT_NOT_NEGATIVE, &C_SslMinDhPrimeBits, 0, 0, NULL,
    "This variable specifies the minimum acceptable prime size (in bits)\n"
    "for use in any Diffie-Hellman key exchange. A value of 0 will use\n"
    "the default from the GNUTLS library. (GnuTLS only)\n"
  },
  /*
  ** .pp
  ** This variable specifies the minimum acceptable prime size (in bits)
  ** for use in any Diffie-Hellman key exchange. A value of 0 will use
  ** the default from the GNUTLS library. (GnuTLS only)
  */
#endif /* USE_SSL_GNUTLS */
  { "ssl_starttls", DT_QUAD, &C_SslStarttls, MUTT_YES, 0, NULL,
    "If _set_ (the default), NeoMutt will attempt to use `STARTTLS` on servers\n"
    "advertising the capability. When _unset_, NeoMutt will not attempt to\n"
    "use `STARTTLS` regardless of the server's capabilities.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP (the default), NeoMutt will attempt to use \fCSTARTTLS\fP on servers
  ** advertising the capability. When \fIunset\fP, NeoMutt will not attempt to
  ** use \fCSTARTTLS\fP regardless of the server's capabilities.
  */
#ifdef USE_SSL_OPENSSL
  { "ssl_use_sslv2", DT_BOOL, &C_SslUseSslv2, false, 0, NULL,
    "If _set_ , NeoMutt will use SSLv2 when communicating with servers that\n"
    "request it. **N.B. As of 2011, SSLv2 is considered insecure, and using\n"
    "is inadvisable. See https://tools.ietf.org/html/rfc6176 .**\n"
    "(OpenSSL only)\n"
  },
  /*
  ** .pp
  ** If \fIset\fP , NeoMutt will use SSLv2 when communicating with servers that
  ** request it. \fBN.B. As of 2011, SSLv2 is considered insecure, and using
  ** is inadvisable. See https://tools.ietf.org/html/rfc6176 .\fP
  ** (OpenSSL only)
  */
#endif /* defined USE_SSL_OPENSSL */
  { "ssl_use_sslv3", DT_BOOL, &C_SslUseSslv3, false, 0, NULL,
    "If _set_ , NeoMutt will use SSLv3 when communicating with servers that\n"
    "request it. **N.B. As of 2015, SSLv3 is considered insecure, and using\n"
    "it is inadvisable. See https://tools.ietf.org/html/rfc7525 .**\n"
  },
  /*
  ** .pp
  ** If \fIset\fP , NeoMutt will use SSLv3 when communicating with servers that
  ** request it. \fBN.B. As of 2015, SSLv3 is considered insecure, and using
  ** it is inadvisable. See https://tools.ietf.org/html/rfc7525 .\fP
  */
  { "ssl_use_tlsv1", DT_BOOL, &C_SslUseTlsv1, false, 0, NULL,
    "If _set_ , NeoMutt will use TLSv1.0 when communicating with servers that\n"
    "request it. **N.B. As of 2015, TLSv1.0 is considered insecure, and using\n"
    "it is inadvisable. See https://tools.ietf.org/html/rfc7525 .**\n"
  },
  /*
  ** .pp
  ** If \fIset\fP , NeoMutt will use TLSv1.0 when communicating with servers that
  ** request it. \fBN.B. As of 2015, TLSv1.0 is considered insecure, and using
  ** it is inadvisable. See https://tools.ietf.org/html/rfc7525 .\fP
  */
  { "ssl_use_tlsv1_1", DT_BOOL, &C_SslUseTlsv11, false, 0, NULL,
    "If _set_ , NeoMutt will use TLSv1.1 when communicating with servers that\n"
    "request it. **N.B. As of 2015, TLSv1.1 is considered insecure, and using\n"
    "it is inadvisable. See https://tools.ietf.org/html/rfc7525 .**\n"
  },
  /*
  ** .pp
  ** If \fIset\fP , NeoMutt will use TLSv1.1 when communicating with servers that
  ** request it. \fBN.B. As of 2015, TLSv1.1 is considered insecure, and using
  ** it is inadvisable. See https://tools.ietf.org/html/rfc7525 .\fP
  */
  { "ssl_use_tlsv1_2", DT_BOOL, &C_SslUseTlsv12, true, 0, NULL,
    "If _set_ , NeoMutt will use TLSv1.2 when communicating with servers that\n"
    "request it.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP , NeoMutt will use TLSv1.2 when communicating with servers that
  ** request it.
  */
  { "ssl_use_tlsv1_3", DT_BOOL, &C_SslUseTlsv13, true, 0, NULL,
    "If _set_ , NeoMutt will use TLSv1.3 when communicating with servers that\n"
    "request it.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP , NeoMutt will use TLSv1.3 when communicating with servers that
  ** request it.
  */
#ifdef USE_SSL_OPENSSL
  { "ssl_usesystemcerts", DT_BOOL, &C_SslUsesystemcerts, true, 0, NULL,
    "If set to _yes_, NeoMutt will use CA certificates in the\n"
    "system-wide certificate store when checking if a server certificate\n"
    "is signed by a trusted CA. (OpenSSL only)\n"
  },
  /*
  ** .pp
  ** If set to \fIyes\fP, NeoMutt will use CA certificates in the
  ** system-wide certificate store when checking if a server certificate
  ** is signed by a trusted CA. (OpenSSL only)
  */
#endif
  { "ssl_verify_dates", DT_BOOL, &C_SslVerifyDates, true, 0, NULL,
    "If _set_ (the default), NeoMutt will not automatically accept a server\n"
    "certificate that is either not yet valid or already expired. You should\n"
    "only unset this for particular known hosts, using the\n"
    "`$<account-hook>` function.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP (the default), NeoMutt will not automatically accept a server
  ** certificate that is either not yet valid or already expired. You should
  ** only unset this for particular known hosts, using the
  ** \fC$<account-hook>\fP function.
  */
  { "ssl_verify_host", DT_BOOL, &C_SslVerifyHost, true, 0, NULL,
    "If _set_ (the default), NeoMutt will not automatically accept a server\n"
    "certificate whose host name does not match the host used in your folder\n"
    "URL. You should only unset this for particular known hosts, using\n"
    "the `$<account-hook>` function.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP (the default), NeoMutt will not automatically accept a server
  ** certificate whose host name does not match the host used in your folder
  ** URL. You should only unset this for particular known hosts, using
  ** the \fC$<account-hook>\fP function.
  */
#ifdef USE_SSL_OPENSSL
#ifdef HAVE_SSL_PARTIAL_CHAIN
  { "ssl_verify_partial_chains", DT_BOOL, &C_SslVerifyPartialChains, false, 0, NULL,
    "This option should not be changed from the default unless you understand\n"
    "what you are doing.\n"
    "\n"
    "Setting this variable to _yes_ will permit verifying partial\n"
    "certification chains, i. e. a certificate chain where not the root,\n"
    "but an intermediate certificate CA, or the host certificate, are\n"
    "marked trusted (in $$certificate_file), without marking the root\n"
    "signing CA as trusted.\n"
    "\n"
    "(OpenSSL 1.0.2b and newer only).\n"
  },
  /*
  ** .pp
  ** This option should not be changed from the default unless you understand
  ** what you are doing.
  ** .pp
  ** Setting this variable to \fIyes\fP will permit verifying partial
  ** certification chains, i. e. a certificate chain where not the root,
  ** but an intermediate certificate CA, or the host certificate, are
  ** marked trusted (in $$certificate_file), without marking the root
  ** signing CA as trusted.
  ** .pp
  ** (OpenSSL 1.0.2b and newer only).
  */
#endif /* defined HAVE_SSL_PARTIAL_CHAIN */
#endif /* defined USE_SSL_OPENSSL */
#endif /* defined(USE_SSL) */
  { "status_chars", DT_MBTABLE|R_INDEX|R_PAGER, &C_StatusChars, IP "-*%A", 0, NULL,
    "Controls the characters used by the \"%r\" indicator in $$status_format.\n"
    "\n"
    "| Character | Default | Description |\n"
    "|-----------+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n"
    "| 1         | -       | Mailbox is unchanged |\n"
    "| 2         | *       | Mailbox has been changed and needs to be resynchronized |\n"
    "| 3         | %       | Mailbox is read-only, or will not be written when exiting.  (You can toggle whether to write changes to a mailbox with the `<toggle-write>` operation, bound by default to \"%\") |\n"
    "| 4         | A       | Folder opened in attach-message mode.  (Certain operations like composing a new mail, replying, forwarding, etc. are not permitted in this mode) |\n"
  },
  /*
  ** .pp
  ** Controls the characters used by the "%r" indicator in $$status_format.
  ** .dl
  ** .dt \fBCharacter\fP .dd \fBDefault\fP .dd \fBDescription\fP
  ** .dt 1 .dd - .dd Mailbox is unchanged
  ** .dt 2 .dd * .dd Mailbox has been changed and needs to be resynchronized
  ** .dt 3 .dd % .dd Mailbox is read-only, or will not be written when exiting.
  **                 (You can toggle whether to write changes to a mailbox
  **                 with the \fC<toggle-write>\fP operation, bound by default
  **                 to "%")
  ** .dt 4 .dd A .dd Folder opened in attach-message mode.
  **                 (Certain operations like composing a new mail, replying,
  **                 forwarding, etc. are not permitted in this mode)
  ** .de
  */
  { "status_format", DT_STRING|R_INDEX|R_PAGER, &C_StatusFormat, IP "-%r-NeoMutt: %D [Msgs:%?M?%M/?%m%?n? New:%n?%?o? Old:%o?%?d? Del:%d?%?F? Flag:%F?%?t? Tag:%t?%?p? Post:%p?%?b? Inc:%b?%?l? %l?]---(%s/%S)-%>-(%P)---", 0, NULL,
    "Controls the format of the status line displayed in the \"index\"\n"
    "menu.  This string is similar to $$index_format, but has its own\n"
    "set of `printf(3)`-like sequences:\n"
    "\n"
    "| .   | Description |\n"
    "|-----+---------------------------------------------------------------------------------------------------------|\n"
    "| %b  | Number of mailboxes with new mail * |\n"
    "| %d  | Number of deleted messages * |\n"
    "| %D  | Description of the mailbox |\n"
    "| %f  | The full pathname of the current mailbox |\n"
    "| %F  | Number of flagged messages * |\n"
    "| %h  | Local hostname |\n"
    "| %l  | Size (in bytes) of the current mailbox (see $formatstrings-size) * |\n"
    "| %L  | Size (in bytes) of the messages shown (i.e., which match the current limit) (see $formatstrings-size) * |\n"
    "| %m  | The number of messages in the mailbox * |\n"
    "| %M  | The number of messages shown (i.e., which match the current limit) * |\n"
    "| %n  | Number of new messages in the mailbox * |\n"
    "| %o  | Number of old unread messages * |\n"
    "| %p  | Number of postponed messages * |\n"
    "| %P  | Percentage of the way through the index |\n"
    "| %r  | Modified/read-only/won't-write/attach-message indicator, According to $$status_chars |\n"
    "| %R  | Number of read messages * |\n"
    "| %s  | Current sorting mode ($$sort) |\n"
    "| %S  | Current aux sorting method ($$sort_aux) |\n"
    "| %t  | Number of tagged messages * |\n"
    "| %u  | Number of unread messages * |\n"
    "| %v  | NeoMutt version string |\n"
    "| %V  | Currently active limit pattern, if any * |\n"
    "| %>X | Right justify the rest of the string and pad with \"X\" |\n"
    "| %|X | Pad to the end of the line with \"X\" |\n"
    "| %*X | Soft-fill with character \"X\" as pad |\n"
    "\n"
    "For an explanation of \"soft-fill\", see the $$index_format documentation.\n"
    "\n"
    "* = can be optionally printed if nonzero\n"
    "\n"
    "Some of the above sequences can be used to optionally print a string\n"
    "if their value is nonzero.  For example, you may only want to see the\n"
    "number of flagged messages if such messages exist, since zero is not\n"
    "particularly meaningful.  To optionally print a string based upon one\n"
    "of the above sequences, the following construct is used:\n"
    "\n"
    " `%?<sequence_char>?<optional_string>?`\n"
    "\n"
    "where _sequence_char_ is a character from the table above, and\n"
    "_optional_string_ is the string you would like printed if\n"
    "_sequence_char_ is nonzero.  _optional_string_ **may** contain\n"
    "other sequences as well as normal text, but you may **not** nest\n"
    "optional strings.\n"
    "\n"
    "Here is an example illustrating how to optionally print the number of\n"
    "new messages in a mailbox:\n"
    "\n"
    "`%?n?%n new messages.?`\n"
    "\n"
    "You can also switch between two strings using the following construct:\n"
    "\n"
    "`%?<sequence_char>?<if_string>&<else_string>?`\n"
    "\n"
    "If the value of _sequence_char_ is non-zero, _if_string_ will\n"
    "be expanded, otherwise _else_string_ will be expanded.\n"
    "\n"
    "You can force the result of any `printf(3)`-like sequence to be lowercase\n"
    "by prefixing the sequence character with an underscore (\"_\") sign.\n"
    "For example, if you want to display the local hostname in lowercase,\n"
    "you would use: \"`%_h`\".\n"
    "\n"
    "If you prefix the sequence character with a colon (\":\") character, NeoMutt\n"
    "will replace any dots in the expansion by underscores. This might be helpful\n"
    "with IMAP folders that don't like dots in folder names.\n"
  },
  /*
  ** .pp
  ** Controls the format of the status line displayed in the "index"
  ** menu.  This string is similar to $$index_format, but has its own
  ** set of \fCprintf(3)\fP-like sequences:
  ** .dl
  ** .dt %b  .dd * .dd Number of mailboxes with new mail
  ** .dt %d  .dd * .dd Number of deleted messages
  ** .dt %D  .dd   .dd Description of the mailbox
  ** .dt %f  .dd   .dd The full pathname of the current mailbox
  ** .dt %F  .dd * .dd Number of flagged messages
  ** .dt %h  .dd   .dd Local hostname
  ** .dt %l  .dd * .dd Size (in bytes) of the current mailbox (see $formatstrings-size)
  ** .dt %L  .dd * .dd Size (in bytes) of the messages shown
  **                   (i.e., which match the current limit) (see $formatstrings-size)
  ** .dt %m  .dd * .dd The number of messages in the mailbox
  ** .dt %M  .dd * .dd The number of messages shown (i.e., which match the current limit)
  ** .dt %n  .dd * .dd Number of new messages in the mailbox (unread, unseen)
  ** .dt %o  .dd * .dd Number of old messages in the mailbox (unread, seen)
  ** .dt %p  .dd * .dd Number of postponed messages
  ** .dt %P  .dd   .dd Percentage of the way through the index
  ** .dt %r  .dd   .dd Modified/read-only/won't-write/attach-message indicator,
  **                   According to $$status_chars
  ** .dt %R  .dd * .dd Number of read messages in the mailbox (read, seen)
  ** .dt %s  .dd   .dd Current sorting mode ($$sort)
  ** .dt %S  .dd   .dd Current aux sorting method ($$sort_aux)
  ** .dt %t  .dd * .dd Number of tagged messages in the mailbox
  ** .dt %u  .dd * .dd Number of unread messages in the mailbox (seen or unseen)
  ** .dt %v  .dd   .dd NeoMutt version string
  ** .dt %V  .dd * .dd Currently active limit pattern, if any
  ** .dt %>X .dd   .dd Right justify the rest of the string and pad with "X"
  ** .dt %|X .dd   .dd Pad to the end of the line with "X"
  ** .dt %*X .dd   .dd Soft-fill with character "X" as pad
  ** .de
  ** .pp
  ** For an explanation of "soft-fill", see the $$index_format documentation.
  ** .pp
  ** * = can be optionally printed if nonzero
  ** .pp
  ** Some of the above sequences can be used to optionally print a string
  ** if their value is nonzero.  For example, you may only want to see the
  ** number of flagged messages if such messages exist, since zero is not
  ** particularly meaningful.  To optionally print a string based upon one
  ** of the above sequences, the following construct is used:
  ** .pp
  **  \fC%?<sequence_char>?<optional_string>?\fP
  ** .pp
  ** where \fIsequence_char\fP is a character from the table above, and
  ** \fIoptional_string\fP is the string you would like printed if
  ** \fIsequence_char\fP is nonzero.  \fIoptional_string\fP \fBmay\fP contain
  ** other sequences as well as normal text, but you may \fBnot\fP nest
  ** optional strings.
  ** .pp
  ** Here is an example illustrating how to optionally print the number of
  ** new messages in a mailbox:
  ** .pp
  ** \fC%?n?%n new messages.?\fP
  ** .pp
  ** You can also switch between two strings using the following construct:
  ** .pp
  ** \fC%?<sequence_char>?<if_string>&<else_string>?\fP
  ** .pp
  ** If the value of \fIsequence_char\fP is non-zero, \fIif_string\fP will
  ** be expanded, otherwise \fIelse_string\fP will be expanded.
  ** .pp
  ** You can force the result of any \fCprintf(3)\fP-like sequence to be lowercase
  ** by prefixing the sequence character with an underscore ("_") sign.
  ** For example, if you want to display the local hostname in lowercase,
  ** you would use: "\fC%_h\fP".
  ** .pp
  ** If you prefix the sequence character with a colon (":") character, NeoMutt
  ** will replace any dots in the expansion by underscores. This might be helpful
  ** with IMAP folders that don't like dots in folder names.
  */
  { "status_on_top", DT_BOOL|R_REFLOW, &C_StatusOnTop, false, 0, NULL,
    "Setting this variable causes the \"status bar\" to be displayed on\n"
    "the first line of the screen rather than near the bottom. If $$help\n"
    "is _set_, too it'll be placed at the bottom.\n"
  },
  /*
  ** .pp
  ** Setting this variable causes the "status bar" to be displayed on
  ** the first line of the screen rather than near the bottom. If $$help
  ** is \fIset\fP, too it'll be placed at the bottom.
  */
  { "strict_threads", DT_BOOL|R_RESORT|R_RESORT_INIT|R_INDEX, &C_StrictThreads, false, 0, pager_validator,
    "If _set_, threading will only make use of the \"In-Reply-To\" and\n"
    "\"References:\" fields when you $$sort by message threads.  By\n"
    "default, messages with the same subject are grouped together in\n"
    "\"pseudo threads.\". This may not always be desirable, such as in a\n"
    "personal mailbox where you might have several unrelated messages with\n"
    "the subjects like \"hi\" which will get grouped together. See also\n"
    "$$sort_re for a less drastic way of controlling this\n"
    "behavior.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, threading will only make use of the "In-Reply-To" and
  ** "References:" fields when you $$sort by message threads.  By
  ** default, messages with the same subject are grouped together in
  ** "pseudo threads.". This may not always be desirable, such as in a
  ** personal mailbox where you might have several unrelated messages with
  ** the subjects like "hi" which will get grouped together. See also
  ** $$sort_re for a less drastic way of controlling this
  ** behavior.
  */
  { "suspend", DT_BOOL, &C_Suspend, true, 0, NULL,
    "When _unset_, NeoMutt won't stop when the user presses the terminal's\n"
    "_susp_ key, usually \"^Z\". This is useful if you run NeoMutt\n"
    "inside an xterm using a command like \"`xterm -e neomutt`\".\n"
  },
  /*
  ** .pp
  ** When \fIunset\fP, NeoMutt won't stop when the user presses the terminal's
  ** \fIsusp\fP key, usually "^Z". This is useful if you run NeoMutt
  ** inside an xterm using a command like "\fCxterm -e neomutt\fP".
  */
  { "text_flowed", DT_BOOL, &C_TextFlowed, false, 0, NULL,
    "When _set_, NeoMutt will generate \"format=flowed\" bodies with a content type\n"
    "of \"`text/plain; format=flowed`\".\n"
    "This format is easier to handle for some mailing software, and generally\n"
    "just looks like ordinary text.  To actually make use of this format's\n"
    "features, you'll need support in your editor.\n"
    "\n"
    "The option only controls newly composed messages.  Postponed messages,\n"
    "resent messages, and draft messages (via -H on the command line) will\n"
    "use the content-type of the source message.\n"
    "\n"
    "Note that $$indent_string is ignored when this option is _set_.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will generate "format=flowed" bodies with a content type
  ** of "\fCtext/plain; format=flowed\fP".
  ** This format is easier to handle for some mailing software, and generally
  ** just looks like ordinary text.  To actually make use of this format's
  ** features, you'll need support in your editor.
  ** .pp
  ** The option only controls newly composed messages.  Postponed messages,
  ** resent messages, and draft messages (via -H on the command line) will
  ** use the content-type of the source message.
  ** .pp
  ** Note that $$indent_string is ignored when this option is \fIset\fP.
  */
  { "thorough_search", DT_BOOL, &C_ThoroughSearch, true, 0, NULL,
    "Affects the `~b` and `~h` search operations described in\n"
    "section \"$patterns\".  If _set_, the headers and body/attachments of\n"
    "messages to be searched are decoded before searching. If _unset_,\n"
    "messages are searched as they appear in the folder.\n"
    "\n"
    "Users searching attachments or for non-ASCII characters should _set_\n"
    "this value because decoding also includes MIME parsing/decoding and possible\n"
    "character set conversions. Otherwise NeoMutt will attempt to match against the\n"
    "raw message received (for example quoted-printable encoded or with encoded\n"
    "headers) which may lead to incorrect search results.\n"
  },
  /*
  ** .pp
  ** Affects the \fC~b\fP and \fC~h\fP search operations described in
  ** section "$patterns".  If \fIset\fP, the headers and body/attachments of
  ** messages to be searched are decoded before searching. If \fIunset\fP,
  ** messages are searched as they appear in the folder.
  ** .pp
  ** Users searching attachments or for non-ASCII characters should \fIset\fP
  ** this value because decoding also includes MIME parsing/decoding and possible
  ** character set conversions. Otherwise NeoMutt will attempt to match against the
  ** raw message received (for example quoted-printable encoded or with encoded
  ** headers) which may lead to incorrect search results.
  */
  { "thread_received", DT_BOOL|R_RESORT|R_RESORT_INIT|R_INDEX, &C_ThreadReceived, false, 0, pager_validator,
    "When _set_, NeoMutt uses the date received rather than the date sent\n"
    "to thread messages by subject.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt uses the date received rather than the date sent
  ** to thread messages by subject.
  */
  { "tilde", DT_BOOL|R_PAGER, &C_Tilde, false, 0, NULL,
    "When _set_, the internal-pager will pad blank lines to the bottom of the\n"
    "screen with a tilde (\"~\").\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, the internal-pager will pad blank lines to the bottom of the
  ** screen with a tilde ("~").
  */
  { "time_inc", DT_NUMBER|DT_NOT_NEGATIVE, &C_TimeInc, 0, 0, NULL,
    "Along with $$read_inc, $$write_inc, and $$net_inc, this\n"
    "variable controls the frequency with which progress updates are\n"
    "displayed. It suppresses updates less than $$time_inc milliseconds\n"
    "apart. This can improve throughput on systems with slow terminals,\n"
    "or when running NeoMutt on a remote system.\n"
    "\n"
    "Also see the \"$tuning\" section of the manual for performance considerations.\n"
  },
  /*
  ** .pp
  ** Along with $$read_inc, $$write_inc, and $$net_inc, this
  ** variable controls the frequency with which progress updates are
  ** displayed. It suppresses updates less than $$time_inc milliseconds
  ** apart. This can improve throughput on systems with slow terminals,
  ** or when running NeoMutt on a remote system.
  ** .pp
  ** Also see the "$tuning" section of the manual for performance considerations.
  */
  { "timeout", DT_NUMBER|DT_NOT_NEGATIVE, &C_Timeout, 600, 0, NULL,
    "When NeoMutt is waiting for user input either idling in menus or\n"
    "in an interactive prompt, NeoMutt would block until input is\n"
    "present. Depending on the context, this would prevent certain\n"
    "operations from working, like checking for new mail or keeping\n"
    "an IMAP connection alive.\n"
    "\n"
    "This variable controls how many seconds NeoMutt will at most wait\n"
    "until it aborts waiting for input, performs these operations and\n"
    "continues to wait for input.\n"
    "\n"
    "A value of zero or less will cause NeoMutt to never time out.\n"
  },
  /*
  ** .pp
  ** When NeoMutt is waiting for user input either idling in menus or
  ** in an interactive prompt, NeoMutt would block until input is
  ** present. Depending on the context, this would prevent certain
  ** operations from working, like checking for new mail or keeping
  ** an IMAP connection alive.
  ** .pp
  ** This variable controls how many seconds NeoMutt will at most wait
  ** until it aborts waiting for input, performs these operations and
  ** continues to wait for input.
  ** .pp
  ** A value of zero or less will cause NeoMutt to never time out.
  */
  { "tmpdir", DT_STRING|DT_PATH, &C_Tmpdir, IP "/tmp", 0, NULL,
    "This variable allows you to specify where NeoMutt will place its\n"
    "temporary files needed for displaying and composing messages.\n"
    "\n"
    "If this variable is not set, the environment variable `$$$TMPDIR` is\n"
    "used.  Failing that, then \"`/tmp`\" is used.\n"
  },
  /*
  ** .pp
  ** This variable allows you to specify where NeoMutt will place its
  ** temporary files needed for displaying and composing messages.
  ** .pp
  ** If this variable is not set, the environment variable \fC$$$TMPDIR\fP is
  ** used.  Failing that, then "\fC/tmp\fP" is used.
  */
  { "to_chars", DT_MBTABLE|R_INDEX|R_PAGER, &C_ToChars, IP " +TCFLR", 0, NULL,
    "Controls the character used to indicate mail addressed to you.\n"
    "\n"
    "| Character | Default | Description |\n"
    "|-----------+---------+----------------------------------------------------------------------------------------------------|\n"
    "| 1         | <space> | The mail is _not_ addressed to your address. |\n"
    "| 2         | +       | You are the only recipient of the message. |\n"
    "| 3         | T       | Your address appears in the \"To:\" header field, but you are not the only recipient of the message. |\n"
    "| 4         | C       | Your address is specified in the \"Cc:\" header field, but you are not the only recipient. |\n"
    "| 5         | F       | Indicates the mail that was sent by _you_. |\n"
    "| 6         | L       | Indicates the mail was sent to a mailing-list you subscribe to. |\n"
    "| 7         | R       | Your address appears in the \"Reply-To:\" header field but none of the above applies. |\n"
  },
  /*
  ** .pp
  ** Controls the character used to indicate mail addressed to you.
  ** .dl
  ** .dt \fBCharacter\fP .dd \fBDefault\fP .dd \fBDescription\fP
  ** .dt 1 .dd <space> .dd The mail is \fInot\fP addressed to your address.
  ** .dt 2 .dd + .dd You are the only recipient of the message.
  ** .dt 3 .dd T .dd Your address appears in the "To:" header field, but you are not the only recipient of the message.
  ** .dt 4 .dd C .dd Your address is specified in the "Cc:" header field, but you are not the only recipient.
  ** .dt 5 .dd F .dd Indicates the mail that was sent by \fIyou\fP.
  ** .dt 6 .dd L .dd Indicates the mail was sent to a mailing-list you subscribe to.
  ** .dt 7 .dd R .dd Your address appears in the "Reply-To:" header field but none of the above applies.
  ** .de
  */
  { "trash", DT_STRING|DT_PATH|DT_MAILBOX, &C_Trash, 0, 0, NULL,
    "If set, this variable specifies the path of the trash folder where the\n"
    "mails marked for deletion will be moved, instead of being irremediably\n"
    "purged.\n"
    "\n"
    "NOTE: When you delete a message in the trash folder, it is really\n"
    "deleted, so that you have a way to clean the trash.\n"
  },
  /*
  ** .pp
  ** If set, this variable specifies the path of the trash folder where the
  ** mails marked for deletion will be moved, instead of being irremediably
  ** purged.
  ** .pp
  ** NOTE: When you delete a message in the trash folder, it is really
  ** deleted, so that you have a way to clean the trash.
  */
  { "ts_enabled", DT_BOOL|R_INDEX|R_PAGER, &C_TsEnabled, false, 0, NULL,
    "Controls whether NeoMutt tries to set the terminal status line and icon name.\n"
    "Most terminal emulators emulate the status line in the window title.\n"
  },
  /* The default must be off to force in the validity checking. */
  /*
  ** .pp
  ** Controls whether NeoMutt tries to set the terminal status line and icon name.
  ** Most terminal emulators emulate the status line in the window title.
  */
  { "ts_icon_format", DT_STRING|R_INDEX|R_PAGER, &C_TsIconFormat, IP "M%?n?AIL&ail?", 0, NULL,
    "Controls the format of the icon title, as long as \"$$ts_enabled\" is set.\n"
    "This string is identical in formatting to the one used by\n"
    "\"$$status_format\".\n"
  },
  /*
  ** .pp
  ** Controls the format of the icon title, as long as "$$ts_enabled" is set.
  ** This string is identical in formatting to the one used by
  ** "$$status_format".
  */
  { "ts_status_format", DT_STRING|R_INDEX|R_PAGER, &C_TsStatusFormat, IP "NeoMutt with %?m?%m messages&no messages?%?n? [%n NEW]?", 0, NULL,
    "Controls the format of the terminal status line (or window title),\n"
    "provided that \"$$ts_enabled\" has been set. This string is identical in\n"
    "formatting to the one used by \"$$status_format\".\n"
  },
  /*
  ** .pp
  ** Controls the format of the terminal status line (or window title),
  ** provided that "$$ts_enabled" has been set. This string is identical in
  ** formatting to the one used by "$$status_format".
  */
#ifdef USE_SOCKET
  { "tunnel", DT_STRING|DT_COMMAND, &C_Tunnel, 0, 0, NULL,
    "Setting this variable will cause NeoMutt to open a pipe to a command\n"
    "instead of a raw socket. You may be able to use this to set up\n"
    "preauthenticated connections to your IMAP/POP3/SMTP server. Example:\n"
    "```\n"
    "set tunnel=\"ssh -q mailhost.net /usr/local/libexec/imapd\"\n"
    "```\n"
    "\n"
    "Note: For this example to work you must be able to log in to the remote\n"
    "machine without having to enter a password.\n"
    "\n"
    "When set, NeoMutt uses the tunnel for all remote connections.\n"
    "Please see \"$account-hook\" in the manual for how to use different\n"
    "tunnel commands per connection.\n"
  },
  /*
  ** .pp
  ** Setting this variable will cause NeoMutt to open a pipe to a command
  ** instead of a raw socket. You may be able to use this to set up
  ** preauthenticated connections to your IMAP/POP3/SMTP server. Example:
  ** .ts
  ** set tunnel="ssh -q mailhost.net /usr/local/libexec/imapd"
  ** .te
  ** .pp
  ** Note: For this example to work you must be able to log in to the remote
  ** machine without having to enter a password.
  ** .pp
  ** When set, NeoMutt uses the tunnel for all remote connections.
  ** Please see "$account-hook" in the manual for how to use different
  ** tunnel commands per connection.
  */
#endif
  { "uncollapse_jump", DT_BOOL, &C_UncollapseJump, false, 0, NULL,
    "When _set_, NeoMutt will jump to the next unread message, if any,\n"
    "when the current thread is _un_collapsed.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will jump to the next unread message, if any,
  ** when the current thread is \fIun\fPcollapsed.
  */
  { "uncollapse_new", DT_BOOL, &C_UncollapseNew, true, 0, NULL,
    "When _set_, NeoMutt will automatically uncollapse any collapsed thread\n"
    "that receives a new message. When _unset_, collapsed threads will\n"
    "remain collapsed. the presence of the new message will still affect\n"
    "index sorting, though.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will automatically uncollapse any collapsed thread
  ** that receives a new message. When \fIunset\fP, collapsed threads will
  ** remain collapsed. the presence of the new message will still affect
  ** index sorting, though.
  */
  { "use_8bitmime", DT_BOOL, &C_Use8bitmime, false, 0, NULL,
    "**Warning:** do not set this variable unless you are using a version\n"
    "of sendmail which supports the `-B8BITMIME` flag (such as sendmail\n"
    "8.8.x) or you may not be able to send mail.\n"
    "\n"
    "When _set_, NeoMutt will invoke $$sendmail with the `-B8BITMIME`\n"
    "flag when sending 8-bit messages to enable ESMTP negotiation.\n"
  },
  /*
  ** .pp
  ** \fBWarning:\fP do not set this variable unless you are using a version
  ** of sendmail which supports the \fC-B8BITMIME\fP flag (such as sendmail
  ** 8.8.x) or you may not be able to send mail.
  ** .pp
  ** When \fIset\fP, NeoMutt will invoke $$sendmail with the \fC-B8BITMIME\fP
  ** flag when sending 8-bit messages to enable ESMTP negotiation.
  */
  { "use_domain", DT_BOOL, &C_UseDomain, true, 0, NULL,
    "When _set_, NeoMutt will qualify all local addresses (ones without the\n"
    "\"@host\" portion) with the value of $$hostname.  If _unset_, no\n"
    "addresses will be qualified.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will qualify all local addresses (ones without the
  ** "@host" portion) with the value of $$hostname.  If \fIunset\fP, no
  ** addresses will be qualified.
  */
  { "use_envelope_from", DT_BOOL, &C_UseEnvelopeFrom, false, 0, NULL,
    "When _set_, NeoMutt will set the _envelope_ sender of the message.\n"
    "If $$envelope_from_address is _set_, it will be used as the sender\n"
    "address. If _unset_, NeoMutt will attempt to derive the sender from the\n"
    "\"From:\" header.\n"
    "\n"
    "Note that this information is passed to sendmail command using the\n"
    "`-f` command line switch. Therefore setting this option is not useful\n"
    "if the $$sendmail variable already contains `-f` or if the\n"
    "executable pointed to by $$sendmail doesn't support the `-f` switch.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will set the \fIenvelope\fP sender of the message.
  ** If $$envelope_from_address is \fIset\fP, it will be used as the sender
  ** address. If \fIunset\fP, NeoMutt will attempt to derive the sender from the
  ** "From:" header.
  ** .pp
  ** Note that this information is passed to sendmail command using the
  ** \fC-f\fP command line switch. Therefore setting this option is not useful
  ** if the $$sendmail variable already contains \fC-f\fP or if the
  ** executable pointed to by $$sendmail doesn't support the \fC-f\fP switch.
  */
  { "use_from", DT_BOOL, &C_UseFrom, true, 0, NULL,
    "When _set_, NeoMutt will generate the \"From:\" header field when\n"
    "sending messages.  If _unset_, no \"From:\" header field will be\n"
    "generated unless the user explicitly sets one using the \"$my_hdr\"\n"
    "command.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will generate the "From:" header field when
  ** sending messages.  If \fIunset\fP, no "From:" header field will be
  ** generated unless the user explicitly sets one using the "$my_hdr"
  ** command.
  */
#ifdef HAVE_GETADDRINFO
  { "use_ipv6", DT_BOOL, &C_UseIpv6, true, 0, NULL,
    "When _set_, NeoMutt will look for IPv6 addresses of hosts it tries to\n"
    "contact.  If this option is _unset_, NeoMutt will restrict itself to IPv4 addresses.\n"
    "Normally, the default should work.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will look for IPv6 addresses of hosts it tries to
  ** contact.  If this option is \fIunset\fP, NeoMutt will restrict itself to IPv4 addresses.
  ** Normally, the default should work.
  */
#endif /* HAVE_GETADDRINFO */
  { "user_agent", DT_BOOL, &C_UserAgent, false, 0, NULL,
    "When _set_, NeoMutt will add a \"User-Agent:\" header to outgoing\n"
    "messages, indicating which version of NeoMutt was used for composing\n"
    "them.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will add a "User-Agent:" header to outgoing
  ** messages, indicating which version of NeoMutt was used for composing
  ** them.
  */
#ifdef USE_NOTMUCH
  { "vfolder_format", DT_STRING|DT_NOT_EMPTY|R_INDEX, &C_VfolderFormat, IP "%2C %?n?%4n/&     ?%4m %f", 0, NULL,
    "This variable allows you to customize the file browser display for virtual\n"
    "folders to your personal taste.  This string uses many of the same\n"
    "expandos as $$folder_format.\n"
  },
  /*
  ** .pp
  ** This variable allows you to customize the file browser display for virtual
  ** folders to your personal taste.  This string uses many of the same
  ** expandos as $$folder_format.
  */
  { "virtual_spoolfile", DT_BOOL, &C_VirtualSpoolfile, false, 0, NULL,
    "When _set_, NeoMutt will use the first defined virtual mailbox (see\n"
    "virtual-mailboxes) as a spool file.\n"
    "**\n"
    "This command is now unnecessary. $$spoolfile has been extended to support\n"
    "mailbox descriptions as a value.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will use the first defined virtual mailbox (see
  ** virtual-mailboxes) as a spool file.
  **
  ** This command is now unnecessary. $$spoolfile has been extended to support
  ** mailbox descriptions as a value.
  */
#endif
  { "visual", DT_STRING|DT_COMMAND, &C_Visual, IP "vi", 0, NULL,
    "Specifies the visual editor to invoke when the \"`~v`\" command is\n"
    "given in the built-in editor.\n"
    "\n"
    "$$visual is overridden by the environment variable `$$$VISUAL` or `$$$EDITOR`.\n"
  },
  /*
  ** .pp
  ** Specifies the visual editor to invoke when the "\fC~v\fP" command is
  ** given in the built-in editor.
  ** .pp
  ** $$visual is overridden by the environment variable \fC$$$VISUAL\fP or \fC$$$EDITOR\fP.
  */
  { "wait_key", DT_BOOL, &C_WaitKey, true, 0, NULL,
    "Controls whether NeoMutt will ask you to press a key after an external command\n"
    "has been invoked by these functions: `<shell-escape>`,\n"
    "`<pipe-message>`, `<pipe-entry>`, `<print-message>`,\n"
    "and `<print-entry>` commands.\n"
    "\n"
    "It is also used when viewing attachments with \"$auto_view\", provided\n"
    "that the corresponding mailcap entry has a _needsterminal_ flag,\n"
    "and the external program is interactive.\n"
    "\n"
    "When _set_, NeoMutt will always ask for a key. When _unset_, NeoMutt will wait\n"
    "for a key only if the external command returned a non-zero status.\n"
  },
  /*
  ** .pp
  ** Controls whether NeoMutt will ask you to press a key after an external command
  ** has been invoked by these functions: \fC<shell-escape>\fP,
  ** \fC<pipe-message>\fP, \fC<pipe-entry>\fP, \fC<print-message>\fP,
  ** and \fC<print-entry>\fP commands.
  ** .pp
  ** It is also used when viewing attachments with "$auto_view", provided
  ** that the corresponding mailcap entry has a \fIneedsterminal\fP flag,
  ** and the external program is interactive.
  ** .pp
  ** When \fIset\fP, NeoMutt will always ask for a key. When \fIunset\fP, NeoMutt will wait
  ** for a key only if the external command returned a non-zero status.
  */
  { "weed", DT_BOOL, &C_Weed, true, 0, NULL,
    "When _set_, NeoMutt will weed headers when displaying, forwarding,\n"
    "printing, or replying to messages.\n"
  },
  /*
  ** .pp
  ** When \fIset\fP, NeoMutt will weed headers when displaying, forwarding,
  ** printing, or replying to messages.
  */
  { "wrap", DT_NUMBER|R_PAGER_FLOW, &C_Wrap, 0, 0, NULL,
    "When set to a positive value, NeoMutt will wrap text at $$wrap characters.\n"
    "When set to a negative value, NeoMutt will wrap text so that there are $$wrap\n"
    "characters of empty space on the right side of the terminal. Setting it\n"
    "to zero makes NeoMutt wrap at the terminal width.\n"
    "\n"
    "Also see $$reflow_wrap.\n"
  },
  /*
  ** .pp
  ** When set to a positive value, NeoMutt will wrap text at $$wrap characters.
  ** When set to a negative value, NeoMutt will wrap text so that there are $$wrap
  ** characters of empty space on the right side of the terminal. Setting it
  ** to zero makes NeoMutt wrap at the terminal width.
  ** .pp
  ** Also see $$reflow_wrap.
  */
  { "wrap_headers", DT_NUMBER|DT_NOT_NEGATIVE|R_PAGER, &C_WrapHeaders, 78, 0, wrapheaders_validator,
    "This option specifies the number of characters to use for wrapping\n"
    "an outgoing message's headers. Allowed values are between 78 and 998\n"
    "inclusive.\n"
    "\n"
    "**Note:** This option usually shouldn't be changed. RFC5233\n"
    "recommends a line length of 78 (the default), so **please only change\n"
    "this setting when you know what you're doing**.\n"
  },
  /*
  ** .pp
  ** This option specifies the number of characters to use for wrapping
  ** an outgoing message's headers. Allowed values are between 78 and 998
  ** inclusive.
  ** .pp
  ** \fBNote:\fP This option usually shouldn't be changed. RFC5233
  ** recommends a line length of 78 (the default), so \fBplease only change
  ** this setting when you know what you're doing\fP.
  */
  { "wrap_search", DT_BOOL, &C_WrapSearch, true, 0, NULL,
    "Controls whether searches wrap around the end.\n"
    "\n"
    "When _set_, searches will wrap around the first (or last) item. When\n"
    "_unset_, incremental searches will not wrap.\n"
  },
  /*
  ** .pp
  ** Controls whether searches wrap around the end.
  ** .pp
  ** When \fIset\fP, searches will wrap around the first (or last) item. When
  ** \fIunset\fP, incremental searches will not wrap.
  */
  { "write_bcc", DT_BOOL, &C_WriteBcc, false, 0, NULL,
    "Controls whether mutt writes out the ``Bcc:'' header when\n"
    "preparing messages to be sent.  Some MTAs, such as Exim and\n"
    "Courier, do not strip the ``Bcc:'' header; so it is advisable to\n"
    "leave this unset unless you have a particular need for the header\n"
    "to be in the sent message.\n"
    "\n"
    "If mutt is set to deliver directly via SMTP(see $$smtp_url),\n"
    "this option does nothing: mutt will never write out the ``Bcc:''\n"
    "header in this case.\n"
    "\n"
    "Note this option only affects the sending of messages.  Fcc'ed\n"
    "copies of a message will always contain the ``Bcc:'' header if\n"
    "one exists.\n"
  },
  /*
  ** .pp
  ** Controls whether mutt writes out the ``Bcc:'' header when
  ** preparing messages to be sent.  Some MTAs, such as Exim and
  ** Courier, do not strip the ``Bcc:'' header; so it is advisable to
  ** leave this unset unless you have a particular need for the header
  ** to be in the sent message.
  ** .pp
  ** If mutt is set to deliver directly via SMTP(see $$smtp_url),
  ** this option does nothing: mutt will never write out the ``Bcc:''
  ** header in this case.
  ** .pp
  ** Note this option only affects the sending of messages.  Fcc'ed
  ** copies of a message will always contain the ``Bcc:'' header if
  ** one exists.
  */
  { "write_inc", DT_NUMBER|DT_NOT_NEGATIVE, &C_WriteInc, 10, 0, NULL,
    "When writing a mailbox, a message will be printed every\n"
    "$$write_inc messages to indicate progress.  If set to 0, only a\n"
    "single message will be displayed before writing a mailbox.\n"
    "\n"
    "Also see the $$read_inc, $$net_inc and $$time_inc variables and the\n"
    "\"$tuning\" section of the manual for performance considerations.\n"
  },
  /*
  ** .pp
  ** When writing a mailbox, a message will be printed every
  ** $$write_inc messages to indicate progress.  If set to 0, only a
  ** single message will be displayed before writing a mailbox.
  ** .pp
  ** Also see the $$read_inc, $$net_inc and $$time_inc variables and the
  ** "$tuning" section of the manual for performance considerations.
  */
#ifdef USE_NNTP
  { "x_comment_to", DT_BOOL, &C_XCommentTo, false, 0, NULL,
    "If _set_, NeoMutt will add \"X-Comment-To:\" field (that contains full\n"
    "name of original article author) to article that followuped to newsgroup.\n"
  },
  /*
  ** .pp
  ** If \fIset\fP, NeoMutt will add "X-Comment-To:" field (that contains full
  ** name of original article author) to article that followuped to newsgroup.
  */
#endif
  /*--*/

  { "ignore_linear_white_space", DT_DEPRECATED|DT_BOOL, &C_IgnoreLinearWhiteSpace, false,   0, NULL, NULL },
  { "pgp_encrypt_self",          DT_DEPRECATED|DT_QUAD, &C_PgpEncryptSelf,         MUTT_NO, 0, NULL, NULL },
  { "smime_encrypt_self",        DT_DEPRECATED|DT_QUAD, &C_SmimeEncryptSelf,       MUTT_NO, 0, NULL, NULL },

  { "abort_noattach_regexp",  DT_SYNONYM, NULL, IP "abort_noattach_regex",     0, NULL, NULL },
  { "attach_keyword",         DT_SYNONYM, NULL, IP "abort_noattach_regex",     0, NULL, NULL },
  { "edit_hdrs",              DT_SYNONYM, NULL, IP "edit_headers",             0, NULL, NULL },
  { "envelope_from",          DT_SYNONYM, NULL, IP "use_envelope_from",        0, NULL, NULL },
  { "forw_decode",            DT_SYNONYM, NULL, IP "forward_decode",           0, NULL, NULL },
  { "forw_decrypt",           DT_SYNONYM, NULL, IP "forward_decrypt",          0, NULL, NULL },
  { "forw_format",            DT_SYNONYM, NULL, IP "forward_format",           0, NULL, NULL },
  { "forw_quote",             DT_SYNONYM, NULL, IP "forward_quote",            0, NULL, NULL },
  { "hdr_format",             DT_SYNONYM, NULL, IP "index_format",             0, NULL, NULL },
  { "indent_str",             DT_SYNONYM, NULL, IP "indent_string",            0, NULL, NULL },
  { "mime_fwd",               DT_SYNONYM, NULL, IP "mime_forward",             0, NULL, NULL },
  { "msg_format",             DT_SYNONYM, NULL, IP "message_format",           0, NULL, NULL },
  { "pgp_autoencrypt",        DT_SYNONYM, NULL, IP "crypt_autoencrypt",        0, NULL, NULL },
  { "pgp_autosign",           DT_SYNONYM, NULL, IP "crypt_autosign",           0, NULL, NULL },
  { "pgp_auto_traditional",   DT_SYNONYM, NULL, IP "pgp_replyinline",          0, NULL, NULL },
  { "pgp_create_traditional", DT_SYNONYM, NULL, IP "pgp_autoinline",           0, NULL, NULL },
  { "pgp_replyencrypt",       DT_SYNONYM, NULL, IP "crypt_replyencrypt",       0, NULL, NULL },
  { "pgp_replysign",          DT_SYNONYM, NULL, IP "crypt_replysign",          0, NULL, NULL },
  { "pgp_replysignencrypted", DT_SYNONYM, NULL, IP "crypt_replysignencrypted", 0, NULL, NULL },
  { "pgp_self_encrypt_as",    DT_SYNONYM, NULL, IP "pgp_default_key",          0, NULL, NULL },
  { "pgp_verify_sig",         DT_SYNONYM, NULL, IP "crypt_verify_sig",         0, NULL, NULL },
  { "post_indent_str",        DT_SYNONYM, NULL, IP "post_indent_string",       0, NULL, NULL },
  { "print_cmd",              DT_SYNONYM, NULL, IP "print_command",            0, NULL, NULL },
  { "quote_regexp",           DT_SYNONYM, NULL, IP "quote_regex",              0, NULL, NULL },
  { "reply_regexp",           DT_SYNONYM, NULL, IP "reply_regex",              0, NULL, NULL },
  { "smime_self_encrypt_as",  DT_SYNONYM, NULL, IP "smime_default_key",        0, NULL, NULL },
  { "xterm_icon",             DT_SYNONYM, NULL, IP "ts_icon_format",           0, NULL, NULL },
  { "xterm_set_titles",       DT_SYNONYM, NULL, IP "ts_enabled",               0, NULL, NULL },
  { "xterm_title",            DT_SYNONYM, NULL, IP "ts_status_format",         0, NULL, NULL },

  { NULL, 0, NULL, 0, 0, NULL, NULL },
};
// clang-format on
