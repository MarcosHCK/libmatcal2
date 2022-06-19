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
#include <clonable.h>
#include <core.h>
#include <object.h>

#define MATH_CORE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), MATH_TYPE_CORE, MatcalCoreClass))
#define MATH_IS_CORE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MATH_TYPE_CORE))
#define MATH_CORE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), MATH_TYPE_CORE, MatcalCoreClass))

typedef struct _MatcalCoreClass MatcalCoreClass;

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
  MatcalObject* head = self->head;
  while (head != NULL)
    {
      head = matcal_object_remove (head, head);
    }
G_OBJECT_CLASS (matcal_core_parent_class)->finalize (pself);
}

static void
matcal_core_class_init (MatcalCoreClass* klass)
{
  GObjectClass* oclass = G_OBJECT_CLASS (klass);
  oclass->finalize = matcal_core_class_finalize;
}

static void
matcal_core_init (MatcalCore* self)
{
  self->head = NULL;
  self->top = 0;
}

static void
matcal_closure_class_init (MatcalClosureClass* klass)
{
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

#define validate_index(index) ((validate_index)(core,index))
#define reverse(index) (core->top-((index))-1)

static inline int
(validate_index) (MatcalCore* core, gint index)
{
  if (index >= 0)
    {
      if (core->top > index)
        return index;
    }
  else
    {
      return core->top + index;
    }
return -1;
}

G_GNUC_INTERNAL
int
_matcal_core_checkidx (MatcalCore* core, int index)
{
  return (validate_index) (core, index); 
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
  if (MATCAL_IS_CLONABLE (object))
    {
      object = matcal_clonable_clone (object);
      _matcal_core_push (core, object);
    }
  else
    {
      /* Push a new reference */
_matcal_core_push(core, object);
    }
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
 * @cclosure: C-style closure to call.
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
  core->top = n_args + 1;

  matcal_object_ref (closure);
  matcal_core_remove (core, 0);

  {
    result = closure->callback (core);
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
  matcal_object_unref (closure);
return (result < 0) ? MATCAL_CLOSURE_ERROR : MATCAL_CLOSURE_SUCCESS;
}
