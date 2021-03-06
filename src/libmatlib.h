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
#ifndef __LIBMATLIB__
#define __LIBMATLIB__ 1
#include <libmatcal.h>

#include <arithmetics.h>
#include <generic.h>
#include <potency.h>
#include <trigonometric.h>

#if __cplusplus
extern "C" {
#endif // __cplusplus

void
matlib_setup (MatcalCore* core);

#if __cplusplus
}
#endif // __cplusplus

#endif // __LIBMATLIB__
