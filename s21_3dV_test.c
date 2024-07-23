#include <check.h>

#include "parcer.h"

double v[] = {0,    0,   0,    -0.5, -0.5, 0.5,  0.5,  -0.5, 0.5,
              -0.5, 0.5, 0.5,  0.5,  0.5,  0.5,  -0.5, 0.5,  -0.5,
              0.5,  0.5, -0.5, -0.5, -0.5, -0.5, 0.5,  -0.5, -0.5};

unsigned int f[] = {1, 2, 2, 3, 3, 1, 3, 2, 2, 4, 4, 3, 3, 4, 4, 5, 5, 3,
                    5, 4, 4, 6, 6, 5, 5, 6, 6, 7, 7, 5, 7, 6, 6, 8, 8, 7,
                    7, 8, 8, 1, 1, 7, 1, 8, 8, 2, 2, 1, 2, 8, 8, 4, 4, 2,
                    4, 8, 8, 6, 6, 4, 7, 1, 1, 5, 5, 7, 5, 1, 1, 3, 3, 5};

START_TEST(s21_3dViewer_t00) {
  char file_path[] = "./obj/file123.obj";
  int f_res = ERR_NO_FILE;
  object_t obj;

  InitDataObject(&obj);
  int s21_res = FillDataFromFile(file_path, &obj);

  ck_assert_int_eq(f_res, s21_res);
  FreeObject(&obj);
}
END_TEST

/* START_TEST(s21_3dViewer_t01) {
  char file_path[] = "./obj/Lion.obj";
  int f_res = ERR_TOO_BIG;
  object_t obj;

  InitDataObject(&obj);
  int s21_res = FillDataFromFile(file_path, &obj);

  ck_assert_int_eq(f_res, s21_res);
  FreeObject(&obj);
}
END_TEST */

START_TEST(s21_3dViewer_t02) {
  char file_path[] = "./obj/cube2.obj";
  int f_res = ERR_NO_3D;
  object_t obj;

  InitDataObject(&obj);
  int s21_res = FillDataFromFile(file_path, &obj);

  ck_assert_int_eq(f_res, s21_res);
  FreeObject(&obj);
}
END_TEST

START_TEST(s21_3dViewer_t1) {
  char file_path[] = "./obj/cube.obj";
  int f_res = OK;
  object_t obj;

  int s21_res = FillDataFromFile(file_path, &obj);

  ck_assert_int_eq(f_res, s21_res);
  ck_assert_uint_eq(obj.number_vertex, 9);
  ck_assert_uint_eq(obj.amount_vertex, 9 * 3);
  ck_assert_uint_eq(obj.number_face, 12);
  ck_assert_int_eq(obj.amount_face, 12 * 3 * 2);
  for (unsigned int i = 0; i < obj.amount_vertex; i++) {
    ck_assert_double_eq(obj.value_vertex[i], v[i]);
  }

  for (int i = 0; i < obj.amount_face; i++) {
    ck_assert_uint_eq(obj.value_face[i], f[i]);
  }

  FreeObject(&obj);
}
END_TEST

START_TEST(s21_3dViewer_t2) {
  char file_path[] = "./obj/cube.obj";
  int f_res = OK;
  object_t obj;

  int s21_res = FillDataFromFile(file_path, &obj);

  Move_Obj(&obj, 2, 0);  // x
  for (unsigned int i = 0; i < obj.amount_vertex; i += 3) {
    ck_assert_double_eq(obj.value_vertex[i], v[i] + 0.02);
  }

  ck_assert_int_eq(f_res, s21_res);
  FreeObject(&obj);
}
END_TEST

START_TEST(s21_3dViewer_t21) {
  char file_path[] = "./obj/cube.obj";
  int f_res = OK;
  object_t obj;

  int s21_res = FillDataFromFile(file_path, &obj);

  Move_Obj(&obj, 20, 1);  // y
  for (unsigned int i = 0; i < obj.amount_vertex; i += 3) {
    v[i + 1] += 0.2;
  }

  for (unsigned int i = 0; i < obj.amount_vertex; i += 3) {
    ck_assert_double_eq(obj.value_vertex[i], v[i]);
  }

  ck_assert_int_eq(f_res, s21_res);
  FreeObject(&obj);
}
END_TEST

START_TEST(s21_3dViewer_t31) {
  char file_path[] = "./obj/cube.obj";
  int f_res = OK;
  object_t obj;
  double zoom = 20;
  double v1[] = {0,    0,   0,    -0.5, -0.5, 0.5,  0.5,  -0.5, 0.5,
                 -0.5, 0.5, 0.5,  0.5,  0.5,  0.5,  -0.5, 0.5,  -0.5,
                 0.5,  0.5, -0.5, -0.5, -0.5, -0.5, 0.5,  -0.5, -0.5};

  double def_v[] = {0,    0,   0,    -0.5, -0.5, 0.5,  0.5,  -0.5, 0.5,
                    -0.5, 0.5, 0.5,  0.5,  0.5,  0.5,  -0.5, 0.5,  -0.5,
                    0.5,  0.5, -0.5, -0.5, -0.5, -0.5, 0.5,  -0.5, -0.5};

  int s21_res = FillDataFromFile(file_path, &obj);

  for (unsigned int i = 0; i < obj.amount_vertex; i++) {
    v1[i] = v1[i] * zoom / 100;
  }

  Change_Scale(def_v, zoom, &obj);

  for (unsigned int i = 0; i < obj.amount_vertex; i++) {
    ck_assert_double_eq(obj.value_vertex[i], v1[i]);
  }

  ck_assert_int_eq(f_res, s21_res);
  FreeObject(&obj);
}
END_TEST

START_TEST(s21_3dViewer_t41) {
  char file_path[] = "./obj/cube.obj";
  int f_res = OK;
  object_t obj;

  int s21_res = FillDataFromFile(file_path, &obj);

  Rotation_X(&obj, 30.0);
  ck_assert_double_eq_tol(obj.value_vertex[4], -0.6830127019, 1e-6);
  ck_assert_double_eq_tol(obj.value_vertex[5], 0.1830127019, 1e-6);

  ck_assert_int_eq(f_res, s21_res);
  FreeObject(&obj);
}
END_TEST

START_TEST(s21_3dViewer_t42) {
  char file_path[] = "./obj/cube.obj";
  int f_res = OK;
  object_t obj;

  int s21_res = FillDataFromFile(file_path, &obj);

  Rotation_Y(&obj, 90);
  ck_assert_double_eq_tol(obj.value_vertex[3], 0.5, 1e-6);
  ck_assert_double_eq_tol(obj.value_vertex[5], 0.5, 1e-6);

  ck_assert_int_eq(f_res, s21_res);
  FreeObject(&obj);
}
END_TEST

START_TEST(s21_3dViewer_t43) {
  char file_path[] = "./obj/cube.obj";
  int f_res = OK;
  object_t obj;

  int s21_res = FillDataFromFile(file_path, &obj);

  Rotation_Z(&obj, 120);
  ck_assert_double_eq_tol(obj.value_vertex[3], 0.683012702, 1e-6);
  ck_assert_double_eq_tol(obj.value_vertex[4], -0.183012702, 1e-6);

  ck_assert_int_eq(f_res, s21_res);
  FreeObject(&obj);
}
END_TEST

Suite *s21_3dViewer_suite(void) {
  Suite *s;
  TCase *tc_core;
  s = suite_create("s21_3dViewer");
  tc_core = tcase_create("Core");

  // Core test case
  // load file with error
  tcase_add_test(tc_core, s21_3dViewer_t00);
  /*   tcase_add_test(tc_core, s21_3dViewer_t01); */
  tcase_add_test(tc_core, s21_3dViewer_t02);
  // parcer file
  tcase_add_test(tc_core, s21_3dViewer_t1);
  // move x
  tcase_add_test(tc_core, s21_3dViewer_t2);
  tcase_add_test(tc_core, s21_3dViewer_t21);
  // scale
  tcase_add_test(tc_core, s21_3dViewer_t31);
  // rotations
  tcase_add_test(tc_core, s21_3dViewer_t41);
  tcase_add_test(tc_core, s21_3dViewer_t42);
  tcase_add_test(tc_core, s21_3dViewer_t43);

  suite_add_tcase(s, tc_core);
  return s;
}

int main() {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = s21_3dViewer_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}