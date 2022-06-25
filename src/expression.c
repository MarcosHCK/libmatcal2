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
#include <corexpr.h>
#include <compiler.h>
#include <expression.h>
#include <gio/gio.h>
#include <rulesext.h>

static void
matree_expression_g_initable_iface (GInitableIface* iface);

#define MATREE_EXPRESSION_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), MATREE_TYPE_EXPRESSION, MatreeExpressionClass))
#define MATREE_IS_EXPRESSION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MATREE_TYPE_EXPRESSION))
#define MATREE_EXPRESSION_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), MATREE_TYPE_EXPRESSION, MatreeExpressionClass))
typedef struct _MatreeExpressionClass MatreeExpressionClass;
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
#define _ast_node_free0(var) ((var == NULL) ? NULL : (var = (ast_node_free (var), NULL)))
#define _g_error_free0(var) ((var == NULL) ? NULL : (var = (g_error_free (var), NULL)))
#define _libjit_free0(var) ((var == NULL) ? NULL : (var = (libjit_free (var), NULL)))
#define _g_strfreev0(var) ((var == NULL) ? NULL : (var = (g_strfreev (var), NULL)))
#define _g_free0(var) ((var == NULL) ? NULL : (var = (g_free (var), NULL)))
typedef union _Symbol Symbol;

struct _MatreeExpression
{
  GObject parent;

  AstNode* ast;
  JitCallback* callback;

  gchar* infix;
  MatreeRules* rules;
};

struct _MatreeExpressionClass
{
  GObjectClass parent;
};

#define KIND_BITS (4)
#define INDEX_BITS ((GLIB_SIZEOF_VOID_P * 8) - KIND_BITS)

union _Symbol
{
  gpointer ptr;

  struct
  {
    guintptr index : INDEX_BITS;
    guintptr kind : KIND_BITS;
  };
};

G_STATIC_ASSERT (sizeof (Symbol) == sizeof (gpointer));

enum
{
  prop_0,
  prop_rules,
  prop_infix,
  prop_ast,
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

static Symbol
_symbol_make (gint index, SymbolKind kind)
{
  Symbol sym;
#if DEVELOPER
  g_assert (((1ll << INDEX_BITS) - 1) >= (gint) index);
  g_assert (((1ll << KIND_BITS) - 1) >= (gint) kind);
#endif // DEVELOPER
  sym.index = (gint) index;
  sym.kind = (gint) kind;
return sym;
}

static gint
calculate_offset (gchar** tokens, guint til)
{
  gchar* token = NULL;
  gsize offset = 0;
  guint i = 0;

  while ((token = tokens[i++]) != NULL)
    {
      offset += g_utf8_strlen (token, -1);
      if (i - 1 >= til)
        break;
    }
return offset;
}

static SymbolClass*
matree_expression_parse_tokens (MatreeExpression* self, GQueue* output, GQueue* operators, gchar** tokens, GError** error)
{
  GError* tmp_err = NULL;
  SymbolClass* klass = NULL;
  SymbolClass* classes = NULL;
  gchar* token = NULL;
  guint i = 0;
  Symbol sym;

  classes = g_new (SymbolClass, g_strv_length (tokens));

  while ((token = tokens [i++]) != NULL)
    {
      klass = _matree_rules_classify (self->rules, token, &tmp_err);
      if (G_UNLIKELY (tmp_err != NULL))
        {
          g_propagate_error (error, tmp_err);
          _g_free0 (classes);
          return NULL;
        }
      else
        {
          if (klass == NULL)
            continue;
          else
            {
              classes[i - 1] = *klass;
            }
        }

      switch (klass->kind)
      {
      case SYMBOL_KIND_CONSTANT:
      case SYMBOL_KIND_VARIABLE:
        sym = _symbol_make (i - 1, klass->kind);
        g_queue_push_head (output, sym.ptr);
        break;
      case SYMBOL_KIND_FUNCTION:
        sym = _symbol_make (i - 1, klass->kind);
        g_queue_push_head (operators, sym.ptr);
        break;
      case SYMBOL_KIND_PARENTHESIS:
        if (token[0] == '(')
          {
            sym = _symbol_make (i - 1, klass->kind);
            g_queue_push_head (operators, sym.ptr);
          } else
        if (token[0] == ')')
        while (TRUE)
          {
            sym = (Symbol) g_queue_pop_head (operators);
            if (sym.ptr == NULL)
              {
                g_set_error
                (error,
                 MATREE_EXPRESSION_ERROR,
                 MATREE_EXPRESSION_ERROR_UNMATCHED_PARENTHESIS,
                 "%i: unmatched ')' parenthesis",
                 calculate_offset (tokens, i - 1));
                _g_free0 (classes);
                return NULL;
              }

            if (sym.kind != SYMBOL_KIND_PARENTHESIS)
              g_queue_push_head (output, sym.ptr);
            else
              {
                sym = (Symbol) g_queue_peek_head (operators);
                if (sym.ptr != NULL && sym.kind == SYMBOL_KIND_FUNCTION)
                  {
                    sym = (Symbol) g_queue_pop_head (operators);
                    g_queue_push_head (output, sym.ptr);
                  }

                break;
              }
          }

        #if DEVELOPER
        else g_assert_not_reached ();
        #endif // DEVELOPER
        break;
      case SYMBOL_KIND_OPERATOR:
        do
        {
          sym = (Symbol) g_queue_peek_head (operators);
          if (sym.ptr == NULL)
            break;

          if (sym.kind != SYMBOL_KIND_PARENTHESIS)
            {
              OperatorClass* opclass1 = & klass->opclass;
              OperatorClass* opclass2 = & classes [sym.index].opclass;
              if ((opclass2->precedence > opclass1->precedence)
                || (opclass2->precedence == opclass1->precedence)
                && (opclass1->assoc == OPERATOR_ASSOC_LEFT))
                {
                  sym = (Symbol) g_queue_pop_head (operators);
                  g_queue_push_head (output, sym.ptr);
                  continue;
                }
            }
        } while (FALSE);

        sym = _symbol_make (i - 1, klass->kind);
        g_queue_push_head (operators, sym.ptr);
        break;
      }
    }

  while ((sym = (Symbol) g_queue_pop_head (operators)).ptr)
    {
      if (sym.kind == SYMBOL_KIND_PARENTHESIS)
        {
          g_set_error
          (error,
           MATREE_EXPRESSION_ERROR,
           MATREE_EXPRESSION_ERROR_UNMATCHED_PARENTHESIS,
           "%i: unmatched '(' parenthesis",
           calculate_offset (tokens, sym.index));
          _g_free0 (classes);
          return NULL;
        }

      g_queue_push_head (output, sym.ptr);
    }
return classes;
}

static AstNode*
matree_expression_build_tree (MatreeExpression* self, GQueue* output, gchar** tokens, SymbolClass* classes, GError** error)
{
  AstNode* node = NULL;
  AstNode* tree = NULL;
  SymbolClass* klass;
  gchar* token;
  OperatorClass* opclass;
  FunctionClass* fnclass;
  GQueue nodes;
  guint i, top;
  Symbol sym;

  g_queue_init (&nodes);
#define nodes (&nodes)

  while ((sym = (Symbol) g_queue_pop_tail (output)).ptr)
    {
      klass = & classes [sym.index];
      token = tokens [sym.index];
      switch (sym.kind)
      {
      case SYMBOL_KIND_CONSTANT:
        tree = ast_node_new (token, AST_SYMBOL_CONSTANT);
        goto pushplain;
      case SYMBOL_KIND_VARIABLE:
        tree = ast_node_new (token, AST_SYMBOL_VARIABLE);
        goto pushplain;
      case SYMBOL_KIND_OPERATOR:
        opclass = & klass->opclass;
        top = (opclass->unary) ? 1 : 2;
        goto pushargs;
      case SYMBOL_KIND_FUNCTION:
        fnclass = & klass->fnclass;
        top = fnclass->n_args;
        goto pushargs;

      pushargs:
        tree = ast_node_new (token, AST_SYMBOL_FUNCTION);
        for (i = 0; i < top; i++)
          {
            node = g_queue_pop_head (nodes);
            g_node_prepend ((gpointer) tree, (gpointer) node);
          }

      pushplain:
        g_queue_push_head (nodes, tree);
        break;
      }
    }

  tree =
  g_queue_pop_tail (nodes);
#undef nodes
  g_queue_clear_full
  (&nodes,
   (GDestroyNotify)
   ast_node_destroy);
return tree;
}

static gboolean
matree_expression_g_initable_iface_init (GInitable* pself, GCancellable* cancellable, GError** error)
{
  MatreeExpression* self = NULL;
  MatreeRules* rules = NULL;
  GError* tmp_err = NULL;

  SymbolClass* classes = NULL;
  AstNode* tree = NULL;

  const gchar* input = NULL;
  const gchar* end = NULL;
  gchar** tokens = NULL;
  gchar* token = NULL;
  gsize length = 0;

  GQueue output;
  GQueue operators;

  self = MATREE_EXPRESSION (pself);
  rules = MATREE_RULES (self->rules);
  input = self->infix;

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
  tokens = _matree_rules_tokenize (rules, input, length, &tmp_err);
  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_propagate_error (error, tmp_err);
      _g_strfreev0 (tokens);
      return FALSE;
    }

  g_queue_init (&output);
  g_queue_init (&operators);

  classes =
  matree_expression_parse_tokens (self, &output, &operators, tokens, &tmp_err);
  g_queue_clear (&operators);

  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_propagate_error (error, tmp_err);
      g_queue_clear (&output);
      _g_strfreev0 (tokens);
      _g_free0 (classes);
      return FALSE;
    }

  tree =
  matree_expression_build_tree (self, &output, tokens, classes, &tmp_err);
  g_queue_clear (&output);
  _g_strfreev0 (tokens);
  _g_free0 (classes);

  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_propagate_error (error, tmp_err);
      _ast_node_free0 (tree);
      return FALSE;
    }

  self->ast = tree;
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
  _libjit_free0 (self->callback);
  _ast_node_free0 (self->ast);
  _g_free0 (self->infix);
G_OBJECT_CLASS (matree_expression_parent_class)->finalize (pself);
}

static void
matree_expression_class_dispose (GObject* pself)
{
  MatreeExpression* self = MATREE_EXPRESSION (pself);
  _g_object_unref0 (self->rules);
G_OBJECT_CLASS (matree_expression_parent_class)->dispose (pself);
}

static void
matree_expression_class_set_property (GObject* pself, guint prop_id, const GValue* value, GParamSpec* pspec)
{
  MatreeExpression* self = MATREE_EXPRESSION (pself);
  switch (prop_id)
  {
  case prop_rules:
    g_set_object (& self->rules, g_value_get_object (value));
    break;
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
  case prop_rules:
    g_value_set_object (value, matree_expression_get_rules (self));
    break;
  case prop_infix:
    g_value_set_string (value, matree_expression_get_infix (self));
    break;
  case prop_ast:
    g_value_set_pointer (value, matree_expression_get_ast (self));
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

  properties [prop_rules] = g_param_spec_object ("rules", "rules", "rules", MATREE_TYPE_RULES, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS);
  properties [prop_infix] = g_param_spec_string ("infix", "infix", "infix", NULL, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS);
  properties [prop_ast] = g_param_spec_pointer ("ast", "ast", "ast", G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);
  g_object_class_install_properties (G_OBJECT_CLASS (klass), prop_number, properties);
}

static void
matree_expression_init (MatreeExpression* self)
{
}

/**
 * matree_expression_new: (constructor)
 *
 * Creates a void instance of #MatreeTokenizer
 *
 * Returns: (transfer full): a new #MatreeTokenizer instance.
 */
MatreeExpression*
matree_expression_new (MatreeRules* rules, const gchar* infix, GError** error)
{
  return
  g_initable_new
  (MATREE_TYPE_EXPRESSION,
   g_cancellable_get_current (),
   error,
   "rules", rules,
   "infix", infix,
   NULL);
}

/**
 * matree_expression_get_rules:
 * Returns: (transfer none):
 */
MatreeRules*
matree_expression_get_rules (MatreeExpression* expression)
{
  g_return_val_if_fail (MATREE_IS_EXPRESSION (expression), NULL);
return expression->rules;
}

/**
 * matree_expression_get_infix:
 * Returns: (transfer none):
 */
const gchar*
matree_expression_get_infix (MatreeExpression* expression)
{
  g_return_val_if_fail (MATREE_IS_EXPRESSION (expression), NULL);
return expression->infix;
}

/**
 * matree_expression_get_ast:
 * Returns: (transfer none):
 */
AstNode*
matree_expression_get_ast (MatreeExpression* expression)
{
  g_return_val_if_fail (MATREE_IS_EXPRESSION (expression), NULL);
return expression->ast;
}

/**
 * matree_expression_compile:
 * @expression: #MatreeExpression instance.
 * @error: return location for a #GError.
 *
 * Compiles an expression into native code, thus
 * sets #MatreeExpression::cclosure property.
 *
 * Returns: wether compilation was successful.
 */
gboolean
matree_expression_compile (MatreeExpression* expression, GError** error)
{
  g_return_val_if_fail (MATREE_IS_EXPRESSION (expression), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);
  JitCallback* callback = NULL;
  GError* tmp_err = NULL;

  callback = libjit_compile (expression->ast, &tmp_err);
  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_propagate_error (error, tmp_err);
      _libjit_free0 (callback);
      return FALSE;
    }

  expression->callback = callback;
return TRUE;
}

/**
 * matree_expression_push:
 * @expression: #MatreeExpression instance.
 * @core: #MatcalCore instance.
 * 
 * Pushes @expression as a function into @core stack.
 * You should previously called @matree_expression_compile
 * on @expression to generate said function or otherwise it
 * will push nothing.
 */
void
matree_expression_push (MatreeExpression* expression, MatcalCore* core)
{
  g_return_if_fail (MATREE_IS_EXPRESSION (expression));
  g_return_if_fail (MATCAL_IS_CORE (core));
  MatreeExpression* self = (expression);

  g_return_if_fail (self->callback);
  matcal_core_pushcfunction (core, self->callback->start);
}
