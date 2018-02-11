// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dm.h"

void it_test() {
  int test(char *result, It *it) {
    Buf *bf = buf_new();
    FNX (cat, char, e) { buf_add(bf, e); }_FN
    it_each(it, cat);
    return !strcmp(result, buf_str(bf));
  }
  It *mk() {
    return it_cat(it_cat(it_unary("a"), it_unary("b")), it_unary("c"));
  }

  It *mk2() {
    return it_cat(it_cat(it_unary("a"), it_unary("b")), it_unary("a"));
  }

  puts("It test");

  assert(!it_has_next(it_empty()));
  It *it1 = it_unary("x");
  assert(it_has_next(it1));
  it1 = it_to_peek(it1);
  assert(!strcmp(it_peek(it1), "x"));
  assert(!strcmp(it_next(it1), "x"));
  assert(!it_has_next(it1));

  assert(test("", it_empty()));
  assert(test("x", it_unary("x")));
  assert(test("", it_unary("")));
  assert(test("x", it_cat(it_empty(), it_unary("x"))));
  assert(test("ax", it_cat(it_unary("a"), it_unary("x"))));
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

  assert(test("", it_take(it_empty(), 0)));
  assert(test("", it_take(it_empty(), 20)));
  assert(test("", it_take(mk(), 0)));
  assert(test("ab", it_take(mk(), 2)));
  assert(test("abc", it_take(mk(), 20000)));

  FNP (take1, char, e) { return strcmp(e, "") < 0; }_FN
  FNP (take2, char, e) { return strcmp(e, "b") < 0; }_FN
  FNP (take3, char, e) { return strcmp(e, "j") < 0; }_FN

  assert(test("", it_takef(it_empty(), take1)));
  assert(test("", it_takef(it_empty(), take3)));
  assert(test("", it_takef(mk(), take1)));
  assert(test("a", it_takef(mk(), take2)));
  assert(test("abc", it_takef(mk(), take3)));

  assert(test("", it_drop(it_empty(), 0)));
  assert(test("", it_drop(it_empty(), 20)));
  assert(test("abc", it_drop(mk(), 0)));
  assert(test("c", it_drop(mk(), 2)));
  assert(test("", it_drop(mk(), 20000)));

  assert(test("", it_dropf(it_empty(), take1)));
  assert(test("", it_dropf(it_empty(), take3)));
  assert(test("abc", it_dropf(mk(), take1)));
  assert(test("bc", it_dropf(mk(), take2)));
  assert(test("", it_dropf(mk(), take3)));

  FNP (filter1, char, e) { return !strcmp(e, "b"); }_FN
  FNP (filter2, char, e) { return strcmp(e, "b"); }_FN
  FNP (filter3, char, e) { return !strcmp(e, "j"); }_FN
  assert(test("", it_filter(it_empty(), filter1)));
  assert(test("", it_filter(it_empty(), filter2)));
  assert(test("b", it_filter(mk(), filter1)));
  assert(test("ac", it_filter(mk(), filter2)));
  assert(test("", it_filter(mk(), filter3)));

  FNM (map, char, e) { return str_cat(e, "-", NULL); }_FN
  assert(test("", it_map(it_empty(), map)));
  assert(test("a-b-c-", it_map(mk(), map)));

  FNM (map1, char, e) { return str_cat("-", e, "-", NULL); }_FN
  assert(test("", it_map2(it_empty(), map1, map)));
  assert(test("-a-", it_map2(it_unary("a"), map1, map)));
  assert(test("-a-b-c-", it_map2(mk(), map1, map)));

  FNM (zip, Tp, tp) { return str_printf("(%s-%s)", tp->e1, tp->e2); }_FN
  assert(test("", it_map(it_zip(it_empty(), it_empty()), zip)));
  assert(test("", it_map(it_zip(it_empty(), mk()), zip)));
  assert(test("(x-a)", it_map(it_zip(it_unary("x"), mk()), zip)));
  assert(test("(a-a)(b-b)(c-c)", it_map(it_zip(mk(), mk()), zip)));

  FNM (zip3, Tp3, tp) {
    return str_printf("(%s-%s-%s)", tp->e1, tp->e2, tp->e3);
  }_FN
  assert(test("",
    it_map(it_zip3(it_empty(), it_empty(), it_empty()), zip3)));
  assert(test("",
    it_map(it_zip3(it_empty(), mk(), mk()), zip3)));
  assert(test("(a-x-a)",
    it_map(it_zip3(mk(), it_unary("x"), mk()), zip3)));
  assert(test("(a-a-a)(b-b-b)(c-c-c)",
    it_map(it_zip3(mk(), mk(), mk()), zip3)));

  assert(it_eq_str(it_empty(), it_empty()));
  assert(it_eq_str(it_unary("x"), it_unary("x")));
  assert(it_eq_str(mk(), mk()));
  assert(!it_eq_str(it_empty(), it_unary("z")));
  assert(!it_eq_str(it_unary("z"), it_empty()));
  assert(!it_eq_str(it_unary("z"), it_unary("a")));
  assert(!it_eq_str(mk(), it_unary("a")));
  assert(it_eq_str(it_take(mk(), 1), it_unary("a")));

  assert(it_index_str(it_empty(), "") == -1);
  assert(it_index_str(it_unary("a"), "") == -1);
  assert(it_index_str(it_empty(), "a") == -1);
  assert(it_index_str(it_unary("a"), "a") == 0);
  assert(it_index_str(mk2(), "a") == 0);
  assert(it_index_str(it_unary("a"), "z") == -1);
  assert(it_index_str(mk2(), "z") == -1);

  assert(!it_contains_str(it_empty(), ""));
  assert(!it_contains_str(it_unary("a"), ""));
  assert(!it_contains_str(it_empty(), "a"));
  assert(it_contains_str(it_unary("a"), "a"));
  assert(it_contains_str(mk2("a"), "a"));
  assert(!it_contains_str(it_unary("a"), "z"));
  assert(!it_contains_str(mk2("a"), "z"));

  assert(it_last_index_str(it_empty(), "") == -1);
  assert(it_last_index_str(it_unary("a"), "") == -1);
  assert(it_last_index_str(it_empty(), "a") == -1);
  assert(it_last_index_str(it_unary("a"), "a") == 0);
  assert(it_last_index_str(mk2(), "a") == 2);
  assert(it_last_index_str(it_unary("a"), "z") == -1);
  assert(it_last_index_str(mk2(), "z") == -1);

  char *to_str(void *e) { return (char *)e; }
  void *to_void(char *e) { return e; }
  Arr *a = arr_new();
  assert(!strcmp(it_serialize(it_from(a), to_str), ""));
  arr_add(a, "");
  assert(!strcmp(it_serialize(it_from(a), to_str), "\"\""));
  arr_add(a, "1");
  assert(!strcmp(it_serialize(it_from(a), to_str), "\"\"\"1\""));
  arr_add(a, "\"a\"\"6\"");
  assert(!strcmp(it_serialize(it_from(a), to_str),
    "\"\"\"1\"\"\\\"a\\\"\\\"6\\\"\""));
  a = arr_new();
  assert(arr_size(a) == 0);
  a = it_to(it_restore("\"\"\"1\"\"\\\"a\\\"\\\"6\\\"\"", to_void));
  assert(arr_size(a) == 3);
  assert(!strcmp(arr_es(a)[0], ""));
  assert(!strcmp(arr_es(a)[1], "1"));
  assert(!strcmp(arr_es(a)[2], "\"a\"\"6\""));
  a = it_to(it_restore("", to_void));
  assert(arr_size(a) == 0);

  puts( "    Finished");
}



