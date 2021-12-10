// Copyright 04-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "aDouble_tests.h"
#include <assert.h>

void aDouble_tests(void) {
  puts("ADouble tests");
  ADouble *ad = aDouble_bf_new(1);

  assert(aDouble_size(ad) == 0);

  char *js = aDouble_to_js(ad);
  ADouble *ad2 = aDouble_from_js(js);
  assert(aDouble_eq(ad, ad2, 0.0001));

  ad2 = aDouble_copy(ad);
  assert(aDouble_eq(ad, ad2, 0.0001));

  ad2 = aDouble_left(ad, 0);
  assert(aDouble_eq(ad, ad2, 0.0001));

  ad2 = aDouble_right(ad, 0);
  assert(aDouble_eq(ad, ad2, 0.0001));

  ad2 = aDouble_sub(ad, 0, 0);
  assert(aDouble_eq(ad, ad2, 0.0001));

  aDouble_push(ad, 1.);
  aDouble_push(ad, 2);
  assert(aDouble_get(ad, 0) == 1);
  assert(aDouble_get(ad, 1) == 2);
  assert(aDouble_size(ad) == 2);

  js = aDouble_to_js(ad);
  ad2 = aDouble_from_js(js);
  assert(aDouble_eq(ad, ad2, 0.0001));

  ad2 = aDouble_copy(ad);
  assert(aDouble_eq(ad, ad2, 0.0001));

  ad2 = aDouble_left(ad, 1);
  assert(aDouble_get(ad2, 0) == 1);
  assert(aDouble_size(ad2) == 1);

  ad2 = aDouble_right(ad, 1);
  assert(aDouble_get(ad2, 0) == 2);
  assert(aDouble_size(ad2) == 1);

  ad2 = aDouble_sub(ad, 0, 2);
  assert(aDouble_eq(ad, ad2, 0.0001));

  ad2 = aDouble_sub(ad, 0, 1);
  assert(aDouble_get(ad2, 0) == 1);
  assert(aDouble_size(ad2) == 1);

  ad2 = aDouble_sub(ad, 1, 2);
  assert(aDouble_get(ad2, 0) == 2);
  assert(aDouble_size(ad2) == 1);

  ad2 = aDouble_new();
  aDouble_cat(ad, ad2);
  assert(aDouble_get(ad, 0) == 1);
  assert(aDouble_get(ad, 1) == 2);
  assert(aDouble_size(ad) == 2);

  aDouble_push(ad2, 1);
  aDouble_push(ad2, 2);
  aDouble_push(ad2, 3);
  assert(aDouble_get(ad2, 0) == 1);
  assert(aDouble_get(ad2, 1) == 2);
  assert(aDouble_get(ad2, 2) == 3);
  assert(aDouble_size(ad2) == 3);
  aDouble_cat(ad, ad2);
  assert(aDouble_get(ad, 0) == 1);
  assert(aDouble_get(ad, 1) == 2);
  assert(aDouble_get(ad, 2) == 1);
  assert(aDouble_get(ad, 3) == 2);
  assert(aDouble_get(ad, 4) == 3);
  assert(aDouble_size(ad) == 5);

  double *p = ad->es;
  double *p_end = aDouble_end(ad);
  while(p < p_end) {
    *p = *p * 2;
    ++p;
  }

  double sum = 0;
  p = ad->es;
  while (p < ad->end) sum += *p++;
  assert(sum == 18);

  aDouble_set(ad, 4, 33);
  aDouble_remove(ad, 1);
  assert(aDouble_get(ad, 1) == 2);
  assert(aDouble_get(ad, 3) == 33);
  aDouble_insert(ad, 0, 110);
  assert(aDouble_get(ad, 0) == 110);
  assert(aDouble_get(ad, 2) == 2);
  assert(aDouble_get(ad, 4) == 33);

  for (int i = 0; i  < aDouble_size(ad); ++i) aDouble_set(ad, i, 101);

  aDouble_insert_arr(ad, 2, ad2);
  assert(aDouble_size(ad) == 8);
  assert(aDouble_get(ad, 0) == 101);
  assert(aDouble_get(ad, 2) == 1);
  assert(aDouble_get(ad, 7) == 101);

  sum = 0;
  p = ad->es;
  while (p < ad->end) sum += *p++;
  assert(sum == 511);

  aDouble_remove_range(ad, 5, 8);
  aDouble_sort(ad);
  aDouble_reverse(ad);

  assert(aDouble_size(ad) == 5);
  assert(aDouble_get(ad, 0) == 101);
  assert(aDouble_get(ad, 1) == 101);
  assert(aDouble_get(ad, 2) == 3);
  assert(aDouble_get(ad, 3) == 2);
  assert(aDouble_get(ad, 4) == 1);

  assert(aDouble_get(ad, aDouble_size(ad) - 1) == 1);
  assert(aDouble_get(ad, aDouble_size(ad) - 2) == 2);
  assert(aDouble_get(ad, aDouble_size(ad) - 3) == 3);
  assert(aDouble_get(ad, aDouble_size(ad) - 4) == 101);
  assert(aDouble_get(ad, aDouble_size(ad) - 5) == 101);

  ad = aDouble_new();
  aDouble_sort(ad);
  aDouble_reverse(ad);
  assert(aDouble_size(ad) == 0);

  ad = aDouble_new_c(3, (double[]){1.0, 1.8, 1.3});
  assert(aDouble_size(ad) == 3);
  assert(aDouble_get(ad, 0) == 1.0);
  assert(aDouble_get(ad, 1) == 1.8);
  assert(aDouble_get(ad, 2) == 1.3);

  puts("    Finished");
}
