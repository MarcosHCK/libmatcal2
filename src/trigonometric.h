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
#ifndef __MATLIB_TRIGONOMETRIC__
#define __MATLIB_TRIGONOMETRIC__ 1
#include <libmatcal.h>

#if __cplusplus
extern "C" {
#endif // __cplusplus

int matlib_sin (MatcalCore* core);
int matlib_cos (MatcalCore* core);
int matlib_tan (MatcalCore* core);
int matlib_asin (MatcalCore* core);
int matlib_acos (MatcalCore* core);
int matlib_atan (MatcalCore* core);
int matlib_sinh (MatcalCore* core);
int matlib_cosh (MatcalCore* core);
int matlib_tanh (MatcalCore* core);
int matlib_asinh (MatcalCore* core);
int matlib_acosh (MatcalCore* core);
int matlib_atanh (MatcalCore* core);

#if __cplusplus
}
#endif // __cplusplus

#endif // __MATLIB_TRIGONOMETRIC__
