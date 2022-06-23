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

static void
matcal_nil_matcal_clonable_iface (MatcalClonableIface* iface);

#define MATCAL_NIL_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), MATCAL_TYPE_NIL, MatcalNilClass))
#define MATCAL_IS_NIL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MATCAL_TYPE_NIL))
#define MATCAL_NIL_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), MATCAL_TYPE_NIL, MatcalNilClass))
typedef struct _MatcalNilClass MatcalNilClass;
typedef union  _Chain Chain;

struct _MatcalObjectPrivate
{
  union _Chain
  {
    GList list_;
    struct
    {
      MatcalObject* link;
      Chain* next;
      Chain* prev;
    };
  } chain;
};

struct _MatcalNil
{
  MatcalObject parent;
};

struct _MatcalNilClass
{
  MatcalObjectClass parent;
};

G_STATIC_ASSERT (G_STRUCT_OFFSET (Chain, link) == G_STRUCT_OFFSET (GList, data));
G_STATIC_ASSERT (G_STRUCT_OFFSET (Chain, next) == G_STRUCT_OFFSET (GList, next));
G_STATIC_ASSERT (G_STRUCT_OFFSET (Chain, prev) == G_STRUCT_OFFSET (GList, prev));

G_DEFINE_FINAL_TYPE_WITH_CODE
(MatcalNil,
 matcal_nil,
 MATCAL_TYPE_OBJECT,
 G_IMPLEMENT_INTERFACE
 (MATCAL_TYPE_CLONABLE,
  matcal_nil_matcal_clonable_iface));

static void
matcal_object_class_init (MatcalObjectClass* klass);
static void
matcal_object_init (MatcalObject* self);

static gint matcal_object_private_offset = 0;

static inline gpointer
matcal_object_get_instance_private (MatcalObject* self)
{
	return G_STRUCT_MEMBER_P (self, matcal_object_private_offset);
}

GType
matcal_object_get_type (void)
{
  static gsize __typeid__ = 0;
  if (g_once_init_enter (&__typeid__))
    {
      GType gtype;

      static const
      GTypeInfo __info__ =
      {
        sizeof (MatcalObjectClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) matcal_object_class_init,
        (GClassFinalizeFunc) NULL,
        NULL,
        sizeof (MatcalObject),
        0,
        (GInstanceInitFunc) matcal_object_init,
      };

      static const
      GTypeFundamentalInfo __fundamental__ =
      {
        (G_TYPE_FLAG_CLASSED
        | G_TYPE_FLAG_INSTANTIATABLE
        | G_TYPE_FLAG_DERIVABLE
        | G_TYPE_FLAG_DEEP_DERIVABLE),
      };

      gtype =
      g_type_register_fundamental
      (g_type_fundamental_next (),
       "MatcalObject",
       &__info__,
       &__fundamental__,
       G_TYPE_FLAG_ABSTRACT);

      matcal_object_private_offset =
      g_type_add_instance_private (gtype, sizeof (MatcalObjectPrivate));
      g_once_init_leave (&__typeid__, gtype);
    }
return (GType) __typeid__;
}

static void
matcal_object_class_finalize (MatcalObject* self)
{
}

static void
matcal_object_class_init (MatcalObjectClass* klass)
{
  g_type_class_adjust_private_offset (klass, & matcal_object_private_offset);
  klass->finalize = matcal_object_class_finalize;
}

static void
matcal_object_init (MatcalObject* self)
{
  self->priv =
  matcal_object_get_instance_private (self);
  g_atomic_ref_count_init (& self->ref_count);

  self->priv->chain.link = self;
  self->priv->chain.next = NULL;
  self->priv->chain.prev = NULL;
}

static MatcalClonable*
matcal_nil_matcal_clonable_iface_clone (MatcalClonable* pself)
{
  return matcal_object_new (MATCAL_TYPE_NIL);
}

static void
matcal_nil_matcal_clonable_iface (MatcalClonableIface* iface)
{
  iface->clone = matcal_nil_matcal_clonable_iface_clone;
}

static void
matcal_nil_class_init (MatcalNilClass* klass)
{
}

static void
matcal_nil_init (MatcalNil* self)
{
}

/* core API */

/**
 * matcal_object_new: (skip)
 * @gtype: the type id of the #MatcalObject subtype to instantiate.
 *
 * Creates a new instance of a #MatcalObject subtype and sets its properties.
 *
 * Returns: (transfer full) (type Matcal.Object): a new instance of @gtype.
 */
gpointer
matcal_object_new (GType gtype)
{
  g_return_val_if_fail (g_type_is_a (gtype, MATCAL_TYPE_OBJECT), NULL);
return (gpointer) g_type_create_instance (gtype);
}

/**
 * matcal_object_ref:
 * @object: (type Matcal.Object): a #MatcalObject.
 *
 * Increases the reference count of @object.
 *
 * Since GLib 2.56, if `GLIB_VERSION_MAX_ALLOWED` is 2.56 or greater, the type
 * of @object will be propagated to the return type (using the GCC typeof()
 * extension), so any casting the caller needs to do on the return type must be
 * explicit.
 *
 * Returns: (type Matcal.Object) (transfer none): the same @object
 */
gpointer
(matcal_object_ref) (gpointer object)
{
  g_return_val_if_fail (MATCAL_IS_OBJECT (object), NULL);
  g_atomic_ref_count_inc (& ((MatcalObject*) object)->ref_count );
return object;
}

/**
 * matcal_object_unref:
 * @object: (type Matcal.Object): a #MatcalObject.
 *
 * Decreases the reference count of @object. When its reference count
 * drops to 0, the object is finalized (i.e. its memory is freed).
 *
 * If the pointer to the #MatcalObject may be reused in future (for example, if it is
 * an instance variable of another object), it is recommended to clear the
 * pointer to %NULL rather than retain a dangling pointer to a potentially
 * invalid #MatcalObject instance. Use g_clear_object() for this.
 */
void
(matcal_object_unref) (gpointer object)
{
  g_return_if_fail (MATCAL_IS_OBJECT (object));
  MatcalObject* self = (MatcalObject*) object;
  if (g_atomic_ref_count_dec (& self->ref_count))
    {
      MATCAL_OBJECT_GET_CLASS (object)->finalize (self);
      g_type_free_instance ((GTypeInstance*) self);
    }
}

static inline
GList*
_list_append_link (GList* list, GList* new_list)
{
  GList *last;
  new_list->next = NULL;

  if (list)
    {
      last = g_list_last (list);
#if DEBUG == 1
      g_assert (last != NULL);
#endif // DEBUG
      last->next = new_list;
      new_list->prev = last;

      return list;
    }
  else
    {
      new_list->prev = NULL;
      return new_list;
    }
}

static inline
GList*
_list_prepend_link (GList* list, GList* new_list)
{
  new_list->next = list;
  
  if (list)
    {
      new_list->prev = list->prev;
      if (list->prev)
        list->prev->next = new_list;
      list->prev = new_list;
    }
  else
    {
      new_list->prev = NULL;
    }
return new_list;
}

static inline
GList*
_list_insert_link (GList* list, GList* new_list, gint position)
{
  GList *tmp_list;

  if (position < 0)
    return _list_append_link (list, new_list);
  else if (position == 0)
    return _list_prepend_link (list, new_list);

  tmp_list = g_list_nth (list, position);
  if (!tmp_list)
    return _list_append_link (list, new_list);

  new_list->prev = tmp_list->prev;
  tmp_list->prev->next = new_list;
  new_list->next = tmp_list;
  tmp_list->prev = new_list;
return list;
}

/* queue API */

gpointer
(matcal_object_append) (gpointer head, gpointer link)
{
  g_return_val_if_fail (head == NULL || MATCAL_IS_OBJECT (head), NULL);
  g_return_val_if_fail (MATCAL_IS_OBJECT (link), NULL);
  MatcalObjectPrivate* phead = (head != NULL) ? ((MatcalObject*) head)->priv : NULL;
  MatcalObjectPrivate* plink = ((MatcalObject*) link)->priv;
  GList* newhead = NULL;

  g_return_val_if_fail (plink->chain.next == NULL, NULL);
  g_return_val_if_fail (plink->chain.prev == NULL, NULL);
  link = matcal_object_ref (link);

  newhead =
  _list_append_link
  ((head != NULL) ? &(phead->chain.list_) : NULL,
   &(plink->chain.list_));
return (newhead) ? newhead->data : NULL;
}

gpointer
(matcal_object_prepend) (gpointer head, gpointer link)
{
  g_return_val_if_fail (head == NULL || MATCAL_IS_OBJECT (head), NULL);
  g_return_val_if_fail (MATCAL_IS_OBJECT (link), NULL);
  MatcalObjectPrivate* phead = (head != NULL) ? ((MatcalObject*) head)->priv : NULL;
  MatcalObjectPrivate* plink = ((MatcalObject*) link)->priv;
  GList* newhead = NULL;

  g_return_val_if_fail (plink->chain.next == NULL, NULL);
  g_return_val_if_fail (plink->chain.prev == NULL, NULL);
  link = matcal_object_ref (link);

  newhead =
  _list_prepend_link
  ((head != NULL) ? &(phead->chain.list_) : NULL,
   &(plink->chain.list_));
return (newhead) ? newhead->data : NULL;
}

gpointer
(matcal_object_insert) (gpointer head, gpointer link, gint position)
{
  g_return_val_if_fail (head == NULL || MATCAL_IS_OBJECT (head), NULL);
  g_return_val_if_fail (MATCAL_IS_OBJECT (link), NULL);
  MatcalObjectPrivate* phead = (head != NULL) ? ((MatcalObject*) head)->priv : NULL;
  MatcalObjectPrivate* plink = ((MatcalObject*) link)->priv;
  GList* newhead = NULL;

  g_return_val_if_fail (plink->chain.next == NULL, NULL);
  g_return_val_if_fail (plink->chain.prev == NULL, NULL);
  link = matcal_object_ref (link);

  newhead =
  _list_insert_link
  ((head != NULL) ? &(phead->chain.list_) : NULL,
   &(plink->chain.list_),
   position);
return (newhead) ? newhead->data : NULL;
}

gpointer
(matcal_object_remove) (gpointer head, gpointer link)
{
  if (head == NULL) return NULL;
  g_return_val_if_fail (MATCAL_IS_OBJECT (head), NULL);
  g_return_val_if_fail (MATCAL_IS_OBJECT (link), NULL);
  MatcalObjectPrivate* phead = ((MatcalObject*) head)->priv;
  MatcalObjectPrivate *plink = ((MatcalObject*) link)->priv;
  GList* newhead = NULL;

  newhead =
  g_list_remove_link
  (&(phead->chain.list_),
   &(plink->chain.list_));
  matcal_object_unref (link);
return (newhead) ? newhead->data : NULL;
}

gpointer
(matcal_object_nth) (gpointer head, gint n)
{
  if (head == NULL) return NULL;
  g_return_val_if_fail (MATCAL_IS_OBJECT (head), NULL);
  MatcalObjectPrivate* phead = ((MatcalObject*) head)->priv;
  GList* newhead = g_list_nth (&(phead->chain.list_), n);
return (newhead) ? newhead->data : NULL;
}

gpointer
(matcal_object_reverse) (gpointer head)
{
  if (head == NULL) return NULL;
  g_return_val_if_fail (MATCAL_IS_OBJECT (head), NULL);
  MatcalObjectPrivate* phead = ((MatcalObject*) head)->priv;
  GList* newhead = g_list_reverse (&(phead->chain.list_));
return (newhead) ? newhead->data : NULL;
}

gpointer
(matcal_object_next) (gpointer head)
{
  if (head == NULL) return NULL;
  g_return_val_if_fail (MATCAL_IS_OBJECT (head), NULL);
  MatcalObjectPrivate* phead = ((MatcalObject*) head)->priv;
  Chain* next = phead->chain.next;
return (next) ? next->link : NULL;
}

gpointer
(matcal_object_prev) (gpointer head)
{
  if (head == NULL) return NULL;
  g_return_val_if_fail (MATCAL_IS_OBJECT (head), NULL);
  MatcalObjectPrivate* phead = ((MatcalObject*) head)->priv;
  Chain* prev = phead->chain.prev;
return (prev) ? prev->link : NULL;
}

gint
matcal_object_length (gpointer head)
{
  if (head == NULL) return 0;
  g_return_val_if_fail (MATCAL_IS_OBJECT (head), 0);
  MatcalObjectPrivate* phead = ((MatcalObject*) head)->priv;
return g_list_length (&(phead->chain.list_));
}
