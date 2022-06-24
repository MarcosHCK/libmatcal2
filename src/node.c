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
#include <node.h>

G_STATIC_ASSERT (sizeof (AstNode) == sizeof (GNode));
G_STATIC_ASSERT (G_STRUCT_OFFSET (AstNode, next) == G_STRUCT_OFFSET (GNode, next));
G_STATIC_ASSERT (G_STRUCT_OFFSET (AstNode, prev) == G_STRUCT_OFFSET (GNode, prev));
G_STATIC_ASSERT (G_STRUCT_OFFSET (AstNode, parent) == G_STRUCT_OFFSET (GNode, parent));
G_STATIC_ASSERT (G_STRUCT_OFFSET (AstNode, children) == G_STRUCT_OFFSET (GNode, children));
#define _g_free0(var) ((var == NULL) ? NULL : (var = (g_free (var), NULL)))

typedef struct
{
  AstNode node_;
  AstData data_;
} RealNode;

AstNode*
ast_node_new (const gchar* symbol, AstSymbolType type)
{
  RealNode* self = NULL;
  AstNode* node = NULL;
  AstData* data = NULL;

  self = g_slice_new0 (RealNode);
  node = & self->node_;
  data = & self->data_;

  node->data = data;
  data->symbol = g_strdup (symbol);
  data->type = type;
return node;
}

void
ast_node_free (AstNode* node)
{
  g_return_if_fail (node != NULL);
  RealNode* self = (RealNode*) node;
  _g_free0 (node->data->symbol);
  g_slice_free (RealNode, self);
}

static void
ast_nodes_free (AstNode *node)
{
  while (node)
    {
      AstNode* next = node->next;
      if (node->children)
        ast_nodes_free (node->children);
      ast_node_free (node);
      node = next;
    }
}

void
ast_node_destroy (AstNode* root)
{
  g_return_if_fail (root != NULL);
  
  if (!G_NODE_IS_ROOT (root))
    g_node_unlink ((GNode*) root);
  ast_nodes_free (root);
}
