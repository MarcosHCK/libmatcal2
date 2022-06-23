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
#include <arithmetics.h>
#include <libmatcalext.h>

int
matlib_add (MatcalCore* core)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), -1);
  g_return_val_if_fail (matcal_core_isnumber (core, 0), -1);
  g_return_val_if_fail (matcal_core_isnumber (core, 1), -1);
  MatcalNumber* number1 = NULL;
  MatcalNumber* number2 = NULL;
  MatcalNumber* result = NULL;
  MatcalNumberKind kind;

  number1 = matcal_core_tonumber (core, 0);
  number2 = matcal_core_tonumber (core, 1);
  kind = matcal_number_kind_equalize (number1->priv->kind, number2->priv->kind);

  matcal_core_pushnumber_as (core, 0, kind);
  matcal_core_pushnumber_as (core, 1, kind);
  matcal_core_pushnumber (core, kind);

  number1 = matcal_core_tonumber (core, -3);
  number2 = matcal_core_tonumber (core, -2);
  result = matcal_core_tonumber (core, -1);

  switch (kind)
  {
  case MATCAL_NUMBER_KIND_INTEGER:
    mpz_add
    (result->priv->integer,
     number1->priv->integer,
     number2->priv->integer);
    break;
  case MATCAL_NUMBER_KIND_RATIONAL:
    mpq_add
    (result->priv->rational,
     number1->priv->rational,
     number2->priv->rational);
    break;
  case MATCAL_NUMBER_KIND_REAL:
    mpf_add
    (result->priv->real,
     number1->priv->real,
     number2->priv->real);
    break;
  }
return 1;
}

int
matlib_sub (MatcalCore* core)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), -1);
  g_return_val_if_fail (matcal_core_isnumber (core, 0), -1);
  g_return_val_if_fail (matcal_core_isnumber (core, 1), -1);
  MatcalNumber* number1 = NULL;
  MatcalNumber* number2 = NULL;
  MatcalNumber* result = NULL;
  MatcalNumberKind kind;

  number1 = matcal_core_tonumber (core, 0);
  number2 = matcal_core_tonumber (core, 1);
  kind = matcal_number_kind_equalize (number1->priv->kind, number2->priv->kind);

  matcal_core_pushnumber_as (core, 0, kind);
  matcal_core_pushnumber_as (core, 1, kind);
  matcal_core_pushnumber (core, kind);

  number1 = matcal_core_tonumber (core, -3);
  number2 = matcal_core_tonumber (core, -2);
  result = matcal_core_tonumber (core, -1);

  switch (kind)
  {
  case MATCAL_NUMBER_KIND_INTEGER:
    mpz_sub
    (result->priv->integer,
     number1->priv->integer,
     number2->priv->integer);
    break;
  case MATCAL_NUMBER_KIND_RATIONAL:
    mpq_sub
    (result->priv->rational,
     number1->priv->rational,
     number2->priv->rational);
    break;
  case MATCAL_NUMBER_KIND_REAL:
    mpf_sub
    (result->priv->real,
     number1->priv->real,
     number2->priv->real);
    break;
  }
return 1;
}

int
matlib_mul (MatcalCore* core)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), -1);
  g_return_val_if_fail (matcal_core_isnumber (core, 0), -1);
  g_return_val_if_fail (matcal_core_isnumber (core, 1), -1);
  MatcalNumber* number1 = NULL;
  MatcalNumber* number2 = NULL;
  MatcalNumber* result = NULL;
  MatcalNumberKind kind;

  number1 = matcal_core_tonumber (core, 0);
  number2 = matcal_core_tonumber (core, 1);
  kind = matcal_number_kind_equalize (number1->priv->kind, number2->priv->kind);

  matcal_core_pushnumber_as (core, 0, kind);
  matcal_core_pushnumber_as (core, 1, kind);
  matcal_core_pushnumber (core, kind);

  number1 = matcal_core_tonumber (core, -3);
  number2 = matcal_core_tonumber (core, -2);
  result = matcal_core_tonumber (core, -1);

  switch (kind)
  {
  case MATCAL_NUMBER_KIND_INTEGER:
    mpz_mul
    (result->priv->integer,
     number1->priv->integer,
     number2->priv->integer);
    break;
  case MATCAL_NUMBER_KIND_RATIONAL:
    mpq_mul
    (result->priv->rational,
     number1->priv->rational,
     number2->priv->rational);
    break;
  case MATCAL_NUMBER_KIND_REAL:
    mpf_mul
    (result->priv->real,
     number1->priv->real,
     number2->priv->real);
    break;
  }
return 1;
}

int
matlib_div (MatcalCore* core)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), -1);
  g_return_val_if_fail (matcal_core_isnumber (core, 0), -1);
  g_return_val_if_fail (matcal_core_isnumber (core, 1), -1);
  MatcalNumber* number1 = NULL;
  MatcalNumber* number2 = NULL;
  MatcalNumber* result = NULL;
  MatcalNumberKind kind;

  number1 = matcal_core_tonumber (core, 0);
  number2 = matcal_core_tonumber (core, 1);
  kind = matcal_number_kind_equalize (number1->priv->kind, number2->priv->kind);

  matcal_core_pushnumber_as (core, 0, kind);
  matcal_core_pushnumber_as (core, 1, kind);
  if (kind == MATCAL_NUMBER_KIND_INTEGER)
    matcal_core_pushnumber (core, MATCAL_NUMBER_KIND_RATIONAL);
  else
    matcal_core_pushnumber (core, kind);

  number1 = matcal_core_tonumber (core, -3);
  number2 = matcal_core_tonumber (core, -2);
  result = matcal_core_tonumber (core, -1);

  switch (kind)
  {
  case MATCAL_NUMBER_KIND_INTEGER:
    {
      mpz_ptr num = mpq_numref (result->priv->rational);
      mpz_ptr den = mpq_denref (result->priv->rational);
      mpz_set (num, number1->priv->integer);
      mpz_set (den, number2->priv->integer);
      mpq_canonicalize (result->priv->rational);
      if (!mpz_cmp_ui (den, 1))
      {
        matcal_core_pushnumber_as (core, -1, kind);
      }
    }
    break;
  case MATCAL_NUMBER_KIND_RATIONAL:
    mpq_div
    (result->priv->rational,
     number1->priv->rational,
     number2->priv->rational);
    break;
  case MATCAL_NUMBER_KIND_REAL:
    mpf_div
    (result->priv->real,
     number1->priv->real,
     number2->priv->real);
    break;
  }
return 1;
}
