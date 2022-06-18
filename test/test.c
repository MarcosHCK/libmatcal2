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
  int dummy;
} Fixture;

#define GTESTROOT "/org/hck/libmatcal"
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
#define _g_free0(var) ((var == NULL) ? NULL : (var = (g_free (var), NULL)))

static void
fixture_set_up (Fixture* fixture, gconstpointer argument)
{
}

static void
fixture_tear_down (Fixture* fixture, gconstpointer argument)
{
}

/*
 * Tests
 *
 */

/*
 * Main
 *
 */

int main(int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);
  gpointer shared = NULL;

#define d_test_add(path,ftest) \
  G_STMT_START { \
    g_test_add \
    ((path), \
     Fixture, NULL, \
     fixture_set_up, \
     (void (*) (Fixture*, gconstpointer)) \
     (ftest), \
     fixture_tear_down); \
  } G_STMT_END


return g_test_run ();
}
