// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "iarr_tests.h"
#include <assert.h>
#include "dmc/Arr.h"

static int greater(double *e1, double *e2) {
  return *e1 > *e2;
}

static double *double_new(double n) {
  double *this = malloc(sizeof(double));
  *this = n;
  return this;
}

static double *double_copy_new(double *n) {
  return double_new(*n);
}

void arr_tests(void) {
  puts("Arr tests");
  // Arr[double]
  Arr *ia = arr_bf_new(1, free);

  assert(arr_size(ia) == 0);
  arr_push(ia, double_new(1));
  arr_push(ia, double_new(2));
  assert(*(double *)arr_get(ia, 0) == 1);
  assert(*(double *)arr_get(ia, 1) == 2);
  assert(arr_size(ia) == 2);

  // Arr[double]
  Arr *ia2 = arr_new(free);
  arr_cat(ia, ia2, (FCOPY) double_copy_new);
  assert(*(double *)arr_get(ia, 0) == 1);
  assert(*(double *)arr_get(ia, 1) == 2);
  assert(arr_size(ia) == 2);

  arr_push(ia2, double_new(1));
  arr_push(ia2, double_new(2));
  arr_push(ia2, double_new(3));
  assert(*(double *)arr_get(ia2, 0) == 1);
  assert(*(double *)arr_get(ia2, 1) == 2);
  assert(*(double *)arr_get(ia2, 2) == 3);
  assert(arr_size(ia2) == 3);
  arr_cat(ia, ia2, (FCOPY) double_copy_new);
  assert(*(double *)arr_get(ia, 0) == 1);
  assert(*(double *)arr_get(ia, 1) == 2);
  assert(*(double *)arr_get(ia, 2) == 1);
  assert(*(double *)arr_get(ia, 3) == 2);
  assert(*(double *)arr_get(ia, 4) == 3);
  assert(arr_size(ia) == 5);

  double **p = (double **)arr_start(ia);
  double **p_end = (double **)arr_end(ia);
  while(p < p_end) {
    double *tmp = double_new(**p * 2);
    free(*p);
    *p = tmp;
    ++p;
  }

  double *sum = malloc(sizeof(double));
  *sum = 0;
  EACH(ia, double, n)
    *sum += *n;
  _EACH
  assert(*sum == 18);

  arr_set(ia, 4, double_new(33));
  arr_remove(ia, 1);
  assert(*(double *)arr_get(ia, 1) == 2);
  assert(*(double *)arr_get(ia, 3) == 33);
  arr_insert(ia, 0, double_new(110));
  assert(*(double *)arr_get(ia, 0) == 110);
  assert(*(double *)arr_get(ia, 2) == 2);
  assert(*(double *)arr_get(ia, 4) == 33);

  RANGE0(i, arr_size(ia))
    arr_set(ia, i, double_new(101));
  _RANGE

  arr_insert_arr(ia, 2, ia2, (FCOPY) double_copy_new);
  assert(arr_size(ia) == 8);
  assert(*(double *)arr_get(ia, 0) == 101);
  assert(*(double *)arr_get(ia, 2) == 1);
  assert(*(double *)arr_get(ia, 7) == 101);

  *sum = 0;
  EACH(ia, double, n)
    *sum += *n;
  _EACH
  assert(*sum = 511);

  arr_remove_range(ia, 5, 8);
  arr_sort(ia, (FGREATER) greater);
  arr_reverse(ia);

  assert(arr_size(ia) == 5);
  assert(*(double *)arr_get(ia, 0) == 101);
  assert(*(double *)arr_get(ia, 1) == 101);
  assert(*(double *)arr_get(ia, 2) == 3);
  assert(*(double *)arr_get(ia, 3) == 2);
  assert(*(double *)arr_get(ia, 4) == 1);

  assert(*(double *)arr_get(ia, -1) == 1);
  assert(*(double *)arr_get(ia, -2) == 2);
  assert(*(double *)arr_get(ia, -3) == 3);
  assert(*(double *)arr_get(ia, -4) == 101);
  assert(*(double *)arr_get(ia, -5) == 101);

  arr_free(ia);
  ia = arr_new(free);
  arr_sort(ia, (FGREATER) greater);
  arr_reverse(ia);
  assert(arr_size(ia) == 0);

  free(sum);
  arr_free(ia2);
  arr_free(ia);
  puts("    Finished");
}
