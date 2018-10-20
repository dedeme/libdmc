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
  s = js_wn_new();
  assert(str_eq((char *)s, "null"));
  free(s);
  assert(js_rb((Js*)" true "));
  assert(js_rb((Js*)" false") == 0);

//  js_rb((Js*)"true and"); // error
//  js_rb((Js*)"xx"); // error

  s = js_wb_new(1);
  assert(js_rb(s));
  free(s);
  s = js_wb_new(0);
  assert(!js_rb(s));
  free(s);

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

  s = js_wi_new(0);
  assert(js_ri(s) == 0);
  free(s);
  s = js_wi_new(254);
  assert(js_ri(s) == 254);
  free(s);
  s = js_wi_new(-1100);
  assert(js_ri(s) == -1100);
  free(s);
  s = js_wd_new(0.0, 5);
  assert(js_rd(s) == 0);
  free(s);
  s = js_wd_new(-0.0, 0);
  assert(js_rd(s) == 0);
  free(s);
  s = js_wd_new(-.0, 2);
  assert(js_rd(s) == 0);
  free(s);
  s = js_wd_new(1.045, 3);
  assert(js_rd(s) == 1.045);
  free(s);
  s = js_wd_new(-21.045, 3);
  assert(js_rd(s) == -21.045);
  free(s);
  s = js_wd_new(-21.045, 2);
  assert(js_rd(s) == -21.05);
  free(s);

  str = js_rs_new((Js*)"  \"\" ");
  assert(str_eq("", str));
  free(str);
  str = js_rs_new((Js*)"  \"a\\u0030\" ");
  assert(str_eq("a0", str));
  free(str);
  str = js_rs_new((Js*)"  \"a\\t\\n\\\"\" ");
  assert(str_eq("a\t\n\"", str));
  free(str);

//  js_rs_new((Js*)" \""); // Error
//  js_rs_new((Js*)" \"a\" l"); // Error
//  js_rs_new((Js*)" \" \\ \" "); // Error
//  js_rs_new((Js*)" \" \\@ \" "); // Error
//  js_rs_new((Js*)" \" \\u30 \" "); // Error

  // Arr[Js]
  Arr *a, *a2;
  a = js_ra_new((Js*)"[]");
  assert(arr_size(a) == 0);
  arr_free(a);
  a = js_ra_new((Js*)"[123]");
  assert(arr_size(a) == 1);
  double rsd = js_rd(arr_get(a, 0));
  assert(rsd == 123);
  arr_free(a);
  a = js_ra_new((Js*)"[-123.56, true]");
  assert(arr_size(a) == 2);
  rsd = js_rd(arr_get(a, 0));
  assert(rsd == -123.56);
  int rs = js_rb(arr_get(a, 1));
  assert(rs == 1);
  arr_free(a);
  a = js_ra_new((Js*)"[-123.56, true, \"a\"]");
  assert(arr_size(a) == 3);
  str = js_rs_new(arr_get(a, 2));
  assert(str_eq("a", str));
  free(str);
  arr_free(a);

  a = js_ra_new((Js*)"[-123.56, true, [], 56]");
  assert(arr_size(a) == 4);
  rsd = js_rd(arr_get(a, 3));
  assert(rsd == 56);
  a2 = js_ra_new(arr_get(a, 2));
  assert(arr_size(a2) == 0);
  arr_free(a2);
  arr_free(a);

  a = js_ra_new((Js*)" [-123.56, true, [\"azf\", false], 56]  ");
  assert(arr_size(a) == 4);
  rsd = js_rd(arr_get(a, 3));
  assert(rsd == 56);
  a2 = js_ra_new(arr_get(a, 2));
  assert(arr_size(a2) == 2);
  rs = js_rb(arr_get(a2, 1));
  assert(rs == 0);
  str = js_rs_new(arr_get(a2, 0));
  assert(str_eq("azf", str));
  free(str);
  arr_free(a);
  arr_free(a2);

//  js_ra_new((Js*)"[-123.56, true, [], 56] h"); // Error
//  js_ra_new((Js*)"[s123.56, true, [], 56] "); // Error
//  js_ra_new((Js*)" "); // Error
//  js_ra_new((Js*)"[-123.56, true, [], true   "); // Error

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
  a = arr_new(free);
  s = js_wa_new(a);
  a3 = js_ra_new(s);
  assert(arr_eq_str(a, a3));
  free(s);
  arr_free(a3);

  arr_push(a, js_wb_new(1));
  s = js_wa_new(a);
  a3 = js_ra_new(s);
  assert(arr_eq_str(a, a3));
  free(s);
  arr_free(a3);

  arr_push(a, js_wi_new(-16));
  s = js_wa_new(a);
  a3 = js_ra_new(s);
  assert(arr_eq_str(a, a3));
  free(s);
  arr_free(a3);

  arr_push(a, js_wd_new(1, 2));
  s = js_wa_new(a);
  a3 = js_ra_new(s);
  assert(arr_eq_str(a, a3));
  free(s);
  arr_free(a3);

  arr_push(a, js_ws_new("caf"));
  s = js_wa_new(a);
  a3 = js_ra_new(s);
  assert(arr_eq_str(a, a3));
  free(s);
  arr_free(a3);

  a2 = arr_new(free);
  arr_push(a, js_wa_new(a2));
  s = js_wa_new(a);
  a3 = js_ra_new(s);
  assert(arr_eq_str(a, a3));
  free(s);
  arr_free(a3);

  arr_push(a2, js_ws_new("a\n\tzzð"));
  arr_push(a, js_wa_new(a2));
  s = js_wa_new(a);
  a3 = js_ra_new(s);
  assert(arr_eq_str(a, a3));
  free(s);
  arr_free(a3);

  arr_free(a);
  arr_free(a2);

  Map *m;
  m = js_ro_new((Js*)"{}");
  assert(map_size(m) == 0);
  map_free(m);

  m = js_ro_new((Js*)" {\"a\":123 } ");
  assert(map_size(m) == 1);
  rsd = js_rd(map_get_null(m, "a"));
  assert(rsd == 123);
  map_free(m);

  m = js_ro_new((Js*)" {\"a\":123, \"b\":true } ");
  assert(map_size(m) == 2);
  rs = js_rb(map_get_null(m, "b"));
  assert(rs);
  map_free(m);

  m = js_ro_new((Js*)" {\"a\":123, \"a\":true } ");
  assert(map_size(m) == 1);
  rs =js_rb(map_get_null(m, "a"));
  assert(rs);
  map_free(m);


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
        !str_eq(map_key(kv1), map_key(kv2)) ||
        !str_eq(map_value(kv1), map_value(kv2))
      ) {
        return 0;
      }
    _RANGE
    return 1;
  }

  // Map[Js]
  Map *m2;
  m = map_new(free);
  s = js_wo_new(m);
  m2 = js_ro_new(s);
  assert(map_eq_str(m, m2));
  free(s); map_free(m2);

  map_put(m, "A", js_wb_new(0));
  s = js_wo_new(m);
  m2 = js_ro_new(s);
  assert(map_eq_str(m, m2));
  free(s); map_free(m2);

  map_put(m, "A", js_wb_new(0));
  s = js_wo_new(m);
  m2 = js_ro_new(s);
  assert(map_eq_str(m, m2));
  free(s); map_free(m2);

  map_put(m, "B", js_wi_new(-34516));
  s = js_wo_new(m);
  m2 = js_ro_new(s);
  assert(map_eq_str(m, m2));
  free(s); map_free(m2);

  map_put(m, "C", js_wd_new(321.189, 2));
  s = js_wo_new(m);
  m2 = js_ro_new(s);
  assert(map_eq_str(m, m2));
  free(s); map_free(m2);

  map_put(m, "D", js_ws_new("caf"));
  s = js_wo_new(m);
  m2 = js_ro_new(s);
  assert(map_eq_str(m, m2));
  free(s); map_free(m2);

  map_put(m, "F", js_wo_new(m));
  s = js_wo_new(m);
  m2 = js_ro_new(s);
  assert(map_eq_str(m, m2));
  free(s); map_free(m2);

  map_free(m);

  puts("    Finished");
}


