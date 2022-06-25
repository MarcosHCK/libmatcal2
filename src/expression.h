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
#ifndef __MATREE_EXPRESSION__
#define __MATREE_EXPRESSION__ 1
#include <node.h>
#include <rules.h>

#define MATREE_EXPRESSION_ERROR (matree_expression_error_quark ())

typedef enum
{
  MATREE_EXPRESSION_ERROR_FAILED,
  MATREE_EXPRESSION_ERROR_NOT_UTF8,
  MATREE_EXPRESSION_ERROR_SCANNER,
  MATREE_EXPRESSION_ERROR_UNMATCHED_PARENTHESIS,
} MatreeExpressionError;

#define MATREE_TYPE_EXPRESSION (matree_expression_get_type ())
#define MATREE_EXPRESSION(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), MATREE_TYPE_EXPRESSION, MatreeExpression))
#define MATREE_IS_EXPRESSION(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MATREE_TYPE_EXPRESSION))
typedef struct _MatreeExpression MatreeExpression;

#if __cplusplus
extern "C" {
#endif // __cplusplus

GQuark
matree_expression_error_quark (void) G_GNUC_CONST;
GType
matree_expression_get_type (void) G_GNUC_CONST;

MatreeExpression*
matree_expression_new (MatreeRules* rules, const gchar* infix, GError** error);
MatreeRules*
matree_expression_get_rules (MatreeExpression* expression);
const gchar*
matree_expression_get_infix (MatreeExpression* expression);
AstNode*
matree_expression_get_ast (MatreeExpression* expression);

#if __cplusplus
}
#endif // __cplusplus

#endif // __MATREE_EXPRESSION__
