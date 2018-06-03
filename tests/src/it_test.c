// Copyright 31-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <assert.h>
#include "dmc/It.h"
#include "dmc/ct/Ichar.h"
#include "dmc/Buf.h"
#include "dmc/str.h"
#include "dmc/Tuples.h"
#include "dmc/ct/Ichar.h"
#include "dmc/ct/Itp.h"
#include "dmc/ct/Itp3.h"
#include "dmc/DEFS.h"

void it_test() {
  int test(char *result, Ichar *it) {
    Buf *bf = buf_new();
    /**/void cat(char *e) { buf_add(bf, e); }
    ichar_each(it, cat);
    return str_eq(result, buf_str(bf));
  }
  Ichar *mk() {
    return ichar_cat(
      ichar_cat(ichar_unary("a"), ichar_unary("b")), ichar_unary("c")
    );
  }

  Ichar *mk2() {
    return ichar_cat(
      ichar_cat(ichar_unary("a"), ichar_unary("b")), ichar_unary("a")
    );
  }

  puts("It test");

  assert(!it_has_next(it_empty()));
  assert(!ichar_has_next(ichar_empty()));
  Ichar *it1 = ichar_unary("x");
  assert(ichar_has_next(it1));
  assert(str_eq(ichar_peek(it1), "x"));
  assert(str_eq(ichar_next(it1), "x"));
  assert(!ichar_has_next(it1));

  assert(test("", ichar_empty()));
  assert(test("x", ichar_unary("x")));
  assert(test("", ichar_unary("")));
  assert(test("x", ichar_cat(ichar_empty(), ichar_unary("x"))));
  assert(test("ax", ichar_cat(ichar_unary("a"), ichar_unary("x"))));
  assert(test("abc", mk()));

  int sum = 0;
  FNX(fsum, int, i) {
    sum += *i;
  }_FN

  it_each(it_range0(4), fsum);
  assert(sum == 6);

  sum = 0;
  it_each(it_range(2, 5), fsum);
  assert(sum == 9);

  sum = 0;
  it_each(it_range(2, 2), fsum);
  assert(sum == 0);

  sum = 0;
  it_each(it_range(3, 2), fsum);
  assert(sum == 0);

  sum = 0;
  it_each(it_range(-4, -1), fsum);
  assert(sum == -9);

  assert(test("", ichar_take(ichar_empty(), 0)));
  assert(test("", ichar_take(ichar_empty(), 20)));
  assert(test("", ichar_take(mk(), 0)));
  assert(test("ab", ichar_take(mk(), 2)));
  assert(test("abc", ichar_take(mk(), 20000)));

  bool take1(char *e) { return str_cmp(e, "") < 0; }
  bool take2(char *e) { return str_cmp(e, "b") < 0; }
  bool take3(char *e) { return str_cmp(e, "j") < 0; }

  assert(test("", ichar_takef(ichar_empty(), take1)));
  assert(test("", ichar_takef(ichar_empty(), take3)));
  assert(test("", ichar_takef(mk(), take1)));
  assert(test("a", ichar_takef(mk(), take2)));
  assert(test("abc", ichar_takef(mk(), take3)));

  assert(test("", ichar_drop(ichar_empty(), 0)));
  assert(test("", ichar_drop(ichar_empty(), 20)));
  assert(test("abc", ichar_drop(mk(), 0)));
  assert(test("c", ichar_drop(mk(), 2)));
  assert(test("", ichar_drop(mk(), 20000)));

  assert(test("", ichar_dropf(ichar_empty(), take1)));
  assert(test("", ichar_dropf(ichar_empty(), take3)));
  assert(test("abc", ichar_dropf(mk(), take1)));
  assert(test("bc", ichar_dropf(mk(), take2)));
  assert(test("", ichar_dropf(mk(), take3)));

  bool filter1(char *e) { return str_eq(e, "b"); }
  bool filter2(char *e) { return !str_eq(e, "b"); }
  bool filter3(char *e) { return str_eq(e, "j"); }
  assert(test("", ichar_filter(ichar_empty(), filter1)));
  assert(test("", ichar_filter(ichar_empty(), filter2)));
  assert(test("b", ichar_filter(mk(), filter1)));
  assert(test("ac", ichar_filter(mk(), filter2)));
  assert(test("", ichar_filter(mk(), filter3)));

  void *map(char *e) { return str_cat(e, "-", NULL); }
  assert(test("", (Ichar *)ichar_map(ichar_empty(), map)));
  assert(test("a-b-c-", (Ichar *)ichar_map(mk(), map)));

  void *map1(char *e) { return str_cat("-", e, "-", NULL); }
  assert(test("", (Ichar *)ichar_map2(ichar_empty(), map1, map)));
  assert(test("-a-", (Ichar *)ichar_map2(ichar_unary("a"), map1, map)));
  assert(test("-a-b-c-", (Ichar *)ichar_map2(mk(), map1, map)));

  /**/void *zip(Tp *tp) { return str_printf("(%s-%s)", tp_e1(tp), tp_e2(tp)); }
  assert(test("",
    (Ichar*)itp_map(ichar_zip(ichar_empty(), ichar_empty()), zip)));
  assert(test("", (Ichar *)itp_map(ichar_zip(ichar_empty(), mk()), zip)));
  assert(test("(x-a)",
    (Ichar *)itp_map(ichar_zip(ichar_unary("x"), mk()), zip)));
  assert(test("(a-a)(b-b)(c-c)", (Ichar *)itp_map(ichar_zip(mk(), mk()), zip)));

  /**/void *zip3(Tp3 *tp3) {
  /**/  return str_printf("(%s-%s-%s)", tp3_e1(tp3), tp3_e2(tp3), tp3_e3(tp3));
  /**/}
  assert(test("",
    (Ichar*)itp3_map(
      ichar_zip3(ichar_empty(), ichar_empty(), ichar_empty()), zip3
    )
  ));
  assert(test("",
    (Ichar *)itp3_map(ichar_zip3(ichar_empty(), mk(), mk()), zip3)));
  assert(test("(a-x-a)",
    (Ichar *)itp3_map(ichar_zip3(mk(), ichar_unary("x"), mk()), zip3)));
  assert(test("(a-a-a)(b-b-b)(c-c-c)",
    (Ichar *)itp3_map(ichar_zip3(mk(), mk(), mk()), zip3)));

  assert(ichar_eq(ichar_empty(), ichar_empty()));
  assert(ichar_eq(ichar_unary("x"), ichar_unary("x")));
  assert(ichar_eq(mk(), mk()));
  assert(!ichar_eq(ichar_empty(), ichar_unary("z")));
  assert(!ichar_eq(ichar_unary("z"), ichar_empty()));
  assert(!ichar_eq(ichar_unary("z"), ichar_unary("a")));
  assert(!ichar_eq(mk(), ichar_unary("a")));
  assert(ichar_eq(ichar_take(mk(), 1), ichar_unary("a")));

  assert(ichar_index(ichar_empty(), "") == -1);
  assert(ichar_index(ichar_unary("a"), "") == -1);
  assert(ichar_index(ichar_empty(), "a") == -1);
  assert(ichar_index(ichar_unary("a"), "a") == 0);
  assert(ichar_index(mk2(), "a") == 0);
  assert(ichar_index(ichar_unary("a"), "z") == -1);
  assert(ichar_index(mk2(), "z") == -1);

  assert(!ichar_contains(ichar_empty(), ""));
  assert(!ichar_contains(ichar_unary("a"), ""));
  assert(!ichar_contains(ichar_empty(), "a"));
  assert(ichar_contains(ichar_unary("a"), "a"));
  assert(ichar_contains(mk2("a"), "a"));
  assert(!ichar_contains(ichar_unary("a"), "z"));
  assert(!ichar_contains(mk2("a"), "z"));

  assert(ichar_last_index(ichar_empty(), "") == -1);
  assert(ichar_last_index(ichar_unary("a"), "") == -1);
  assert(ichar_last_index(ichar_empty(), "a") == -1);
  assert(ichar_last_index(ichar_unary("a"), "a") == 0);
  assert(ichar_last_index(mk2(), "a") == 2);
  assert(ichar_last_index(ichar_unary("a"), "z") == -1);
  assert(ichar_last_index(mk2(), "z") == -1);

  puts( "    Finished");
}



