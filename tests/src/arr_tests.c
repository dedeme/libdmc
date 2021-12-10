// Copyright 21-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "arr_tests.h"
#include <assert.h>
#include <string.h>
#include "dmc/Arr.h"
#include "dmc/Buf.h"

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
  void **p = a->es;
  while (p < a->end) buf_add(bf, *p++);
  return !strcmp(result, bf->str);
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

  double **p = (double **)ia->es;
  double **p_end = (double **)ia->end;
  while(p < p_end) {
    double *tmp = double_new(**p * 2);
    *p = tmp;
    ++p;
  }

  double *sum = ATOMIC(sizeof(double));
  *sum = 0;
  for (void **p = ia->es; p < ia->end; ++p) *sum += **(double **)p;
  assert(*sum == 18);

  // Arr[double]
  Arr *acp = arr_copy(ia);
  *sum = 0;
  for (void **p = acp->es; p < acp->end; ++p) *sum += **(double **)p;
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

  for(int i = 0; i < arr_size(ia); ++i)
    arr_set(ia, i, double_new(101));

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
  for (void **p = ia->es; p < ia->end; ++p) *sum += **(double **)p;
  assert(*sum == 511);

  arr_remove_range(ia, 5, 8);
  arr_shuffle(ia);
  *sum = 0;
  for (void **p = ia->es; p < ia->end; ++p) *sum += **(double **)p;
  assert(*sum == 208);

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

  ia = arr_new();
  arr_sort(ia, (FCMP) greater);
  arr_reverse(ia);
  assert(arr_size(ia) == 0);

  Arr *sa = arr_new_from("a", "b", NULL);
  assert(arr_size(sa) == 2);
  assert(!strcmp(arr_get(sa, 0), "a"));
  assert(!strcmp(arr_get(sa, 1), "b"));

  sa = arr_new_c(3, (void *[]){"c", "d", "e"});
  assert(arr_size(sa) == 3);
  assert(!strcmp(arr_get(sa, 0), "c"));
  assert(!strcmp(arr_get(sa, 1), "d"));
  assert(!strcmp(arr_get(sa, 2), "e"));

  int pred (void *s) { return *((char *)s) < 'd'; }
  assert(arr_index(arr_new(), pred) == -1);
  assert(arr_last_index(arr_new(), pred) == -1);
  assert(opt_none(arr_find(arr_new(), pred)));
  assert(opt_none(arr_find_last(arr_new(), pred)));
  assert(!arr_any(arr_new(), pred));
  assert(arr_all(arr_new(), pred));
  assert(arr_index(arr_new_from("a", "b", NULL), pred) == 0);
  assert(arr_index(arr_new_from("d", "e", "b", NULL), pred) == 2);
  assert(arr_index(arr_new_from("d", "e", NULL), pred) == -1);
  assert(arr_last_index(arr_new_from("a", "b", NULL), pred) == 1);
  assert(arr_last_index(arr_new_from("a", "b", "a", NULL), pred) == 2);
  assert(arr_last_index(arr_new_from("d", "e", "b", NULL), pred) == 2);
  assert(arr_last_index(arr_new_from("d", "e", NULL), pred) == -1);
  assert(str_eq("a",
    opt_some(arr_find(arr_new_from("a", "b", NULL), pred))));
  assert(str_eq("b",
    opt_some(arr_find(arr_new_from("d", "e", "b", NULL), pred))));
  assert(opt_none(arr_find(arr_new_from("d", "e", NULL), pred)));
  assert(str_eq("a",
    opt_some(arr_find_last(arr_new_from("a", "b", "a", NULL), pred))));
  assert(str_eq("b",
    opt_some(arr_find_last(arr_new_from("d", "e", "b", NULL), pred))));
  assert(str_eq("b",
    opt_some(arr_find_last(arr_new_from("a", "b", NULL), pred))));
  assert(opt_none(arr_find_last(arr_new_from("d", "e", NULL), pred)));
  assert(arr_all(arr_new_from("a", "b", NULL), pred));
  assert(!arr_all(arr_new_from("d", "e", "b", NULL), pred));
  assert(!arr_all(arr_new_from("d", "e", NULL), pred));
  assert(arr_any(arr_new_from("a", "b", NULL), pred));
  assert(arr_any(arr_new_from("d", "e", "b", NULL), pred));
  assert(!arr_any(arr_new_from("d", "e", NULL), pred));

  assert(test("", arr_take(arr_new(), 0)));
  assert(test("", arr_take(arr_new(), 20)));
  assert(test("", arr_take(mk(), 0)));
  assert(test("", arr_take(mk(), -1)));
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
  assert(test("abc", arr_drop(mk(), -12)));
  assert(test("c", arr_drop(mk(), 2)));
  assert(test("", arr_drop(mk(), 20000)));

  assert(test("", arr_dropf(arr_new(), (FPRED)take1)));
  assert(test("", arr_dropf(arr_new(), (FPRED)take3)));
  assert(test("abc", arr_dropf(mk(), (FPRED)take1)));
  assert(test("bc", arr_dropf(mk(), (FPRED)take2)));
  assert(test("", arr_dropf(mk(), (FPRED)take3)));

  int filter1(char *e) { return !strcmp(e, "b"); }
  int filter2(char *e) { return strcmp(e, "b"); }
  int filter3(char *e) { return !strcmp(e, "j"); }
  assert(test("", arr_filter_to(arr_new(), (FPRED)filter1)));
  assert(test("", arr_filter_to(arr_new(), (FPRED)filter2)));
  assert(test("b", arr_filter_to(mk(), (FPRED)filter1)));
  assert(test("ac", arr_filter_to(mk(), (FPRED)filter2)));
  assert(test("", arr_filter_to(mk(), (FPRED)filter3)));

  void *map(char *e) {
    char *s = ATOMIC(80);
    sprintf(s, "%s-", e);
    return s;
  }
  assert(test("", arr_map(arr_new(), (FMAP)map)));
  assert(test("a-b-c-", arr_map(mk(), (FMAP)map)));

  void *map1(char *e) {
    char *s = ATOMIC(80);
    sprintf(s, "-%s-", e);
    return s;
  }
  assert(test("", arr_map2(arr_new(), (FMAP)map1, (FMAP)map)));
  assert(test("-a-", arr_map2(
    arr_new_from("a", NULL), (FMAP)map1, (FMAP)map
  )));
  assert(test("-a-b-c-", arr_map2(mk(), (FMAP)map1, (FMAP)map)));

  // ------------------------------------------------------------------------ //
  void *zip(void *e1, void *e2) {
    char *s = ATOMIC(80);
    sprintf(s, "(%s-%s)", (char*)e1, (char *)e2);
    return s;
  }
  // ------------------------------------------------------------------------ //
  assert(test("", arr_zip(arr_new(), arr_new(), zip)));
  assert(test("", arr_zip(arr_new(), mk(), zip)));
  assert(test("(x-a)", arr_zip(arr_new_from("x", NULL), mk(), zip)));
  assert(test("(a-a)(b-b)(c-c)", arr_zip(mk(), mk(), zip)));

  // ------------------------------------------------------------------------ //
  void *zip3(void *e1, void *e2, void *e3) {                                                      //
    char *s = ATOMIC(80);
    sprintf(s, "(%s-%s-%s)", (char*)e1, (char *)e2, (char *)e3);
    return s;
  }                                                                           //
  // ------------------------------------------------------------------------ //
  assert(test("", arr_zip3(arr_new(), arr_new(), arr_new(),zip3)));
  assert(test("", arr_zip3(arr_new(), mk(), mk(), zip3)));
  assert(test("(a-x-a)",
    arr_zip3(mk(), arr_new_from("x", NULL), mk(), zip3)));
  assert(test("(a-a-a)(b-b-b)(c-c-c)",
    arr_zip3(mk(), mk(), mk(), zip3)));

  sa = mk2();
  Arr *d = arr_duplicates(sa, (FCMP)str_eq);
  assert(arr_size(d) == 1);
  assert(arr_size(sa) == 2);
  assert(str_eq(arr_get(d, 0), "a"));
  assert(str_eq(arr_get(sa, 0), "a"));
  assert(str_eq(arr_get(sa, 1), "b"));

  sa = mk();
  d = arr_duplicates(sa, (FCMP)str_eq);
  assert(arr_size(d) == 0);
  assert(arr_size(sa) == 3);
  assert(str_eq(arr_get(sa, 0), "a"));
  assert(str_eq(arr_get(sa, 1), "b"));
  assert(str_eq(arr_get(sa, 2), "c"));

  puts("    Finished");
}
