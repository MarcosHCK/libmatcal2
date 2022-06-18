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
#include <libmatcal.h>
#include <glib.h>

typedef struct
{
  MatcalCore* core;
} Fixture;

#define GTESTROOT "/org/hck/libmatcal"
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
#define _g_free0(var) ((var == NULL) ? NULL : (var = (g_free (var), NULL)))

static void
fixture_set_up (Fixture* fixture, gconstpointer argument)
{
  fixture->core = matcal_core_new ();
}

static void
fixture_tear_down (Fixture* fixture, gconstpointer argument)
{
  _g_object_unref0 (fixture->core);
}

/*
 * Tests
 *
 */

static void
matcal_test_object_append (Fixture* fixture, gpointer shared)
{
  MatcalObject* object1 = NULL;
  MatcalObject* object2 = NULL;
  MatcalObject* object3 = NULL;
  MatcalObject* list = NULL;
  int nth = 0;

  object1 = matcal_object_new (MATCAL_TYPE_NIL);
  object2 = matcal_object_new (MATCAL_TYPE_NIL);
  object3 = matcal_object_new (MATCAL_TYPE_NIL);

  list = matcal_object_append (object1, object2);
  list = matcal_object_prepend (object1, object3);

  g_assert (matcal_object_nth (object3, 0) == object3);
  g_assert (matcal_object_nth (object3, 1) == object1);
  g_assert (matcal_object_nth (object3, 2) == object2);
}

/*
 * Main
 *
 */

int main(int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);
  gpointer shared = NULL;

#define test_add(path,ftest) \
  G_STMT_START { \
    g_test_add \
    ((path), \
     Fixture, NULL, \
     fixture_set_up, \
     (void (*) (Fixture*, gconstpointer)) \
     (ftest), \
     fixture_tear_down); \
  } G_STMT_END

  test_add (GTESTROOT "/object/append", matcal_test_object_append);
return g_test_run ();
}
