// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "arr_tests.h"
#include <assert.h>
#include "dmc/Arr.h"

static int greater(double *e1, double *e2) {
  return *e1 > *e2;
}

static double *double_new(double n) {
  double *this = ATOMIC(sizeof(double));
  *this = n;
  return this;
}

static int test(char *result, Arr *a) {
  Buf *bf = buf_new();
  EACH(a, char, e) {
    buf_add(bf, e);
  }_EACH
  return str_eq(result, buf_str(bf));
}

Arr *mk() {
  return arr_new_from("a", "b", "c", NULL);
}

Arr *mk2() {
  return arr_new_from("a", "b", "a", NULL);
}

void arr_tests(void) {
  puts("Arr tests");
  // Arr[double]
  Arr *ia = arr_bf_new(1);

  assert(arr_size(ia) == 0);
  arr_push(ia, double_new(1));
  arr_push(ia, double_new(2));
  assert(*(double *)arr_get(ia, 0) == 1);
  assert(*(double *)arr_get(ia, 1) == 2);
  assert(arr_size(ia) == 2);

  // Arr[double]
  Arr *ia2 = arr_new();
  arr_cat(ia, ia2);
  assert(*(double *)arr_get(ia, 0) == 1);
  assert(*(double *)arr_get(ia, 1) == 2);
  assert(arr_size(ia) == 2);

  arr_push(ia2, double_new(1));
  arr_push(ia2, double_new(2));
  arr_push(ia2, double_new(3));
  assert(*(double *)arr_get(ia2, 0) == 1);
  assert(*(double *)arr_get(ia2, 1) == 2);
  assert(*(double *)arr_get(ia2, 2) == 3);
  assert(arr_size(ia2) == 3);
  arr_cat(ia, ia2);
  assert(*(double *)arr_get(ia, 0) == 1);
  assert(*(double *)arr_get(ia, 1) == 2);
  assert(*(double *)arr_get(ia, 2) == 1);
  assert(*(double *)arr_get(ia, 3) == 2);
  assert(*(double *)arr_get(ia, 4) == 3);
  assert(arr_size(ia) == 5);

  double **p = (double **)arr_start(ia);
  double **p_end = (double **)arr_end(ia);
  while(p < p_end) {
    double *tmp = double_new(**p * 2);
    *p = tmp;
    ++p;
  }

  double *sum = ATOMIC(sizeof(double));
  *sum = 0;
  EACH(ia, double, n)
    *sum += *n;
  _EACH
  assert(*sum == 18);

  // Arr[double]
  Arr *acp = arr_copy(ia);
  *sum = 0;
  EACH(acp, double, n)
    *sum += *n;
  _EACH
  assert(*sum == 18);

  arr_clear(acp);
  assert(arr_size(acp) == 0);


  arr_set(ia, 4, double_new(33));
  arr_remove(ia, 1);
  assert(*(double *)arr_get(ia, 1) == 2);
  assert(*(double *)arr_get(ia, 3) == 33);
  arr_insert(ia, 0, double_new(110));
  assert(*(double *)arr_get(ia, 0) == 110);
  assert(*(double *)arr_get(ia, 2) == 2);
  assert(*(double *)arr_get(ia, 4) == 33);

  RANGE0(i, arr_size(ia))
    arr_set(ia, i, double_new(101));
  _RANGE

  arr_insert_arr(ia, 2, ia2);
  assert(arr_size(ia) == 8);
  assert(*(double *)arr_get(ia, 0) == 101);
  assert(*(double *)arr_get(ia, 2) == 1);
  assert(*(double *)arr_get(ia, 3) == 2);
  assert(*(double *)arr_get(ia, 4) == 3);
  assert(*(double *)arr_get(ia, 7) == 101);

  arr_insert_arr(ia, 2, arr_new());
  assert(arr_size(ia) == 8);
  assert(*(double *)arr_get(ia, 0) == 101);
  assert(*(double *)arr_get(ia, 2) == 1);
  assert(*(double *)arr_get(ia, 3) == 2);
  assert(*(double *)arr_get(ia, 4) == 3);
  assert(*(double *)arr_get(ia, 7) == 101);

  *sum = 0;
  EACH(ia, double, n)
    *sum += *n;
  _EACH
  assert(*sum == 511);

  arr_remove_range(ia, 5, 8);
  arr_sort(ia, (FCMP) greater);
  arr_reverse(ia);

  assert(arr_size(ia) == 5);
  assert(*(double *)arr_get(ia, 0) == 101);
  assert(*(double *)arr_get(ia, 1) == 101);
  assert(*(double *)arr_get(ia, 2) == 3);
  assert(*(double *)arr_get(ia, 3) == 2);
  assert(*(double *)arr_get(ia, 4) == 1);

  arr_remove_range(ia, 0, 0);
  arr_remove_range(ia, 4, 4);
  arr_remove_range(ia, 5, 5);

  assert(arr_size(ia) == 5);
  assert(*(double *)arr_get(ia, 0) == 101);
  assert(*(double *)arr_get(ia, 1) == 101);
  assert(*(double *)arr_get(ia, 2) == 3);
  assert(*(double *)arr_get(ia, 3) == 2);
  assert(*(double *)arr_get(ia, 4) == 1);

  Js *double_to_js_new(double *d) {
    return js_wd(*d);
  }
  double *double_from_js_new(Js *js) {
    double *this = malloc(sizeof(double));
    *this = js_rd(js);
    return this;
  }
  Js *js = arr_to_js(ia, (FTO)double_to_js_new);
  // Arr[double]
  Arr *ia3 = arr_from_js(js, (FFROM)double_from_js_new);
  assert(arr_size(ia3) == 5);
  assert(*(double *)arr_get(ia3, 0) == 101);
  assert(*(double *)arr_get(ia3, 1) == 101);
  assert(*(double *)arr_get(ia3, 2) == 3);
  assert(*(double *)arr_get(ia3, 3) == 2);
  assert(*(double *)arr_get(ia3, 4) == 1);

  ia = arr_new();
  arr_sort(ia, (FCMP) greater);
  arr_reverse(ia);
  assert(arr_size(ia) == 0);

  Arr *sa = arr_new_from("a", "b", NULL);
  assert(arr_size(sa) == 2);
  assert(str_eq(arr_get(sa, 0), "a"));
  assert(str_eq(arr_get(sa, 1), "b"));

  sa = arr_new_c(3, (void *[]){"c", "d", "e"});
  assert(arr_size(sa) == 3);
  assert(str_eq(arr_get(sa, 0), "c"));
  assert(str_eq(arr_get(sa, 1), "d"));
  assert(str_eq(arr_get(sa, 2), "e"));

  assert(test("", arr_take(arr_new(), 0)));
  assert(test("", arr_take(arr_new(), 20)));
  assert(test("", arr_take(mk(), 0)));
  assert(test("ab", arr_take(mk(), 2)));
  assert(test("abc", arr_take(mk(), 20000)));

  int take1(char *e) { return strcmp(e, "") < 0; }
  int take2(char *e) { return strcmp(e, "b") < 0; }
  int take3(char *e) { return strcmp(e, "j") < 0; }

  assert(test("", arr_takef(arr_new(), (FPRED)take1)));
  assert(test("", arr_takef(arr_new(), (FPRED)take3)));
  assert(test("", arr_takef(mk(), (FPRED)take1)));
  assert(test("a", arr_takef(mk(), (FPRED)take2)));
  assert(test("abc", arr_takef(mk(), (FPRED)take3)));

  assert(test("", arr_drop(arr_new(), 0)));
  assert(test("", arr_drop(arr_new(), 20)));
  assert(test("abc", arr_drop(mk(), 0)));
  assert(test("c", arr_drop(mk(), 2)));
  assert(test("", arr_drop(mk(), 20000)));

  assert(test("", arr_dropf(arr_new(), (FPRED)take1)));
  assert(test("", arr_dropf(arr_new(), (FPRED)take3)));
  assert(test("abc", arr_dropf(mk(), (FPRED)take1)));
  assert(test("bc", arr_dropf(mk(), (FPRED)take2)));
  assert(test("", arr_dropf(mk(), (FPRED)take3)));

  int filter1(char *e) { return str_eq(e, "b"); }
  int filter2(char *e) { return !str_eq(e, "b"); }
  int filter3(char *e) { return str_eq(e, "j"); }
  assert(test("", arr_filter_to(arr_new(), (FPRED)filter1)));
  assert(test("", arr_filter_to(arr_new(), (FPRED)filter2)));
  assert(test("b", arr_filter_to(mk(), (FPRED)filter1)));
  assert(test("ac", arr_filter_to(mk(), (FPRED)filter2)));
  assert(test("", arr_filter_to(mk(), (FPRED)filter3)));

  void *map(char *e) { return str_cat(e, "-", NULL); }
  assert(test("", arr_map(arr_new(), (FCOPY)map)));
  assert(test("a-b-c-", arr_map(mk(), (FCOPY)map)));

  void *map1(char *e) { return str_cat("-", e, "-", NULL); }
  assert(test("", arr_map2(arr_new(), (FCOPY)map1, (FCOPY)map)));
  assert(test("-a-", arr_map2(
    arr_new_from("a", NULL), (FCOPY)map1, (FCOPY)map
  )));
  assert(test("-a-b-c-", arr_map2(mk(), (FCOPY)map1, (FCOPY)map)));

  // ------------------------------------------------------------------------ //
  void *zip(Tp *tp) { return str_f("(%s-%s)", tp_e1(tp), tp_e2(tp)); }        //
  // ------------------------------------------------------------------------ //
  assert(test("",
    arr_map(arr_zip(arr_new(), arr_new()), (FCOPY)zip)));
  assert(test("", arr_map(arr_zip(arr_new(), mk()), (FCOPY)zip)));
  assert(test("(x-a)", arr_map(arr_zip(
    arr_new_from("x", NULL), mk()), (FCOPY)zip
  )));
  assert(test("(a-a)(b-b)(c-c)", arr_map(arr_zip(mk(), mk()), (FCOPY)zip)));

  // ------------------------------------------------------------------------ //
  void *zip3(Tp3 *tp3) {                                                      //
    return str_f("(%s-%s-%s)", tp3_e1(tp3), tp3_e2(tp3), tp3_e3(tp3));        //
  }                                                                           //
  // ------------------------------------------------------------------------ //
  assert(test("",
    arr_map(arr_zip3(arr_new(), arr_new(), arr_new()), (FCOPY)zip3)
  ));
  assert(test("", arr_map(arr_zip3(arr_new(), mk(), mk()), (FCOPY)zip3)));
  assert(test("(a-x-a)",
    arr_map(arr_zip3(mk(), arr_new_from("x", NULL), mk()), (FCOPY)zip3)));
  assert(test("(a-a-a)(b-b-b)(c-c-c)",
    arr_map(arr_zip3(mk(), mk(), mk()), (FCOPY)zip3)));

  Tp *dr = arr_duplicates(mk2(), (FCMP)str_eq);
  assert(arr_size(tp_e1(dr)) == 1);
  assert(arr_size(tp_e2(dr)) == 2);
  assert(str_eq(arr_get(tp_e1(dr), 0), "a"));
  assert(str_eq(arr_get(tp_e2(dr), 0), "a"));
  assert(str_eq(arr_get(tp_e2(dr), 1), "b"));

  dr = arr_duplicates(mk(), (FCMP)str_eq);
  assert(arr_size(tp_e1(dr)) == 0);
  assert(arr_size(tp_e2(dr)) == 3);
  assert(str_eq(arr_get(tp_e2(dr), 0), "a"));
  assert(str_eq(arr_get(tp_e2(dr), 1), "b"));
  assert(str_eq(arr_get(tp_e2(dr), 2), "c"));

  puts("    Finished");
}
