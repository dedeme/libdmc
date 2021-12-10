// Copyright 21-Nov-2021 ºDeme
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
  char *js;

  b1 = bytes_new();
  assert(b1->len == 0);

  js = bytes_to_js(b1);
  bjs = bytes_from_js(js);
  assert(bjs->len == 0);

  b2 = bytes_from_str(s2);
  assert(b2->len == 1);

  bytes_add_str(b1, s1);
  assert(b1->len == 2);
  assert(*b1->bs == 'a');
  assert(b1->bs[1] == 'b');

  bytes_add_str(b1, s3);
  assert(b1->len == 2);
  assert(*b1->bs == 'a');
  assert(b1->bs[1] == 'b');

  bytes_add(b1, b2);
  assert(b1->len == 3);
  assert(*b1->bs == 'a');
  assert(b1->bs[1] == 'b');
  assert(b1->bs[2] == 'c');

  js = bytes_to_js(b1);
  bjs = bytes_from_js(js);
  assert(*bjs->bs == 'a');
  assert(bjs->bs[1] == 'b');
  assert(bjs->bs[2] == 'c');

  b1 = bytes_new();
  bytes_add(b1, b2);
  assert(b1->len == 1);
  assert(*b1->bs == 'c');

  puts("    Finished");
}

