// Copyright 22-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "rnd_tests.h"
#include <assert.h>
#include "dmc/rnd.h"

void rnd_tests(void) {
  puts("Rnd tests");

  assert(rnd_d() < 1);
  assert(rnd_d() >= 0);
  assert(rnd_i(3) < 3);
  assert(rnd_i(3) >= 0);

  Arr *a = arr_new();
  arr_push(a, "0");
  arr_push(a, "1");
  arr_push(a, "2");

  Box *box = rnd_box_new(a);
  for (int i = 0; i < 7; ++i) {
    char *s = rnd_box_next(box);
//    puts(s);
    assert(str_eq(s, "0") ||str_eq(s, "1") || str_eq(s, "2"));
  }

  puts("    Finished");
}
