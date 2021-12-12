// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "aInt_tests.h"
#include <assert.h>
#include <stdio.h>
#include "dmc/AInt.h"

void aInt_tests(void) {
  puts("AInt tests");
  AInt *ia = aInt_bf_new(1);

  assert(aInt_size(ia) == 0);

  char *js = aInt_to_js(ia);
  AInt *ia2 = aInt_from_js(js);
  assert(aInt_eq(ia, ia2));

  ia2 = aInt_copy(ia);
  assert(aInt_eq(ia, ia2));

  ia2 = aInt_left(ia, 0);
  assert(aInt_eq(ia, ia2));

  ia2 = aInt_right(ia, 0);
  assert(aInt_eq(ia, ia2));

  ia2 = aInt_sub(ia, 0, 0);
  assert(aInt_eq(ia, ia2));

  aInt_push(ia, 1);
  aInt_push(ia, 2);
  assert(aInt_get(ia, 0) == 1);
  assert(aInt_get(ia, 1) == 2);
  assert(aInt_size(ia) == 2);

  js = aInt_to_js(ia);
  ia2 = aInt_from_js(js);
  assert(aInt_eq(ia, ia2));

  ia2 = aInt_copy(ia);
  assert(aInt_eq(ia, ia2));

  ia2 = aInt_left(ia, 1);
  assert(aInt_get(ia2, 0) == 1);
  assert(aInt_size(ia2) == 1);

  ia2 = aInt_right(ia, 1);
  assert(aInt_get(ia2, 0) == 2);
  assert(aInt_size(ia2) == 1);

  ia2 = aInt_sub(ia, 0, 2);
  assert(aInt_eq(ia, ia2));

  ia2 = aInt_sub(ia, 0, 1);
  assert(aInt_get(ia2, 0) == 1);
  assert(aInt_size(ia2) == 1);

  ia2 = aInt_sub(ia, 1, 2);
  assert(aInt_get(ia2, 0) == 2);
  assert(aInt_size(ia2) == 1);

  ia2 = aInt_new();
  aInt_cat(ia, ia2);
  assert(aInt_get(ia, 0) == 1);
  assert(aInt_get(ia, 1) == 2);
  assert(aInt_size(ia) == 2);

  aInt_push(ia2, 1);
  aInt_push(ia2, 2);
  aInt_push(ia2, 3);
  assert(aInt_get(ia2, 0) == 1);
  assert(aInt_get(ia2, 1) == 2);
  assert(aInt_get(ia2, 2) == 3);
  assert(aInt_size(ia2) == 3);
  aInt_cat(ia, ia2);
  assert(aInt_get(ia, 0) == 1);
  assert(aInt_get(ia, 1) == 2);
  assert(aInt_get(ia, 2) == 1);
  assert(aInt_get(ia, 3) == 2);
  assert(aInt_get(ia, 4) == 3);
  assert(aInt_size(ia) == 5);

  int *p = aInt_start(ia);
  int *p_end = aInt_end(ia);
  while(p < p_end) {
    *p = *p * 2;
    ++p;
  }

  int sum = 0;
  p = ia->es;
  while (p < ia->end) sum += *p++;
  assert(sum == 18);

  aInt_set(ia, 4, 33);
  aInt_remove(ia, 1);
  assert(aInt_get(ia, 1) == 2);
  assert(aInt_get(ia, 3) == 33);
  aInt_insert(ia, 0, 110);
  assert(aInt_get(ia, 0) == 110);
  assert(aInt_get(ia, 2) == 2);
  assert(aInt_get(ia, 4) == 33);

  for (int i = 0; i < aInt_size(ia); ++i) aInt_set(ia, i, 101);

  aInt_insert_arr(ia, 2, ia2);
  assert(aInt_size(ia) == 8);
  assert(aInt_get(ia, 0) == 101);
  assert(aInt_get(ia, 2) == 1);
  assert(aInt_get(ia, 7) == 101);

  sum = 0;
  p = ia->es;
  while (p < ia->end) sum += *p++;
  assert(sum == 511);

  aInt_remove_range(ia, 5, 8);
  aInt_sort(ia);
  aInt_reverse(ia);

  assert(aInt_size(ia) == 5);
  assert(aInt_get(ia, 0) == 101);
  assert(aInt_get(ia, 1) == 101);
  assert(aInt_get(ia, 2) == 3);
  assert(aInt_get(ia, 3) == 2);
  assert(aInt_get(ia, 4) == 1);

  assert(aInt_get(ia, aInt_size(ia) - 1) == 1);
  assert(aInt_get(ia, aInt_size(ia) - 2) == 2);
  assert(aInt_get(ia, aInt_size(ia) - 3) == 3);
  assert(aInt_get(ia, aInt_size(ia) - 4) == 101);
  assert(aInt_get(ia, aInt_size(ia) - 5) == 101);

  ia = aInt_new();
  aInt_sort(ia);
  aInt_reverse(ia);
  assert(aInt_size(ia) == 0);

  ia = aInt_new_c(3, (int[]){2, 0, 4});
  assert(aInt_size(ia) == 3);
  assert(aInt_get(ia, 0) == 2);
  assert(aInt_get(ia, 1) == 0);
  assert(aInt_get(ia, 2) == 4);

  puts("    Finished");
}
