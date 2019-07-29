// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "iarr_tests.h"
#include <assert.h>
#include "dmc/Darr.h"

void darr_tests(void) {
  puts("Darr tests");
  Darr *ia = darr_bf_new(1);

  assert(darr_size(ia) == 0);

  Js *js = darr_to_js(ia);
  Darr *ia2 = darr_from_js(js);
  assert(darr_eq(ia, ia2, 0.0001));

  ia2 = darr_copy(ia);
  assert(darr_eq(ia, ia2, 0.0001));

  ia2 = darr_left(ia, 0);
  assert(darr_eq(ia, ia2, 0.0001));

  ia2 = darr_right(ia, 0);
  assert(darr_eq(ia, ia2, 0.0001));

  ia2 = darr_sub(ia, 0, 0);
  assert(darr_eq(ia, ia2, 0.0001));

  darr_push(ia, 1.);
  darr_push(ia, 2);
  assert(darr_get(ia, 0) == 1);
  assert(darr_get(ia, 1) == 2);
  assert(darr_size(ia) == 2);

  js = darr_to_js(ia);
  ia2 = darr_from_js(js);
  assert(darr_eq(ia, ia2, 0.0001));

  ia2 = darr_copy(ia);
  assert(darr_eq(ia, ia2, 0.0001));

  ia2 = darr_left(ia, 1);
  assert(darr_get(ia2, 0) == 1);
  assert(darr_size(ia2) == 1);

  ia2 = darr_right(ia, 1);
  assert(darr_get(ia2, 0) == 2);
  assert(darr_size(ia2) == 1);

  ia2 = darr_sub(ia, 0, 2);
  assert(darr_eq(ia, ia2, 0.0001));

  ia2 = darr_sub(ia, 0, 1);
  assert(darr_get(ia2, 0) == 1);
  assert(darr_size(ia2) == 1);

  ia2 = darr_sub(ia, 1, 2);
  assert(darr_get(ia2, 0) == 2);
  assert(darr_size(ia2) == 1);

  ia2 = darr_new();
  darr_cat(ia, ia2);
  assert(darr_get(ia, 0) == 1);
  assert(darr_get(ia, 1) == 2);
  assert(darr_size(ia) == 2);

  darr_push(ia2, 1);
  darr_push(ia2, 2);
  darr_push(ia2, 3);
  assert(darr_get(ia2, 0) == 1);
  assert(darr_get(ia2, 1) == 2);
  assert(darr_get(ia2, 2) == 3);
  assert(darr_size(ia2) == 3);
  darr_cat(ia, ia2);
  assert(darr_get(ia, 0) == 1);
  assert(darr_get(ia, 1) == 2);
  assert(darr_get(ia, 2) == 1);
  assert(darr_get(ia, 3) == 2);
  assert(darr_get(ia, 4) == 3);
  assert(darr_size(ia) == 5);

  double *p = darr_start(ia);
  double *p_end = darr_end(ia);
  while(p < p_end) {
    *p = *p * 2;
    ++p;
  }

  double sum = 0;
  DEACH(ia, n)
    sum += n;
  _EACH
  assert(sum == 18);

  darr_set(ia, 4, 33);
  darr_remove(ia, 1);
  assert(darr_get(ia, 1) == 2);
  assert(darr_get(ia, 3) == 33);
  darr_insert(ia, 0, 110);
  assert(darr_get(ia, 0) == 110);
  assert(darr_get(ia, 2) == 2);
  assert(darr_get(ia, 4) == 33);

  RANGE0(i, darr_size(ia))
    darr_set(ia, i, 101);
  _RANGE

  darr_insert_arr(ia, 2, ia2);
  assert(darr_size(ia) == 8);
  assert(darr_get(ia, 0) == 101);
  assert(darr_get(ia, 2) == 1);
  assert(darr_get(ia, 7) == 101);

  sum = 0;
  DEACH(ia, n)
    sum += n;
  _EACH
  assert(sum == 511);

  darr_remove_range(ia, 5, 8);
  darr_sort(ia);
  darr_reverse(ia);

  assert(darr_size(ia) == 5);
  assert(darr_get(ia, 0) == 101);
  assert(darr_get(ia, 1) == 101);
  assert(darr_get(ia, 2) == 3);
  assert(darr_get(ia, 3) == 2);
  assert(darr_get(ia, 4) == 1);

  assert(darr_get(ia, darr_size(ia) - 1) == 1);
  assert(darr_get(ia, darr_size(ia) - 2) == 2);
  assert(darr_get(ia, darr_size(ia) - 3) == 3);
  assert(darr_get(ia, darr_size(ia) - 4) == 101);
  assert(darr_get(ia, darr_size(ia) - 5) == 101);

  ia = darr_new();
  darr_sort(ia);
  darr_reverse(ia);
  assert(darr_size(ia) == 0);

  puts("    Finished");
}
