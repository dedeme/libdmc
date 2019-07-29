// Copyright 17-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "js_tests.h"
#include <assert.h>
#include "dmc/std.h"

void js_tests(void) {
  puts("Js tests");

  Js *s;
  char *str;

  assert(js_is_null((Js*)"null"));
  assert(js_is_null((Js*)"  null   "));
  assert(!js_is_null((Js*)""));
  assert(!js_is_null((Js*)"nuln"));
  s = js_wn();
  assert(str_eq((char *)s, "null"));
  assert(js_rb((Js*)" true "));
  assert(js_rb((Js*)" false") == 0);

//  js_rb((Js*)"true and"); // error
//  js_rb((Js*)"xx"); // error

  s = js_wb(1);
  assert(js_rb(s));
  s = js_wb(0);
  assert(!js_rb(s));

  assert(js_ri((Js*)" 0 ") == 0);
  assert(js_rd((Js*)" 0.23 ") == 0.23);
  assert(js_ri((Js*)" -0 ") == 0);
  assert(js_rd((Js*)" -0.0 ") == 0);
  assert(js_rd((Js*)" -12.11 ") == -12.11);
  assert(js_rd((Js*)" -12.11e22 ") == -12.11e22);

//  js_ri((Js*)" 12abc "); // error
//  js_ri((Js*)" 12] "); // error
//  js_ri((Js*)" 12 }"); // error
//  js_rd((Js*)" .12"); // error
//  js_rd((Js*)" z.12"); // error

  s = js_wi(0);
  assert(js_ri(s) == 0);
  s = js_wi(254);
  assert(js_ri(s) == 254);
  s = js_wi(-1100);
  assert(js_ri(s) == -1100);
  s = js_wd(0.0);
  assert(js_rd(s) == 0);
  s = js_wd(-0.0);
  assert(js_rd(s) == 0);
  s = js_wd(-.0);
  assert(js_rd(s) == 0);
  s = js_wd(1.045);
  assert(js_rd(s) == 1.045);
  s = js_wd(-21.045);
  assert(js_rd(s) == -21.045);
  s = js_wd(-21.04);
  assert(js_rd(s) == -21.04);

  str = js_rs((Js*)"  \"\" ");
  assert(str_eq("", str));
  str = js_rs((Js*)"  \"a\\u0030\" ");
  assert(str_eq("a0", str));
  str = js_rs((Js*)"  \"a\\t\\n\\\"\" ");
  assert(str_eq("a\t\n\"", str));

//  js_rs((Js*)" \""); // Error
//  js_rs((Js*)" \"a\" l"); // Error
//  js_rs((Js*)" \" \\ \" "); // Error
//  js_rs((Js*)" \" \\@ \" "); // Error
//  js_rs((Js*)" \" \\u30 \" "); // Error

  // Arr[Js]
  Arr *a, *a2;
  a = js_ra((Js*)"[]");
  assert(arr_size(a) == 0);
  a = js_ra((Js*)"[123]");
  assert(arr_size(a) == 1);
  double rsd = js_rd(arr_get(a, 0));
  assert(rsd == 123);
  a = js_ra((Js*)"[-123.56, true]");
  assert(arr_size(a) == 2);
  rsd = js_rd(arr_get(a, 0));
  assert(rsd == -123.56);
  int rs = js_rb(arr_get(a, 1));
  assert(rs == 1);
  a = js_ra((Js*)"[-123.56, true, \"a\"]");
  assert(arr_size(a) == 3);
  str = js_rs(arr_get(a, 2));
  assert(str_eq("a", str));

  a = js_ra((Js*)"[-123.56, true, [], 56]");
  assert(arr_size(a) == 4);
  rsd = js_rd(arr_get(a, 3));
  assert(rsd == 56);
  a2 = js_ra(arr_get(a, 2));
  assert(arr_size(a2) == 0);

  a = js_ra((Js*)" [-123.56, true, [\"azf\", false], 56]  ");
  assert(arr_size(a) == 4);
  rsd = js_rd(arr_get(a, 3));
  assert(rsd == 56);
  a2 = js_ra(arr_get(a, 2));
  assert(arr_size(a2) == 2);
  rs = js_rb(arr_get(a2, 1));
  assert(rs == 0);
  str = js_rs(arr_get(a2, 0));
  assert(str_eq("azf", str));

//  js_ra((Js*)"[-123.56, true, [], 56] h"); // Error
//  js_ra((Js*)"[s123.56, true, [], 56] "); // Error
//  js_ra((Js*)" "); // Error
//  js_ra((Js*)"[-123.56, true, [], true   "); // Error

  // a1 and a2 are Arr[Js]
  int arr_eq_str(Arr *a1, Arr *a2) {
    int size = arr_size(a1);
    if (size != arr_size(a2)) {
      return 0;
    }
    RANGE0(i, size)
      if (!str_eq(arr_get(a1, i), arr_get(a2, i))) {
        return 0;
      }
    _RANGE
    return 1;
  }

  // Arr[Js]
  Arr *a3;
  a = arr_new();
  s = js_wa(a);
  a3 = js_ra(s);
  assert(arr_eq_str(a, a3));

  arr_push(a, js_wb(1));
  s = js_wa(a);
  a3 = js_ra(s);
  assert(arr_eq_str(a, a3));

  arr_push(a, js_wi(-16));
  s = js_wa(a);
  a3 = js_ra(s);
  assert(arr_eq_str(a, a3));

  arr_push(a, js_wd(1));
  s = js_wa(a);
  a3 = js_ra(s);
  assert(arr_eq_str(a, a3));

  arr_push(a, js_ws("caf"));
  s = js_wa(a);
  a3 = js_ra(s);
  assert(arr_eq_str(a, a3));

  a2 = arr_new();
  arr_push(a, js_wa(a2));
  s = js_wa(a);
  a3 = js_ra(s);
  assert(arr_eq_str(a, a3));

  arr_push(a2, js_ws("a\n\tzzð"));
  arr_push(a, js_wa(a2));
  s = js_wa(a);
  a3 = js_ra(s);
  assert(arr_eq_str(a, a3));

  Map *m;
  m = js_ro((Js*)"{}");
  assert(map_size(m) == 0);

  m = js_ro((Js*)" {\"a\":123 } ");
  assert(map_size(m) == 1);
  rsd = js_rd(opt_get(map_get(m, "a")));
  assert(rsd == 123);

  m = js_ro((Js*)" {\"a\":123, \"b\":true } ");
  assert(map_size(m) == 2);
  rs = js_rb(opt_get(map_get(m, "b")));
  assert(rs);

  m = js_ro((Js*)" {\"a\":123, \"a\":true } ");
  assert(map_size(m) == 1);
  rs =js_rb(opt_get(map_get(m, "a")));
  assert(rs);


  // m1 and m2 are Map[Js]
  int map_eq_str(Map *m1, Map *m2) {
    int size = map_size(m1);
    if (size != map_size(m2)) {
      return 0;
    }
    RANGE0(i, size)
      Kv *kv1 = arr_get((Arr *)m1, i);
      Kv *kv2 = arr_get((Arr *)m2, i);
      if (
        !str_eq(kv_key(kv1), kv_key(kv2)) ||
        !str_eq(kv_value(kv1), kv_value(kv2))
      ) {
        return 0;
      }
    _RANGE
    return 1;
  }

  // Map[Js]
  Map *m2;
  m = map_new();
  s = js_wo(m);
  m2 = js_ro(s);
  assert(map_eq_str(m, m2));

  map_put(m, "A", js_wb(0));
  s = js_wo(m);
  m2 = js_ro(s);
  assert(map_eq_str(m, m2));

  map_put(m, "A", js_wb(0));
  s = js_wo(m);
  m2 = js_ro(s);
  assert(map_eq_str(m, m2));

  map_put(m, "B", js_wi(-34516));
  s = js_wo(m);
  m2 = js_ro(s);
  assert(map_eq_str(m, m2));

  map_put(m, "C", js_wd(321.19));
  s = js_wo(m);
  m2 = js_ro(s);
  assert(map_eq_str(m, m2));

  map_put(m, "D", js_ws("caf"));
  s = js_wo(m);
  m2 = js_ro(s);
  assert(map_eq_str(m, m2));

  map_put(m, "F", js_wo(m));
  s = js_wo(m);
  m2 = js_ro(s);
  assert(map_eq_str(m, m2));

  puts("    Finished");
}


