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
#include <numberext.h>

/**
 * matcal_number_kind_equalize:
 * @kind1: first kind to equalize.
 * @kind2: second kind to equalize.
 * 
 * Equalizes two kinds.
 *
 * Returns: equalized kind.
 */
MatcalNumberKind
matcal_number_kind_equalize (MatcalNumberKind kind1, MatcalNumberKind kind2)
{
  int kind1_ = (int) kind1;
  int kind2_ = (int) kind2;

  if (kind1_ > kind2_)
    return kind1_;
  else
    return kind2_;
}

static inline void
matcal_convert_z2q (MatcalCore* core, int index)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  g_return_if_fail (matcal_core_isnumber (core, index));
  MatcalNumber* number = matcal_core_tonumber (core, index);
  matcal_core_pushnumber (core, MATCAL_NUMBER_KIND_RATIONAL);
  MatcalNumber* result = matcal_core_tonumber (core, -1);
  mpq_set_z (result->priv->rational, number->priv->integer);
}

static inline void
matcal_convert_z2f (MatcalCore* core, int index)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  g_return_if_fail (matcal_core_isnumber (core, index));
  MatcalNumber* number = matcal_core_tonumber (core, index);
  matcal_core_pushnumber (core, MATCAL_NUMBER_KIND_REAL);
  MatcalNumber* result = matcal_core_tonumber (core, -1);
  mpf_set_z (result->priv->real, number->priv->integer);
}

static inline void
matcal_convert_q2z (MatcalCore* core, int index)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  g_return_if_fail (matcal_core_isnumber (core, index));
  MatcalNumber* number = matcal_core_tonumber (core, index);
  matcal_core_pushnumber (core, MATCAL_NUMBER_KIND_INTEGER);
  MatcalNumber* result = matcal_core_tonumber (core, -1);
  mpz_set_q (result->priv->integer, number->priv->rational);
}

static inline void
matcal_convert_q2f (MatcalCore* core, int index)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  g_return_if_fail (matcal_core_isnumber (core, index));
  MatcalNumber* number = matcal_core_tonumber (core, index);
  matcal_core_pushnumber (core, MATCAL_NUMBER_KIND_REAL);
  MatcalNumber* result = matcal_core_tonumber (core, -1);
  mpf_set_q (result->priv->real, number->priv->rational);
}

static inline void
matcal_convert_f2z (MatcalCore* core, int index)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  g_return_if_fail (matcal_core_isnumber (core, index));
  MatcalNumber* number = matcal_core_tonumber (core, index);
  matcal_core_pushnumber (core, MATCAL_NUMBER_KIND_INTEGER);
  MatcalNumber* result = matcal_core_tonumber (core, -1);
  mpz_set_f (result->priv->integer, number->priv->real);
}

static inline void
matcal_convert_f2q (MatcalCore* core, int index)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  g_return_if_fail (matcal_core_isnumber (core, index));
  MatcalNumber* number = matcal_core_tonumber (core, index);
  matcal_core_pushnumber (core, MATCAL_NUMBER_KIND_RATIONAL);
  MatcalNumber* result = matcal_core_tonumber (core, -1);
  mpq_set_f (result->priv->rational, number->priv->real);
}

/**
 * matcal_core_pushnumber_as:
 * @core: #MatcalCore instance.
 * @newkind: numeric value kind to convert to.
 * 
 * Transforms a mnumeric value at index @index
 * and pushes it onto stack.
 */
void
matcal_core_pushnumber_as (MatcalCore* core, int index, MatcalNumberKind newkind)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  g_return_if_fail (matcal_core_isnumber (core, index));
  MatcalNumber* number = matcal_core_tonumber (core, index);
  MatcalNumberKind kind = number->priv->kind;

  switch (kind)
  {
  case MATCAL_NUMBER_KIND_INTEGER:
    switch (newkind)
    {
    case MATCAL_NUMBER_KIND_INTEGER:
      matcal_core_pushvalue (core, index);
      break;
    case MATCAL_NUMBER_KIND_RATIONAL:
      matcal_convert_z2q (core, index);
      break;
    case MATCAL_NUMBER_KIND_REAL:
      matcal_convert_z2f (core, index);
      break;
    }
    break;
  case MATCAL_NUMBER_KIND_RATIONAL:
    switch (newkind)
    {
    case MATCAL_NUMBER_KIND_INTEGER:
      matcal_convert_q2z (core, index);
      break;
    case MATCAL_NUMBER_KIND_RATIONAL:
      matcal_core_pushvalue (core, index);
      break;
    case MATCAL_NUMBER_KIND_REAL:
      matcal_convert_q2f (core, index);
      break;
    }
    break;
  case MATCAL_NUMBER_KIND_REAL:
    switch (newkind)
    {
    case MATCAL_NUMBER_KIND_INTEGER:
      matcal_convert_f2z (core, index);
      break;
    case MATCAL_NUMBER_KIND_RATIONAL:
      matcal_convert_f2q (core, index);
      break;
    case MATCAL_NUMBER_KIND_REAL:
      matcal_core_pushvalue (core, index);
      break;
    }
    break;
  }
}
