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
#include <compiler_override.h>
#include <dynasm/dasm_proto.h>
#include <dynasm/dasm_x86.h>
#include <libmatcal.h>

typedef struct _AstStat AstStat;

#if __DASC__
|.arch x64
|.section code, data
|.globals globl_
|.actionlist actions
|.globalnames globl_names
|.externnames extern_names
|.include compiler_x86_64.h
#else // !__DASC__
# define DASM_MAXSECTION (0)
# define globl_main (0)
# define globl__MAX (0)
# define actions (NULL)
extern const gchar** globl_names;
# define MAP_ANONYMOUS 0
#endif // __DASC__

struct _AstStat
{
  GHashTable* constants;
  GHashTable* variables;
  dasm_State** Dst;

  union
  {
    guint pcs;
    guint n_constants;
  };

  guint n_variables;
};

static void
dumpbuffer (dasm_State** Dst, gpointer buf, gsize length)
{
  gsize dwords = (length / 4);
  gsize i;

  for (i = 0; i < dwords; i++)
    {
#if __DASC__
      |.dword (*(guint32*) buf)
#endif // __DASC__
      buf += 4;
    }

  for (i = 0; i < (length % 4); i++)
    {
#if __DASC__
      |.byte (*(guint8*) buf)
#endif // __DASC__
      ++buf;
    }
}

static gboolean
scan_tree (AstNode* node, AstStat* q)
{
  gchar* name = node->data->symbol;
  switch (node->data->type)
  {
  case AST_SYMBOL_CONSTANT:
  case AST_SYMBOL_FUNCTION:
    {
      gsize length = strlen (name) + 1;
      dasm_State** Dst = q->Dst;
      guint idx = q->n_constants++;

      dasm_growpc (Dst, q->pcs);
#if __DASC__
      |=>idx:
#endif // __DASC__
      gpointer pidx = GINT_TO_POINTER (idx);
      g_hash_table_insert (q->constants, name, pidx);
      dumpbuffer (Dst, name, length);
    }
    break;
  case AST_SYMBOL_VARIABLE:
    {
      guint idx = q->n_variables++;
      gpointer pidx = GINT_TO_POINTER (idx);
      g_hash_table_insert (q->variables, name, pidx);
    }
    break;
  }
return FALSE;
}

#define stacksz \
  ( 0 \
    + sizeof (gpointer)  /* core */ \
  )
#if __DASC__
|.define core, [rsp+8*0]
#endif // __DASC__

static void
pushnamedfn (MatcalCore* core, const gchar* name)
{
  matcal_core_getglobal (core, name);
  if (!matcal_core_isfunction (core, -1))
    {
      g_error ("global '%s' is not a function", name);
      g_assert_not_reached ();
    }
}

static void
compile (AstNode* node, AstStat* q)
{
  AstNode* child = NULL;
  AstData* data = node->data;
  gchar* name = data->symbol;
  dasm_State** Dst = q->Dst;
  guint n_children = 0;
  gpointer pidx;
  guint idx;
#if DEVELOPER
  gboolean has = FALSE;
#endif // DEVELOPER

  switch (data->type)
  {
  case AST_SYMBOL_CONSTANT:
#if DEVELOPER
    has =
#endif // DEVELOPER
    g_hash_table_lookup_extended (q->constants, name, NULL, &pidx);
    idx = GPOINTER_TO_UINT (pidx);
#if DEVELOPER
    g_assert (has);
#endif // DEVELOPER
#if __DASC__
# ifdef G_OS_WIN32
    | mov rcx, core
    | lea rdx, [=>(idx)]
    | mov r8, 10
# endif // G_OS_WIN32
# ifdef G_OS_UNIX
    | mov rdi, core
    | lea rsi, [=>(idx)]
    | mov rdx, 10
# endif // G_OS_UNIX
    | invoke matcal_core_pushnumber_string
#endif // __DASC__
    break;

  case AST_SYMBOL_VARIABLE:
#if DEVELOPER
    has =
#endif // DEVELOPER
    g_hash_table_lookup_extended (q->variables, name, NULL, &pidx);
    idx = GPOINTER_TO_UINT (pidx);
#if DEVELOPER
    g_assert (has);
#endif // DEVELOPER
#if __DASC__
# ifdef G_OS_WIN32
    | mov rcx, core
    | mov rdx, (idx)
# endif // G_OS_WIN32
# ifdef G_OS_UNIX
    | mov rdi, core
    | mov rsi, (idx)
# endif // G_OS_UNIX
    | invoke matcal_core_pushvalue
#endif // __DASC__
    break;

  case AST_SYMBOL_FUNCTION:
#if DEVELOPER
    has =
#endif // DEVELOPER
    g_hash_table_lookup_extended (q->constants, name, NULL, &pidx);
    idx = GPOINTER_TO_UINT (pidx);
#if DEVELOPER
    g_assert (has);
#endif // DEVELOPER
#if __DASC__
# ifdef G_OS_WIN32
    | mov rcx, core
    | lea rdx, [=>(idx)]
# endif // G_OS_WIN32
# ifdef G_OS_UNIX
    | mov rdi, core
    | lea rsi, [=>(idx)]
# endif // G_OS_UNIX
    | invoke pushnamedfn
#endif // __DASC__
    break;
  }

  for (child = node->children; child; child = child->next, ++n_children)
    compile (child, q);

  if (data->type == AST_SYMBOL_FUNCTION)
    {
#if DEVELOPER
  g_assert (G_MAXINT32 >= n_children);
#endif // DEVELOPER
#if __DASC__
# ifdef G_OS_WIN32
      | mov rcx, core
      | mov rdx, (n_children)
      | mov r8, 1
# endif // G_OS_WIN32
# ifdef G_OS_UNIX
      | mov rdi, core
      | mov rsi, (n_children)
      | mov rdx, 1
# endif // G_OS_UNIX
      | invoke matcal_core_call
#endif // __DASC__
    }
}

static inline void
libjit_do_compile (dasm_State** Dst, AstNode* ast, GError** error)
{
  AstStat stat = {0};
  GSList* list = NULL;

#if __DASC__
  |.data
#endif // __DASC__

  stat.pcs = 0;
  stat.Dst = Dst;
  stat.constants = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, NULL);
  stat.variables = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, NULL);
  g_node_traverse ((GNode*) ast, G_IN_ORDER, G_TRAVERSE_ALL, -1, (GNodeTraverseFunc) scan_tree, &stat);

#if __DASC__
  |.code
  |->main:
  | sub rsp, (stacksz)
# ifdef G_OS_WIN32
  | mov core, rcx
# endif // G_OS_WIN32
# ifdef G_OS_UNIX
  | mov core, rdi
# endif // G_OS_UNIX
#endif // __DASC__

  compile (ast, &stat);

#if __DASC__
  | add rsp, (stacksz)
  | mov rax, 1
  | ret
#endif // __DASC__

  g_hash_table_remove_all (stat.constants);
  g_hash_table_unref (stat.constants);
  g_hash_table_remove_all (stat.variables);
  g_hash_table_unref (stat.variables);
}

JitCallback*
libjit_compile (AstNode* ast, GError** error)
{
  g_return_val_if_fail (ast != NULL, NULL);
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  gpointer labels [globl__MAX];
  GError* tmp_err = NULL;
  dasm_State* state = NULL;
  dasm_State** Dst = &state;
  JitCallback* callback;
  size_t bufsz = 0;
  gpointer buf;

  dasm_init (Dst, DASM_MAXSECTION);
  dasm_setupglobal (Dst, labels, globl__MAX);
  dasm_setup (Dst, actions);
  dasm_growpc (Dst, 0);

  libjit_do_compile (Dst, ast, &tmp_err);
  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_propagate_error (error, tmp_err);
      dasm_free (Dst);
      return NULL;
    }

  dasm_link (Dst, &bufsz);
#ifdef G_OS_WINDOWS
  buf = VirtualAlloc (0, bufsz, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#else // !G_OS_WINDOWS
  buf = mmap (0, bufsz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif // G_OS_WINDOWS

  dasm_encode (Dst, buf);
#ifdef G_OS_WINDOWS
  G_STMT_START {
    DWORD dwOld;
    VirtualProtect (buf, bufsz, PAGE_EXECUTE_READ, &dwOld);
  } G_STMT_END;
#else // !G_OS_WINDOWS
  mprotect (buf, bufsz, PROT_READ | PROT_EXEC);
#endif // G_OS_WINDOWS
  dasm_free (Dst);

  callback = g_slice_new (JitCallback);
  callback->start = labels [globl_main];
  callback->block = buf;
  callback->blocksz = bufsz;
return callback;
}

void
libjit_free (JitCallback* callback)
{
  gpointer buf = callback->block;
  gsize bufsz = callback->blocksz;
  g_slice_free (JitCallback, callback);
#ifdef G_OS_WINDOWS
  VirtualFree (buf, bufsz, 0);
#else // !G_OS_WINDOWS
  munmap (buf, bufsz);
#endif // G_OS_WINDOWS
}
