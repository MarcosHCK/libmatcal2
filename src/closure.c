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
#include <coreext.h>
#include <object.h>

#define MATCAL_TYPE_CLOSURE (matcal_closure_get_type ())
#define MATCAL_CLOSURE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), MATCAL_TYPE_CLOSURE, MatcalClosure))
#define MATCAL_CLOSURE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), MATCAL_TYPE_CLOSURE, MatcalClosureClass))
#define MATCAL_IS_CLOSURE(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MATCAL_TYPE_CLOSURE))
#define MATCAL_IS_CLOSURE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MATCAL_TYPE_CLOSURE))
#define MATCAL_CLOSURE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), MATCAL_TYPE_CLOSURE, MatcalClosureClass))

typedef struct _MatcalClosure MatcalClosure;
typedef struct _MatcalClosurePrivate MatcalClosurePrivate;
typedef struct _MatcalClosureClass MatcalClosureClass;

struct _MatcalClosure
{
  MatcalObject parent_instance;
  MatcalClosurePrivate* priv;
};

struct _MatcalClosureClass
{
  MatcalObjectClass parent_class;
};

struct _MatcalClosurePrivate
{
  MatcalCFunction callback;
};

G_DEFINE_TYPE_WITH_PRIVATE
(MatcalClosure,
 matcal_closure,
 MATCAL_TYPE_OBJECT);

static void
matcal_closure_class_init (MatcalClosureClass* klass)
{
}

static void
matcal_closure_init (MatcalClosure* self)
{
  self->priv = matcal_closure_get_instance_private (self);
  self->priv->callback = NULL;
}

/*
 * API
 *
 */

void
matcal_core_pushcfunction (MatcalCore* core, MatcalCFunction cclosure)
{
  g_return_if_fail (MATCAL_CORE (core));
  g_return_if_fail (cclosure != NULL);
  MatcalClosure* closure = matcal_object_new (MATCAL_TYPE_CLOSURE);
  closure->priv->callback = cclosure;
  _matcal_core_push (core, closure);
  matcal_object_unref (closure);
}

static inline void
leave_n_items (MatcalCore* core, int n)
{
  int i, top = matcal_core_gettop (core);
  if (n >= top)
    matcal_core_settop (core, n);
  else while (top-- > n)
    matcal_core_remove (core, 0);
}

MatcalClosureResult
matcal_core_call (MatcalCore* core, int n_args, int n_res)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), MATCAL_CLOSURE_ERROR);
  g_return_val_if_fail (n_args >= 0, MATCAL_CLOSURE_ERROR);
  g_return_val_if_fail (matcal_core_gettop (core) >= (n_args + 1), MATCAL_CLOSURE_ERROR);
  g_return_val_if_fail (n_res == MATCAL_CLOSURE_MULTIRET || n_res >= 0, MATCAL_CLOSURE_ERROR);
  MatcalClosure* closure = _matcal_core_peek (core, -n_args-1);
  g_return_val_if_fail (MATCAL_IS_CLOSURE (closure), MATCAL_CLOSURE_ERROR);
  MatcalClosureResult result;
  int i, oldindex;

  oldindex =
  switch_index (n_args);
  oldindex -= (n_args + 1);
  {
    int result_;

    result_ =
    closure->priv->callback (core);
    if (result < 0)
      {
        matcal_core_settop (core, 0);
        result = MATCAL_CLOSURE_ERROR;
      }
    else
      {
        leave_n_items (core, result_);
        if (n_res == MATCAL_CLOSURE_MULTIRET)
          oldindex += result_;
        else
          {
            matcal_core_settop (core, n_res);
            oldindex += n_res;
          }
        result = MATCAL_CLOSURE_SUCCESS;
      } 
  }
  switch_index (oldindex);
return result;
}
