// Copyright 02-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <assert.h>
#include "dmc/cryp.h"
#include "dmc/str.h"

void cryp_test() {
  printf("cryp test\n");

  char *b64;
  char *s;

  b64 = cryp_genk(6);
  assert(6 == str_len(b64));

  b64 = cryp_key("deme", 6);
  assert(str_eq("wiWTB9", b64));
  b64 = cryp_key("Generaro", 5);
  assert(str_eq("Ixy8I", b64));
  b64 = cryp_key("Generara", 5);
  assert(str_eq("0DIih", b64));

  b64 = cryp_cryp("abc", "01");
  s = cryp_decryp("abc", b64);
  assert(str_eq(s, "01"));
  b64 = cryp_cryp("abcd", "11");
  s = cryp_decryp("abcd", b64);
  assert(str_eq(s, "11"));
  b64 = cryp_cryp("abc", "");
  s = cryp_decryp("abc", b64);
  assert(str_eq(s, ""));
  b64 = cryp_cryp("c", "a");
  s = cryp_decryp("c", b64);
  assert(str_eq(s, "a"));
  b64 = cryp_cryp("xxx", "ab c");
  s = cryp_decryp("xxx", b64);
  assert(str_eq(s, "ab c"));
  b64 = cryp_cryp("abc", "\n\ta€b c");
  s = cryp_decryp("abc", b64);
  assert(str_eq(s, "\n\ta€b c"));

  b64 = cryp_auto_cryp(8, "01");
  s = cryp_auto_decryp(b64);
  assert(str_eq(s, "01"));
  b64 = cryp_auto_cryp(4, "11");
  s = cryp_auto_decryp(b64);
  assert(str_eq(s, "11"));
  b64 = cryp_auto_cryp(2, "");
  s = cryp_auto_decryp(b64);
  assert(str_eq(s, ""));
  b64 = cryp_auto_cryp(8, "a");
  s = cryp_auto_decryp(b64);
  assert(str_eq(s, "a"));
  b64 = cryp_auto_cryp(4, "ab c");
  s = cryp_auto_decryp(b64);
  assert(str_eq(s, "ab c"));
  b64 = cryp_auto_cryp(2, "\n\ta€b c");
  s = cryp_auto_decryp(b64);
  assert(str_eq(s, "\n\ta€b c"));
  b64 = cryp_encode("abc", 2, "01");
  s = cryp_decode("abc", b64);
  assert(str_eq(s, "01"));
  b64 = cryp_encode("abcd", 1, "11");
  s = cryp_decode("abcd", b64);
  assert(str_eq(s, "11"));
  b64 = cryp_encode("abc", 2, "");
  s = cryp_decode("abc", b64);
  assert(str_eq(s, ""));
  b64 = cryp_encode("c", 6, "a");
  s = cryp_decode("c", b64);
  assert(str_eq(s, "a"));
  b64 = cryp_encode("xxx", 40, "ab c");
  s = cryp_decode("xxx", b64);
  assert(str_eq(s, "ab c"));
  b64 = cryp_encode("abc", 2, "\n\ta€b c");
  s = cryp_decode("abc", b64);
  assert(str_eq(s, "\n\ta€b c"));

  printf( "    Finished\n");
}


