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
#ifndef __MATCAL_OBJECT__
#define __MATCAL_OBJECT__ 1
#include <glib-object.h>

#define MATCAL_TYPE_OBJECT (matcal_object_get_type ())
#define MATCAL_OBJECT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), MATCAL_TYPE_OBJECT, MatcalObject))
#define MATCAL_OBJECT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), MATCAL_TYPE_OBJECT, MatcalObjectClass))
#define MATCAL_IS_OBJECT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MATCAL_TYPE_OBJECT))
#define MATCAL_IS_OBJECT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MATCAL_TYPE_OBJECT))
#define MATCAL_OBJECT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), MATCAL_TYPE_OBJECT, MatcalObjectClass))

typedef struct _MatcalObject MatcalObject;
typedef struct _MatcalObjectPrivate MatcalObjectPrivate;
typedef struct _MatcalObjectClass MatcalObjectClass;

#define MATCAL_TYPE_NIL (matcal_nil_get_type ())
#define MATCAL_NIL(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), MATCAL_TYPE_NIL, MatcalNil))
#define MATCAL_IS_NIL(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MATCAL_TYPE_NIL))

typedef struct _MatcalNil MatcalNil;

#if __cplusplus
extern "C" {
#endif // __cplusplus

GType
matcal_object_get_type (void) G_GNUC_CONST;
GType
matcal_nil_get_type (void) G_GNUC_CONST;

struct _MatcalObject
{
  GTypeInstance parent;
  guint ref_count;

  /*<private>*/
  MatcalObjectPrivate* priv;
};

struct _MatcalObjectClass
{
  GTypeClass parent;
  void (*finalize) (MatcalObject* object);
};

/* core API */

gpointer
matcal_object_new (GType gtype);
gpointer
matcal_object_ref (gpointer object);
void
matcal_object_unref (gpointer object);

/* queue API */

gpointer
matcal_object_append (gpointer head, gpointer link) G_GNUC_WARN_UNUSED_RESULT;
gpointer
matcal_object_prepend (gpointer head, gpointer link) G_GNUC_WARN_UNUSED_RESULT;
gpointer
matcal_object_insert (gpointer head, gpointer link, gint position) G_GNUC_WARN_UNUSED_RESULT;
gpointer
matcal_object_remove (gpointer head, gpointer link) G_GNUC_WARN_UNUSED_RESULT;
gpointer
matcal_object_nth (gpointer head, gint n);
gpointer
matcal_object_next (gpointer head);
gpointer
matcal_object_prev (gpointer head);
gint
matcal_object_length (gpointer head);

/* glue code */

#if defined(glib_typeof)
/* Make reference APIs type safe with macros */
# define matcal_object_ref(obj) ((glib_typeof (obj)) (matcal_object_ref) (obj))
# define matcal_object_append(obj,link) ((glib_typeof (obj)) (matcal_object_append) (obj,link))
# define matcal_object_prepend(obj,link) ((glib_typeof (obj)) (matcal_object_prepend) (obj,link))
# define matcal_object_insert(obj,link,position) ((glib_typeof (obj)) (matcal_object_insert) (obj,link,position))
# define matcal_object_remove(obj,link) ((glib_typeof (obj)) (matcal_object_remove) (obj,link))
# define matcal_object_nth(obj, n) ((glib_typeof (obj)) (matcal_object_nth) (obj, n))
# define matcal_object_next(obj) ((glib_typeof (obj)) (matcal_object_next) (obj))
# define matcal_object_prev(obj) ((glib_typeof (obj)) (matcal_object_prev) (obj))
#endif

G_DEFINE_AUTOPTR_CLEANUP_FUNC (MatcalObject, matcal_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (MatcalNil, matcal_object_unref)

#if __cplusplus
}
#endif // __cplusplus

#endif // __MATCAL_OBJECT__
