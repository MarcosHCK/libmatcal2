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
#ifndef __MATCAL_NUMBEREXT__
#define __MATCAL_NUMBEREXT__ 1
#include <number.h>
#include <core.h>
#include <gmp.h>

#if __cplusplus
extern "C" {
#endif // __cplusplus

typedef enum
{
  MATCAL_NUMBER_KIND_INTEGER,
  MATCAL_NUMBER_KIND_RATIONAL,
  MATCAL_NUMBER_KIND_REAL,
} MatcalNumberKind;

struct _MatcalNumberPrivate
{
  MatcalNumberKind kind;
  union
  {
    mpz_t integer;
    mpq_t rational;
    mpf_t real;
  };
};

/* number kind API */

MatcalNumberKind
matcal_number_kind_equalize (MatcalNumberKind kind1, MatcalNumberKind kind2);

/* extended API */

void
matcal_core_pushnumber_as (MatcalCore* core, int index, MatcalNumberKind newkind);
void
matcal_core_pushnumber (MatcalCore* core, MatcalNumberKind kind);
MatcalNumber*
matcal_core_tonumber (MatcalCore* core, int index);

#if __cplusplus
}
#endif // __cplusplus

#endif // __MATCAL_NUMBEREXT__
