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
#include <libmatcal.h>
#include <libmatlib.h>
#include <libmatree.h>
#include <libast.h>
#include <glib.h>

typedef struct
{
  MatcalCore* core;
  MatreeRules* rules;
} Fixture;

#define GTESTROOT "/org/hck/libmatcal"
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
#define _g_free0(var) ((var == NULL) ? NULL : (var = (g_free (var), NULL)))

static void
fixture_set_up (Fixture* fixture, gconstpointer argument)
{
  fixture->core = matcal_core_new ();
  fixture->rules = matree_rules_new_default ();
}

static void
fixture_tear_down (Fixture* fixture, gconstpointer argument)
{
  _g_object_unref0 (fixture->core);
  _g_object_unref0 (fixture->rules);
}

/*
 * Tests
 *
 */

static void
matcal_test_ast_append (Fixture* fixture, gpointer shared)
{
  AstNode* node1 = ast_node_new ("node 1", 0);
  AstNode* node2 = ast_node_new ("node 2", 0);
  AstNode* node3 = ast_node_new ("node 3", 0);

  g_node_append ((gpointer) node3, (gpointer) node1);
  g_node_append ((gpointer) node3, (gpointer) node2);
  g_assert (G_NODE_IS_ROOT (node3));
  g_assert (g_node_get_root ((gpointer) node2) == (GNode*) node3);
  ast_node_destroy (node3);
}

static void
matcal_test_object_append (Fixture* fixture, gpointer shared)
{
  MatcalObject* object1 = NULL;
  MatcalObject* object2 = NULL;
  MatcalObject* object3 = NULL;
  MatcalObject* list = NULL;
  int nth = 0;

  object1 = matcal_object_new (MATCAL_TYPE_NIL);
  object2 = matcal_object_new (MATCAL_TYPE_NIL);
  object3 = matcal_object_new (MATCAL_TYPE_NIL);

  list = matcal_object_append (object1, object2);
  list = matcal_object_prepend (object1, object3);

  g_assert (matcal_object_nth (object3, 0) == object3);
  g_assert (matcal_object_nth (object3, 1) == object1);
  g_assert (matcal_object_nth (object3, 2) == object2);
  g_assert (matcal_object_nth (list, 0) == object3);
  g_assert (matcal_object_nth (list, 1) == object1);
  g_assert (matcal_object_nth (list, 2) == object2);
  g_assert (matcal_object_length (list) == 3);
}


static void
matcal_test_object_remove (Fixture* fixture, gpointer shared)
{
  MatcalObject* object1 = NULL;
  MatcalObject* object2 = NULL;
  MatcalObject* object3 = NULL;
  MatcalObject* list = NULL;
  int nth = 0;

  object1 = matcal_object_new (MATCAL_TYPE_NIL);
  object2 = matcal_object_new (MATCAL_TYPE_NIL);
  object3 = matcal_object_new (MATCAL_TYPE_NIL);

  list = matcal_object_append (object1, object2);
  list = matcal_object_prepend (object1, object3);
  list = matcal_object_remove (object3, object1);

  g_assert (matcal_object_nth (object3, 0) == object3);
  g_assert (matcal_object_nth (object3, 1) == object2);
  g_assert (matcal_object_nth (list, 0) == object3);
  g_assert (matcal_object_nth (list, 1) == object2);
  g_assert (matcal_object_length (list) == 2);
}

static void
matcal_test_core_push (Fixture* fixture, gpointer shared)
{
  g_assert (matcal_core_gettop (fixture->core) == 0);

  matcal_core_pushnil (fixture->core);
  g_assert (matcal_core_gettop (fixture->core) == 1);

  matcal_core_pushnil (fixture->core);
  g_assert (matcal_core_gettop (fixture->core) == 2);

  matcal_core_remove (fixture->core, 1);
  g_assert (matcal_core_gettop (fixture->core) == 1);

  matcal_core_settop (fixture->core, 0);
  g_assert (matcal_core_gettop (fixture->core) == 0);
}

static void
matcal_test_number_push (Fixture* fixture, gpointer shared)
{
  matcal_core_pushnumber_uint (fixture->core, 987);
  matcal_core_pushnumber_uint (fixture->core, 322);
  matcal_core_pushnumber_uint (fixture->core, 111);

  g_assert (matcal_core_tonumber_uint (fixture->core, -1) == 111);
  g_assert (matcal_core_tonumber_uint (fixture->core, -2) == 322);
  g_assert (matcal_core_tonumber_uint (fixture->core,  0) == 987);
  g_assert (matcal_core_tonumber_uint (fixture->core,  1) == 322);
  g_assert (matcal_core_tonumber_uint (fixture->core,  2) == 111);
}

static void
matcal_test_number_pushstring (Fixture* fixture, gpointer shared)
{
  matcal_core_pushnumber_string (fixture->core, "987", 10);
  g_assert (matcal_core_tonumber_uint (fixture->core,  0) == 987);
  matcal_core_pop (fixture->core, 1);

  matcal_core_pushnumber_string (fixture->core, "9.87", 10);
  g_assert (matcal_core_tonumber_double (fixture->core,  0) == 9.87);
  matcal_core_pop (fixture->core, 1);
}

static void
matcal_test_number_calculate (Fixture* fixture, gpointer shared)
{
  matcal_core_pushcfunction (fixture->core, matlib_add);
  matcal_core_pushnumber_uint (fixture->core, 20);
  matcal_core_pushcfunction (fixture->core, matlib_mul);
  matcal_core_pushnumber_uint (fixture->core, 10);
  matcal_core_pushnumber_uint (fixture->core, 10);

  matcal_core_call (fixture->core, 2, 1);
  matcal_core_call (fixture->core, 2, 1);

  g_assert (matcal_core_gettop (fixture->core) == 1);
  g_assert (matcal_core_tonumber_uint (fixture->core, -1) == 120);
  matcal_core_pop (fixture->core, 1);
}

static int
testclosure (MatcalCore* core)
{
  matcal_core_pushupvalue (core, 0);
  matcal_core_pushupvalue (core, 1);

  g_assert (matcal_core_gettop (core) == 2);
  g_assert (matcal_core_tonumber_uint (core, -2) == 12);
  g_assert (matcal_core_tonumber_uint (core, -1) == 10);
return 0;
}

static void
matcal_test_closure_push (Fixture* fixture, gpointer shared)
{
  matcal_core_pushnumber_uint (fixture->core, 21);
  matcal_core_pushnumber_uint (fixture->core, 12);
  matcal_core_pushnumber_uint (fixture->core, 10);
  matcal_core_pushclosure (fixture->core, testclosure, 2);

  g_assert (matcal_core_gettop (fixture->core) == 2);
  g_assert (matcal_core_tonumber_uint (fixture->core, -2) == 21);
  matcal_core_call (fixture->core, 0, 0);

  g_assert (matcal_core_gettop (fixture->core) == 1);
  g_assert (matcal_core_tonumber_uint (fixture->core, -1) == 21);
}

static void
matcal_test_closure_clone (Fixture* fixture, gpointer shared)
{
  matcal_core_pushnumber_uint (fixture->core, 21);
  matcal_core_pushnumber_uint (fixture->core, 12);
  matcal_core_pushnumber_uint (fixture->core, 10);

  matcal_core_pushclosure (fixture->core, testclosure, 2);
  matcal_core_pushvalue (fixture->core, -1);
  matcal_core_remove (fixture->core, -2);

  g_assert (matcal_core_gettop (fixture->core) == 2);
  g_assert (matcal_core_tonumber_uint (fixture->core, -2) == 21);
  matcal_core_call (fixture->core, 0, 0);

  g_assert (matcal_core_gettop (fixture->core) == 1);
  g_assert (matcal_core_tonumber_uint (fixture->core, -1) == 21);
}

static void
matcal_test_expression_parse (Fixture* fixture, gpointer shared)
{
  MatreeExpression* exp = NULL;
  GError* tmp_err = NULL;

  exp = matree_expression_new (fixture->rules, "3+4*2 /(1\r-\n5)^2^3", &tmp_err);
  g_assert_no_error (tmp_err);
  _g_object_unref0 (exp);

  exp = matree_expression_new (fixture->rules, "sin ( max ( 2, 3 ) / 3 * p )", &tmp_err);
  g_assert_no_error (tmp_err);
  _g_object_unref0 (exp);
}

/*
 * Main
 *
 */

int main(int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);
  gpointer shared = NULL;

#define test_add(path,ftest) \
  G_STMT_START { \
    g_test_add \
    ((path), \
     Fixture, NULL, \
     fixture_set_up, \
     (void (*) (Fixture*, gconstpointer)) \
     (ftest), \
     fixture_tear_down); \
  } G_STMT_END

  test_add (GTESTROOT "/ast/append", matcal_test_ast_append);
  test_add (GTESTROOT "/object/append", matcal_test_object_append);
  test_add (GTESTROOT "/object/remove", matcal_test_object_remove);
  test_add (GTESTROOT "/core/push", matcal_test_core_push);
  test_add (GTESTROOT "/number/push", matcal_test_number_push);
  test_add (GTESTROOT "/number/pushstring", matcal_test_number_pushstring);
  test_add (GTESTROOT "/number/calculate", matcal_test_number_calculate);
  test_add (GTESTROOT "/closure/push", matcal_test_closure_push);
  test_add (GTESTROOT "/closure/clone", matcal_test_closure_clone);
  test_add (GTESTROOT "/expression/parse", matcal_test_expression_parse);
return g_test_run ();
}
