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
#include <core.h>
#include <expression.h>
#include <object.h>
#include <rules.h>

#define MATCAL_CORE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), MATCAL_TYPE_CORE, MatcalCoreClass))
#define MATCAL_IS_CORE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MATCAL_TYPE_CORE))
#define MATCAL_CORE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), MATCAL_TYPE_CORE, MatcalCoreClass))
typedef struct _MatcalCoreClass MatcalCoreClass;
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
#define _g_bytes_unref0(var) ((var == NULL) ? NULL : (var = (g_bytes_unref (var), NULL)))

#define MATCAL_TYPE_CLOSURE (matcal_closure_get_type ())
#define MATCAL_CLOSURE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), MATCAL_TYPE_CLOSURE, MatcalClosure))
#define MATCAL_CLOSURE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), MATCAL_TYPE_CLOSURE, MatcalClosureClass))
#define MATCAL_IS_CLOSURE(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MATCAL_TYPE_CLOSURE))
#define MATCAL_IS_CLOSURE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MATCAL_TYPE_CLOSURE))
#define MATCAL_CLOSURE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), MATCAL_TYPE_CLOSURE, MatcalClosureClass))

typedef struct _MatcalClosure MatcalClosure;
typedef struct _MatcalClosurePrivate MatcalClosurePrivate;
typedef struct _MatcalClosureClass MatcalClosureClass;

struct _MatcalCore
{
  GObject parent_instance;
  GHashTable* globals;
  MatcalRules* rules;
  MatcalObject* head;
  gint top;
};

struct _MatcalCoreClass
{
  GObjectClass parent_class;
};

struct _MatcalClosure
{
  MatcalObject parent_instance;
  MatcalCFunction callback;
  MatcalObject* upvalues;
  guint n_upvalues;
};

struct _MatcalClosureClass
{
  MatcalObjectClass parent_class;
};


G_DEFINE_FINAL_TYPE (MatcalCore, matcal_core, G_TYPE_OBJECT);
G_DEFINE_FINAL_TYPE (MatcalClosure, matcal_closure, MATCAL_TYPE_OBJECT);

static void
matcal_core_class_finalize (GObject* pself)
{
  MatcalCore* self = MATCAL_CORE (pself);
  g_hash_table_unref (self->globals);
G_OBJECT_CLASS (matcal_core_parent_class)->finalize (pself);
}

static void
matcal_core_class_dispose (GObject* pself)
{
  MatcalCore* self = MATCAL_CORE (pself);
  g_hash_table_remove_all (self->globals);
  _g_object_unref0 (self->rules);
  MatcalObject* head = self->head;
  while ((head = matcal_object_remove (head, head)));
G_OBJECT_CLASS (matcal_core_parent_class)->dispose (pself);
}

static void
matcal_core_class_init (MatcalCoreClass* klass)
{
  GObjectClass* oclass = G_OBJECT_CLASS (klass);

  oclass->finalize = matcal_core_class_finalize;
  oclass->dispose = matcal_core_class_dispose;
}

static void
matcal_core_init (MatcalCore* self)
{
  self->globals = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, matcal_object_unref);
  self->rules = matcal_rules_new_default ();
  self->head = NULL;
  self->top = 0;
}

static MatcalObject*
matcal_closure_class_clone (MatcalObject* pself)
{
  MatcalClosure* self = MATCAL_CLOSURE (pself);
  MatcalClosure* clone = matcal_object_new (MATCAL_TYPE_CLOSURE);
  MatcalObject* cloned = NULL;
  MatcalObject* object = NULL;
  MatcalObject* list = NULL;
  guint i, n_upvalues;

  clone->callback = self->callback;
  clone->n_upvalues = self->n_upvalues;
  n_upvalues = self->n_upvalues;
  object = self->upvalues;

  for (i = 0; i < n_upvalues; i++)
    {
      cloned = matcal_object_clone (object);
      list = matcal_object_prepend (list, cloned);
      object = matcal_object_next (object);
    }

  list = matcal_object_reverse (list);
  clone->upvalues = list;
return (MatcalObject*) clone;
}

static void
matcal_closure_class_finalize (MatcalObject* pself)
{
  MatcalClosure* self = MATCAL_CLOSURE (pself);
  MatcalObject* upvalues = self->upvalues;
  while (upvalues != NULL)
    {
      upvalues = matcal_object_remove (upvalues, upvalues);
    }
MATCAL_OBJECT_CLASS (matcal_closure_parent_class)->finalize (pself);
}

static void
matcal_closure_class_init (MatcalClosureClass* klass)
{
  MatcalObjectClass* oclass = MATCAL_OBJECT_CLASS (klass);
  oclass->finalize = matcal_closure_class_finalize;
  oclass->clone = matcal_closure_class_clone;
}

static void
matcal_closure_init (MatcalClosure* self)
{
  self->callback = NULL;
}

/* core API */

#ifdef validate_index
# undef validate_index
#endif // validate_index

#define validate_index(index) ((validate_index)(core->top,index))
#define reverse(index) (core->top-((index))-1)

static inline int
(validate_index) (gint top, gint index)
{
  if (index >= 0)
    {
      if (top > index)
        return index;
    }
  else
    {
      return top + index;
    }
return -1;
}

G_GNUC_INTERNAL
int
_matcal_core_checkidx (MatcalCore* core, int index)
{
  return validate_index (index);
}

G_GNUC_INTERNAL
void
_matcal_core_push (MatcalCore* core, gpointer object)
{
  g_return_if_fail (MATCAL_IS_OBJECT (object));
  core->head = matcal_object_prepend (core->head, (MatcalObject*) object);
  ++core->top;
}

G_GNUC_INTERNAL
void
_matcal_core_pop (MatcalCore* core)
{
  core->head = matcal_object_remove (core->head, core->head);
  --core->top;
}

G_GNUC_INTERNAL
gpointer
_matcal_core_peek (MatcalCore* core, gint index)
{
  g_return_val_if_fail ((index = validate_index (index)) >= 0, NULL);
#if DEBUG == 1
  MatcalObject* object = matcal_object_nth (core->head, reverse (index));
  g_assert (object != NULL);
return object;
#else
return math_object_nth (core->head, reverse (index));
#endif // DEBUG
}

G_GNUC_INTERNAL
gpointer
_matcal_core_peek_upvalue (MatcalCore* core, gint index)
{
  MatcalObject* base = NULL;

  if (core->top == 0)
    {
      if (core->head == NULL)
        {
          _matcal_core_peek (core, 0);
          return NULL;
        }

      base = core->head;
    }
  else
    {
      base = _matcal_core_peek (core, 0);
      base = matcal_object_next (base);
    }

  g_return_val_if_fail (MATCAL_IS_CLOSURE (base), NULL);
  MatcalClosure* closure = (MatcalClosure*) base;
  g_return_val_if_fail ((index = (validate_index) (closure->n_upvalues, index)) >= 0, NULL);
return matcal_object_nth (closure->upvalues, index);
}

/* public API */

MatcalCore*
matcal_core_new ()
{
  return (MatcalCore*)
  g_object_new (MATCAL_TYPE_CORE, NULL);
}

/**
 * matcal_core_gettop:
 * @core: #MatcalCore instance.
 * 
 * Gets top of @core's stack.
 * 
 * Returns: stack top index.
 */
int
matcal_core_gettop (MatcalCore* core)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), -1);
return core->top;
}

/**
 * matcal_core_settop:
 * @core: #MatcalCore instance.
 * @newtop: new top value.
 * 
 * Sets top of @core's stack.
 */
void
matcal_core_settop (MatcalCore* core, int newtop)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  g_return_if_fail (newtop >= 0);
  int i, top = matcal_core_gettop (core);

  if (newtop > top)
    {
      int extra = newtop - top;
      for (i = 0; i < extra; i++)
      {
        matcal_core_pushnil (core);
      }
    }
  else
    {
      int left = top - newtop;
      for (i = 0; i < left; i++)
      {
        _matcal_core_pop (core);
      }
    }
}

/**
 * matcal_core_setglobal:
 * @core: #MatcalCore instance.
 * @name: global object's name.
 * 
 * Sets element at top of stack as
 * global named by @name.
 */
void
matcal_core_setglobal (MatcalCore* core, const gchar* name)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  g_return_if_fail (matcal_core_gettop (core) >= 1);
  g_return_if_fail (name != NULL);
  MatcalCore* self = (core);
  MatcalObject* object;

  object = _matcal_core_peek (core, -1);
  object = matcal_object_ref (object);
  matcal_core_pop (core, 1);

  g_hash_table_insert (self->globals, g_strdup (name), object);
}

/**
 * matcal_core_getglobal:
 * @core: #MatcalCore instance.
 * @name: global object's name.
 * 
 * Gets global object named by @name
 * and pushes it onto stack if exists,
 * otherwise pushes nil.
 */
void
matcal_core_getglobal (MatcalCore* core, const gchar* name)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  g_return_if_fail (name != NULL);
  MatcalCore* self = (core);
  MatcalObject* object;
  gboolean has;

  has =
  g_hash_table_lookup_extended (self->globals, name, NULL, (gpointer*) &object);
  if (!has)
    matcal_core_pushnil (core);
  else
    {
      object = matcal_object_clone (object);
      _matcal_core_push (core, object);
    }
}

/**
 * matcal_core_pushvalue:
 * @core: #MatcalCore instance.
 * @index: value's index.
 *
 * Pushes onto stack the value at
 * index @index.
 */
void
matcal_core_pushvalue (MatcalCore* core, int index)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  g_return_if_fail ((index = validate_index (index)) >= 0);
  MatcalObject* object = NULL;

  object = _matcal_core_peek (core, index);
  object = matcal_object_clone (object);
  _matcal_core_push (core, object);
}

/**
 * matcal_core_pushupvalue:
 * @core: #MatcalCore instance.
 * @index: upvalue's index.
 *
 * Pushes onto stack the upvalue at
 * index @index. This only works inside
 * a closure function.
 */
void
matcal_core_pushupvalue (MatcalCore* core, int index)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  MatcalObject* object = NULL;

  if ((object = _matcal_core_peek_upvalue (core, index)) == NULL)
    return;

  object = matcal_object_clone (object);
  _matcal_core_push (core, object);
}

/**
 * matcal_core_pop:
 * @core: #MatcalCore instance.
 * @n_values: number of values to pop from.
 *
 * Pops @n_values elements from stack.
 */
void
matcal_core_pop (MatcalCore* core, int n_values)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  g_return_if_fail (core->top >= n_values);
  matcal_core_settop (core, core->top - n_values);
}

/**
 * matcal_core_remove:
 * @core: #MatcalCore instance.
 * @index: which index to remove at.
 *
 * Removes value at index @index.
 */
void
matcal_core_remove (MatcalCore* core, int index)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  g_return_if_fail ((index = validate_index (index)) >= 0);
  MatcalObject* nth = NULL;

  nth = _matcal_core_peek (core, index);
  core->head = matcal_object_remove (core->head, nth);
  --core->top;
}

/**
 * matcal_core_insert:
 * @core: #MatcalCore instance.
 * @index: which index to insert at.
 *
 * Inserts value at top of stack onto index @index.
 */
void
matcal_core_insert (MatcalCore* core, int index)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  g_return_if_fail ((index = validate_index (index)) >= 0);
  g_return_if_fail (core->top > 0);
  MatcalObject* object = NULL;

  object = matcal_object_nth (core->head, 0 /* top */);
  object = matcal_object_ref (object);
  core->head = matcal_object_remove (core->head, object);
  core->head = matcal_object_insert (core->head, object, reverse (index));
  matcal_object_unref (object);
}

/* nil API */

/**
 * matcal_core_pushnil:
 * @core: #MatcalCore instance.
 * 
 * Pushes onto stack a nil value.
 */
void
matcal_core_pushnil (MatcalCore* core)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  MatcalNil* nil = matcal_object_new (MATCAL_TYPE_NIL);
  _matcal_core_push (core, nil);
  matcal_object_unref (nil);
}

/**
 * matcal_core_isnil:
 * @core: #MatcalCore instance.
 * @index: index to inspect on.
 *
 * See return section.
 * 
 * Returns: if @index is nil.
 */
gboolean
matcal_core_isnil (MatcalCore* core, int index)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), FALSE);
  g_return_val_if_fail ((index = validate_index (index)) >= 0, FALSE);
  MatcalObject* value = _matcal_core_peek (core, index);
return MATCAL_IS_NIL (value);
}

/**
 * matcal_core_isnone:
 * @core: #MatcalCore instance.
 * @index: index to inspect on.
 *
 * See return section.
 * 
 * Returns: if @index don't exists.
 */
gboolean
matcal_core_isnone (MatcalCore* core, int index)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), FALSE);
return validate_index (index) < 0;
}

/**
 * matcal_core_pushcfunction:
 * @core: #MatcalCore instance.
 * @cclosure: (scope call): C-style closure to call.
 *
 * Pushes @cclosure on @core.
 *
 */
void
matcal_core_pushcfunction (MatcalCore* core, MatcalCFunction cclosure)
{
  g_return_if_fail (MATCAL_CORE (core));
  g_return_if_fail (cclosure != NULL);
  MatcalClosure* closure = NULL;

  closure = matcal_object_new (MATCAL_TYPE_CLOSURE);
  closure->callback = cclosure;
  _matcal_core_push (core, closure);
  matcal_object_unref (closure);
}

/**
 * matcal_core_pushclosure:
 * @core: #MatcalCore instance.
 * @cclosure: (scope call): C-style closure to call.
 * @n_upvalues: closure values taken from stack.
 *
 * Similar in function to @matcal_core_pushcfunction but
 * it also takes @n_upvalues elements from stack at stores
 * them as closure upvalues (pops it from stack).
 *
 */
void
matcal_core_pushclosure (MatcalCore* core, MatcalCFunction cclosure, guint n_upvalues)
{
  g_return_if_fail (MATCAL_CORE (core));
  g_return_if_fail (cclosure != NULL);
  g_return_if_fail (matcal_core_gettop (core) >= n_upvalues);
  MatcalClosure* closure = NULL;
  MatcalObject* object = NULL;
  MatcalObject* list = NULL;
  guint i;

  closure = matcal_object_new (MATCAL_TYPE_CLOSURE);
  closure->callback = cclosure;

  for (i = 0; i < n_upvalues; i++)
    {
      object = _matcal_core_peek (core, -1);
      object = matcal_object_ref (object);
      _matcal_core_pop (core);

      list = matcal_object_prepend (list, object);
    }

  closure->n_upvalues = n_upvalues;
  closure->upvalues = list;

  _matcal_core_push (core, closure);
  matcal_object_unref (closure);
}

/**
 * matcal_core_isfunction:
 * @core: #MatcalCore instance.
 * @index: index to inspect on.
 * 
 * See return.
 * 
 * Returns: whether object at @index is a function.
 */
gboolean
matcal_core_isfunction (MatcalCore* core, int index)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), FALSE);
  g_return_val_if_fail ((index = validate_index (index)) >= 0, FALSE);
  MatcalObject* value = _matcal_core_peek (core, index);
return MATCAL_IS_CLOSURE (value);
}

/**
 * matcal_core_call:
 * @core: #MatcalCore instance.
 * @n_args: arguments to pass to.
 * @n_results: total number of returned values.
 *
 * Performs a call onto @core. Arguments are pushed in
 * natural order: function is pushed first, then the arguments.
 *
 * Returns: if call was successful.
 */
MatcalClosureResult
matcal_core_call (MatcalCore* core, int n_args, int n_results)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), MATCAL_CLOSURE_ERROR);
  g_return_val_if_fail (n_args >= 0, MATCAL_CLOSURE_ERROR);
  g_return_val_if_fail (matcal_core_gettop (core) >= (n_args + 1), MATCAL_CLOSURE_ERROR);
  g_return_val_if_fail (n_results >= 0 || n_results == MATCAL_CLOSURE_MULTIRET, MATCAL_CLOSURE_ERROR);
  MatcalClosure* closure = _matcal_core_peek (core, -n_args-1);
  g_return_val_if_fail (MATCAL_IS_CLOSURE (closure), MATCAL_CLOSURE_ERROR);
  int top, oldindex, result;

  oldindex = core->top - (n_args + 1);
  core->top = n_args;

  {
    result = closure->callback (core);

    ++core->top;
    matcal_core_remove (core, 0);

    if (result < 0)
      matcal_core_settop (core, 0);
    else
      {
        top = matcal_core_gettop (core);
        if (result >= top)
          matcal_core_settop (core, result);
        else while (top-- > result)
          matcal_core_remove (core, 0);

        if (n_results == MATCAL_CLOSURE_MULTIRET)
          oldindex += result;
        else
          {
            matcal_core_settop (core, n_results);
            oldindex += n_results;
          }
      }
  }

  core->top = oldindex;
return (result < 0) ? MATCAL_CLOSURE_ERROR : MATCAL_CLOSURE_SUCCESS;
}

/**
 * matcal_core_loadbytes:
 * @core: a #MatcalCore instance.
 * @code: a #GBytes instance containing mathematical source.
 * @error: return location for #GError.
 * 
 * Compiles code contained in @code and pushes it onto stack.
 * 
 * Returns: if code was compiled successfully.
 */
gboolean
matcal_core_loadbytes (MatcalCore* core, GBytes* code, GError** error)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), FALSE);
  g_return_val_if_fail (code != NULL, FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);
  MatcalCore* self = (core);
  GError* tmp_err = NULL;
  MatcalExpression* exp;

  exp =
  matcal_expression_new (self->rules, code, &tmp_err);
  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_propagate_error (error, tmp_err);
      _g_object_unref0 (exp);
      return FALSE;
    }

  matcal_expression_compile (exp, &tmp_err);
  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_propagate_error (error, tmp_err);
      _g_object_unref0 (exp);
      return FALSE;
    }

  matcal_expression_push (exp, core);
  _g_object_unref0 (exp);
}

/**
 * matcal_core_loadstring:
 * @core: a #MatcalCore instance.
 * @code: mathematical source.
 * @error: return location for #GError.
 *
 * Same as matcal_core_loadbytes but compiles
 * a C string instead.
 *
 * Returns: if code was compiled successfully.
 */
gboolean
matcal_core_loadstring (MatcalCore* core, const gchar* code, GError** error)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), FALSE);
  g_return_val_if_fail (code != NULL, FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);
  GBytes* bytes = g_bytes_new_static (code, strlen (code));
  gboolean result = FALSE;

  result =
  matcal_core_loadbytes (core, bytes, error);
  _g_bytes_unref0 (bytes);
return result;
}
