// Copyright 1-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <assert.h>
#include "dmc/ct/Lchar.h"
#include "dmc/Buf.h"
#include "dmc/ct/Ichar.h"
#include "dmc/str.h"
#include "dmc/Json.h"

void list_test() {

  char *paste (Ichar *it) {
    Buf *bf = buf_new();
    while (ichar_has_next(it)) buf_add(bf, ichar_next(it));
    return buf_to_str(bf);
  }

  puts("List test");

  Lchar *s = lchar_new();
  assert(lchar_empty(s));
  s = lchar_from_json(lchar_to_json(s, json_wstring), json_rstring);
  assert(lchar_empty(s));
  s = lchar_cons(s, "a");
  assert(!lchar_empty(s));
  assert(str_eq("a", lchar_head(s)));
  s = lchar_tail(s);
  assert(lchar_empty(s));
  s = lchar_cons(s, "a");
  s = lchar_cons(s, "b");

  assert(str_eq("ba", paste(lchar_to_it(s))));
  assert(str_eq("ba", paste(lchar_to_it(s))));

  s = lchar_from_json(lchar_to_json(s, json_wstring), json_rstring);

  assert(str_eq("ba", paste(lchar_to_it(s))));
  assert(str_eq("ba", paste(lchar_to_it(s))));

  Lchar *s2 = lchar_from_it(lchar_to_it(s));
  assert(str_eq("ab", paste(lchar_to_it(s2))));
  assert(str_eq("ab", paste(lchar_to_it(s2))));

  assert(str_eq("ba", paste(lchar_to_it(lchar_reverse(s2)))));
  assert(str_eq("", paste(lchar_to_it(lchar_new()))));

  Lchar *s3 = lchar_cons(s2, "c");
  assert(str_eq("abcab", paste(lchar_to_it(lchar_cat(s2, s3)))));

  puts( "    Finished");
}

