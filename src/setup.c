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
#include <libmatlib.h>

void
matlib_setup (MatcalCore* core)
{
  matcal_core_pushcfunction (core, matlib_add);
  matcal_core_setglobal (core, "+");
  matcal_core_pushcfunction (core, matlib_sub);
  matcal_core_setglobal (core, "-");
  matcal_core_pushcfunction (core, matlib_mul);
  matcal_core_setglobal (core, "*");
  matcal_core_pushcfunction (core, matlib_div);
  matcal_core_setglobal (core, "/");
}
