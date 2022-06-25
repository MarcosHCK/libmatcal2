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
#ifndef __MATREE_COREXPR__
#define __MATREE_COREXPR__ 1

#include <core.h>
#include <expression.h>

#if __cplusplus
extern "C" {
#endif // __cplusplus

gboolean
matree_expression_compile (MatreeExpression* expression, GError** error);
void
matree_expression_push (MatreeExpression* expression, MatcalCore* core);

#if __cplusplus
}
#endif // __cplusplus

#endif // __MATREE_COREXPR__
