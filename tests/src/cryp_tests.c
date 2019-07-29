// Copyright 17-Oct-2018 ºDeme
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

  b64 = cryp_cryp("01", "abc");
  assert(!str_eq(b64, "01"));
  b64 = cryp_decryp(b64, "abc");
  assert(str_eq(b64, "01"));
  b64 = cryp_cryp("11", "abcd");
  assert(!str_eq(b64, "11"));
  b64 = cryp_decryp(b64, "abcd");
  assert(str_eq(b64, "11"));
  b64 = cryp_cryp("", "abc");
  assert(str_eq(b64, ""));
  b64 = cryp_decryp(b64, "abc");
  assert(str_eq(b64, ""));
  b64 = cryp_cryp("a", "c");
  assert(!str_eq(b64, "a"));
  b64 = cryp_decryp(b64, "c");
  assert(str_eq(b64, "a"));
  b64 = cryp_cryp("ab c", "xxx");
  assert(!str_eq(b64, "ab c"));
  b64 = cryp_decryp(b64, "xxx");
  assert(str_eq(b64, "ab c"));
  b64 = cryp_cryp("\n\ta€b c", "abc");
  assert(!str_eq(b64, "\n\ta€b c"));
  b64 = cryp_decryp(b64, "abc");
  assert(str_eq(b64, "\n\ta€b c"));

  b64 = cryp_auto_cryp("01", 8);
  assert(!str_eq(b64, "01"));
  b64 = cryp_auto_decryp(b64);
  assert(str_eq(b64, "01"));
  b64 = cryp_auto_cryp("11", 4);
  assert(!str_eq(b64, "11"));
  b64 = cryp_auto_decryp(b64);
  assert(str_eq(b64, "11"));
  b64 = cryp_auto_cryp("", 2);
  assert(!str_eq(b64, ""));
  b64 = cryp_auto_decryp(b64);
  assert(str_eq(b64, ""));
  b64 = cryp_auto_cryp("a", 8);
  assert(!str_eq(b64, "a"));
  b64 = cryp_auto_decryp(b64);
  assert(str_eq(b64, "a"));
  b64 = cryp_auto_cryp("ab c", 4);
  assert(!str_eq(b64, "ab c"));
  b64 = cryp_auto_decryp(b64);
  assert(str_eq(b64, "ab c"));
  b64 = cryp_auto_cryp("\n\ta€b c", 2);
  assert(!str_eq(b64, "\n\ta€b c"));
  b64 = cryp_auto_decryp(b64);
  assert(str_eq(b64, "\n\ta€b c"));

  b64 = cryp_encode("01", 2, "abc");
  assert(!str_eq(b64, "01"));
  b64 = cryp_decode(b64, "abc");
  assert(str_eq(b64, "01"));
  b64 = cryp_encode("11", 1, "abcd");
  assert(!str_eq(b64, "11"));
  b64 = cryp_decode(b64, "abcd");
  assert(str_eq(b64, "11"));
  b64 = cryp_encode("", 2, "abc");
  assert(!str_eq(b64, ""));
  b64 = cryp_decode(b64, "abc");
  assert(str_eq(b64, ""));
  b64 = cryp_encode("a", 6, "c");
  assert(!str_eq(b64, "a"));
  b64 = cryp_decode(b64, "c");
  assert(str_eq(b64, "a"));
  b64 = cryp_encode("ab c", 40, "xxx");
  assert(!str_eq(b64, "ab c"));
  b64 = cryp_decode(b64, "xxx");
  assert(str_eq(b64, "ab c"));
  b64 = cryp_encode("\n\ta€b c", 2, "abc");
  assert(!str_eq(b64, "\n\ta€b c"));
  b64 = cryp_decode(b64, "abc");
  assert(str_eq(b64, "\n\ta€b c"));

  puts("    Finished");
}

