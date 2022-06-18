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
#ifndef __MATCAL_CLONABLE__
#define __MATCAL_CLONABLE__ 1
#include <glib-object.h>

#define MATCAL_TYPE_CLONABLE (matcal_clonable_get_type ())
#define MATCAL_CLONABLE(obj) (G_TYPE_CHECK_INSTANCE_CAST (obj, MATCAL_TYPE_CLONABLE, MatcalClonable))
#define MATCAL_IS_CLONABLE(obj) (G_TYPE_CHECK_INSTANCE_TYPE (obj, MATCAL_TYPE_CLONABLE))
#define MATCAL_CLONABLE_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE (obj, MATCAL_TYPE_CLONABLE, MatcalClonableIface))

typedef struct _MatcalClonable MatcalClonable;
typedef struct _MatcalClonableIface MatcalClonableIface;

#if __cplusplus
extern "C" {
#endif // __cplusplus

GType
matcal_clonable_get_type (void) G_GNUC_CONST;

struct _MatcalClonableIface
{
  GTypeInterface parent_iface;
  MatcalClonable* (*clone) (MatcalClonable* clonable);
};

/* public API */

gpointer
matcal_clonable_clone (gpointer object);

#if defined(glib_typeof)
/* Make reference APIs type safe with macros */
# define matcal_clonable_clone(obj) ((glib_typeof (obj)) (matcal_clonable_clone) (obj))
#endif

#if __cplusplus
}
#endif // __cplusplus

#endif // __MATCAL_CLONABLE__
