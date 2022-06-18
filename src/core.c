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
#include <object.h>

#define MATH_CORE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), MATH_TYPE_CORE, MatcalCoreClass))
#define MATH_IS_CORE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MATH_TYPE_CORE))
#define MATH_CORE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), MATH_TYPE_CORE, MatcalCoreClass))

typedef struct _MatcalCoreClass MatcalCoreClass;

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

G_DEFINE_FINAL_TYPE (MatcalCore, matcal_core, G_TYPE_OBJECT);

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
int
_matcal_core_switchidx (MatcalCore* core, int index)
{
  int oldidx = core->top;
  core->top = index;
return oldidx;
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

int
matcal_core_gettop (MatcalCore* core)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), -1);
return core->top;
}

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
      _matcal_core_push (core, object);
    }
}

void
matcal_core_pop (MatcalCore* core, int n_values)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  g_return_if_fail (core->top >= n_values);
  matcal_core_settop (core, core->top - n_values);
}

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
