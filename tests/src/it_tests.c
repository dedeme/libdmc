// Copyright 23-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "it_tests.h"
#include <stdio.h>
#include <assert.h>
#include "dmc/It.h"
#include "dmc/Buf.h"
#include "dmc/str.h"
#include "dmc/DEFS.h"

void it_tests() {
  int test(char *result, It *it) {
    Buf *bf = buf_new();
    // -----------------------------------------------------------------------//
    void cat(char *e) { buf_add(bf, e); }                                     //
    // -----------------------------------------------------------------------//
    it_each(it, (FPROC)cat);
    return str_eq(result, buf_str(bf));
  }
  It *mk() {
    return it_cat(
      it_cat(it_unary("a"), it_unary("b")), it_unary("c")
    );
  }

  It *mk2() {
    return it_cat(
      it_cat(it_unary("a"), it_unary("b")), it_unary("a")
    );
  }

  puts("It test");

  assert(!it_has_next(it_empty()));
  It *it1 = it_unary("x");
  assert(it_has_next(it1));
  assert(str_eq(it_peek(it1), "x"));
  assert(str_eq(it_next(it1), "x"));
  assert(!it_has_next(it1));

  assert(test("", it_empty()));
  assert(test("x", it_unary("x")));
  assert(test("", it_unary("")));
  assert(test("x", it_cat(it_empty(), it_unary("x"))));
  assert(test("ax", it_cat(it_unary("a"), it_unary("x"))));
  assert(test("abc", mk()));

  int sum = 0;
  void fsum(int *i) {
    sum += *i;
  }

  it_each(it_range0(4), (FPROC)fsum);
  assert(sum == 6);

  sum = 0;
  it_each(it_range(2, 5), (FPROC)fsum);
  assert(sum == 9);

  sum = 0;
  it_each(it_range(2, 2), (FPROC)fsum);
  assert(sum == 0);

  sum = 0;
  it_each(it_range(3, 2), (FPROC)fsum);
  assert(sum == 0);

  sum = 0;
  it_each(it_range(-4, -1), (FPROC)fsum);
  assert(sum == -9);

  assert(test("", it_take(it_empty(), 0)));
  assert(test("", it_take(it_empty(), 20)));
  assert(test("", it_take(mk(), 0)));
  assert(test("ab", it_take(mk(), 2)));
  assert(test("abc", it_take(mk(), 20000)));

  int take1(char *e) { return strcmp(e, "") < 0; }
  int take2(char *e) { return strcmp(e, "b") < 0; }
  int take3(char *e) { return strcmp(e, "j") < 0; }

  assert(test("", it_takef(it_empty(), (FPRED)take1)));
  assert(test("", it_takef(it_empty(), (FPRED)take3)));
  assert(test("", it_takef(mk(), (FPRED)take1)));
  assert(test("a", it_takef(mk(), (FPRED)take2)));
  assert(test("abc", it_takef(mk(), (FPRED)take3)));

  assert(test("", it_drop(it_empty(), 0)));
  assert(test("", it_drop(it_empty(), 20)));
  assert(test("abc", it_drop(mk(), 0)));
  assert(test("c", it_drop(mk(), 2)));
  assert(test("", it_drop(mk(), 20000)));

  assert(test("", it_dropf(it_empty(), (FPRED)take1)));
  assert(test("", it_dropf(it_empty(), (FPRED)take3)));
  assert(test("abc", it_dropf(mk(), (FPRED)take1)));
  assert(test("bc", it_dropf(mk(), (FPRED)take2)));
  assert(test("", it_dropf(mk(), (FPRED)take3)));

  int filter1(char *e) { return str_eq(e, "b"); }
  int filter2(char *e) { return !str_eq(e, "b"); }
  int filter3(char *e) { return str_eq(e, "j"); }
  assert(test("", it_filter(it_empty(), (FPRED)filter1)));
  assert(test("", it_filter(it_empty(), (FPRED)filter2)));
  assert(test("b", it_filter(mk(), (FPRED)filter1)));
  assert(test("ac", it_filter(mk(), (FPRED)filter2)));
  assert(test("", it_filter(mk(), (FPRED)filter3)));

  void *map(char *e) { return str_cat(e, "-", NULL); }
  assert(test("", it_map(it_empty(), (FCOPY)map)));
  assert(test("a-b-c-", it_map(mk(), (FCOPY)map)));

  void *map1(char *e) { return str_cat("-", e, "-", NULL); }
  assert(test("", it_map2(it_empty(), (FCOPY)map1, (FCOPY)map)));
  assert(test("-a-", it_map2(it_unary("a"), (FCOPY)map1, (FCOPY)map)));
  assert(test("-a-b-c-", it_map2(mk(), (FCOPY)map1, (FCOPY)map)));

  // ------------------------------------------------------------------------ //
  void *zip(Tp *tp) { return str_f("(%s-%s)", tp_e1(tp), tp_e2(tp)); }        //
  // ------------------------------------------------------------------------ //
  assert(test("",
    it_map(it_zip(it_empty(), it_empty()), (FCOPY)zip)));
  assert(test("", it_map(it_zip(it_empty(), mk()), (FCOPY)zip)));
  assert(test("(x-a)", it_map(it_zip(it_unary("x"), mk()), (FCOPY)zip)));
  assert(test("(a-a)(b-b)(c-c)", it_map(it_zip(mk(), mk()), (FCOPY)zip)));

  // ------------------------------------------------------------------------ //
  void *zip3(Tp3 *tp3) {                                                      //
    return str_f("(%s-%s-%s)", tp3_e1(tp3), tp3_e2(tp3), tp3_e3(tp3));        //
  }                                                                           //
  // ------------------------------------------------------------------------ //
  assert(test("",
    it_map(it_zip3(it_empty(), it_empty(), it_empty()), (FCOPY)zip3)
  ));
  assert(test("", it_map(it_zip3(it_empty(), mk(), mk()), (FCOPY)zip3)));
  assert(test("(a-x-a)",
    it_map(it_zip3(mk(), it_unary("x"), mk()), (FCOPY)zip3)));
  assert(test("(a-a-a)(b-b-b)(c-c-c)",
    it_map(it_zip3(mk(), mk(), mk()), (FCOPY)zip3)));

  assert(it_eq(it_empty(), it_empty(), (FCMP)str_eq));
  assert(it_eq(it_unary("x"), it_unary("x"), (FCMP)str_eq));
  assert(it_eq(mk(), mk(), (FCMP)str_eq));
  assert(!it_eq(it_empty(), it_unary("z"), (FCMP)str_eq));
  assert(!it_eq(it_unary("z"), it_empty(), (FCMP)str_eq));
  assert(!it_eq(it_unary("z"), it_unary("a"), (FCMP)str_eq));
  assert(!it_eq(mk(), it_unary("a"), (FCMP)str_eq));
  assert(it_eq(it_take(mk(), 1), it_unary("a"), (FCMP)str_eq));

  // ------------------------------------------------------------------------ //
  int eq0 (char *e) { return !*e; }                                           //
  int eqa (char *e) { return str_eq(e, "a"); }                                //
  int eqz (char *e) { return str_eq(e, "z"); }                                //
  // ------------------------------------------------------------------------ //
  assert(it_index(it_empty(), (FPRED)eq0) == -1);
  assert(it_index(it_unary("a"), (FPRED)eq0) == -1);
  assert(it_index(it_empty(), (FPRED)eqa) == -1);
  assert(it_index(it_unary("a"), (FPRED)eqa) == 0);
  assert(it_index(mk2(), (FPRED)eqa) == 0);
  assert(it_index(it_unary("a"), (FPRED)eqz) == -1);
  assert(it_index(mk2(), (FPRED)eqz) == -1);

  assert(!it_contains(it_empty(), (FPRED)eq0));
  assert(!it_contains(it_unary("a"), (FPRED)eq0));
  assert(!it_contains(it_empty(), (FPRED)eqa));
  assert(it_contains(it_unary("a"), (FPRED)eqa));
  assert(it_contains(mk2("a"), (FPRED)eqa));
  assert(!it_contains(it_unary("a"), (FPRED)eqz));
  assert(!it_contains(mk2("a"), (FPRED)eqz));

  assert(it_last_index(it_empty(), (FPRED)eq0) == -1);
  assert(it_last_index(it_unary("a"), (FPRED)eq0) == -1);
  assert(it_last_index(it_empty(), (FPRED)eqa) == -1);
  assert(it_last_index(it_unary("a"), (FPRED)eqa) == 0);
  assert(it_last_index(mk2(), (FPRED)eqa) == 2);
  assert(it_last_index(it_unary("a"), (FPRED)eqz) == -1);
  assert(it_last_index(mk2(), (FPRED)eqz) == -1);

  Tp *dr = it_duplicates(mk2(), (FCMP)str_eq);
  assert(arr_size(tp_e1(dr)) == 1);
  assert(arr_size(tp_e2(dr)) == 2);
  assert(str_eq(arr_get(tp_e1(dr), 0), "a"));
  assert(str_eq(arr_get(tp_e2(dr), 0), "a"));
  assert(str_eq(arr_get(tp_e2(dr), 1), "b"));

  dr = it_duplicates(mk(), (FCMP)str_eq);
  assert(arr_size(tp_e1(dr)) == 0);
  assert(arr_size(tp_e2(dr)) == 3);
  assert(str_eq(arr_get(tp_e2(dr), 0), "a"));
  assert(str_eq(arr_get(tp_e2(dr), 1), "b"));
  assert(str_eq(arr_get(tp_e2(dr), 2), "c"));

  puts( "    Finished");

}
