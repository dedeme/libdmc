// Copyright 21-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "cryp_tests.h"
#include <assert.h>
#include "string.h"
#include "dmc/cryp.h"

void cryp_tests(void) {
  puts("Cryp tests");
  Gc *gc = gc_new();

  char *b64;

  b64 = cryp_genk(gc, 6);
  assert(6 == strlen(b64));

  b64 = cryp_key(gc, "deme", 6);
  assert(str_eq("wiWTB9", b64));
  b64 = cryp_key(gc, "Generaro", 5);
  assert(str_eq("Ixy8I", b64));
  b64 = cryp_key(gc, "Generara", 5);
  assert(str_eq("0DIih", b64));

  b64 = cryp_cryp(gc, "01", "abc");
  assert(!str_eq(b64, "01"));
  b64 = cryp_decryp(gc, b64, "abc");
  assert(str_eq(b64, "01"));
  b64 = cryp_cryp(gc, "11", "abcd");
  assert(!str_eq(b64, "11"));
  b64 = cryp_decryp(gc, b64, "abcd");
  assert(str_eq(b64, "11"));
  b64 = cryp_cryp(gc, "", "abc");
  assert(str_eq(b64, ""));
  b64 = cryp_decryp(gc, b64, "abc");
  assert(str_eq(b64, ""));
  b64 = cryp_cryp(gc, "a", "c");
  assert(!str_eq(b64, "a"));
  b64 = cryp_decryp(gc, b64, "c");
  assert(str_eq(b64, "a"));
  b64 = cryp_cryp(gc, "ab c", "xxx");
  assert(!str_eq(b64, "ab c"));
  b64 = cryp_decryp(gc, b64, "xxx");
  assert(str_eq(b64, "ab c"));
  b64 = cryp_cryp(gc, "\n\ta€b c", "abc");
  assert(!str_eq(b64, "\n\ta€b c"));
  b64 = cryp_decryp(gc, b64, "abc");
  assert(str_eq(b64, "\n\ta€b c"));

  b64 = cryp_auto_cryp(gc, "01", 8);
  assert(!str_eq(b64, "01"));
  b64 = cryp_auto_decryp(gc, b64);
  assert(str_eq(b64, "01"));
  b64 = cryp_auto_cryp(gc, "11", 4);
  assert(!str_eq(b64, "11"));
  b64 = cryp_auto_decryp(gc, b64);
  assert(str_eq(b64, "11"));
  b64 = cryp_auto_cryp(gc, "", 2);
  assert(!str_eq(b64, ""));
  b64 = cryp_auto_decryp(gc, b64);
  assert(str_eq(b64, ""));
  b64 = cryp_auto_cryp(gc, "a", 8);
  assert(!str_eq(b64, "a"));
  b64 = cryp_auto_decryp(gc, b64);
  assert(str_eq(b64, "a"));
  b64 = cryp_auto_cryp(gc, "ab c", 4);
  assert(!str_eq(b64, "ab c"));
  b64 = cryp_auto_decryp(gc, b64);
  assert(str_eq(b64, "ab c"));
  b64 = cryp_auto_cryp(gc, "\n\ta€b c", 2);
  assert(!str_eq(b64, "\n\ta€b c"));
  b64 = cryp_auto_decryp(gc, b64);
  assert(str_eq(b64, "\n\ta€b c"));

  b64 = cryp_encode(gc, "01", 2, "abc");
  assert(!str_eq(b64, "01"));
  b64 = cryp_decode(gc, b64, "abc");
  assert(str_eq(b64, "01"));
  b64 = cryp_encode(gc, "11", 1, "abcd");
  assert(!str_eq(b64, "11"));
  b64 = cryp_decode(gc, b64, "abcd");
  assert(str_eq(b64, "11"));
  b64 = cryp_encode(gc, "", 2, "abc");
  assert(!str_eq(b64, ""));
  b64 = cryp_decode(gc, b64, "abc");
  assert(str_eq(b64, ""));
  b64 = cryp_encode(gc, "a", 6, "c");
  assert(!str_eq(b64, "a"));
  b64 = cryp_decode(gc, b64, "c");
  assert(str_eq(b64, "a"));
  b64 = cryp_encode(gc, "ab c", 40, "xxx");
  assert(!str_eq(b64, "ab c"));
  b64 = cryp_decode(gc, b64, "xxx");
  assert(str_eq(b64, "ab c"));
  b64 = cryp_encode(gc, "\n\ta€b c", 2, "abc");
  assert(!str_eq(b64, "\n\ta€b c"));
  b64 = cryp_decode(gc, b64, "abc");
  assert(str_eq(b64, "\n\ta€b c"));

  gc_free(gc);
  puts("    Finished");
}

