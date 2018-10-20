// Copyright 17-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "bytes_tests.h"
#include <assert.h>

void bytes_tests(void) {
  puts("Bytes tests");

  char *s1 = "ab";
  char *s2 = "c";
  char *s3 = "";

  Bytes *b1;
  Bytes *b2;
  Bytes *bjs;
  Js *js;

  b1 = bytes_new();
  assert(bytes_len(b1) == 0);
  js = bytes_to_js_new(b1);
  bjs = bytes_from_js_new(js);
  assert(bytes_len(bjs) == 0);
  free(js);
  bytes_free(bjs);

  b2 = bytes_from_str_new(s2);
  assert(bytes_len(b2) == 1);

  bytes_add_str(b1, s1);
  assert(bytes_len(b1) == 2);
  assert(*bytes_bs(b1) == 'a');
  assert(bytes_bs(b1)[1] == 'b');

  bytes_add_str(b1, s3);
  assert(bytes_len(b1) == 2);
  assert(*bytes_bs(b1) == 'a');
  assert(bytes_bs(b1)[1] == 'b');

  bytes_add(b1, b2);
  assert(bytes_len(b1) == 3);
  assert(*bytes_bs(b1) == 'a');
  assert(bytes_bs(b1)[1] == 'b');
  assert(bytes_bs(b1)[2] == 'c');
  js = bytes_to_js_new(b1);
  bjs = bytes_from_js_new(js);
  assert(*bytes_bs(bjs) == 'a');
  assert(bytes_bs(bjs)[1] == 'b');
  assert(bytes_bs(bjs)[2] == 'c');
  free(js);
  bytes_free(bjs);

  bytes_free(b1);
  b1 = bytes_new();
  bytes_add(b1, b2);
  assert(bytes_len(b1) == 1);
  assert(*bytes_bs(b1) == 'c');

  bytes_free(b1);
  bytes_free(b2);

  puts("    Finished");
}

