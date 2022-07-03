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
#include <potency.h>

int
matlib_pow (MatcalCore* core)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), -1);
  g_return_val_if_fail (matcal_core_isnumber (core, 0), -1);
  g_return_val_if_fail (matcal_core_isnumber (core, 1), -1);
  MatcalNumber* number1 = NULL;
  MatcalNumber* number2 = NULL;
  MatcalNumber* result = NULL;
  MatcalNumberKind kind1, kind2;

  number1 = matcal_core_tonumber (core, 0);
  number2 = matcal_core_tonumber (core, 1);
  kind1 = number1->priv->kind;
  kind2 = number2->priv->kind;

  if (kind2 == MATCAL_NUMBER_KIND_INTEGER)
    {
      if (mpz_fits_uint_p (number2->priv->integer))
        {
          if (kind1 == MATCAL_NUMBER_KIND_INTEGER)
            {
              matcal_core_pushnumber (core, kind1);
              result = matcal_core_tonumber (core, -1);

              mpz_pow_ui
              (result->priv->integer,
               number1->priv->integer,
               mpz_get_ui
               (number2->priv->integer));
            }
          else
            {
              if (kind1 != MATCAL_NUMBER_KIND_REAL)
                {
                  kind1 = MATCAL_NUMBER_KIND_REAL;
                  matcal_core_pushnumber_as (core, 0, kind1);
                  number1 = matcal_core_tonumber (core, -1);
                }

              matcal_core_pushnumber (core, kind1);
              result = matcal_core_tonumber (core, -1);

              mpf_pow_ui
              (result->priv->real,
               number1->priv->real,
               mpz_get_ui
               (number2->priv->integer));
            }
        }
      else
        {
          mpfr_t y, x, n;
          mpfr_inits (y, x, n, NULL);
          mpfr_set_z (n, number2->priv->integer, 0);

          switch (kind1)
          {
          case MATCAL_NUMBER_KIND_INTEGER:
            mpfr_set_z (x, number1->priv->integer, 0);
            break;
          case MATCAL_NUMBER_KIND_RATIONAL:
            mpfr_set_q (x, number1->priv->rational, 0);
            break;
          case MATCAL_NUMBER_KIND_REAL:
            mpfr_set_f (x, number1->priv->real, 0);
            break;
          }

          mpfr_pow (y, x, n, 0);

          matcal_core_pushnumber (core, MATCAL_NUMBER_KIND_REAL);
          result = matcal_core_tonumber (core, -1);
          mpfr_get_f (result->priv->real, y, 0);
          mpfr_clears (x, y, n, NULL);
        }
    }
  else
    {
      if (kind2 != MATCAL_NUMBER_KIND_RATIONAL)
        {
          kind2 = MATCAL_NUMBER_KIND_RATIONAL;
          matcal_core_pushnumber_as (core, 1, kind2);
          number2 = matcal_core_tonumber (core, -1);
        }

      mpz_ptr num = mpq_numref (number2->priv->rational);
      mpz_ptr den = mpq_denref (number2->priv->rational);

      if (mpz_fits_uint_p (num) && mpz_fits_uint_p (den))
        {
          if (kind1 == MATCAL_NUMBER_KIND_INTEGER)
            {
              matcal_core_pushnumber (core, kind1);
              result = matcal_core_tonumber (core, -1);

              mpz_pow_ui
              (result->priv->integer,
               number1->priv->integer,
               mpz_get_ui (num));
              mpz_root
              (result->priv->integer,
               number1->priv->integer,
               mpz_get_ui (den));
            }
          else
            {
              if (kind1 != MATCAL_NUMBER_KIND_REAL)
                {
                  kind1 = MATCAL_NUMBER_KIND_REAL;
                  matcal_core_pushnumber_as (core, 0, kind1);
                  number1 = matcal_core_tonumber (core, -1);
                }

              matcal_core_pushnumber (core, kind1);
              result = matcal_core_tonumber (core, -1);

              mpf_pow_ui
              (result->priv->real,
               number1->priv->real,
               mpz_get_ui (num));

              if (mpz_cmp_ui (den, 2))
                {
                  mpf_sqrt
                  (result->priv->real,
                   number1->priv->real);
                }
              else
                {
                  mpfr_t y, x;
                  mpfr_init (y);
                  mpfr_init_set_f (x, result->priv->real, 0);
                  mpfr_rootn_ui (y, x, mpz_get_ui (den), 0);
                  mpfr_get_f (result->priv->real, y, 0);
                  mpfr_clears (x, y, NULL);
                }
            }
        }
      else
        {
          mpfr_t y, x, n;
          mpfr_inits (y, x, n, NULL);

          switch (kind1)
          {
          case MATCAL_NUMBER_KIND_INTEGER:
            mpfr_set_z (x, number1->priv->integer, 0);
            break;
          case MATCAL_NUMBER_KIND_RATIONAL:
            mpfr_set_q (x, number1->priv->rational, 0);
            break;
          case MATCAL_NUMBER_KIND_REAL:
            mpfr_set_f (x, number1->priv->real, 0);
            break;
          }

          switch (kind2)
          {
          case MATCAL_NUMBER_KIND_INTEGER:
            mpfr_set_z (n, number2->priv->integer, 0);
            break;
          case MATCAL_NUMBER_KIND_RATIONAL:
            mpfr_set_q (n, number2->priv->rational, 0);
            break;
          case MATCAL_NUMBER_KIND_REAL:
            mpfr_set_f (n, number2->priv->real, 0);
            break;
          }

          mpfr_pow (y, x, n, 0);

          matcal_core_pushnumber (core, MATCAL_NUMBER_KIND_REAL);
          result = matcal_core_tonumber (core, -1);
          mpfr_get_f (result->priv->real, y, 0);
          mpfr_clears (x, y, n, NULL);
        }
    }
return 1;
}

int
matlib_log (MatcalCore* core)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), -1);
  g_return_val_if_fail (matcal_core_isnumber (core, 0), -1);
  g_return_val_if_fail (matcal_core_isnumber (core, 1), -1);
  MatcalNumber* number = NULL;
  MatcalNumber* result = NULL;
  MatcalNumberKind kind;

  number = matcal_core_tonumber (core, 0);
  kind = number->priv->kind;

  mpfr_t y, x;
  mpfr_inits (y, x, NULL);

  switch (kind)
  {
  case MATCAL_NUMBER_KIND_INTEGER:
    mpfr_set_z (x, number->priv->integer, 0);
    break;
  case MATCAL_NUMBER_KIND_RATIONAL:
    mpfr_set_q (x, number->priv->rational, 0);
    break;
  case MATCAL_NUMBER_KIND_REAL:
    mpfr_set_f (x, number->priv->real, 0);
    break;
  }

  mpfr_log (y, x, 0);

  matcal_core_pushnumber (core, MATCAL_NUMBER_KIND_REAL);
  result = matcal_core_tonumber (core, -1);
  mpfr_get_f (result->priv->real, y, 0);
  mpfr_clears (x, y, NULL);
return 1;
}
