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
#include <config.h>
#include <tokenizer.h>

#define MATREE_TOKENIZER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), MATREE_TYPE_TOKENIZER, MatreeTokenizerClass))
#define MATREE_IS_TOKENIZER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MATREE_TYPE_TOKENIZER))
#define MATREE_TOKENIZER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), MATREE_TYPE_TOKENIZER, MatreeTokenizerClass))
typedef struct _MatreeTokenizerClass MatreeTokenizerClass;

struct _MatreeTokenizer
{
  GObject parent;
};

struct _MatreeTokenizerClass
{
  GObjectClass parent;
};

G_DEFINE_FINAL_TYPE (MatreeTokenizer, matree_tokenizer, G_TYPE_OBJECT);

static void
matree_tokenizer_class_init (MatreeTokenizerClass* klass)
{
}

static void
matree_tokenizer_init (MatreeTokenizer* self)
{
}

/**
 * matree_tokenizer_new: (constructor)
 *
 * Creates a void instance of #MatreeTokenizer
 *
 * Returns: (transfer full): a new #MatreeTokenizer instance.
 */
MatreeTokenizer*
matree_tokenizer_new ()
{
  return
  g_object_new (MATREE_TYPE_TOKENIZER, NULL);
}
