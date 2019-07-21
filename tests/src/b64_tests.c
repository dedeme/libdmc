// Copyright 21-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "b64_tests.h"
#include <assert.h>
#include "dmc/b64.h"

void b64_tests(void) {
  puts("B64 tests");
  Gc *gc = gc_new();

  char *b64 = b64_encode(gc, "Cañónç䍆");
  assert(str_eq(b64, "Q2HDscOzbsOn5I2G"));

  b64 = b64_encode(gc, "");
  assert(str_eq(b64, ""));

  b64 = b64_encode(gc, "Cañónç䍆");
  char *s = b64_decode(gc, b64);
  assert(str_eq(s, "Cañónç䍆"));

  b64 = b64_encode(gc, "");
  s = b64_decode(gc, b64);
  assert(str_eq(s, ""));

  for (int len = 120; len < 130; ++len) {
    Bytes *bs = bytes_new_bf(gc, len);
    unsigned char *bss = bytes_bs(bs);
    for (int i = 0; i < len; ++i) {
      bss[i] = (unsigned char)i +  10;
    }
    b64 = b64_encode_bytes(gc, bs);
    Bytes *bs2 = b64_decode_bytes(gc, b64);
    unsigned char *bss2 = bytes_bs(bs2);
    for (int i = 0; i < len; ++i) {
      assert(bss[i] = bss2[i]);
    }
  }

  gc_free(gc);
  puts("    Finished");
}


