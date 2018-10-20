// Copyright 17-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "cryp_tests.h"
#include <assert.h>
#include "string.h"
#include "dmc/cryp.h"

void cryp_tests(void) {
  puts("Cryp tests");

  char *b64;

  b64 = cryp_genk_new(6);
  assert(6 == strlen(b64));
  free(b64);

  b64 = str_new("deme");
  cryp_key(&b64, 6);
  assert(str_eq("wiWTB9", b64));
  free(b64);
  b64 = str_new("Generaro");
  cryp_key(&b64, 5);
  assert(str_eq("Ixy8I", b64));
  free(b64);
  b64 = str_new("Generara");
  cryp_key(&b64, 5);
  assert(str_eq("0DIih", b64));
  free(b64);

  b64 = str_new("01");
  cryp_cryp(&b64, "abc");
  assert(!str_eq(b64, "01"));
  cryp_decryp(&b64, "abc");
  assert(str_eq(b64, "01"));
  free(b64);
  b64 = str_new("11");
  cryp_cryp(&b64, "abcd");
  assert(!str_eq(b64, "11"));
  cryp_decryp(&b64, "abcd");
  assert(str_eq(b64, "11"));
  free(b64);
  b64 = str_new("");
  cryp_cryp(&b64, "abc");
  assert(str_eq(b64, ""));
  cryp_decryp(&b64, "abc");
  assert(str_eq(b64, ""));
  free(b64);
  b64 = str_new("a");
  cryp_cryp(&b64, "c");
  assert(!str_eq(b64, "a"));
  cryp_decryp(&b64, "c");
  assert(str_eq(b64, "a"));
  free(b64);
  b64 = str_new("ab c");
  cryp_cryp(&b64, "xxx");
  assert(!str_eq(b64, "ab c"));
  cryp_decryp(&b64, "xxx");
  assert(str_eq(b64, "ab c"));
  free(b64);
  b64 = str_new("\n\ta€b c");
  cryp_cryp(&b64, "abc");
  assert(!str_eq(b64, "\n\ta€b c"));
  cryp_decryp(&b64, "abc");
  assert(str_eq(b64, "\n\ta€b c"));
  free(b64);


  b64 = str_new("01");
  cryp_auto_cryp(&b64, 8);
  assert(!str_eq(b64, "01"));
  cryp_auto_decryp(&b64);
  assert(str_eq(b64, "01"));
  free(b64);
  b64 = str_new("11");
  cryp_auto_cryp(&b64, 4);
  assert(!str_eq(b64, "11"));
  cryp_auto_decryp(&b64);
  assert(str_eq(b64, "11"));
  free(b64);
  b64 = str_new("");
  cryp_auto_cryp(&b64, 2);
  assert(!str_eq(b64, ""));
  cryp_auto_decryp(&b64);
  assert(str_eq(b64, ""));
  free(b64);
  b64 = str_new("a");
  cryp_auto_cryp(&b64, 8);
  assert(!str_eq(b64, "a"));
  cryp_auto_decryp(&b64);
  assert(str_eq(b64, "a"));
  free(b64);
  b64 = str_new("ab c");
  cryp_auto_cryp(&b64, 4);
  assert(!str_eq(b64, "ab c"));
  cryp_auto_decryp(&b64);
  assert(str_eq(b64, "ab c"));
  free(b64);
  b64 = str_new("\n\ta€b c");
  cryp_auto_cryp(&b64, 2);
  assert(!str_eq(b64, "\n\ta€b c"));
  cryp_auto_decryp(&b64);
  assert(str_eq(b64, "\n\ta€b c"));
  free(b64);

  b64 = str_new("01");
  cryp_encode(&b64, 2, "abc");
  assert(!str_eq(b64, "01"));
  cryp_decode(&b64, "abc");
  assert(str_eq(b64, "01"));
  free(b64);
  b64 = str_new("11");
  cryp_encode(&b64, 1, "abcd");
  assert(!str_eq(b64, "11"));
  cryp_decode(&b64, "abcd");
  assert(str_eq(b64, "11"));
  free(b64);
  b64 = str_new("");
  cryp_encode(&b64, 2, "abc");
  assert(!str_eq(b64, ""));
  cryp_decode(&b64, "abc");
  assert(str_eq(b64, ""));
  free(b64);
  b64 = str_new("a");
  cryp_encode(&b64, 6, "c");
  assert(!str_eq(b64, "a"));
  cryp_decode(&b64, "c");
  assert(str_eq(b64, "a"));
  free(b64);
  b64 = str_new("ab c");
  cryp_encode(&b64, 40, "xxx");
  assert(!str_eq(b64, "ab c"));
  cryp_decode(&b64, "xxx");
  assert(str_eq(b64, "ab c"));
  free(b64);
  b64 = str_new("\n\ta€b c");
  cryp_encode(&b64, 2, "abc");
  assert(!str_eq(b64, "\n\ta€b c"));
  cryp_decode(&b64, "abc");
  assert(str_eq(b64, "\n\ta€b c"));
  free(b64);

  puts("    Finished");
}

