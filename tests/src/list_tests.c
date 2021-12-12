// Copyright 01-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "list_tests.h"
#include <assert.h>
#include <stdio.h>
#include "dmc/char/Lchar.h"
#include "dmc/Buf.h"
#include "dmc/js.h"
#include "dmc/str.h"

// List[char]
char *paste (Lchar *l) {
  Buf *bf = buf_new();
  while (l->next) {
    buf_add(bf, l->value);
    l = l->next;
  }
  return buf_to_str(bf);
}

void list_tests() {


  puts("List test");

  Lchar *s = lchar_new();
  assert(lchar_empty(s));
  assert(lchar_size(s) == 0);
  assert(ochar_none(lchar_get(s, 0)));
  s = lchar_from_js(lchar_to_js(s, js_ws), js_rs);
  assert(lchar_empty(s));
  s = lchar_cons(s, "a");
  assert(!lchar_empty(s));
  assert(ochar_nsome(lchar_get(s, 0)));
  assert(str_eq("a", ochar_some(lchar_get(s, 0))));
  assert(str_eq("a", lchar_head(s)));

  s = lchar_tail(s);
  assert(lchar_empty(s));
  s = lchar_cons(s, "a");
  s = lchar_cons(s, "b");
  assert(lchar_size(s) == 2);
  assert(ochar_none(lchar_get(s, 110)));
  assert(ochar_nsome(lchar_get(s, 1)));
  assert(str_eq("b", ochar_some(lchar_get(s, 0))));
  assert(str_eq("a", ochar_some(lchar_get(s, 1))));

  assert(str_eq("ba", paste(s)));
  assert(str_eq("ba", paste(s)));

  s = lchar_from_js(lchar_to_js(s, js_ws), js_rs);

  assert(str_eq("ba", paste(s)));
  assert(str_eq("ba", paste(s)));

  Lchar *s2 = lchar_from_js(lchar_to_js(s, js_ws), js_rs);
  assert(str_eq("ba", paste(s2)));
  assert(str_eq("ba", paste(s2)));

  assert(str_eq("ab", paste(lchar_reverse(s2))));
  assert(str_eq("", paste(lchar_new())));

  Lchar *s3 = lchar_cons(s2, "c");
  assert(str_eq("bacba", paste(lchar_cat(s2, s3))));

  Lchar *l = lchar_cons(lchar_cons(lchar_cons(
    lchar_cons(lchar_cons(lchar_new(), "03"), "01"),
    "02"), "12"), "12");
  l = lchar_sort(l, str_greater);
  assert(str_eq("0102031212", paste(l)));

  /**/int less30 (char *e) { return strcmp(e, "30") < 0; }
  assert(lchar_all(l, less30));
  assert(lchar_any(l, less30));
  assert(str_eq("0102031212", paste(lchar_filter(l, less30))));
  assert(str_eq("0102031212", paste(lchar_takef(l, less30))));
  assert(str_eq("", paste(lchar_dropf(l, less30))));
  /**/int less10 (char *e) { return strcmp(e, "10") < 0; }
  assert(!lchar_all(l, less10));
  assert(lchar_any(l, less10));
  assert(str_eq("010203", paste(lchar_filter(l, less10))));
  assert(str_eq("010203", paste(lchar_takef(l, less10))));
  assert(str_eq("1212", paste(lchar_dropf(l, less10))));
  /**/int less00 (char *e) { return strcmp(e, "00") < 0; }
  assert(!lchar_all(l, less00));
  assert(!lchar_any(l, less00));
  assert(str_eq("", paste(lchar_filter(l, less00))));
  assert(str_eq("", paste(lchar_takef(l, less00))));
  assert(str_eq("0102031212", paste(lchar_dropf(l, less00))));

  /**/int index07 (char *e) { return str_eq(e, "07"); }
  assert(lchar_index(lchar_new(), index07) == -1);
  assert(lchar_index(l, index07) == -1);
  assert(lchar_last_index(lchar_new(), index07) == -1);
  assert(lchar_last_index(l, index07) == -1);
  assert(ochar_none(lchar_find(lchar_new(), index07)));
  assert(ochar_none(lchar_find(l, index07)));
  assert(ochar_none(lchar_find_last(lchar_new(), index07)));
  assert(ochar_none(lchar_find_last(l, index07)));
  /**/int index12 (char *e) { return str_eq(e, "12"); }
  assert(str_eq(ochar_some(lchar_find(l, index12)), "12"));
  assert(str_eq(ochar_some(lchar_find_last(l, index12)), "12"));

  assert(str_eq("0102", paste(lchar_take(l, 2))));
  assert(str_eq("031212", paste(lchar_drop(l, 2))));

  assert(str_eq("0102031212", paste(lchar_take(l, 12))));
  assert(str_eq("", paste(lchar_drop(l, 12))));

  assert(str_eq("", paste(lchar_take(l, 0))));
  assert(str_eq("0102031212", paste(lchar_drop(l, 0))));

  assert(str_eq("0102", paste(lchar_take(l, 2))));
  assert(str_eq("031212", paste(lchar_drop(l, 2))));

  assert(str_eq("0102031212", paste(lchar_take(l, 12))));
  assert(str_eq("", paste(lchar_drop(l, 12))));

  /**/void *conv1 (char *e) { return str_cat("'", e, "'", NULL); }
  /**/void *conv2 (char *e) { return str_cat(" - '", e, "'", NULL); }
  assert(str_eq("", paste((Lchar *)lchar_map(lchar_new(), conv1))));
  assert(str_eq("'01''02''03''12''12'", paste((Lchar *)lchar_map(l, conv1))));
  assert(str_eq(
    "'01' - '02' - '03' - '12' - '12'",
    paste((Lchar *)lchar_map2(l, conv1, conv2))
  ));

  /**/void *zconv (char *e1, char *e2) {
  /**/  return str_cat("|", e1, "-", e2, "|", NULL);
  /**/}
  Lchar *l2 = lchar_cons(lchar_cons(lchar_new(), "b"), "a");
  assert(str_eq("", paste((Lchar *)lchar_zip(l, lchar_new(), zconv))));
  assert(str_eq("", paste((Lchar *)lchar_zip(lchar_new(), l2, zconv))));
  assert(str_eq("|01-a||02-b|", paste((Lchar *)lchar_zip(l, l2, zconv))));
  assert(str_eq("|a-01||b-02|", paste((Lchar *)lchar_zip(l2, l, zconv))));

  /**/void *z3conv (char *e1, char *e2, char *e3) {
  /**/  return str_cat(
  /**/    "|", e1, "-", e2,"-", e3, "|", NULL
  /**/  );
  /**/}
  Lchar *l3 = lchar_cons(lchar_cons(lchar_cons(lchar_new(), "C"), "B"), "A");
  assert(str_eq("", paste((Lchar *)lchar_zip3(l, l, lchar_new(), z3conv))));
  assert(str_eq("", paste((Lchar *)lchar_zip3(l, lchar_new(), l2, z3conv))));
  assert(str_eq("", paste((Lchar *)lchar_zip3(lchar_new(), l2, l3, z3conv))));
  assert(str_eq("|01-a-A||02-b-B|", paste((Lchar *)lchar_zip3(l, l2, l3, z3conv))));
  assert(str_eq("|a-01-A||b-02-B|", paste((Lchar *)lchar_zip3(l2, l, l3, z3conv))));
  assert(str_eq("|A-a-01||B-b-02|", paste((Lchar *)lchar_zip3(l3, l2, l, z3conv))));

  puts( "    Finished");
}


