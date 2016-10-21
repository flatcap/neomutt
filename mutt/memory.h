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

#ifndef _MUTT_MEMORY_H
#define _MUTT_MEMORY_H

#include <stddef.h>

#undef MAX
#undef MIN
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define mutt_array_size(x) (sizeof(x) / sizeof((x)[0]))

void *_safe_calloc(size_t nmemb, size_t size, const char *func, const char *file, int line);
void *_safe_malloc(size_t siz, const char *func, const char *file, int line);
void _safe_free(void *ptr, const char *func, const char *file, int line);
void _safe_realloc(void *ptr, size_t siz, const char *func, const char *file, int line);

#define mutt_mem_calloc(X,Y)  _safe_calloc (X,Y,__func__,__FILE__,__LINE__)
#define mutt_mem_malloc(X)    _safe_malloc (X,__func__,__FILE__,__LINE__)
#define mutt_mem_free(X)      _safe_free (X,__func__,__FILE__,__LINE__)
#define mutt_mem_realloc(X,Y) _safe_realloc (X,Y,__func__,__FILE__,__LINE__)

#define FREE(X) _safe_free (X,__func__,__FILE__,__LINE__)

#endif /* _MUTT_MEMORY_H */
