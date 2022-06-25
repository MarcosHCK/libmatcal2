/* Copyright 2021-2025 MarcosHCK
 * This file is part of libmatcal.
 *
 * libmatcal is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libmatcal is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libmatcal.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef __DYNASM_OVERRIDE__
#define __DYNASM_OVERRIDE__ 1
#include <glib.h>

#define DASM_FDEF G_GNUC_INTERNAL

#define DASM_EXTERN(ctx, addr, idx, type) 0

#define DASM_M_GROW(ctx, t, p, sz, need) \
  G_STMT_START { \
    size_t _sz = (sz), _need = (need); \
    if (_sz < _need) \
      { \
        if (_sz < 16) _sz = 16; \
        while (_sz < _need) _sz += _sz; \
        (p) = (t *) g_realloc ((p), _sz); \
        (sz) = _sz; \
      } \
  } G_STMT_END

#define DASM_M_FREE(ctx, p, sz) \
  g_free (p);

#ifdef G_OS_WINDOWS
# include <windows.h>
#else // !G_OS_WINDOWS
# include <sys/mman.h>
# if !defined(MAP_ANONYMOUS) && defined(MAP_ANON)
#   define MAP_ANONYMOUS MAP_ANON
# endif // !MAP_ANONYMOUS && MAP_ANON
#endif // G_OS_WINDOWS 

#include <compiler.h>
#endif // __DYNASM_OVERRIDE__
