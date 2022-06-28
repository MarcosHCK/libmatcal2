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

G_GNUC_INTERNAL
int
_matlib_equalize (MatcalCore* core, int from, int n)
{
  MatcalNumberKind kind;
  MatcalNumber* number;

  g_return_val_if_fail (from > 0, -1);
  g_return_val_if_fail (n > 0, -1);
  int i, diff = 0;

  number = matcal_core_tonumber (core, from);
  kind = number->priv->kind;

  for (i = 1; i < n; i++)
    {
      MatcalNumberKind kind2;
      number = matcal_core_tonumber (core, from + i);
      kind2 = number->priv->kind;

      if (kind != kind2)
        diff = TRUE;
      else
        {
          kind =
          matcal_number_kind_equalize (kind, kind2);
        }
    }

  if (diff)
    {
      int top = matcal_core_gettop (core);
      for (i = 0; i < n; i++)
        matcal_core_pushnumber_as (core, from + i, kind);
      return top;
    }
return from;
}

int
matlib_max (MatcalCore* core)
{
  MatcalNumber* number1 = NULL;
  MatcalNumber* number2 = NULL;
  int i, top, max = 0;

  top = matcal_core_gettop (core);
  g_return_val_if_fail (top > 0, 0);

  for (i = 1; i < top; i++)
    {
      number1 = matcal_core_tonumber (core, max);
      number2 = matcal_core_tonumber (core, i);
      if (matcal_number_cmp (number2, number1) > 0)
        max = i;
    }

  matcal_core_pushvalue (core, max);
return 1;
}

int
matlib_min (MatcalCore* core)
{
  MatcalNumber* number1 = NULL;
  MatcalNumber* number2 = NULL;
  int i, top, max = 0;

  top = matcal_core_gettop (core);
  g_return_val_if_fail (top > 0, 0);

  for (i = 1; i < top; i++)
    {
      number1 = matcal_core_tonumber (core, max);
      number2 = matcal_core_tonumber (core, i);
      if (matcal_number_cmp (number2, number1) < 0)
        max = i;
    }

  matcal_core_pushvalue (core, max);
return 1;
}
