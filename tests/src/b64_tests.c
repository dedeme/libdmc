// Copyright 17-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "b64_tests.h"
#include <assert.h>
#include "dmc/b64.h"

void b64_tests(void) {
  puts("B64 tests");

  char *b64 = b64_encode("Cañónç䍆");
  assert(str_eq(b64, "Q2HDscOzbsOn5I2G"));

  b64 = b64_encode("");
  assert(str_eq(b64, ""));

  b64 = b64_encode("Cañónç䍆");
  char *s = b64_decode(b64);
  assert(str_eq(s, "Cañónç䍆"));

  b64 = b64_encode("");
  s = b64_decode(b64);
  assert(str_eq(s, ""));

  for (int len = 120; len < 130; ++len) {
    Bytes *bs = bytes_bf_new(len);
    unsigned char *bss = bytes_bs(bs);
    for (int i = 0; i < len; ++i) {
      bss[i] = (unsigned char)i +  10;
    }
    b64 = b64_encode_bytes(bs);
    Bytes *bs2 = b64_decode_bytes(b64);
    unsigned char *bss2 = bytes_bs(bs2);
    for (int i = 0; i < len; ++i) {
      assert(bss[i] = bss2[i]);
    }
  }

  puts("    Finished");
}

