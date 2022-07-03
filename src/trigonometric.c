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
#include <libmatcalext.h>
#include <mpfr.h>
#include <trigonometric.h>

#define MAKE_TRIG(func) \
  int matlib_##func (MatcalCore* core) \
  { \
    g_return_val_if_fail (MATCAL_IS_CORE (core), -1); \
    g_return_val_if_fail (matcal_core_isnumber (core, 0), -1); \
    g_return_val_if_fail (matcal_core_isnumber (core, 1), -1); \
    MatcalNumber* number = NULL; \
    MatcalNumber* result = NULL; \
    MatcalNumberKind kind; \
 \
    number = matcal_core_tonumber (core, 0); \
    kind = number->priv->kind; \
 \
    mpfr_t y, x; \
    mpfr_inits (y, x, NULL); \
 \
    switch (kind) \
    { \
    case MATCAL_NUMBER_KIND_INTEGER: \
      mpfr_set_z (x, number->priv->integer, 0); \
      break; \
    case MATCAL_NUMBER_KIND_RATIONAL: \
      mpfr_set_q (x, number->priv->rational, 0); \
      break; \
    case MATCAL_NUMBER_KIND_REAL: \
      mpfr_set_f (x, number->priv->real, 0); \
      break; \
    } \
 \
    mpfr_##func (y, x, 0); \
 \
    matcal_core_pushnumber (core, MATCAL_NUMBER_KIND_REAL); \
    result = matcal_core_tonumber (core, -1); \
    mpfr_get_f (result->priv->real, y, 0); \
    mpfr_clears (x, y, NULL); \
  return 1; \
  }

MAKE_TRIG (sin);
MAKE_TRIG (cos);
MAKE_TRIG (tan);
MAKE_TRIG (asin);
MAKE_TRIG (acos);
MAKE_TRIG (atan);
MAKE_TRIG (sinh);
MAKE_TRIG (cosh);
MAKE_TRIG (tanh);
MAKE_TRIG (asinh);
MAKE_TRIG (acosh);
MAKE_TRIG (atanh);

#undef MAKE_TRIG
