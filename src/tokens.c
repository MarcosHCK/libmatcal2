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
#include <tokens.h>

static MatreeTokens* __default__ = NULL;

#define MATREE_TOKENS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), MATREE_TYPE_TOKENS, MatreeTokensClass))
#define MATREE_IS_TOKENS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MATREE_TYPE_TOKENS))
#define MATREE_TOKENS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), MATREE_TYPE_TOKENS, MatreeTokensClass))
typedef struct _MatreeTokensClass MatreeTokensClass;
#define _g_match_info_unref0(var) ((var == NULL) ? NULL : (var = (g_match_info_unref (var), NULL)))
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
#define _g_regex_unref0(var) ((var == NULL) ? NULL : (var = (g_regex_unref (var), NULL)))
#define _g_error_free0(var) ((var == NULL) ? NULL : (var = (g_error_free (var), NULL)))
#define _g_strfreev0(var) ((var == NULL) ? NULL : (var = (g_strfreev (var), NULL)))
#define _g_free0(var) ((var == NULL) ? NULL : (var = (g_free (var), NULL)))

struct _MatreeTokens
{
  GObject parent;
  GPtrArray* regex;
};

struct _MatreeTokensClass
{
  GObjectClass parent;
};

G_DEFINE_FINAL_TYPE (MatreeTokens, matree_tokens, G_TYPE_OBJECT);

static GObject*
matree_tokens_class_constructor (GType gtype, guint n_construct_properties, GObjectConstructParam* construct_properties)
{
  if (__default__ == NULL)
    {
      __default__ = (MatreeTokens*)
      G_OBJECT_CLASS (matree_tokens_parent_class)->constructor (gtype, n_construct_properties, construct_properties);
      return (GObject*) g_object_ref (__default__);
    }
  else
    {
      return (GObject*) g_object_ref (__default__);
    }
}

static void
matree_tokens_class_finalize (GObject* pself)
{
  MatreeTokens* self = MATREE_TOKENS (pself);
  g_ptr_array_unref (self->regex);
G_OBJECT_CLASS (matree_tokens_parent_class)->finalize (pself);
}

static void
matree_tokens_class_dispose (GObject* pself)
{
  MatreeTokens* self = MATREE_TOKENS (pself);
G_OBJECT_CLASS (matree_tokens_parent_class)->dispose (pself);
}

static void
matree_tokens_class_init (MatreeTokensClass* klass)
{
  GObjectClass* oclass = G_OBJECT_CLASS (klass);

  oclass->constructor = matree_tokens_class_constructor;
  oclass->finalize = matree_tokens_class_finalize;
  oclass->dispose = matree_tokens_class_dispose;
}

static void
matree_tokens_init (MatreeTokens* self)
{
  GPtrArray* array =
  g_ptr_array_sized_new (20);
  g_ptr_array_set_free_func (array, g_free);
  self->regex = (gpointer) array;
}

/**
 * matree_tokens_get_default:
 * Gets process default #MatreeTokens instance.
 * Returns: (transfer full): default #MatreeTokens instance.
 */
MatreeTokens*
matree_tokens_get_default ()
{
  if (!__default__)
    return g_object_new (MATREE_TYPE_TOKENS, NULL);
  return g_object_ref (__default__);
}

/**
 * matree_tokens_add:
 * @tokens: #MatreeTokens instance.
 * @token: token regular expression.
 * @error: return location for a #GError.
 *
 * Adds a token regular expression to @tokens.
 * 
 * Returns: if regular expression is valid.
 */
gboolean
matree_tokens_add (MatreeTokens* tokens, const gchar* token, GError** error)
{
  g_return_val_if_fail (MATREE_TOKENS (tokens), FALSE);
  g_return_val_if_fail (token != NULL, FALSE);
  MatreeTokens* self = (tokens);
  GError* tmp_err = NULL;
  GRegex* regex = NULL;

  regex =
  g_regex_new (token, G_REGEX_OPTIMIZE, 0, &tmp_err);
  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_propagate_error (error, tmp_err);
      _g_regex_unref0 (regex);
      return FALSE;
    }

  g_ptr_array_add (self->regex, regex);
return TRUE;
}

/**
 * matree_tokens_split:
 * @tokens: #MatreeTokens instance.
 * @input: input string.
 * @length: input string length.
 * @error: return location for a #GError.
 *
 * Splits input string into tokens.
 *
 * Returns: (transfer full) (array zero-terminated=1): token list.
 */
gchar**
matree_tokens_split (MatreeTokens* tokens, const gchar* input, gssize length, GError** error)
{
  const gchar* end = NULL;
  g_return_val_if_fail (MATREE_IS_TOKENS (tokens), NULL);
  g_return_val_if_fail (input != NULL, NULL);
  g_return_val_if_fail (g_utf8_validate (input, length, &end), NULL);
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  GPtrArray* list = g_ptr_array_new_with_free_func (g_free);
  MatreeTokens* self = (tokens);
  gboolean matches = FALSE;
  GError* tmp_err = NULL;
  GMatchInfo* info = NULL;
  GRegex* regex = NULL;
  gchar** subtoks = NULL;
  gchar* match = NULL;
  guint i, n_tokens;
  gint start, stop;
  gint last = 0;

  n_tokens = self->regex->len;
  length = end - input;

  for (i = 0; i < n_tokens; i++)
    {
      regex = self->regex->pdata [i];
      matches = g_regex_match_full (regex, input, length, 0, 0, &info, &tmp_err);

      if (G_UNLIKELY (tmp_err != NULL))
        {
          g_propagate_error (error, tmp_err);
          g_ptr_array_unref (list);
          return NULL;
        }

      if (!matches)
        {
          _g_match_info_unref0 (info);
          continue;
        }

      while (g_match_info_matches (info))
        {
          g_match_info_fetch_pos (info, 0, &start, &stop);

          if (start > last)
            {
              subtoks =
              matree_tokens_split (tokens, input + last, start - last, &tmp_err);
              if (G_UNLIKELY (tmp_err != NULL))
                {
                  g_propagate_error (error, tmp_err);
                  _g_match_info_unref0 (info);
                  g_ptr_array_unref (list);
                  _g_strfreev0 (subtoks);
                  return FALSE;
                }

              gchar* subtok = NULL;
              guint i = 0;

              while ((subtok = subtoks[i++]) != NULL)
                g_ptr_array_add (list, subtok);
              g_free (subtoks);
            }

          last = stop;

          match =
          g_strndup (input + start, stop - start);
          g_ptr_array_add (list, match);

          g_match_info_next (info, &tmp_err);
          if (G_UNLIKELY (tmp_err != NULL))
            {
              g_propagate_error (error, tmp_err);
              _g_match_info_unref0 (info);
              g_ptr_array_unref (list);
              return FALSE;
            }
        }

      _g_match_info_unref0 (info);

      if (length > last)
        {
          subtoks =
          matree_tokens_split (tokens, input + last, length - last, &tmp_err);
          if (G_UNLIKELY (tmp_err != NULL))
            {
              g_propagate_error (error, tmp_err);
              g_ptr_array_unref (list);
              _g_strfreev0 (subtoks);
              return FALSE;
            }

          gchar* subtok = NULL;
          guint i = 0;

          while ((subtok = subtoks[i++]) != NULL)
            g_ptr_array_add (list, subtok);
          g_free (subtoks);
        }

      break;
    }

  if (list->len == 0)
  g_ptr_array_add (list, g_strndup (input, length));
  g_ptr_array_add (list, NULL);
return (gchar**)
  g_ptr_array_free (list, FALSE);
}
