// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "it_tests.h"
#include <assert.h>

void it_tests() {
  int test(char *result, It *it) {
    Gc *gc = gc_new();
    Buf *bf = buf_new(gc);
    // -----------------------------------------------------------------------//
    void cat(char *e) { buf_add(bf, e); }                                     //
    // -----------------------------------------------------------------------//
    it_each(it, (FPROC)cat);
    int r = str_eq(result, buf_str(bf));
    gc_free(gc);
    return r;
  }
  It *mk(Gc *gc) {
    return it_cat(
      gc, it_cat(gc, it_unary(gc, "a"), it_unary(gc, "b")), it_unary(gc, "c")
    );
  }

  It *mk2(Gc *gc) {
    return it_cat(
      gc, it_cat(gc, it_unary(gc, "a"), it_unary(gc, "b")), it_unary(gc, "a")
    );
  }

  puts("It test");
  Gc *gc = gc_new();

  assert(!it_has_next(it_empty(gc)));
  It *it1 = it_unary(gc, "x");
  assert(it_has_next(it1));
  assert(str_eq(it_peek(it1), "x"));
  assert(str_eq(it_next(it1), "x"));
  assert(!it_has_next(it1));

  assert(test("", it_empty(gc)));
  assert(test("x", it_unary(gc, "x")));
  assert(test("", it_unary(gc, "")));
  assert(test("x", it_cat(gc, it_empty(gc), it_unary(gc, "x"))));
  assert(test("ax", it_cat(gc, it_unary(gc, "a"), it_unary(gc, "x"))));
  assert(test("abc", mk(gc)));

  int sum = 0;
  void fsum(int *i) {
    sum += *i;
  }

  it_each(it_range0(gc, 4), (FPROC)fsum);
  assert(sum == 6);

  sum = 0;
  it_each(it_range(gc, 2, 5), (FPROC)fsum);
  assert(sum == 9);

  sum = 0;
  it_each(it_range(gc, 2, 2), (FPROC)fsum);
  assert(sum == 0);

  sum = 0;
  it_each(it_range(gc, 3, 2), (FPROC)fsum);
  assert(sum == 0);

  sum = 0;
  it_each(it_range(gc, -4, -1), (FPROC)fsum);
  assert(sum == -9);

  assert(test("", it_take(gc, it_empty(gc), 0)));
  assert(test("", it_take(gc, it_empty(gc), 20)));
  assert(test("", it_take(gc, mk(gc), 0)));
  assert(test("ab", it_take(gc, mk(gc), 2)));
  assert(test("abc", it_take(gc, mk(gc), 20000)));

  int take1(char *e) { return strcmp(e, "") < 0; }
  int take2(char *e) { return strcmp(e, "b") < 0; }
  int take3(char *e) { return strcmp(e, "j") < 0; }

  assert(test("", it_takef(gc, it_empty(gc), (FPRED)take1)));
  assert(test("", it_takef(gc, it_empty(gc), (FPRED)take3)));
  assert(test("", it_takef(gc, mk(gc), (FPRED)take1)));
  assert(test("a", it_takef(gc, mk(gc), (FPRED)take2)));
  assert(test("abc", it_takef(gc, mk(gc), (FPRED)take3)));

  assert(test("", it_drop(gc, it_empty(gc), 0)));
  assert(test("", it_drop(gc, it_empty(gc), 20)));
  assert(test("abc", it_drop(gc, mk(gc), 0)));
  assert(test("c", it_drop(gc, mk(gc), 2)));
  assert(test("", it_drop(gc, mk(gc), 20000)));

  assert(test("", it_dropf(gc, it_empty(gc), (FPRED)take1)));
  assert(test("", it_dropf(gc, it_empty(gc), (FPRED)take3)));
  assert(test("abc", it_dropf(gc, mk(gc), (FPRED)take1)));
  assert(test("bc", it_dropf(gc, mk(gc), (FPRED)take2)));
  assert(test("", it_dropf(gc, mk(gc), (FPRED)take3)));

  int filter1(char *e) { return str_eq(e, "b"); }
  int filter2(char *e) { return !str_eq(e, "b"); }
  int filter3(char *e) { return str_eq(e, "j"); }
  assert(test("", it_filter(gc, it_empty(gc), (FPRED)filter1)));
  assert(test("", it_filter(gc, it_empty(gc), (FPRED)filter2)));
  assert(test("b", it_filter(gc, mk(gc), (FPRED)filter1)));
  assert(test("ac", it_filter(gc, mk(gc), (FPRED)filter2)));
  assert(test("", it_filter(gc, mk(gc), (FPRED)filter3)));

  void *map(Gc *gc, char *e) { return str_cat(gc, e, "-", NULL); }
  assert(test("", it_map(gc, it_empty(gc), (FCOPY)map)));
  assert(test("a-b-c-", it_map(gc, mk(gc), (FCOPY)map)));

  void *map1(Gc *gc, char *e) { return str_cat(gc, "-", e, "-", NULL); }
  assert(test("", it_map2(gc, it_empty(gc), (FCOPY)map1, (FCOPY)map)));
  assert(test("-a-", it_map2(gc, it_unary(gc, "a"), (FCOPY)map1, (FCOPY)map)));
  assert(test("-a-b-c-", it_map2(gc, mk(gc), (FCOPY)map1, (FCOPY)map)));

  // ------------------------------------------------------------------------ //
  void *zip(Gc *gc, Tp *tp) {                                                 //
    return str_f(gc, "(%s-%s)", tp_e1(tp), tp_e2(tp));                        //
  }                                                                           //
  // ------------------------------------------------------------------------ //
  assert(test("",
    it_map(gc, it_zip(gc, it_empty(gc), it_empty(gc)), (FCOPY)zip)));
  assert(test("", it_map(gc, it_zip(gc, it_empty(gc), mk(gc)), (FCOPY)zip)));
  assert(test("(x-a)",
    it_map(gc, it_zip(gc, it_unary(gc, "x"), mk(gc)), (FCOPY)zip)));
  assert(test("(a-a)(b-b)(c-c)",
    it_map(gc, it_zip(gc, mk(gc), mk(gc)), (FCOPY)zip)));

  // ------------------------------------------------------------------------ //
  void *zip3(Gc *gc, Tp3 *tp3) {                                              //
    return str_f(gc, "(%s-%s-%s)", tp3_e1(tp3), tp3_e2(tp3), tp3_e3(tp3));    //
  }                                                                           //
  // ------------------------------------------------------------------------ //
  assert(test("",
    it_map(gc,
      it_zip3(gc, it_empty(gc), it_empty(gc), it_empty(gc)), (FCOPY)zip3)
  ));
  assert(test("",
    it_map(gc, it_zip3(gc, it_empty(gc), mk(gc), mk(gc)), (FCOPY)zip3)));
  assert(test("(a-x-a)",
    it_map(gc, it_zip3(gc, mk(gc), it_unary(gc, "x"), mk(gc)), (FCOPY)zip3)));
  assert(test("(a-a-a)(b-b-b)(c-c-c)",
    it_map(gc, it_zip3(gc, mk(gc), mk(gc), mk(gc)), (FCOPY)zip3)));

  assert(it_eq(it_empty(gc), it_empty(gc), (FCMP)str_eq));
  assert(it_eq(it_unary(gc, "x"), it_unary(gc, "x"), (FCMP)str_eq));
  assert(it_eq(mk(gc), mk(gc), (FCMP)str_eq));
  assert(!it_eq(it_empty(gc), it_unary(gc, "z"), (FCMP)str_eq));
  assert(!it_eq(it_unary(gc, "z"), it_empty(gc), (FCMP)str_eq));
  assert(!it_eq(it_unary(gc, "z"), it_unary(gc, "a"), (FCMP)str_eq));
  assert(!it_eq(mk(gc), it_unary(gc, "a"), (FCMP)str_eq));
  assert(it_eq(it_take(gc, mk(gc), 1), it_unary(gc, "a"), (FCMP)str_eq));

  // ------------------------------------------------------------------------ //
  int eq0 (char *e) { return !*e; }                                           //
  int eqa (char *e) { return str_eq(e, "a"); }                                //
  int eqz (char *e) { return str_eq(e, "z"); }                                //
  // ------------------------------------------------------------------------ //
  assert(it_index(it_empty(gc), (FPRED)eq0) == -1);
  assert(it_index(it_unary(gc, "a"), (FPRED)eq0) == -1);
  assert(it_index(it_empty(gc), (FPRED)eqa) == -1);
  assert(it_index(it_unary(gc, "a"), (FPRED)eqa) == 0);
  assert(it_index(mk2(gc), (FPRED)eqa) == 0);
  assert(it_index(it_unary(gc, "a"), (FPRED)eqz) == -1);
  assert(it_index(mk2(gc), (FPRED)eqz) == -1);

  assert(!it_contains(it_empty(gc), (FPRED)eq0));
  assert(!it_contains(it_unary(gc, "a"), (FPRED)eq0));
  assert(!it_contains(it_empty(gc), (FPRED)eqa));
  assert(it_contains(it_unary(gc, "a"), (FPRED)eqa));
  assert(it_contains(mk2(gc), (FPRED)eqa));
  assert(!it_contains(it_unary(gc, "a"), (FPRED)eqz));
  assert(!it_contains(mk2(gc), (FPRED)eqz));

  assert(it_last_index(it_empty(gc), (FPRED)eq0) == -1);
  assert(it_last_index(it_unary(gc, "a"), (FPRED)eq0) == -1);
  assert(it_last_index(it_empty(gc), (FPRED)eqa) == -1);
  assert(it_last_index(it_unary(gc, "a"), (FPRED)eqa) == 0);
  assert(it_last_index(mk2(gc), (FPRED)eqa) == 2);
  assert(it_last_index(it_unary(gc, "a"), (FPRED)eqz) == -1);
  assert(it_last_index(mk2(gc), (FPRED)eqz) == -1);

  gc_free(gc);
  puts( "    Finished");

}

