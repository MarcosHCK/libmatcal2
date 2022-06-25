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
#ifndef __MATCAL_RULES__
#define __MATCAL_RULES__ 1
#include <glib-object.h>

#define MATCAL_TYPE_RULES (matcal_rules_get_type ())
#define MATCAL_RULES(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), MATCAL_TYPE_RULES, MatcalRules))
#define MATCAL_IS_RULES(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MATCAL_TYPE_RULES))
typedef struct _MatcalRules MatcalRules;

#if __cplusplus
extern "C" {
#endif // __cplusplus

GType
matcal_rules_get_type (void) G_GNUC_CONST;

MatcalRules*
matcal_rules_new ();
MatcalRules*
matcal_rules_new_default ();
gboolean
matcal_rules_register_function (MatcalRules* rules, const gchar* name, guint n_args);

#if __cplusplus
}
#endif // __cplusplus

#endif // __MATCAL_RULES__
