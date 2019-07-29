// Copyright 23-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "list_tests.h"
#include <stdio.h>
#include <assert.h>
#include "dmc/List.h"
#include "dmc/Buf.h"
#include "dmc/It.h"
#include "dmc/str.h"
#include "dmc/Js.h"

void list_tests() {

  char *paste (It *it) {
    Buf *bf = buf_new();
    while (it_has_next(it)) buf_add(bf, it_next(it));
    return buf_to_str(bf);
  }

  puts("List test");

  // List[char]
  List *s = list_new();
  assert(list_empty(s));
  assert(opt_is_empty(list_get(s, 0)));
  s = list_from_js(list_to_js(s, (FTO)js_ws), (FFROM)js_rs);
  assert(list_empty(s));
  s = list_cons(s, "a");
  assert(!list_empty(s));
  assert(opt_is_full(list_get(s, 0)));
  assert(str_eq("a", opt_get(list_get(s, 0))));
  assert(str_eq("a", list_head(s)));

  s = list_tail(s);
  assert(list_empty(s));
  s = list_cons(s, "a");
  s = list_cons(s, "b");
  assert(opt_is_empty(list_get(s, 110)));
  assert(opt_is_full(list_get(s, 1)));
  assert(str_eq("b", opt_get(list_get(s, 0))));
  assert(str_eq("a", opt_get(list_get(s, 1))));

  assert(str_eq("ba", paste(list_to_it(s))));
  assert(str_eq("ba", paste(list_to_it(s))));

  s = list_from_js(list_to_js(s, (FTO)js_ws), (FFROM)js_rs);

  assert(str_eq("ba", paste(list_to_it(s))));
  assert(str_eq("ba", paste(list_to_it(s))));

  // List[char]
  List *s2 = list_from_js(list_to_js(s, (FTO)js_ws), (FFROM)js_rs);
  assert(str_eq("ba", paste(list_to_it(s2))));
  assert(str_eq("ba", paste(list_to_it(s2))));

  assert(str_eq("ab", paste(list_to_it(list_reverse(s2)))));
  assert(str_eq("", paste(list_to_it(list_new()))));

  // List[char]
  List *s3 = list_cons(s2, "c");
  assert(str_eq("bacba", paste(list_to_it(list_cat(s2, s3)))));

  puts( "    Finished");
}


