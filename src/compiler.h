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
#ifndef __MATREE_COMPILER__
#define __MATREE_COMPILER__ 1
#include <node.h>

typedef struct _JitCallback JitCallback;

struct _JitCallback
{
  gpointer start;
  gpointer block;
  gsize blocksz;
};

#if __cplusplus
extern "C" {
#endif // __cplusplus

JitCallback*
libjit_compile (AstNode* ast, GError** error);
void
libjit_free (JitCallback* callback);

#if __cplusplus
}
#endif // __cplusplus

#endif // __MATREE_COMPILER__
