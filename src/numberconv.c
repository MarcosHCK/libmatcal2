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

G_GNUC_INTERNAL
MatcalNumber*
_matcal_number_new (MatcalNumberKind kind);
G_GNUC_INTERNAL
void
_matcal_number_transform (MatcalNumberPrivate* dst, MatcalNumberPrivate* src)
{
  switch (dst->kind)
  {
  case MATCAL_NUMBER_KIND_INTEGER:
    switch (src->kind)
    {
    case MATCAL_NUMBER_KIND_INTEGER:
      mpz_set (dst->integer, src->integer);
      break;
    case MATCAL_NUMBER_KIND_RATIONAL:
      mpz_set_q (dst->integer, src->rational);
      break;
    case MATCAL_NUMBER_KIND_REAL:
      mpz_set_f (dst->integer, src->real);
      break;
    }
    break;
  case MATCAL_NUMBER_KIND_RATIONAL:
    switch (src->kind)
    {
    case MATCAL_NUMBER_KIND_INTEGER:
      mpq_set_z (dst->rational, src->integer);
      break;
    case MATCAL_NUMBER_KIND_RATIONAL:
      mpq_set (dst->rational, src->rational);
      break;
    case MATCAL_NUMBER_KIND_REAL:
      mpq_set_f (dst->rational, src->real);
      break;
    }
    break;
  case MATCAL_NUMBER_KIND_REAL:
    switch (src->kind)
    {
    case MATCAL_NUMBER_KIND_INTEGER:
      mpf_set_z (dst->real, src->integer);
      break;
    case MATCAL_NUMBER_KIND_RATIONAL:
      mpf_set_q (dst->real, src->rational);
      break;
    case MATCAL_NUMBER_KIND_REAL:
      mpf_set (dst->real, src->real);
      break;
    }
    break;
  }
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
  MatcalNumber* number1 = matcal_core_tonumber (core, index);
  MatcalNumber* number2 = _matcal_number_new (newkind);
  _matcal_number_transform (number2->priv, number1->priv);
  _matcal_core_push (core, number2);
  matcal_object_unref (number2);
}
