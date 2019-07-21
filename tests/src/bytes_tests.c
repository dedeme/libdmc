// Copyright 21-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "bytes_tests.h"
#include <assert.h>

void bytes_tests(void) {
  puts("Bytes tests");
  Gc *gc = gc_new();

  char *s1 = "ab";
  char *s2 = "c";
  char *s3 = "";

  Bytes *b1;
  Bytes *b2;
  Bytes *bjs;
  Js *js;

  b1 = bytes_new(gc);
  assert(bytes_len(b1) == 0);
  js = bytes_to_js(gc, b1);
  bjs = bytes_from_js(gc, js);
  assert(bytes_len(bjs) == 0);

  b2 = bytes_from_str(gc, s2);
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
  js = bytes_to_js(gc, b1);
  bjs = bytes_from_js(gc, js);
  assert(*bytes_bs(bjs) == 'a');
  assert(bytes_bs(bjs)[1] == 'b');
  assert(bytes_bs(bjs)[2] == 'c');

  b1 = bytes_new(gc);
  bytes_add(b1, b2);
  assert(bytes_len(b1) == 1);
  assert(*bytes_bs(b1) == 'c');

  gc_free(gc);
  puts("    Finished");
}


