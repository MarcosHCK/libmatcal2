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
#ifndef __MATREE_TOKENS__
#define __MATREE_TOKENS__ 1
#include <glib-object.h>

#define MATREE_TYPE_TOKENS (matree_tokens_get_type ())
#define MATREE_TOKENS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), MATREE_TYPE_TOKENS, MatreeTokens))
#define MATREE_IS_TOKENS(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MATREE_TYPE_TOKENS))
typedef struct _MatreeTokens MatreeTokens;

#if __cplusplus
extern "C" {
#endif // __cplusplus

GType
matree_tokens_get_type (void) G_GNUC_CONST;
MatreeTokens*
matree_tokens_get_default (void) G_GNUC_CONST;
gboolean
matree_tokens_add (MatreeTokens* tokens, const gchar* token, GError** error);
gchar**
matree_tokens_split (MatreeTokens* tokens, const gchar* input, gssize length, GError** error);

#if __cplusplus
}
#endif // __cplusplus

#endif // __MATREE_TOKENS__
