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
#ifndef __MATCAL_COREEXT__
#define __MATCAL_COREEXT__ 1
#include <core.h>

#if !__LIBMATCAL_INSIDE__
# error "You shouldn't be using this header"
#endif // !__LIBMATCAL_INSIDE__

#if __cplusplus
extern "C" {
#endif // __cplusplus

#define validate_index(index) ((_matcal_core_checkidx)(core,(index)))

G_GNUC_INTERNAL
int
_matcal_core_checkidx (MatcalCore* core, int index);
G_GNUC_INTERNAL
void
_matcal_core_push (MatcalCore* core, gpointer object);
G_GNUC_INTERNAL
void
_matcal_core_pop (MatcalCore* core);
G_GNUC_INTERNAL
gpointer
_matcal_core_peek (MatcalCore* core, gint index);

#if __cplusplus
}
#endif // __cplusplus

#endif // __MATCAL_COREEXT__
