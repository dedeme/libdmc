// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "iarr_tests.h"
#include <assert.h>
#include "dmc/Varr.h"

static int greater(double *e1, double *e2) {
  return *e1 > *e2;
}

void varr_tests(void) {
  puts("Varr tests");
  // Varr[double]
  Varr *ia = varr_2_new(1);

  double d1 = 1;
  double d2 = 2;
  double d3 = 3;
  double d33 = 33;
  double d101 = 101;
  double d110 = 110;

  assert(varr_size(ia) == 0);
  varr_push(ia, &d1);
  varr_push(ia, &d2);
  assert(*(double *)varr_get(ia, 0) == 1);
  assert(*(double *)varr_get(ia, 1) == 2);
  assert(varr_size(ia) == 2);

  Varr *ia2 = varr_new();
  varr_cat(ia, ia2);
  assert(*(double *)varr_get(ia, 0) == 1);
  assert(*(double *)varr_get(ia, 1) == 2);
  assert(varr_size(ia) == 2);

  varr_push(ia2, &d1);
  varr_push(ia2, &d2);
  varr_push(ia2, &d3);
  assert(*(double *)varr_get(ia2, 0) == 1);
  assert(*(double *)varr_get(ia2, 1) == 2);
  assert(*(double *)varr_get(ia2, 2) == 3);
  assert(varr_size(ia2) == 3);
  varr_cat(ia, ia2);
  assert(*(double *)varr_get(ia, 0) == 1);
  assert(*(double *)varr_get(ia, 1) == 2);
  assert(*(double *)varr_get(ia, 2) == 1);
  assert(*(double *)varr_get(ia, 3) == 2);
  assert(*(double *)varr_get(ia, 4) == 3);
  assert(varr_size(ia) == 5);

  double *sum = malloc(sizeof(double));
  *sum = 0;
  EACH(ia, double, n)
    *sum += *n;
  _EACH
  assert(*sum == 9);

  varr_set(ia, 4, &d33);
  varr_remove(ia, 1);
  assert(*(double *)varr_get(ia, 1) == 1);
  assert(*(double *)varr_get(ia, 3) == 33);
  varr_insert(ia, 0, &d110);
  assert(*(double *)varr_get(ia, 0) == 110);
  assert(*(double *)varr_get(ia, 2) == 1);
  assert(*(double *)varr_get(ia, 4) == 33);

  RANGE0(i, varr_size(ia))
    varr_set(ia, i, &d101);
  _RANGE

  varr_insert_arr(ia, 2, ia2);
  assert(varr_size(ia) == 8);
  assert(*(double *)varr_get(ia, 0) == 101);
  assert(*(double *)varr_get(ia, 2) == 1);
  assert(*(double *)varr_get(ia, 7) == 101);

  *sum = 0;
  EACH(ia, double, n)
    *sum += *n;
  _EACH
  assert(*sum = 511);

  varr_remove_range(ia, 5, 8);
  varr_sort(ia, (FGREATER) greater);
  varr_reverse(ia);

  assert(varr_size(ia) == 5);
  assert(*(double *)varr_get(ia, 0) == 101);
  assert(*(double *)varr_get(ia, 1) == 101);
  assert(*(double *)varr_get(ia, 2) == 3);
  assert(*(double *)varr_get(ia, 3) == 2);
  assert(*(double *)varr_get(ia, 4) == 1);

  varr_free(ia);
  ia = varr_new();
  varr_sort(ia, (FGREATER) greater);
  varr_reverse(ia);
  assert(varr_size(ia) == 0);

  free(sum);
  varr_free(ia2);
  varr_free(ia);
  puts("    Finished");
}
