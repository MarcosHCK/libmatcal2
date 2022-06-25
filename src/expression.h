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
#ifndef __MATCAL_EXPRESSION__
#define __MATCAL_EXPRESSION__ 1
#include <core.h>
#include <node.h>
#include <rules.h>

#define MATCAL_EXPRESSION_ERROR (matcal_expression_error_quark ())

typedef enum
{
  MATCAL_EXPRESSION_ERROR_FAILED,
  MATCAL_EXPRESSION_ERROR_NOT_UTF8,
  MATCAL_EXPRESSION_ERROR_SCANNER,
  MATCAL_EXPRESSION_ERROR_UNMATCHED_PARENTHESIS,
} MatcalExpressionError;

#define MATCAL_TYPE_EXPRESSION (matcal_expression_get_type ())
#define MATCAL_EXPRESSION(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), MATCAL_TYPE_EXPRESSION, MatcalExpression))
#define MATCAL_IS_EXPRESSION(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MATCAL_TYPE_EXPRESSION))
typedef struct _MatcalExpression MatcalExpression;

#if __cplusplus
extern "C" {
#endif // __cplusplus

GQuark
matcal_expression_error_quark (void) G_GNUC_CONST;
GType
matcal_expression_get_type (void) G_GNUC_CONST;

MatcalExpression*
matcal_expression_new (MatcalRules* rules, GBytes* infix, GError** error);
MatcalRules*
matcal_expression_get_rules (MatcalExpression* expression);
GBytes*
matcal_expression_get_infix (MatcalExpression* expression);
gboolean
matcal_expression_compile (MatcalExpression* expression, GError** error);
void
matcal_expression_push (MatcalExpression* expression, MatcalCore* core);

#if __cplusplus
}
#endif // __cplusplus

#endif // __MATCAL_EXPRESSION__
