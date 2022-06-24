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
#ifndef __AST_NODE__
#define __AST_NODE__ 1
#include <glib.h>

typedef struct _AstNode AstNode;
typedef struct _AstData AstData;

typedef enum
{
  AST_SYMBOL_CONSTANT,
  AST_SYMBOL_VARIABLE,
  AST_SYMBOL_FUNCTION,
} AstSymbolType;

struct _AstNode
{
  AstData* data;
  AstNode* next;
  AstNode* prev;
  AstNode* parent;
  AstNode* children;
};

struct _AstData
{
  gchar* symbol;
  AstSymbolType type;
};

#if __cplusplus
extern "C" {
#endif // __cplusplus

AstNode*
ast_node_new (const gchar* symbol, AstSymbolType type);
void
ast_node_free (AstNode* node);
void
ast_node_destroy (AstNode* root);

#if __cplusplus
}
#endif // __cplusplus

#endif // __AST_NODE__
