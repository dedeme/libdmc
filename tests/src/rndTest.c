// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/all.h"

void rnd_test() {
  printf("rnd test\n");

  assert(rnd_d() < 1);
  assert(rnd_d() >= 0);
  assert(rnd_i(3) < 3);
  assert(rnd_i(3) >= 0);

  Arr *a = arr_new();
  It *box = rnd_box(a);
  assert(!it_has_next(box));

  arr_add(a, "0");
  arr_add(a, "1");
  arr_add(a, "2");

  FNX(each, char, s) {
//    printf("%s\n", s);
    assert(!strcmp(s, "0") ||!strcmp(s, "1") ||!strcmp(s, "2"));
  }_FN

  it_each(it_take(rnd_box(a), 7), each);

  printf("    Finished\n");
}
