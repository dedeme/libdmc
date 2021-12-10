// Copyright 22-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "cryp_tests.h"
#include <assert.h>
#include "string.h"
#include "dmc/cryp.h"

void cryp_tests(void) {
  puts("Cryp tests");

  char *b64;

  b64 = cryp_genk(6);
  assert(6 == strlen(b64));

  b64 = cryp_key("deme", 6);
  assert(str_eq("wiWTB9", b64));
  b64 = cryp_key("Generaro", 5);
  assert(str_eq("Ixy8I", b64));
  b64 = cryp_key("Generara", 5);
  assert(str_eq("0DIih", b64));

  b64 = cryp_cryp("abc", "01");
  assert(!str_eq(b64, "01"));
  b64 = cryp_decryp("abc", b64);
  assert(str_eq(b64, "01"));
  b64 = cryp_cryp("abcd", "11");
  assert(!str_eq(b64, "11"));
  b64 = cryp_decryp("abcd", b64);
  assert(str_eq(b64, "11"));
  b64 = cryp_cryp("abc", "");
  assert(str_eq(b64, ""));
  b64 = cryp_decryp("abc", b64);
  assert(str_eq(b64, ""));
  b64 = cryp_cryp("c", "a");
  assert(!str_eq(b64, "a"));
  b64 = cryp_decryp("c", b64);
  assert(str_eq(b64, "a"));
  b64 = cryp_cryp("xxx", "ab c");
  assert(!str_eq(b64, "ab c"));
  b64 = cryp_decryp("xxx", b64);
  assert(str_eq(b64, "ab c"));
  b64 = cryp_cryp("abc", "\n\ta€b c");
  assert(!str_eq(b64, "\n\ta€b c"));
  b64 = cryp_decryp("abc", b64);
  assert(str_eq(b64, "\n\ta€b c"));

  b64 = cryp_auto_cryp(8, "01");
  assert(!str_eq(b64, "01"));
  b64 = cryp_auto_decryp(b64);
  assert(str_eq(b64, "01"));
  b64 = cryp_auto_cryp(4, "11");
  assert(!str_eq(b64, "11"));
  b64 = cryp_auto_decryp(b64);
  assert(str_eq(b64, "11"));
  b64 = cryp_auto_cryp(2, "");
  assert(!str_eq(b64, ""));
  b64 = cryp_auto_decryp(b64);
  assert(str_eq(b64, ""));
  b64 = cryp_auto_cryp(8, "a");
  assert(!str_eq(b64, "a"));
  b64 = cryp_auto_decryp(b64);
  assert(str_eq(b64, "a"));
  b64 = cryp_auto_cryp(4, "ab c");
  assert(!str_eq(b64, "ab c"));
  b64 = cryp_auto_decryp(b64);
  assert(str_eq(b64, "ab c"));
  b64 = cryp_auto_cryp(2, "\n\ta€b c");
  assert(!str_eq(b64, "\n\ta€b c"));
  b64 = cryp_auto_decryp(b64);
  assert(str_eq(b64, "\n\ta€b c"));

  b64 = cryp_encode("abc", 2, "01");
  assert(!str_eq(b64, "01"));
  b64 = cryp_decode("abc", b64);
  assert(str_eq(b64, "01"));
  b64 = cryp_encode("abcd", 1, "11");
  assert(!str_eq(b64, "11"));
  b64 = cryp_decode("abcd", b64);
  assert(str_eq(b64, "11"));
  b64 = cryp_encode("abc", 2, "");
  assert(!str_eq(b64, ""));
  b64 = cryp_decode("abc", b64);
  assert(str_eq(b64, ""));
  b64 = cryp_encode("c", 6, "a");
  assert(!str_eq(b64, "a"));
  b64 = cryp_decode("c", b64);
  assert(str_eq(b64, "a"));
  b64 = cryp_encode("xxx", 40, "ab c");
  assert(!str_eq(b64, "ab c"));
  b64 = cryp_decode("xxx", b64);
  assert(str_eq(b64, "ab c"));
  b64 = cryp_encode("abc", 2, "\n\ta€b c");
  assert(!str_eq(b64, "\n\ta€b c"));
  b64 = cryp_decode("abc", b64);
  assert(str_eq(b64, "\n\ta€b c"));

  puts("    Finished");
}

