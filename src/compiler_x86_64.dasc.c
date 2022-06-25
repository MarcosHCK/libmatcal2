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

#if __DASC__
|.arch x64
|.section code
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

static inline void
libjit_do_compile (dasm_State** Dst, AstNode* ast, GError** error)
{
#if __DASC__
  | xor rax, rax
#endif // __DASC__
}

MatcalCFunction
libjit_compile (AstNode* ast, GError** error)
{
  g_return_val_if_fail (ast != NULL, NULL);
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  gpointer labels [globl__MAX];
  GError* tmp_err = NULL;
  dasm_State* state = NULL;
  dasm_State** Dst = &state;
  size_t bufsz = 0;
  gpointer buf;

  dasm_init (Dst, DASM_MAXSECTION);
  dasm_setupglobal (Dst, labels, globl__MAX);
  dasm_setup (Dst, actions);
  dasm_growpc (Dst, 0);

#if __DASC__
  | .dword 0
  |->main:
#endif // __DASC__

  libjit_do_compile (Dst, ast, &tmp_err);
  if (G_UNLIKELY (tmp_err != NULL))
    {
      g_propagate_error (error, tmp_err);
      dasm_free (Dst);
      return NULL;
    }

#if __DASC__
  | ret
#endif // __DASC__

  dasm_link (Dst, &bufsz);
#ifdef G_OS_WINDOWS
  buf = VirtualAlloc (0, bufsz, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#else // !G_OS_WINDOWS
  buf = mmap (0, bufsz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif // G_OS_WINDOWS

  dasm_encode (Dst, buf);
  g_assert (G_MAXUINT32 >= bufsz);
  ((guint32*) buf) [0] = bufsz;
#if DEVELOPER
  g_assert (labels [globl_main] - buf == sizeof (guint32));
#endif // DEVELOPER

#ifdef G_OS_WINDOWS
  G_STMT_START {
    DWORD dwOld;
    VirtualProtect (buf, bufsz, PAGE_EXECUTE_READ, &dwOld);
  } G_STMT_END;
#else // !G_OS_WINDOWS
  mprotect (buf, bufsz, PROT_READ | PROT_EXEC);
#endif // G_OS_WINDOWS
  dasm_free (Dst);

  {
    g_file_set_contents ("../program.bin", buf, bufsz, &tmp_err);
    if (G_UNLIKELY (tmp_err != NULL))
      {
        g_warning
        ("(%s): %s: %i: %s",
         G_STRLOC,
         g_quark_to_string
         (tmp_err->domain),
         tmp_err->code,
         tmp_err->message);
        g_error_free (tmp_err);
      }
  }
return labels [globl_main];
}

void
libjit_free (MatcalCFunction function)
{
  gpointer buf = function;
  size_t bufsz = 0;

  buf -= sizeof (guint32);
  bufsz = *(guint32*) buf;
#ifdef G_OS_WINDOWS
  VirtualFree (buf, bufsz, 0);
#else // !G_OS_WINDOWS
  munmap (buf, bufsz);
#endif // G_OS_WINDOWS
}
