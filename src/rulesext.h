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
#ifndef __MATCAL_RULESEXT__
#define __MATCAL_RULESEXT__ 1
#include <rules.h>

typedef struct _OperatorClass OperatorClass;
typedef struct _FunctionClass FunctionClass;
typedef struct _SymbolClass SymbolClass;

typedef enum
{
  SYMBOL_KIND_UKNOWN = 0,

  SYMBOL_KIND_PARENTHESIS,

  SYMBOL_KIND_CONSTANT,
  SYMBOL_KIND_VARIABLE,
  SYMBOL_KIND_OPERATOR,
  SYMBOL_KIND_FUNCTION,
} SymbolKind;

typedef enum
{
  OPERATOR_ASSOC_LEFT = 0,
  OPERATOR_ASSOC_RIGHT,
} OperatorAssoc;

struct _OperatorClass
{
  OperatorAssoc assoc : 1;
  guint precedence : 6;
  guint unary : 1;
};

struct _FunctionClass
{
  guint n_args;
};

struct _SymbolClass
{
  SymbolKind kind;

  union
  {
    OperatorClass opclass;
    FunctionClass fnclass;
  };
};

#if __cplusplus
extern "C" {
#endif // __cplusplus

/* lexer */

G_GNUC_INTERNAL
gchar**
_matcal_rules_tokenize (MatcalRules* rules, const gchar* input, gssize length, GError** error);

/* classifier */

G_GNUC_INTERNAL
SymbolClass*
_matcal_rules_classify (MatcalRules* rules, const gchar* input, GError** error);

/* lexer & classifier adders */

G_GNUC_INTERNAL
void
_matcal_rules_add_token (MatcalRules* self, const gchar* expr, gssize pos, GError** error);
G_GNUC_INTERNAL
void
_matcal_rules_add_class (MatcalRules* self, const gchar* expr, const SymbolClass* klass, gssize pos, GError** error);

#if __cplusplus
}
#endif // __cplusplus

#endif // __MATCAL_RULES__
