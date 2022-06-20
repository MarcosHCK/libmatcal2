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
#include <coreext.h>
#include <numberext.h>

G_DEFINE_QUARK
(matcal-number-error-quark,
 matcal_number_error);

static void
matcal_number_matcal_clonable_iface (MatcalClonableIface* iface);
static inline MatcalNumber*
matcal_number_new (MatcalNumberKind kind);

G_DEFINE_TYPE_WITH_CODE
(MatcalNumber,
 matcal_number,
 MATCAL_TYPE_OBJECT,
 G_IMPLEMENT_INTERFACE
 (MATCAL_TYPE_CLONABLE,
  matcal_number_matcal_clonable_iface)
 G_ADD_PRIVATE (MatcalNumber));

static MatcalClonable*
matcal_number_matcal_clonable_iface_clone (MatcalClonable* pself)
{
  MatcalNumber* self1 = MATCAL_NUMBER (pself);
  MatcalNumberPrivate* priv1 = self1->priv;
  MatcalNumber* self2 = matcal_number_new (priv1->kind);
  MatcalNumberPrivate* priv2 = self2->priv;

  switch (priv1->kind)
  {
  case MATCAL_NUMBER_KIND_INTEGER:
    mpz_set (priv2->integer, priv1->integer);
    break;
  case MATCAL_NUMBER_KIND_RATIONAL:
    mpq_set (priv2->rational, priv1->rational);
    break;
  case MATCAL_NUMBER_KIND_REAL:
    mpf_set (priv2->real, priv1->real);
    break;
  }
return (MatcalClonable*) self2;
}

static void
matcal_number_matcal_clonable_iface (MatcalClonableIface* iface)
{
  iface->clone = matcal_number_matcal_clonable_iface_clone;
}

static void
matcal_number_class_finalize (MatcalObject* pself)
{
  MatcalNumber* self = MATCAL_NUMBER (pself);
  MatcalNumberPrivate* priv = self->priv;
  switch (priv->kind)
  {
  case MATCAL_NUMBER_KIND_INTEGER:
    mpz_clear (priv->integer);
    break;
  case MATCAL_NUMBER_KIND_RATIONAL:
    mpq_clear (priv->rational);
    break;
  case MATCAL_NUMBER_KIND_REAL:
    mpf_clear (priv->real);
    break;
  }
MATCAL_OBJECT_CLASS (matcal_number_parent_class)->finalize (pself);
}

static void
matcal_number_class_init (MatcalNumberClass* klass)
{
  MatcalObjectClass* oclass = MATCAL_OBJECT_CLASS (klass);
  oclass->finalize = matcal_number_class_finalize;
}

static void
matcal_number_init (MatcalNumber* self)
{
  self->priv = matcal_number_get_instance_private (self);

  static gsize gmp_once = 0;
  if (g_once_init_enter (&gmp_once))
    {
      mp_set_memory_functions
      ((void* (*) (size_t))
       g_malloc,
       (void* (*) (void*, size_t, size_t))
       g_realloc,
       (void (*) (void*, size_t))
       g_free);
      g_once_init_leave (&gmp_once, TRUE);
    }
}

/* public API */

static inline MatcalNumber*
matcal_number_new (MatcalNumberKind kind)
{
  MatcalNumber* number;
  number = matcal_object_new (MATCAL_TYPE_NUMBER);
  number->priv->kind = kind;

  switch (kind)
  {
  case MATCAL_NUMBER_KIND_INTEGER:
    mpz_init (number->priv->integer);
    break;
  case MATCAL_NUMBER_KIND_RATIONAL:
    mpq_init (number->priv->rational);
    break;
  case MATCAL_NUMBER_KIND_REAL:
    mpf_init (number->priv->real);
    break;
  }
return number;
}

/**
 * matcal_core_pushnumber_uint:
 * @core: #MatcalNumber instance.
 * @value: numeric value.
 *
 * Pushes a numberic value onto stack
 * with value @value.
 */
void
matcal_core_pushnumber_uint (MatcalCore* core, unsigned int value)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  MatcalNumber* n = matcal_number_new (MATCAL_NUMBER_KIND_INTEGER);
  mpz_set_ui (n->priv->integer, value);
  _matcal_core_push (core, n);
  matcal_object_unref (n);
}

/**
 * matcal_core_tonumber_uint:
 * @core: #MatcalNumber instance.
 * @index: which value to take from.
 *
 * Converts value at index @index to
 * an unsigned int value an return it.
 *
 * Returns: see description.
 */
unsigned int
matcal_core_tonumber_uint (MatcalCore* core, int index)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), -1);
  g_return_val_if_fail ((index = validate_index (index)) >= 0, -1);
  MatcalNumber* number = _matcal_core_peek (core, index);
  g_return_val_if_fail (MATCAL_IS_NUMBER (number), -1);

  switch (number->priv->kind)
  {
  case MATCAL_NUMBER_KIND_INTEGER:
    return mpz_get_ui (number->priv->integer);
  case MATCAL_NUMBER_KIND_RATIONAL:
#if DEBUG == 1
    g_warning ("Loosing precision on conversion");
#endif // DEBUG
    {
      unsigned int result;
      mpz_t mpz;

      mpz_init (mpz);
      mpz_set_q (mpz, number->priv->rational);
      result = mpz_get_ui (mpz);
      mpz_clear (mpz);
      return result;
    }
    break;
  case MATCAL_NUMBER_KIND_REAL:
#if DEBUG == 1
    g_warning ("Loosing precision on conversion");
#endif // DEBUG
    return mpf_get_ui (number->priv->real);
  }
}

/**
 * matcal_core_pushnumber_double:
 * @core: #MatcalNumber instance.
 * @value: numeric value.
 *
 * Pushes a numberic value onto stack
 * with value @value.
 */
void
matcal_core_pushnumber_double (MatcalCore* core, double value)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  MatcalNumber* n = matcal_number_new (MATCAL_NUMBER_KIND_REAL);
  mpf_set_d (n->priv->real, value);
  _matcal_core_push (core, n);
  matcal_object_unref (n);
}

/**
 * matcal_core_tonumber_double:
 * @core: #MatcalNumber instance.
 * @index: which value to take from.
 *
 * Converts value at index @index to
 * an double value an return it.
 *
 * Returns: see description.
 */
double
matcal_core_tonumber_double (MatcalCore* core, int index)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), -1);
  g_return_val_if_fail ((index = validate_index (index)) >= 0, -1);
  MatcalNumber* number = _matcal_core_peek (core, index);
  g_return_val_if_fail (MATCAL_IS_NUMBER (number), -1);

  switch (number->priv->kind)
  {
  case MATCAL_NUMBER_KIND_INTEGER:
    return mpz_get_d (number->priv->integer);
  case MATCAL_NUMBER_KIND_RATIONAL:
    return mpq_get_d (number->priv->rational);
  case MATCAL_NUMBER_KIND_REAL:
    return mpf_get_d (number->priv->real);
  }
}

/**
 * matcal_core_pushnumber_string:
 * @core: #MatcalNumber instance.
 * @value: numeric value.
 *
 * Pushes a numberic value onto stack
 * with value @value.
 */
gboolean
matcal_core_pushnumber_string (MatcalCore* core, const gchar* value, int base)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), FALSE);
  g_return_val_if_fail (value != NULL, FALSE);

  size_t i, length = strlen (value);
  g_return_val_if_fail (g_utf8_validate (value, length, NULL), FALSE);

  const gchar* val = value;
  MatcalNumber* number = NULL;
  mpz_ptr num = NULL, den = NULL;
  gchar* next = NULL;
  int success = 0;
  gunichar c;

  do
  {
    c = g_utf8_get_char (val);
    if (c == 0)
      break;

    switch (c)
    {
    case '.':
      {
        number = matcal_number_new (MATCAL_NUMBER_KIND_RATIONAL);
        num = mpq_numref (number->priv->rational);
        den = mpq_denref (number->priv->rational);

        /*
         * Numerator
         *
         */

        length -= g_utf8_skip[*(guchar*) val];
        next = g_malloc (length + 1);
        i = (size_t) (val - value);
        memset (next, '.', length);
        memcpy (&(next[0]), value, i);
        memcpy (&(next[i]), g_utf8_next_char (val), length - i);
        next[length] = 0;

        success = mpz_set_str (num, next, base);
        if (G_UNLIKELY (success < 0))
          {
            matcal_object_unref (number);
            g_free (next);
            return FALSE;
          }

        /*
         * Denominator
         *
         */

        length = g_utf8_strlen (val, -1);
        next = g_realloc (next, length + 1);
        memset (&(next[1]), '0', length - 1);
        next[0] = '1'; next[length] = 0;

        success = mpz_set_str (den, next, base);
        if (G_UNLIKELY (success < 0))
          {
            matcal_object_unref (number);
            g_free (next);
            return FALSE;
          }
        
        /*
         * Canonicalize and push it
         *
         */

        mpq_canonicalize (number->priv->rational);
        _matcal_core_push (core, number);
        matcal_object_unref (number);
        g_free (next);
        return TRUE;
      }
      break;
    case '/':
      {
        number = matcal_number_new (MATCAL_NUMBER_KIND_RATIONAL);
        mpq_set_str (number->priv->rational, value, base);
        mpq_canonicalize (number->priv->rational);
        _matcal_core_push (core, number);
        matcal_object_unref (number);
        return TRUE;
      }
      break;
    }
  }
  while ((val = g_utf8_next_char (val)) != NULL);

  number = matcal_number_new (MATCAL_NUMBER_KIND_INTEGER);
  mpz_set_str (number->priv->integer, value, base);
  _matcal_core_push (core, number);
  matcal_object_unref (number);
return TRUE;
}

/**
 * matcal_core_tonumber_string:
 * @core: #MatcalNumber instance.
 * @index: which value to take from.
 *
 * Converts value at index @index to
 * an string value an return it.
 *
 * Returns: see description.
 */
gchar*
matcal_core_tonumber_string (MatcalCore* core, int index, int base)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), NULL);
  g_return_val_if_fail ((index = validate_index (index)) >= 0, NULL);
  MatcalNumber* number = _matcal_core_peek (core, index);
  g_return_val_if_fail (MATCAL_IS_NUMBER (number), NULL);

  switch (number->priv->kind)
  {
  case MATCAL_NUMBER_KIND_INTEGER:
    return mpz_get_str (NULL, base, number->priv->integer);
  case MATCAL_NUMBER_KIND_RATIONAL:
    return mpq_get_str (NULL, base, number->priv->rational);
  case MATCAL_NUMBER_KIND_REAL:
    return mpf_get_str (NULL, NULL, base, 0, number->priv->real);
  }
}

/**
 * matcal_core_isnumber:
 * @core: #MatcalCore instance.
 * @index: index to inspect on.
 *
 * See return section.
 *
 * Returns: if @index is a number.
 */
gboolean
matcal_core_isnumber (MatcalCore* core, int index)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), FALSE);
  g_return_val_if_fail ((index = validate_index (index)) >= 0, FALSE);
return MATCAL_IS_NUMBER (_matcal_core_peek (core, index));
}

/*
 * ext API
 *
 */

/**
 * matcal_core_pushnumber:
 * @core: #MatcalNumber instance.
 * @kind: numeric value kind.
 *
 * Pushes a numberic value onto stack.
 */
void
matcal_core_pushnumber (MatcalCore* core, MatcalNumberKind kind)
{
  g_return_if_fail (MATCAL_IS_CORE (core));
  MatcalNumber* n = matcal_number_new (kind);
  _matcal_core_push (core, n);
  matcal_object_unref (n);
}

/**
 * matcal_core_tonumber:
 * @core: #MatcalNumber instance.
 * @index: which value to take from.
 *
 * See return.
 *
 * Returns: (transfer none): number object at index @index.
 */
MatcalNumber*
matcal_core_tonumber (MatcalCore* core, int index)
{
  g_return_val_if_fail (MATCAL_IS_CORE (core), NULL);
  g_return_val_if_fail ((index = validate_index (index)) >= 0, FALSE);
  MatcalNumber* number = _matcal_core_peek (core, index);
  g_return_val_if_fail (MATCAL_IS_NUMBER (number), NULL);
return number;
}
