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
#include <libmatlib.h>

static gint iterations = 10000;
static gint starts = 1;

gboolean
docalculations (MatcalCore* core, int argc, char* argv[], GError** error)
{
  GError* tmp_err = NULL;
  gint i, j;

  const int SOLVE = 0;
  const int ITER = 1;

  starts = (starts >= 1) ? starts : 1;

  for (i = 1; i < argc; i++)
    {
      matcal_core_loadstring (core, argv [i], &tmp_err);
      if (G_UNLIKELY (tmp_err != NULL))
        {
          g_propagate_error (error, tmp_err);
          return FALSE;
        }

      matcal_core_pushnumber_uint (core, 0);

      for (j = starts; j < iterations; j++)
        {
          matcal_core_pushcfunction (core, matlib_add);
          matcal_core_pushvalue (core, ITER);
          matcal_core_pushvalue (core, SOLVE);
          matcal_core_pushnumber_uint (core, j);
          matcal_core_call (core, 1, 1);
          matcal_core_call (core, 2, 1);
          matcal_core_remove (core, ITER);
          g_assert (matcal_core_gettop (core) == 2);
        }

      g_print
      ("bigsum (%s, %i) = %f\r\n",
       argv [i],
       iterations,
       matcal_core_tonumber_double
       (core, ITER));

      matcal_core_pop (core, 2);
    }
return TRUE;
}

int
main (int argc, char** argv)
{
  GError* tmp_err = NULL;
  gboolean success = TRUE;

  GOptionEntry entries[] =
  {
    { "iterations", 'i', 0, G_OPTION_ARG_INT, &iterations,       "Make forcefully N", "N" },
    {      "start", 's', 0, G_OPTION_ARG_INT,     &starts, "Start iterations from N", "N" },
    { NULL }
  };

  GOptionContext* ctx =
  g_option_context_new (NULL);
  g_option_context_add_main_entries (ctx, entries, "en_US");
  g_option_context_set_ignore_unknown_options (ctx, FALSE);

  success =
  g_option_context_parse (ctx, &argc, &argv, &tmp_err);
  g_option_context_free (ctx);

  if (G_UNLIKELY(tmp_err != NULL))
  {
    g_critical
    ("(%s): %s: %i: %s",
     G_STRLOC,
     __LINE__,
     g_quark_to_string
     (tmp_err->domain),
     tmp_err->code,
     tmp_err->message);
    g_error_free (tmp_err);
    g_assert_not_reached ();
  }

  MatcalCore* core =
  matcal_core_new ();
  matlib_setup (core);

  success =
  docalculations (core, argc, argv, &tmp_err);
  g_object_unref (core);

  if (G_UNLIKELY(tmp_err != NULL))
  {
    g_critical
    ("(%s): %s: %i: %s",
     G_STRLOC,
     __LINE__,
     g_quark_to_string
     (tmp_err->domain),
     tmp_err->code,
     tmp_err->message);
    g_error_free (tmp_err);
    g_assert_not_reached ();
  }
return (success) ? 0 : -1;
}
