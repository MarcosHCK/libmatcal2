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
#include <clonable.h>
#include <object.h>

typedef struct _MatcalClonableIface MatcalClonableInterface;
static void
matcal_clonable_default_init (MatcalClonableIface* iface);

G_DEFINE_INTERFACE (MatcalClonable, matcal_clonable, MATCAL_TYPE_OBJECT);

static MatcalClonable*
matcal_clonable_default_clone (MatcalClonable* clonable)
{
  MatcalObject* object = MATCAL_OBJECT (clonable);
  return (MatcalClonable*) matcal_object_ref (object);
}

static void
matcal_clonable_default_init (MatcalClonableIface* iface)
{
  iface->clone = matcal_clonable_default_clone;
}

/*
 * API
 *
 */

/**
 * matcal_clonable_clone:
 * @object: a #MatcalClonable object
 * 
 * Clones @object instance.
 * 
 * Returns: (transfer full): a cloned #MatcalClonable instance.
 */
gpointer
(matcal_clonable_clone) (gpointer object)
{
  g_return_val_if_fail (MATCAL_IS_CLONABLE (object), NULL);
  MatcalClonableIface* iface = MATCAL_CLONABLE_GET_IFACE (object);
return (gpointer) iface->clone ((MatcalClonable*) object);
}
