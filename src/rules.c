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

#define MATCAL_RULES_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), MATCAL_TYPE_RULES, MatcalRulesClass))
#define MATCAL_IS_RULES_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MATCAL_TYPE_RULES))
#define MATCAL_RULES_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), MATCAL_TYPE_RULES, MatcalRulesClass))
typedef struct _MatcalRulesClass MatcalRulesClass;
#define _g_match_info_unref0(var) ((var == NULL) ? NULL : (var = (g_match_info_unref (var), NULL)))
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
#define _g_regex_unref0(var) ((var == NULL) ? NULL : (var = (g_regex_unref (var), NULL)))
#define _g_error_free0(var) ((var == NULL) ? NULL : (var = (g_error_free (var), NULL)))
#define _g_strfreev0(var) ((var == NULL) ? NULL : (var = (g_strfreev (var), NULL)))
#define _g_free0(var) ((var == NULL) ? NULL : (var = (g_free (var), NULL)))
typedef struct _SymbolClassEntry SymbolClassEntry;

const int prealloc = 16;

struct _MatcalRules
{
  GObject parent;

  guint fn_token;
  guint fn_class;

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
      struct _SymbolClassEntry
      {
        GRegex* regex;
        SymbolClass klass;
      } *e;

      guint len;
    };
  } *classes;
};

struct _MatcalRulesClass
{
  GObjectClass parent;
};

G_DEFINE_FINAL_TYPE (MatcalRules, matcal_rules, G_TYPE_OBJECT);
G_STATIC_ASSERT (MATCAL_ASSOC_LEFT == OPERATOR_ASSOC_LEFT);
G_STATIC_ASSERT (MATCAL_ASSOC_RIGHT == OPERATOR_ASSOC_RIGHT);

static void
matcal_rules_class_finalize (GObject* pself)
{
  MatcalRules* self = MATCAL_RULES (pself);
  g_ptr_array_unref ((gpointer) self->tokenize);
  g_array_unref ((gpointer) self->classes);
G_OBJECT_CLASS (matcal_rules_parent_class)->finalize (pself);
}

static void
matcal_rules_class_dispose (GObject* pself)
{
  MatcalRules* self = MATCAL_RULES (pself);
G_OBJECT_CLASS (matcal_rules_parent_class)->dispose (pself);
}

static void
matcal_rules_class_init (MatcalRulesClass* klass)
{
  GObjectClass* oclass = G_OBJECT_CLASS (klass);

  oclass->finalize = matcal_rules_class_finalize;
  oclass->dispose = matcal_rules_class_dispose;
}

static void
symbol_class_entry_free (SymbolClassEntry* entry)
{
  g_regex_unref (entry->regex);
}

static void
matcal_rules_init (MatcalRules* self)
{
  gsize bytes = sizeof (SymbolClassEntry);
  self->tokenize = (gpointer) g_ptr_array_sized_new (prealloc);
  self->classes = (gpointer) g_array_sized_new (0, 1, bytes, prealloc);

  self->fn_token = -1;
  self->fn_class = -1;

  g_ptr_array_set_free_func ((gpointer) self->tokenize, (GDestroyNotify) g_regex_unref);
  g_array_set_clear_func ((gpointer) self->classes, (GDestroyNotify) symbol_class_entry_free);
}

/**
 * matcal_rules_new: (constructor)
 * 
 * See return.
 * 
 * Returns: (transfer full): an empty #MatcalRules instance.
 */
MatcalRules*
matcal_rules_new ()
{
  return g_object_new (MATCAL_TYPE_RULES, NULL);
}

static inline void
load_default_rules (MatcalRules* rules, GError** error)
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
    _matcal_rules_add_token (rules, "[^a-zA-Z0-9\\.]", -1, &tmp_err);
    throw (error, tmp_err);

    {
      SymbolClass klass = {SYMBOL_KIND_PARENTHESIS, };
      _matcal_rules_add_class (rules, "[\\(\\)]", &klass, -1, &tmp_err);
      throw (error, tmp_err);
    }
  }

  rules->fn_token = rules->tokenize->len;
  rules->fn_class = rules->classes->len;

  { /* variables */
    SymbolClass klass = {SYMBOL_KIND_VARIABLE, };
    _matcal_rules_add_token (rules, "[a-zA-Z]", -1, &tmp_err);
    throw (error, tmp_err);
    _matcal_rules_add_class (rules, "[a-zA-Z]", &klass, -1, &tmp_err);
    throw (error, tmp_err);
  }

  { /* numbers */
    SymbolClass klass = {SYMBOL_KIND_CONSTANT, };
    _matcal_rules_add_token (rules, "[0-9\\.]+", -1, &tmp_err);
    throw (error, tmp_err);
    _matcal_rules_add_class (rules, "[0-9\\.]+", &klass, -1, &tmp_err);
    throw (error, tmp_err);
  }

#undef throw
}

/**
 * matcal_rules_new_default: (constructor)
 * 
 * See return.
 * 
 * Returns: (transfer full): a #MatcalRules instance.
 */
MatcalRules*
matcal_rules_new_default ()
{
  MatcalRules* rules = NULL;
  GError* tmp_err = NULL;

  rules = matcal_rules_new ();
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
_matcal_rules_tokenize (MatcalRules* self, const gchar* input, gssize length, GError** error)
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
              _matcal_rules_tokenize (self, input + last, start - last, &tmp_err);
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
          _matcal_rules_tokenize (self, input + last, length - last, &tmp_err);
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
_matcal_rules_classify (MatcalRules* self, const gchar* input, GError** error)
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
          return & self->classes->e [i].klass;
        }
    }
return NULL;
}

/* lexer & classifier adders */

static inline void
_matcal_rules_tokenize_add (MatcalRules* self, GRegex* regex, gssize pos)
{
  pos = (pos == -1) ? self->tokenize->len : pos;
  g_ptr_array_insert ((gpointer) self->tokenize, pos, g_regex_ref (regex));
}

G_GNUC_INTERNAL
void
_matcal_rules_add_token (MatcalRules* self, const gchar* expr, gssize pos, GError** error)
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

  _matcal_rules_tokenize_add (self, regex, pos);
  _g_regex_unref0 (regex);
}

static inline void
_matcal_rules_classify_add (MatcalRules* self, GRegex* regex, const SymbolClass* klass, gssize pos)
{
  SymbolClassEntry entry = {0};
  pos = (pos == -1) ? self->classes->len : pos;
  entry.regex = g_regex_ref (regex);
  entry.klass = *klass;
  g_array_insert_vals ((gpointer) self->classes, pos, &entry, 1);
}

G_GNUC_INTERNAL
void
_matcal_rules_add_class (MatcalRules* self, const gchar* expr, const SymbolClass* klass, gssize pos, GError** error)
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

  _matcal_rules_classify_add (self, regex, klass, pos);
  _g_regex_unref0 (regex);
}

gboolean
matcal_rules_add_operator (MatcalRules* rules, const gchar* name, gboolean assoc, guint precedence, gboolean unary)
{
  g_return_val_if_fail (MATCAL_IS_RULES (rules), FALSE);
  g_return_val_if_fail (name != NULL, FALSE);
  MatcalRules* self = (rules);
  GError* tmp_err = NULL;

  const OperatorClass opclass = {assoc, precedence, unary};
  const SymbolClass klass = {SYMBOL_KIND_OPERATOR, .opclass = opclass};

  _matcal_rules_add_class (self, name, &klass, (self->fn_class++ - 1), &tmp_err);
  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_warning
      ("(%s): %s: %i: %s",
       G_STRLOC,
       g_quark_to_string
       (tmp_err->domain),
       tmp_err->code,
       tmp_err->message);
      _g_error_free0 (tmp_err);
      return FALSE;
    }
}

gboolean
matcal_rules_add_function (MatcalRules* rules, const gchar* name, guint n_args)
{
  g_return_val_if_fail (MATCAL_IS_RULES (rules), FALSE);
  g_return_val_if_fail (name != NULL, FALSE);
  MatcalRules* self = (rules);
  GError* tmp_err = NULL;

  const FunctionClass fnclass = {n_args};
  const SymbolClass klass = {SYMBOL_KIND_FUNCTION, .fnclass = fnclass};

  _matcal_rules_add_token (self, name, self->fn_token, &tmp_err);
  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_warning
      ("(%s): %s: %i: %s",
       G_STRLOC,
       g_quark_to_string
       (tmp_err->domain),
       tmp_err->code,
       tmp_err->message);
      _g_error_free0 (tmp_err);
      return FALSE;
    }

  _matcal_rules_add_class (self, name, &klass, self->fn_class, &tmp_err);
  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_warning
      ("(%s): %s: %i: %s",
       G_STRLOC,
       g_quark_to_string
       (tmp_err->domain),
       tmp_err->code,
       tmp_err->message);
      _g_error_free0 (tmp_err);
      return FALSE;
    }
}
