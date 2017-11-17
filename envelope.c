/**
 * @file
 * Representation of an email header (envelope)
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

/**
 * @page envelope Representation of an email header (envelope)
 *
 * Representation of an email header (envelope)
 *
 * | Function                     | Description
 * | :--------------------------- | :---------------------------------------------------------
 * | mutt_free_envelope           | Free an Envelope
 * | mutt_merge_envelopes         | Merge the headers of two Envelopes
 * | mutt_new_envelope            | Create a new Envelope
 */

#include "config.h"
#include <stddef.h>
#include "mutt/buffer.h"
#include "mutt/memory.h"
#include "mutt/queue.h"
#include "envelope.h"
#include "rfc822.h"

/**
 * mutt_new_envelope - Create a new Envelope
 * @retval ptr New Envelope
 */
struct Envelope *mutt_new_envelope(void)
{
  struct Envelope *e = mutt_mem_calloc(1, sizeof(struct Envelope));
  STAILQ_INIT(&e->references);
  STAILQ_INIT(&e->in_reply_to);
  STAILQ_INIT(&e->userhdrs);
  return e;
}

/**
 * mutt_free_envelope - Free an Envelope
 * @param p Envelope to free
 */
void mutt_free_envelope(struct Envelope **p)
{
  if (!*p)
    return;
  rfc822_free_address(&(*p)->return_path);
  rfc822_free_address(&(*p)->from);
  rfc822_free_address(&(*p)->to);
  rfc822_free_address(&(*p)->cc);
  rfc822_free_address(&(*p)->bcc);
  rfc822_free_address(&(*p)->sender);
  rfc822_free_address(&(*p)->reply_to);
  rfc822_free_address(&(*p)->mail_followup_to);

  FREE(&(*p)->list_post);
  FREE(&(*p)->subject);
  /* real_subj is just an offset to subject and shouldn't be freed */
  FREE(&(*p)->disp_subj);
  FREE(&(*p)->message_id);
  FREE(&(*p)->supersedes);
  FREE(&(*p)->date);
  FREE(&(*p)->x_label);
  FREE(&(*p)->organization);
#ifdef USE_NNTP
  FREE(&(*p)->newsgroups);
  FREE(&(*p)->xref);
  FREE(&(*p)->followup_to);
  FREE(&(*p)->x_comment_to);
#endif

  mutt_buffer_free(&(*p)->spam);

  mutt_list_free(&(*p)->references);
  mutt_list_free(&(*p)->in_reply_to);
  mutt_list_free(&(*p)->userhdrs);
  FREE(p);
}

/**
 * mutt_merge_envelopes - Merge the headers of two Envelopes
 * @param base  Envelope destination for all the headers
 * @param extra Envelope to copy from
 *
 * Any fields that are missing from base will be copied from extra.
 * extra will be freed afterwards.
 */
void mutt_merge_envelopes(struct Envelope *base, struct Envelope **extra)
{
/* copies each existing element if necessary, and sets the element
 * to NULL in the source so that mutt_free_envelope doesn't leave us
 * with dangling pointers. */
#define MOVE_ELEM(h)                                                           \
  if (!base->h)                                                                \
  {                                                                            \
    base->h = (*extra)->h;                                                     \
    (*extra)->h = NULL;                                                        \
  }

#define MOVE_STAILQ(h)                                                         \
  if (STAILQ_EMPTY(&base->h))                                                  \
  {                                                                            \
    STAILQ_SWAP(&base->h, &((*extra))->h, ListNode);                           \
  }

  MOVE_ELEM(return_path);
  MOVE_ELEM(from);
  MOVE_ELEM(to);
  MOVE_ELEM(cc);
  MOVE_ELEM(bcc);
  MOVE_ELEM(sender);
  MOVE_ELEM(reply_to);
  MOVE_ELEM(mail_followup_to);
  MOVE_ELEM(list_post);
  MOVE_ELEM(message_id);
  MOVE_ELEM(supersedes);
  MOVE_ELEM(date);
  MOVE_ELEM(x_label);
  MOVE_ELEM(x_original_to);
  if (!base->refs_changed)
  {
    MOVE_STAILQ(references);
  }
  if (!base->irt_changed)
  {
    MOVE_STAILQ(in_reply_to);
  }

  /* real_subj is subordinate to subject */
  if (!base->subject)
  {
    base->subject = (*extra)->subject;
    base->real_subj = (*extra)->real_subj;
    base->disp_subj = (*extra)->disp_subj;
    (*extra)->subject = NULL;
    (*extra)->real_subj = NULL;
    (*extra)->disp_subj = NULL;
  }
  /* spam and user headers should never be hashed, and the new envelope may
    * have better values. Use new versions regardless. */
  mutt_buffer_free(&base->spam);
  mutt_list_free(&base->userhdrs);
  MOVE_ELEM(spam);
  MOVE_STAILQ(userhdrs);
#undef MOVE_ELEM

  mutt_free_envelope(extra);
}

int strict_cmp_envelopes(const struct Envelope *e1, const struct Envelope *e2)
{
  if (e1 && e2)
  {
    if ((mutt_str_strcmp(e1->message_id, e2->message_id) != 0) ||
        (mutt_str_strcmp(e1->subject, e2->subject) != 0) ||
        !mutt_list_compare(&e1->references, &e2->references) ||
        !strict_addrcmp(e1->from, e2->from) || !strict_addrcmp(e1->sender, e2->sender) ||
        !strict_addrcmp(e1->reply_to, e2->reply_to) ||
        !strict_addrcmp(e1->to, e2->to) || !strict_addrcmp(e1->cc, e2->cc) ||
        !strict_addrcmp(e1->return_path, e2->return_path))
      return 0;
    else
      return 1;
  }
  else
  {
    if (!e1 && !e2)
      return 1;
    else
      return 0;
  }
}
