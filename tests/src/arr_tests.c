// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "arr_tests.h"
#include <assert.h>
#include "dmc/Arr.h"
#include "dmc/DEFS.h"

static int greater(double *e1, double *e2) {
  return *e1 > *e2;
}

static double *double_new(Gc *gc, double n) {
  double *this = gc_add(gc, malloc(sizeof(double)));
  *this = n;
  return this;
}

void arr_tests(void) {
  puts("Arr tests");
  Gc *gc = gc_new();

  // Arr[double]
  Arr *ia = arr_new_bf(gc, 1);

  assert(arr_size(ia) == 0);
  arr_push(ia, double_new(gc, 1));
  arr_push(ia, double_new(gc, 2));
  assert(*(double *)arr_get(ia, 0) == 1);
  assert(*(double *)arr_get(ia, 1) == 2);
  assert(arr_size(ia) == 2);

  // Arr[double]
  Arr *ia2 = arr_new(gc);
  arr_cat(ia, ia2);
  assert(*(double *)arr_get(ia, 0) == 1);
  assert(*(double *)arr_get(ia, 1) == 2);
  assert(arr_size(ia) == 2);

  arr_push(ia2, double_new(gc, 1));
  arr_push(ia2, double_new(gc, 2));
  arr_push(ia2, double_new(gc, 3));
  assert(*(double *)arr_get(ia2, 0) == 1);
  assert(*(double *)arr_get(ia2, 1) == 2);
  assert(*(double *)arr_get(ia2, 2) == 3);
  assert(arr_size(ia2) == 3);
  arr_cat(ia, ia2);
  assert(*(double *)arr_get(ia, 0) == 1);
  assert(*(double *)arr_get(ia, 1) == 2);
  assert(*(double *)arr_get(ia, 2) == 1);
  assert(*(double *)arr_get(ia, 3) == 2);
  assert(*(double *)arr_get(ia, 4) == 3);
  assert(arr_size(ia) == 5);

  double **p = (double **)arr_start(ia);
  double **p_end = (double **)arr_end(ia);
  while(p < p_end) {
    double *tmp = double_new(gc, **p * 2);
    *p = tmp;
    ++p;
  }

  double *sum = gc_add(gc, malloc(sizeof(double)));
  *sum = 0;
  EACH(ia, double, n)
    *sum += *n;
  _EACH
  assert(*sum == 18);

  // Arr[double]
  Arr *acp = arr_copy(gc, ia);
  *sum = 0;
  EACH(acp, double, n)
    *sum += *n;
  _EACH
  assert(*sum == 18);

  arr_clear(acp);
  assert(arr_size(acp) == 0);


  arr_set(ia, 4, double_new(gc, 33));
  arr_remove(ia, 1);
  assert(*(double *)arr_get(ia, 1) == 2);
  assert(*(double *)arr_get(ia, 3) == 33);
  arr_insert(ia, 0, double_new(gc, 110));
  assert(*(double *)arr_get(ia, 0) == 110);
  assert(*(double *)arr_get(ia, 2) == 2);
  assert(*(double *)arr_get(ia, 4) == 33);

  RANGE0(i, arr_size(ia))
    arr_set(ia, i, double_new(gc, 101));
  _RANGE

  arr_insert_arr(ia, 2, ia2);
  assert(arr_size(ia) == 8);
  assert(*(double *)arr_get(ia, 0) == 101);
  assert(*(double *)arr_get(ia, 2) == 1);
  assert(*(double *)arr_get(ia, 3) == 2);
  assert(*(double *)arr_get(ia, 4) == 3);
  assert(*(double *)arr_get(ia, 7) == 101);

  arr_insert_arr(ia, 2, arr_new(gc));
  assert(arr_size(ia) == 8);
  assert(*(double *)arr_get(ia, 0) == 101);
  assert(*(double *)arr_get(ia, 2) == 1);
  assert(*(double *)arr_get(ia, 3) == 2);
  assert(*(double *)arr_get(ia, 4) == 3);
  assert(*(double *)arr_get(ia, 7) == 101);

  *sum = 0;
  EACH(ia, double, n)
    *sum += *n;
  _EACH
  assert(*sum == 511);

  *sum = 0;
  EACHR(ia, double, n)
    *sum += *n;
  _EACH
  assert(*sum == 511);

  arr_remove_range(ia, 5, 8);
  arr_sort(ia, (FCMP) greater);
  arr_reverse(ia);

  assert(arr_size(ia) == 5);
  assert(*(double *)arr_get(ia, 0) == 101);
  assert(*(double *)arr_get(ia, 1) == 101);
  assert(*(double *)arr_get(ia, 2) == 3);
  assert(*(double *)arr_get(ia, 3) == 2);
  assert(*(double *)arr_get(ia, 4) == 1);

  arr_remove_range(ia, 0, 0);
  arr_remove_range(ia, 4, 4);
  arr_remove_range(ia, 5, 5);

  assert(arr_size(ia) == 5);
  assert(*(double *)arr_get(ia, 0) == 101);
  assert(*(double *)arr_get(ia, 1) == 101);
  assert(*(double *)arr_get(ia, 2) == 3);
  assert(*(double *)arr_get(ia, 3) == 2);
  assert(*(double *)arr_get(ia, 4) == 1);

/*
  Js *double_to_js_new(double *d) {
    return js_wd(*d);
  }
  double *double_from_js_new(Js *js) {
    double *this = malloc(sizeof(double));
    *this = js_rd(js);
    return this;
  }
  Js *js = arr_to_js(ia, (FTO)double_to_js_new);
  // Arr[double]
  Arr *ia3 = arr_from_js(js, (FFROM)double_from_js_new);
  assert(arr_size(ia3) == 5);
  assert(*(double *)arr_get(ia3, 0) == 101);
  assert(*(double *)arr_get(ia3, 1) == 101);
  assert(*(double *)arr_get(ia3, 2) == 3);
  assert(*(double *)arr_get(ia3, 3) == 2);
  assert(*(double *)arr_get(ia3, 4) == 1);
*/

  ia = arr_new(gc);
  arr_sort(ia, (FCMP) greater);
  arr_reverse(ia);
  assert(arr_size(ia) == 0);

  gc_free(gc);
  puts("    Finished");
}
