// Copyright 21-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "buf_tests.h"
#include <assert.h>

void buf_tests(void) {
  puts("Buf tests");
  Gc *gc = gc_new();

  char *s0 = "";
  char *s1 = "1";
  char *s2 = "abc";
  Buf *bf = buf_new(gc);

  buf_add(bf, s0);
  buf_cadd(bf, '+');
  buf_add(bf, "0");
  buf_add(bf, s1);
  buf_add(bf, s2);
  buf_add(bf, s0);
  buf_add(bf, s2);
  buf_add(bf, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  char *s = buf_to_str(gc, bf);
  assert(str_eq(s, "+01abcabc"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));

  buf_reset(bf);
  s = buf_to_str(gc, bf);
  assert(str_eq(s, ""));
  assert(!buf_len(bf));

  gc_free(gc);
  puts("    Finished");
}
