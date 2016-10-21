/**
 * @file
 * Memory management wrappers
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
 * @page memory Memory management wrappers
 *
 * "Safe" memory management routines.
 *
 * @note If any of the allocators fail, the user is notified and the program is
 *       stopped immediately.
 *
 * | Function           | Description
 * | :----------------- | :-----------------------------------
 * | mutt_mem_calloc()  | Allocate zeroed memory on the heap
 * | mutt_mem_free()    | Release memory allocated on the heap
 * | mutt_mem_malloc()  | Allocate memory on the heap
 * | mutt_mem_realloc() | Resize a block of memory on the heap
 */

#include "config.h"
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "memory.h"
#include "debug.h"
#include "exit.h"
#include "message.h"

struct MemCheck
{
  const char *file;
  const char *func;
  size_t size;
  unsigned int line;
};

/**
 * mutt_mem_calloc - Allocate zeroed memory on the heap
 * @param nmemb Number of blocks
 * @param size  Size of blocks
 * @retval ptr Memory on the heap
 *
 * @note This function will never return NULL.
 *       It will print and error and exit the program.
 *
 * The caller should call mutt_mem_free() to release the memory
 */
void *_safe_calloc(size_t nmemb, size_t size, const char *func, const char *file, int line)
{
  void *p = NULL;

  if (!nmemb || !size)
    return NULL;

  if (nmemb > (SIZE_MAX / size))
  {
    mutt_error(_("Integer overflow -- can't allocate memory!"));
    sleep(1);
    mutt_exit(1);
  }

  size_t mcs = sizeof(struct MemCheck);
  size += mcs;

  p = calloc(nmemb, size);
  if (!p)
  {
    mutt_error(_("Out of memory!"));
    sleep(1);
    mutt_exit(1);
  }

  struct MemCheck *mc = p;
  mc->func = func;
  mc->file = file;
  mc->line = line;
  mc->size = size;
  mutt_debug(1, "CALLOC %ld, %s:%d %s\n", size - mcs, file, line, func);

  return ((unsigned char *) p + mcs);
}

/**
 * mutt_mem_free - Release memory allocated on the heap
 * @param ptr Memory to release
 */
void _safe_free(void *ptr, const char *func, const char *file, int line)
{
  if (!ptr)
    return;

  unsigned char **p = ptr;
  if (!*p)
    return;

  size_t mcs = sizeof(struct MemCheck);
  *p -= mcs;

  struct MemCheck *mc = (struct MemCheck *) *p;
  mutt_debug(1, "FREE %ld, %s:%d %s\n", mc->size, mc->file, mc->line, mc->func);

  free(*p);
  *p = 0;
}

/**
 * mutt_mem_malloc - Allocate memory on the heap
 * @param size Size of block to allocate
 * @retval ptr Memory on the heap
 *
 * @note This function will never return NULL.
 *       It will print and error and exit the program.
 *
 * The caller should call mutt_mem_free() to release the memory
 */
void *_safe_malloc(size_t siz, const char *func, const char *file, int line)
{
  unsigned char *p;

  if (siz == 0)
    return NULL;

  size_t mcs = sizeof(struct MemCheck);
  siz += mcs;

  p = malloc(siz);
  if (!p)
  {
    mutt_error(_("Out of memory!"));
    sleep(1);
    mutt_exit(1);
  }

  struct MemCheck *mc = (struct MemCheck *) p;
  mc->func = func;
  mc->file = file;
  mc->line = line;
  mc->size = siz;
  mutt_debug(1, "MALLOC %ld, %s:%d %s\n", siz - mcs, file, line, func);

  return (p + mcs);
}

/**
 * mutt_mem_realloc - Resize a block of memory on the heap
 * @param ptr Memory block to resize
 * @param size New size
 *
 * @note This function will never return NULL.
 *       It will print and error and exit the program.
 *
 * If the new size is zero, the block will be freed.
 */
void _safe_realloc(void *ptr, size_t siz, const char *func, const char *file, int line)
{
  unsigned char *r;
  unsigned char **p = ptr;

  size_t mcs = sizeof(struct MemCheck);
  siz += mcs;

  if (siz == 0)
  {
    if (*p)
    {
      _safe_free(*p, func, file, line);
    }
    return;
  }

  if (*p)
  {
    *p -= mcs;
    siz += mcs;
    r = realloc(*p, siz);
    mutt_debug(1, "REALLOC %ld, %s:%d %s\n", siz - mcs, file, line, func);
  }
  else
  {
    *p -= mcs;
    siz += mcs;
    r = malloc(siz);
    mutt_debug(1, "MALLOC %ld, %s:%d %s\n", siz - mcs, file, line, func);
  }
 
  if (!r)
  {
    mutt_error(_("Out of memory!"));
    sleep(1);
    mutt_exit(1);
  }

  *p = (r + mcs);
}
