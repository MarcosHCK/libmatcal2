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
#ifndef __MATREE_TOKENIZER__
#define __MATREE_TOKENIZER__ 1
#include <glib-object.h>

#define MATREE_TYPE_TOKENIZER (matree_tokenizer_get_type ())
#define MATREE_TOKENIZER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), MATREE_TYPE_TOKENIZER, MatreeTokenizer))
#define MATREE_IS_TOKENIZER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MATREE_TYPE_TOKENIZER))

typedef struct _MatreeTokenizer MatreeTokenizer;

#if __cplusplus
extern "C" {
#endif // __cplusplus

GType
matree_tokenizer_get_type (void) G_GNUC_CONST;
MatreeTokenizer*
matree_tokenizer_new ();

#if __cplusplus
}
#endif // __cplusplus

#endif // __MATREE_TOKENIZER__
