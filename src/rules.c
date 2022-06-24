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
#include <rulesext.h>

#define MATREE_RULES_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), MATREE_TYPE_RULES, MatreeRulesClass))
#define MATREE_IS_RULES_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MATREE_TYPE_RULES))
#define MATREE_RULES_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), MATREE_TYPE_RULES, MatreeRulesClass))
typedef struct _MatreeRulesClass MatreeRulesClass;
#define _g_match_info_unref0(var) ((var == NULL) ? NULL : (var = (g_match_info_unref (var), NULL)))
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
#define _g_regex_unref0(var) ((var == NULL) ? NULL : (var = (g_regex_unref (var), NULL)))
#define _g_error_free0(var) ((var == NULL) ? NULL : (var = (g_error_free (var), NULL)))
#define _g_strfreev0(var) ((var == NULL) ? NULL : (var = (g_strfreev (var), NULL)))
#define _g_free0(var) ((var == NULL) ? NULL : (var = (g_free (var), NULL)))

const int prealloc = 16;

struct _MatreeRules
{
  GObject parent;

  union
  {
    GPtrArray array_;
    struct
    {
      GRegex** regex;
      guint len;
    };
  } *tokenize;

  union
  {
    GArray array_;
    struct
    {
      SymbolClass *e;
      guint len;
    };
  } *classes;
};

struct _MatreeRulesClass
{
  GObjectClass parent;
};

G_DEFINE_FINAL_TYPE (MatreeRules, matree_rules, G_TYPE_OBJECT);

static void
matree_rules_class_finalize (GObject* pself)
{
  MatreeRules* self = MATREE_RULES (pself);
  g_ptr_array_unref ((gpointer) self->tokenize);
  g_array_unref ((gpointer) self->classes);
G_OBJECT_CLASS (matree_rules_parent_class)->finalize (pself);
}

static void
matree_rules_class_dispose (GObject* pself)
{
  MatreeRules* self = MATREE_RULES (pself);
G_OBJECT_CLASS (matree_rules_parent_class)->dispose (pself);
}

static void
matree_rules_class_init (MatreeRulesClass* klass)
{
  GObjectClass* oclass = G_OBJECT_CLASS (klass);

  oclass->finalize = matree_rules_class_finalize;
  oclass->dispose = matree_rules_class_dispose;
}

static void
symbol_class_free (SymbolClass* klass)
{
  g_regex_unref (klass->regex);
}

static void
matree_rules_init (MatreeRules* self)
{
  self->tokenize = (gpointer) g_ptr_array_sized_new (prealloc);
  self->classes = (gpointer) g_array_sized_new (0, 1, sizeof (SymbolClass), prealloc);

  g_ptr_array_set_free_func ((gpointer) self->tokenize, (GDestroyNotify) g_regex_unref);
  g_array_set_clear_func ((gpointer) self->classes, (GDestroyNotify) symbol_class_free);
}

/**
 * matree_rules_new: (constructor)
 * 
 * See return.
 * 
 * Returns: (transfer full): an empty #MatreeRules instance.
 */
MatreeRules*
matree_rules_new ()
{
  return g_object_new (MATREE_TYPE_RULES, NULL);
}

static inline void
load_default_rules (MatreeRules* rules, GError** error)
{
  GError* tmp_err = NULL;
#define throw(loc,err) \
  G_STMT_START { \
    if (G_UNLIKELY ((err) != NULL)) \
      { \
        g_propagate_error ((loc), (err)); \
        _g_object_unref0 (rules); \
        return; \
      } \
  } G_STMT_END

  { /* all things which is not a number or a letter */
    _matree_rules_add_token (rules, "[^a-zA-Z0-9\\.]", &tmp_err);
    throw (error, tmp_err);

    {
      SymbolClass klass = {NULL, SYMBOL_KIND_PARENTHESIS, };
      _matree_rules_add_class (rules, "[\\(\\)]", &klass, -1, &tmp_err);
      throw (error, tmp_err);
    }

    {
      SymbolClass klass = {NULL, SYMBOL_KIND_OPERATOR, };
      const OperatorClass pwclass = {OPERATOR_ASSOC_RIGHT, 4, FALSE};
      const OperatorClass mlclass = {OPERATOR_ASSOC_LEFT, 3, FALSE};
      const OperatorClass dvclass = {OPERATOR_ASSOC_LEFT, 3, FALSE};
      const OperatorClass sbclass = {OPERATOR_ASSOC_LEFT, 2, FALSE};
      const OperatorClass plclass = {OPERATOR_ASSOC_LEFT, 2, FALSE};

      klass.opclass = pwclass;
      _matree_rules_add_class (rules, "[\\^]", &klass, -1, &tmp_err);
      throw (error, tmp_err);

      klass.opclass = mlclass;
      _matree_rules_add_class (rules, "[\\*]", &klass, -1, &tmp_err);
      throw (error, tmp_err);

      klass.opclass = dvclass;
      _matree_rules_add_class (rules, "[\\/]", &klass, -1, &tmp_err);
      throw (error, tmp_err);

      klass.opclass = sbclass;
      _matree_rules_add_class (rules, "[\\-]", &klass, -1, &tmp_err);
      throw (error, tmp_err);

      klass.opclass = plclass;
      _matree_rules_add_class (rules, "[\\+]", &klass, -1, &tmp_err);
      throw (error, tmp_err);
    }
  }

  { /* some functions */
    FunctionClass fnclass = {1};
    _matree_rules_add_function (rules, "sin", &fnclass, -1, &tmp_err);
    throw (error, tmp_err);
    _matree_rules_add_function (rules, "cos", &fnclass, -1, &tmp_err);
    throw (error, tmp_err);
    _matree_rules_add_function (rules, "tan", &fnclass, -1, &tmp_err);
    throw (error, tmp_err);
    _matree_rules_add_function (rules, "lg[0-9]+", &fnclass, -1, &tmp_err);
    throw (error, tmp_err);
  }

  {
    FunctionClass fnclass = {2};
    _matree_rules_add_function (rules, "max", &fnclass, -1, &tmp_err);
    throw (error, tmp_err);
    _matree_rules_add_function (rules, "min", &fnclass, -1, &tmp_err);
    throw (error, tmp_err);
  }

  { /* variables */
    SymbolClass klass = {NULL, SYMBOL_KIND_VARIABLE, };
    _matree_rules_add_token (rules, "[a-zA-Z]", &tmp_err);
    throw (error, tmp_err);
    _matree_rules_add_class (rules, "[a-zA-Z]", &klass, -1, &tmp_err);
    throw (error, tmp_err);
  }

  { /* numbers */
    SymbolClass klass = {NULL, SYMBOL_KIND_CONSTANT, };
    _matree_rules_add_token (rules, "[0-9\\.]+", &tmp_err);
    throw (error, tmp_err);
    _matree_rules_add_class (rules, "[0-9\\.]+", &klass, -1, &tmp_err);
    throw (error, tmp_err);
  }

#undef throw
}

/**
 * matree_rules_new_default: (constructor)
 * 
 * See return.
 * 
 * Returns: (transfer full): a #MatreeRules instance.
 */
MatreeRules*
matree_rules_new_default ()
{
  MatreeRules* rules = NULL;
  GError* tmp_err = NULL;

  rules = matree_rules_new ();
  load_default_rules (rules, &tmp_err);
  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_critical
      ("(%s): %s: %i: %s",
       G_STRLOC,
       g_quark_to_string
       (tmp_err->domain),
       tmp_err->code,
       tmp_err->message);
      _g_error_free0 (tmp_err);
      g_assert_not_reached ();
    }
return rules;
}

/* lexer */

G_GNUC_INTERNAL
gchar**
_matree_rules_tokenize (MatreeRules* self, const gchar* input, gssize length, GError** error)
{
  const gchar* end = NULL;
  g_return_val_if_fail (input != NULL, NULL);
  g_return_val_if_fail (g_utf8_validate (input, length, &end), NULL);
  GPtrArray* list = g_ptr_array_new_with_free_func (g_free);
  gboolean matches = FALSE;
  GError* tmp_err = NULL;
  GMatchInfo* info = NULL;
  GRegex* regex = NULL;
  gchar** subtoks = NULL;
  gchar* match = NULL;
  guint i, n_tokens;
  gint start, stop;
  gint last = 0;

  n_tokens = self->tokenize->len;
  length = end - input;

  for (i = 0; i < n_tokens; i++)
    {
      regex = self->tokenize->regex [i];
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
              _matree_rules_tokenize (self, input + last, start - last, &tmp_err);
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
          _matree_rules_tokenize (self, input + last, length - last, &tmp_err);
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

/* classifier */

G_GNUC_INTERNAL
SymbolClass*
_matree_rules_classify (MatreeRules* self, const gchar* input, GError** error)
{
  const gchar* end = NULL;
  g_return_val_if_fail (input != NULL, NULL);
  g_return_val_if_fail (g_utf8_validate (input, -1, &end), NULL);
  GError* tmp_err = NULL;
  GRegex* regex = NULL;
  gboolean matches = FALSE;
  guint i, n_classes;
  gsize length;

  n_classes = self->classes->len;
  length = end - input;

  for (i = 0; i < n_classes; i++)
    {
      regex = self->classes->e [i].regex;
      matches = g_regex_match_full (regex, input, length, 0, 0, NULL, &tmp_err);

      if (G_UNLIKELY (tmp_err != NULL))
        {
          g_propagate_error (error, tmp_err);
          return NULL;
        }

      if (matches)
        {
          return & self->classes->e [i];
        }
    }
return NULL;
}

/* lexer & classifier adders */

static inline void
_matree_rules_tokenize_add (MatreeRules* self, GRegex* regex)
{
  g_ptr_array_add ((gpointer) self->tokenize, g_regex_ref (regex));
}

static inline void
_matree_rules_classify_add (MatreeRules* self, GRegex* regex, SymbolClass* klass, gint pos)
{
  SymbolClass entry = *klass;
  pos = (pos == -1) ? self->classes->len : pos;
  entry.regex = g_regex_ref (regex);
  g_array_insert_vals ((gpointer) self->classes, pos, &entry, 1);
}

G_GNUC_INTERNAL
void
_matree_rules_add_token (MatreeRules* self, const gchar* expr, GError** error)
{
  GError* tmp_err = NULL;
  GRegex* regex = NULL;

  regex =
  g_regex_new (expr, G_REGEX_OPTIMIZE, 0, &tmp_err);
  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_propagate_error (error, tmp_err);
      _g_regex_unref0 (regex);
      return;
    }

  _matree_rules_tokenize_add (self, regex);
  _g_regex_unref0 (regex);
}

G_GNUC_INTERNAL
void
_matree_rules_add_class (MatreeRules* self, const gchar* expr, SymbolClass* klass, gint pos, GError** error)
{
  GError* tmp_err = NULL;
  GRegex* regex = NULL;

  regex =
  g_regex_new (expr, G_REGEX_OPTIMIZE, 0, &tmp_err);
  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_propagate_error (error, tmp_err);
      _g_regex_unref0 (regex);
      return;
    }

  _matree_rules_classify_add (self, regex, klass, pos);
  _g_regex_unref0 (regex);
}

G_GNUC_INTERNAL
void
_matree_rules_add_operator (MatreeRules* self, const gchar* expr, OperatorClass* opclass, gint pos, GError** error)
{
  GError* tmp_err = NULL;
  SymbolClass klass = {0};

  klass.kind = SYMBOL_KIND_OPERATOR;
  klass.opclass = *opclass;

  _matree_rules_add_token (self, expr, &tmp_err);
  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_propagate_error (error, tmp_err);
      return;
    }

  _matree_rules_add_class (self, expr, &klass, pos, &tmp_err);
  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_propagate_error (error, tmp_err);
      return;
    }
}

G_GNUC_INTERNAL
void
_matree_rules_add_function (MatreeRules* self, const gchar* expr, FunctionClass* fnclass, gint pos, GError** error)
{
  GError* tmp_err = NULL;
  SymbolClass klass = {0};

  klass.kind = SYMBOL_KIND_FUNCTION;
  klass.fnclass = *fnclass;

  _matree_rules_add_token (self, expr, &tmp_err);
  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_propagate_error (error, tmp_err);
      return;
    }

  _matree_rules_add_class (self, expr, &klass, pos, &tmp_err);
  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_propagate_error (error, tmp_err);
      return;
    }
}

