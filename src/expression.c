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
#include <expression.h>
#include <gio/gio.h>
#include <tokens.h>

static void
matree_expression_g_initable_iface (GInitableIface* iface);

#define MATREE_EXPRESSION_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), MATREE_TYPE_EXPRESSION, MatreeExpressionClass))
#define MATREE_IS_EXPRESSION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MATREE_TYPE_EXPRESSION))
#define MATREE_EXPRESSION_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), MATREE_TYPE_EXPRESSION, MatreeExpressionClass))
typedef struct _MatreeExpressionClass MatreeExpressionClass;
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
#define _g_error_free0(var) ((var == NULL) ? NULL : (var = (g_error_free (var), NULL)))
#define _g_strfreev0(var) ((var == NULL) ? NULL : (var = (g_strfreev (var), NULL)))
#define _g_free0(var) ((var == NULL) ? NULL : (var = (g_free (var), NULL)))

struct _MatreeExpression
{
  GObject parent;

  GNode* ast;
  gchar* infix;

  MatreeTokens* tokens;
};

struct _MatreeExpressionClass
{
  GObjectClass parent;
};

enum
{
  prop_0,
  prop_infix,
  prop_number,
};

static
GParamSpec* properties [prop_number] = {0};

G_DEFINE_FINAL_TYPE_WITH_CODE
(MatreeExpression,
 matree_expression,
 G_TYPE_OBJECT,
 G_IMPLEMENT_INTERFACE
 (G_TYPE_INITABLE,
  matree_expression_g_initable_iface));

G_DEFINE_QUARK
(matree-expression-error-quark,
 matree_expression_error);

static void
emit_token (MatreeExpression* self, const gchar* token)
{
  g_print ("token '%s'\r\n", token);
}

static gboolean
matree_expression_g_initable_iface_init (GInitable* pself, GCancellable* cancellable, GError** error)
{
  MatreeExpression* self = NULL;
  GScanner* scanner = NULL;
  const gchar* rawtoken = NULL;
  const gchar* input = NULL;
  const gchar* end = NULL;
  gchar** tokens = NULL;
  gchar store[2] = {0};
  gsize length = 0;
  GTokenType type;

  self = MATREE_EXPRESSION (pself);
  input = self->infix;

  const GScannerConfig config =
  {
    "\x20\t\n\r",                             /* cset_skip_characters */
    G_CSET_a_2_z G_CSET_A_2_Z G_CSET_DIGITS,  /* cset_identifier_first */
    G_CSET_a_2_z G_CSET_A_2_Z G_CSET_DIGITS,  /* cset_identifier_nth */
    "#\n",                                    /* cpair_comment_single */
    TRUE,   /* case_sensitive */
    TRUE,   /* skip_comment_multi */
    TRUE,   /* skip_comment_single */
    FALSE,  /* scan_comment_multi */
    TRUE,   /* scan_identifier */
    TRUE,   /* scan_identifier_1char */
    FALSE,  /* scan_identifier_NULL */
    FALSE,  /* scan_symbols */
    FALSE,  /* scan_binary */
    FALSE,  /* scan_octal */
    FALSE,  /* scan_float */
    FALSE,  /* scan_hex */
    FALSE,  /* scan_hex_dollar */
    FALSE,  /* scan_string_sq */
    FALSE,  /* scan_string_dq */
    FALSE,  /* numbers_2_int */
    FALSE,  /* int_2_float */
    FALSE,  /* identifier_2_string */
    FALSE,  /* char_2_token */
    FALSE,  /* symbol_2_token */
    FALSE,  /* scope_0_fallback */
    FALSE,  /* store_int64 */
  };

  if (!g_utf8_validate (input, -1, &end))
    {
      g_set_error_literal
      (error,
       MATREE_EXPRESSION_ERROR,
       MATREE_EXPRESSION_ERROR_NOT_UTF8,
       "invalid formatted input");
      return FALSE;
    }

  length = end - input;
  scanner = g_scanner_new (&config);

  g_scanner_input_text (scanner, input, length);

  do
  {
    type = g_scanner_get_next_token (scanner);

    switch (type)
    {
    case G_TOKEN_EOF:
      break;
    case G_TOKEN_ERROR:
      g_set_error
      (error,
       MATREE_EXPRESSION_ERROR,
       MATREE_EXPRESSION_ERROR_SCANNER,
       "%i:%i: g_scanner_get_next_token()!: crashed",
       g_scanner_cur_line (scanner),
       g_scanner_cur_position (scanner));
      g_scanner_destroy (scanner);
      return FALSE;

    case G_TOKEN_CHAR:
      store[0] = scanner->value.v_char;
      rawtoken = (gchar*) store;
      goto makein;
    case G_TOKEN_IDENTIFIER:
      rawtoken = scanner->value.v_identifier;
      goto makein;

    makein:
      {
        GError* tmp_err = NULL;
        gchar* token = NULL;
        guint i = 0;

        tokens = matree_tokens_split (self->tokens, rawtoken, -1, &tmp_err);
        if (G_UNLIKELY (tmp_err != NULL))
          {
            g_propagate_error (error, tmp_err);
            g_scanner_destroy (scanner);
            _g_strfreev0 (tokens);
            return FALSE;
          }

        while ((token = tokens[i++]) != NULL)
          emit_token (self, token);
        _g_strfreev0 (tokens);
      }
      break;

    default:
      g_set_error
      (error,
       MATREE_EXPRESSION_ERROR,
       MATREE_EXPRESSION_ERROR_SCANNER,
       "%i:%i: g_scanner_get_next_token()!: uknown token %u",
       g_scanner_cur_line (scanner),
       g_scanner_cur_position (scanner),
       (guint) type);
      g_scanner_destroy (scanner);
      return FALSE;
    }
  } while (type != G_TOKEN_EOF);

  g_scanner_destroy (scanner);
return TRUE;
}

static void
matree_expression_g_initable_iface (GInitableIface* iface)
{
  iface->init = matree_expression_g_initable_iface_init;
}

static void
matree_expression_class_finalize (GObject* pself)
{
  MatreeExpression* self = MATREE_EXPRESSION (pself);
G_OBJECT_CLASS (matree_expression_parent_class)->finalize (pself);
}

static void
matree_expression_class_dispose (GObject* pself)
{
  MatreeExpression* self = MATREE_EXPRESSION (pself);
  _g_object_unref0 (self->tokens);
G_OBJECT_CLASS (matree_expression_parent_class)->dispose (pself);
}

static void
matree_expression_class_set_property (GObject* pself, guint prop_id, const GValue* value, GParamSpec* pspec)
{
  MatreeExpression* self = MATREE_EXPRESSION (pself);
  switch (prop_id)
  {
  case prop_infix:
    _g_free0 (self->infix);
    self->infix = g_value_dup_string (value);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (pself, prop_id, pspec);
    break;
  }
}

static void
matree_expression_class_get_property (GObject* pself, guint prop_id, GValue* value, GParamSpec* pspec)
{
  MatreeExpression* self = MATREE_EXPRESSION (pself);
  switch (prop_id)
  {
  case prop_infix:
    g_value_set_string (value, matree_expression_get_expression (self));
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (pself, prop_id, pspec);
    break;
  }
}

static void
matree_expression_class_init (MatreeExpressionClass* klass)
{
  GObjectClass* oclass = G_OBJECT_CLASS (klass);

  oclass->set_property = matree_expression_class_set_property;
  oclass->get_property = matree_expression_class_get_property;
  oclass->finalize = matree_expression_class_finalize;
  oclass->dispose = matree_expression_class_dispose;

  properties [prop_infix] = g_param_spec_string ("infix", "infix", "infix", NULL, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS);
  g_object_class_install_properties (G_OBJECT_CLASS (klass), prop_number, properties);
}

static void
matree_expression_init (MatreeExpression* self)
{
  self->tokens = matree_tokens_get_default ();
}

/**
 * matree_expression_new: (constructor)
 *
 * Creates a void instance of #MatreeTokenizer
 *
 * Returns: (transfer full): a new #MatreeTokenizer instance.
 */
MatreeExpression*
matree_expression_new (const gchar* infix, GError** error)
{
  return
  g_initable_new
  (MATREE_TYPE_EXPRESSION,
   g_cancellable_get_current (),
   error,
   "infix", infix,
   NULL);
}

const gchar*
matree_expression_get_expression (MatreeExpression* expression)
{
  g_return_val_if_fail (MATREE_IS_EXPRESSION (expression), NULL);
return expression->infix;
}
