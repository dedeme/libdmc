// Copyright 17-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "b64_tests.h"
#include <assert.h>
#include "dmc/b64.h"

void b64_tests(void) {
  puts("B64 tests");

  char *b64 = b64_encode_new("Cañónç䍆");
  assert(str_eq(b64, "Q2HDscOzbsOn5I2G"));
  free(b64);

  b64 = b64_encode_new("");
  assert(str_eq(b64, ""));
  free(b64);

  b64 = b64_encode_new("Cañónç䍆");
  char *s = b64_decode_new(b64);
  assert(str_eq(s, "Cañónç䍆"));
  free(b64); free(s);

  b64 = b64_encode_new("");
  s = b64_decode_new(b64);
  assert(str_eq(s, ""));
  free(b64); free(s);

  for (int len = 120; len < 130; ++len) {
    Bytes *bs = bytes_bf_new(len);
    unsigned char *bss = bytes_bs(bs);
    for (int i = 0; i < len; ++i) {
      bss[i] = (unsigned char)i +  10;
    }
    b64 = b64_encode_bytes_new(bs);
    Bytes *bs2 = b64_decode_bytes_new(b64);
    unsigned char *bss2 = bytes_bs(bs2);
    for (int i = 0; i < len; ++i) {
      assert(bss[i] = bss2[i]);
    }
    bytes_free(bs);
    bytes_free(bs2);
    free(b64);
  }

  puts("    Finished");
}

