// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "iarr_tests.h"
#include <assert.h>
#include "dmc/Iarr.h"
#include "dmc/DEFS.h"

void iarr_tests(void) {
  puts("Iarr tests");
  Iarr *ia = iarr_bf_new(1);

  assert(iarr_size(ia) == 0);

  Js *js = iarr_to_js(ia);
  Iarr *ia2 = iarr_from_js(js);
  assert(iarr_eq(ia, ia2));

  ia2 = iarr_copy(ia);
  assert(iarr_eq(ia, ia2));

  ia2 = iarr_left(ia, 0);
  assert(iarr_eq(ia, ia2));

  ia2 = iarr_right(ia, 0);
  assert(iarr_eq(ia, ia2));

  ia2 = iarr_sub(ia, 0, 0);
  assert(iarr_eq(ia, ia2));

  iarr_push(ia, 1);
  iarr_push(ia, 2);
  assert(iarr_get(ia, 0) == 1);
  assert(iarr_get(ia, 1) == 2);
  assert(iarr_size(ia) == 2);

  js = iarr_to_js(ia);
  ia2 = iarr_from_js(js);
  assert(iarr_eq(ia, ia2));

  ia2 = iarr_copy(ia);
  assert(iarr_eq(ia, ia2));

  ia2 = iarr_left(ia, 1);
  assert(iarr_get(ia2, 0) == 1);
  assert(iarr_size(ia2) == 1);

  ia2 = iarr_right(ia, 1);
  assert(iarr_get(ia2, 0) == 2);
  assert(iarr_size(ia2) == 1);

  ia2 = iarr_sub(ia, 0, 2);
  assert(iarr_eq(ia, ia2));

  ia2 = iarr_sub(ia, 0, 1);
  assert(iarr_get(ia2, 0) == 1);
  assert(iarr_size(ia2) == 1);

  ia2 = iarr_sub(ia, 1, 2);
  assert(iarr_get(ia2, 0) == 2);
  assert(iarr_size(ia2) == 1);

  ia2 = iarr_new();
  iarr_cat(ia, ia2);
  assert(iarr_get(ia, 0) == 1);
  assert(iarr_get(ia, 1) == 2);
  assert(iarr_size(ia) == 2);

  iarr_push(ia2, 1);
  iarr_push(ia2, 2);
  iarr_push(ia2, 3);
  assert(iarr_get(ia2, 0) == 1);
  assert(iarr_get(ia2, 1) == 2);
  assert(iarr_get(ia2, 2) == 3);
  assert(iarr_size(ia2) == 3);
  iarr_cat(ia, ia2);
  assert(iarr_get(ia, 0) == 1);
  assert(iarr_get(ia, 1) == 2);
  assert(iarr_get(ia, 2) == 1);
  assert(iarr_get(ia, 3) == 2);
  assert(iarr_get(ia, 4) == 3);
  assert(iarr_size(ia) == 5);

  int *p = iarr_start(ia);
  int *p_end = iarr_end(ia);
  while(p < p_end) {
    *p = *p * 2;
    ++p;
  }

  int sum = 0;
  IEACH(ia, n)
    sum += n;
  _EACH
  assert(sum == 18);

  iarr_set(ia, 4, 33);
  iarr_remove(ia, 1);
  assert(iarr_get(ia, 1) == 2);
  assert(iarr_get(ia, 3) == 33);
  iarr_insert(ia, 0, 110);
  assert(iarr_get(ia, 0) == 110);
  assert(iarr_get(ia, 2) == 2);
  assert(iarr_get(ia, 4) == 33);

  RANGE0(i, iarr_size(ia))
    iarr_set(ia, i, 101);
  _RANGE

  iarr_insert_arr(ia, 2, ia2);
  assert(iarr_size(ia) == 8);
  assert(iarr_get(ia, 0) == 101);
  assert(iarr_get(ia, 2) == 1);
  assert(iarr_get(ia, 7) == 101);

  sum = 0;
  IEACH(ia, n)
    sum += n;
  _EACH
  assert(sum == 511);

  iarr_remove_range(ia, 5, 8);
  iarr_sort(ia);
  iarr_reverse(ia);

  assert(iarr_size(ia) == 5);
  assert(iarr_get(ia, 0) == 101);
  assert(iarr_get(ia, 1) == 101);
  assert(iarr_get(ia, 2) == 3);
  assert(iarr_get(ia, 3) == 2);
  assert(iarr_get(ia, 4) == 1);

  assert(iarr_get(ia, iarr_size(ia) - 1) == 1);
  assert(iarr_get(ia, iarr_size(ia) - 2) == 2);
  assert(iarr_get(ia, iarr_size(ia) - 3) == 3);
  assert(iarr_get(ia, iarr_size(ia) - 4) == 101);
  assert(iarr_get(ia, iarr_size(ia) - 5) == 101);

  ia = iarr_new();
  iarr_sort(ia);
  iarr_reverse(ia);
  assert(iarr_size(ia) == 0);

  puts("    Finished");
}
