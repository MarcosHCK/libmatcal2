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
  MatcalRules* rules = NULL;
  rules = matcal_core_get_rules (core);

  /* arithmetic operators */

  matcal_rules_add_operator (rules, "[\\+]", MATCAL_ASSOC_LEFT, 2, 0);
  matcal_core_pushcfunction (core, matlib_add);
  matcal_core_setglobal (core, "+");

  matcal_rules_add_operator (rules, "[\\-]", MATCAL_ASSOC_LEFT, 2, 0);
  matcal_core_pushcfunction (core, matlib_sub);
  matcal_core_setglobal (core, "-");

  matcal_rules_add_operator (rules, "[\\*]", MATCAL_ASSOC_LEFT, 3, 0);
  matcal_core_pushcfunction (core, matlib_mul);
  matcal_core_setglobal (core, "*");

  matcal_rules_add_operator (rules, "[\\/]", MATCAL_ASSOC_LEFT, 3, 0);
  matcal_core_pushcfunction (core, matlib_div);
  matcal_core_setglobal (core, "/");

  matcal_rules_add_operator (rules, "[\\^]", MATCAL_ASSOC_RIGHT, 4, 0);
  matcal_core_pushcfunction (core, matlib_pow);
  matcal_core_setglobal (core, "^");

  matcal_core_pushcfunction (core, matlib_log);
  matcal_core_register_function (core, "log", 1);

  matcal_core_pushcfunction (core, matlib_sin);
  matcal_core_register_function (core, "sin", 1);
  matcal_core_pushcfunction (core, matlib_cos);
  matcal_core_register_function (core, "cos", 1);
  matcal_core_pushcfunction (core, matlib_tan);
  matcal_core_register_function (core, "tan", 1);
  matcal_core_pushcfunction (core, matlib_asin);
  matcal_core_register_function (core, "asin", 1);
  matcal_core_pushcfunction (core, matlib_acos);
  matcal_core_register_function (core, "acos", 1);
  matcal_core_pushcfunction (core, matlib_atan);
  matcal_core_register_function (core, "atan", 1);
  matcal_core_pushcfunction (core, matlib_sinh);
  matcal_core_register_function (core, "sinh", 1);
  matcal_core_pushcfunction (core, matlib_cosh);
  matcal_core_register_function (core, "cosh", 1);
  matcal_core_pushcfunction (core, matlib_tanh);
  matcal_core_register_function (core, "tanh", 1);
  matcal_core_pushcfunction (core, matlib_asinh);
  matcal_core_register_function (core, "sinh", 1);
  matcal_core_pushcfunction (core, matlib_acosh);
  matcal_core_register_function (core, "cosh", 1);
  matcal_core_pushcfunction (core, matlib_atanh);
  matcal_core_register_function (core, "tanh", 1);

  /* generic functions */

  matcal_core_pushcfunction (core, matlib_max);
  matcal_core_register_function (core, "max", 2);

  matcal_core_pushcfunction (core, matlib_min);
  matcal_core_register_function (core, "min", 2);
}
