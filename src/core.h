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
#ifndef __MATCAL_CORE__
#define __MATCAL_CORE__ 1
#include <glib-object.h>

#define MATCAL_TYPE_CORE (matcal_core_get_type ())
#define MATCAL_CORE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), MATCAL_TYPE_CORE, MatcalCore))
#define MATCAL_IS_CORE(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MATCAL_TYPE_CORE))

typedef struct _MatcalCore MatcalCore;

#if __cplusplus
extern "C" {
#endif // __cplusplus

GType
matcal_core_get_type (void) G_GNUC_CONST;

/* public API */

MatcalCore*
matcal_core_new ();
int
matcal_core_gettop (MatcalCore* core);
void
matcal_core_settop (MatcalCore* core, int newtop);
void
matcal_core_setglobal (MatcalCore* core, const gchar* name);
void
matcal_core_getglobal (MatcalCore* core, const gchar* name);
void
matcal_core_pushvalue (MatcalCore* core, int index);
void
matcal_core_pushupvalue (MatcalCore* core, int index);
void
matcal_core_pop (MatcalCore* core, int n_values);
void
matcal_core_remove (MatcalCore* core, int index);
void
matcal_core_insert (MatcalCore* core, int index);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (MatcalCore, g_object_unref)

/* C-Core API */

gboolean
matcal_core_isnone (MatcalCore* core, int index);
void
matcal_core_pushnil (MatcalCore* core);
gboolean
matcal_core_isnil (MatcalCore* core, int index);
void
matcal_core_pushnumber_uint (MatcalCore* core, unsigned int value);
unsigned int
matcal_core_tonumber_uint (MatcalCore* core, int index);
void
matcal_core_pushnumber_double (MatcalCore* core, double value);
double
matcal_core_tonumber_double (MatcalCore* core, int index);
gboolean
matcal_core_pushnumber_string (MatcalCore* core, const gchar* value, int base);
gchar*
matcal_core_tonumber_string (MatcalCore* core, int index, int base);
gboolean
matcal_core_isnumber (MatcalCore* core, int index);

#define matcal_core_isnoneornil(core,index) \
  (matcal_core_isnone ((core),(index))\
  ||matcal_core_isnil ((core),(index)))

/* closure API */

typedef int (*MatcalCFunction) (MatcalCore* core);

#define MATCAL_CLOSURE_MULTIRET (-1)

typedef enum
{
  MATCAL_CLOSURE_SUCCESS = 0,
  MATCAL_CLOSURE_ERROR = -1,
} MatcalClosureResult;

void
matcal_core_pushcfunction (MatcalCore* core, MatcalCFunction cclosure);
void
matcal_core_pushclosure (MatcalCore* core, MatcalCFunction cclosure, guint n_upvalues);
gboolean
matcal_core_isfunction (MatcalCore* core, int index);
MatcalClosureResult
matcal_core_call (MatcalCore* core, int n_args, int n_results);

#if __cplusplus
}
#endif // __cplusplus

#endif // __MATCAL_CORE__
