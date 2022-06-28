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
#ifndef __MATCAL_NUMBER__
#define __MATCAL_NUMBER__ 1
#include <object.h>

#define MATCAL_NUMBER_ERROR (matcal_number_error_quark ())

typedef enum
{
  MATCAL_NUMBER_ERROR_FAILED,
  MATCAL_NUMBER_ERROR_PARSE,
} MatcalNumberError;

#define MATCAL_TYPE_NUMBER (matcal_number_get_type ())
#define MATCAL_NUMBER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), MATCAL_TYPE_NUMBER, MatcalNumber))
#define MATCAL_NUMBER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), MATCAL_TYPE_NUMBER, MatcalNumberClass))
#define MATCAL_IS_NUMBER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MATCAL_TYPE_NUMBER))
#define MATCAL_IS_NUMBER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MATCAL_TYPE_NUMBER))
#define MATCAL_NUMBER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), MATCAL_TYPE_NUMBER, MatcalNumberClass))

typedef struct _MatcalNumber MatcalNumber;
typedef struct _MatcalNumberPrivate MatcalNumberPrivate;
typedef struct _MatcalNumberClass MatcalNumberClass;

#if __cplusplus
extern "C" {
#endif // __cplusplus

GQuark
matcal_number_error_quark (void) G_GNUC_CONST;
GType
matcal_number_get_type (void) G_GNUC_CONST;

struct _MatcalNumber
{
  MatcalObject parent_instance;
  MatcalNumberPrivate* priv;
};

struct _MatcalNumberClass
{
  MatcalObjectClass parent_class;
};

int
matcal_number_cmp (MatcalNumber* number1, MatcalNumber* number2);

#if __cplusplus
}
#endif // __cplusplus

#endif // __MATCAL_NUMBER__
