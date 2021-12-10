// Copyright 01-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "js_tests.h"
#include <assert.h>
#include "dmc/std.h"

void js_tests(void) {
  puts("Js tests");

  char *s;
  char *str;

  assert(js_is_null("null"));
  assert(js_is_null("  null   "));
  assert(!js_is_null(""));
  assert(!js_is_null("nuln"));
  s = js_wn();
  assert(str_eq((char *)s, "null"));
  assert(js_rb(" true "));
  assert(js_rb(" false") == 0);

//  js_rb("true and"); // error
//  js_rb("xx"); // error

  s = js_wb(1);
  assert(js_rb(s));
  s = js_wb(0);
  assert(!js_rb(s));

  assert(js_ri(" 0 ") == 0);
  assert(js_rd(" 0.23 ") == 0.23);
  assert(js_ri(" -0 ") == 0);
  assert(js_rd(" -0.0 ") == 0);
  assert(js_rd(" -12.11 ") == -12.11);
  assert(js_rd(" -12.11e22 ") == -12.11e22);

//  js_ri(" 12abc "); // error
//  js_ri(" 12] "); // error
//  js_ri(" 12 }"); // error
//  js_rd(" .12"); // error
//  js_rd(" z.12"); // error

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

  str = js_rs("  \"\" ");
  assert(str_eq("", str));
  str = js_rs("  \"a\\u0030\" ");
  assert(str_eq("a0", str));
  str = js_rs("  \"a\\t\\n\\\"\" ");
  assert(str_eq("a\t\n\"", str));

//  js_rs(" \""); // Error
//  js_rs(" \"a\" l"); // Error
//  js_rs(" \" \\ \" "); // Error
//  js_rs(" \" \\@ \" "); // Error
//  js_rs(" \" \\u30 \" "); // Error

  Achar *a, *a2;
  a = js_ra("[]");
  assert(achar_size(a) == 0);
  a = js_ra("[123]");
  assert(achar_size(a) == 1);
  double rsd = js_rd(achar_get(a, 0));
  assert(rsd == 123);
  a = js_ra("[-123.56, true]");
  assert(achar_size(a) == 2);
  rsd = js_rd(achar_get(a, 0));
  assert(rsd == -123.56);
  int rs = js_rb(achar_get(a, 1));
  assert(rs == 1);
  a = js_ra("[-123.56, true, \"a\"]");
  assert(achar_size(a) == 3);
  str = js_rs(achar_get(a, 2));
  assert(str_eq("a", str));

  a = js_ra("[-123.56, true, [], 56]");
  assert(achar_size(a) == 4);
  rsd = js_rd(achar_get(a, 3));
  assert(rsd == 56);
  a2 = js_ra(achar_get(a, 2));
  assert(achar_size(a2) == 0);

  a = js_ra(" [-123.56, true, [\"azf\", false], 56]  ");
  assert(achar_size(a) == 4);
  rsd = js_rd(achar_get(a, 3));
  assert(rsd == 56);
  a2 = js_ra(achar_get(a, 2));
  assert(achar_size(a2) == 2);
  rs = js_rb(achar_get(a2, 1));
  assert(rs == 0);
  str = js_rs(achar_get(a2, 0));
  assert(str_eq("azf", str));

//  js_ra("[-123.56, true, [], 56] h"); // Error
//  js_ra("[s123.56, true, [], 56] "); // Error
//  js_ra(" "); // Error
//  js_ra("[-123.56, true, [], true   "); // Error

  int achar_eq_str(Achar *a1, Achar *a2) {
    int size = achar_size(a1);
    if (size != achar_size(a2)) {
      return 0;
    }
    for (int i = 0; i < size; ++i) {
      if (!str_eq(achar_get(a1, i), achar_get(a2, i))) {
        return 0;
      }
    }
    return 1;
  }

  Achar *a3;
  a = achar_new();
  s = js_wa(a);
  a3 = js_ra(s);
  assert(achar_eq_str(a, a3));

  achar_push(a, js_wb(1));
  s = js_wa(a);
  a3 = js_ra(s);
  assert(achar_eq_str(a, a3));

  achar_push(a, js_wi(-16));
  s = js_wa(a);
  a3 = js_ra(s);
  assert(achar_eq_str(a, a3));

  achar_push(a, js_wd(1));
  s = js_wa(a);
  a3 = js_ra(s);
  assert(achar_eq_str(a, a3));

  achar_push(a, js_ws("caf"));
  s = js_wa(a);
  a3 = js_ra(s);
  assert(achar_eq_str(a, a3));

  a2 = achar_new();
  achar_push(a, js_wa(a2));
  s = js_wa(a);
  a3 = js_ra(s);
  assert(achar_eq_str(a, a3));

  achar_push(a2, js_ws("a\n\tzzð"));
  achar_push(a, js_wa(a2));
  s = js_wa(a);
  a3 = js_ra(s);
  assert(achar_eq_str(a, a3));

  Mchar *m;
  m = js_ro("{}");
  assert(mchar_size(m) == 0);

  m = js_ro(" {\"a\":123 } ");
  assert(mchar_size(m) == 1);
  rsd = js_rd(ochar_some(mchar_get(m, "a")));
  assert(rsd == 123);

  m = js_ro(" {\"a\":123, \"b\":true } ");
  assert(mchar_size(m) == 2);
  rs = js_rb(ochar_some(mchar_get(m, "b")));
  assert(rs);

  m = js_ro(" {\"a\":123, \"a\":true } ");
  assert(mchar_size(m) == 1);
  rs =js_rb(ochar_some(mchar_get(m, "a")));
  assert(rs);

  int mchar_eq_str(Mchar *m1, Mchar *m2) {
    int size = mchar_size(m1);
    if (size != mchar_size(m2)) {
      return 0;
    }

    for (int i = 0; i < size; ++i) {
      Kchar *kv1 = aKchar_get(mchar_to_array(m1), i);
      Kchar *kv2 = aKchar_get(mchar_to_array(m2), i);
      if (
        !str_eq(kv1->k, kv2->k) ||
        !str_eq(kv1->v, kv2->v)
      ) {
        return 0;
      }
    }
    return 1;
  }

  Mchar *m2;
  m = mchar_new();
  s = js_wo(m);
  m2 = js_ro(s);
  assert(mchar_eq_str(m, m2));

  mchar_put(m, "A", js_wb(0));
  s = js_wo(m);
  m2 = js_ro(s);
  assert(mchar_eq_str(m, m2));

  mchar_put(m, "A", js_wb(0));
  s = js_wo(m);
  m2 = js_ro(s);
  assert(mchar_eq_str(m, m2));

  mchar_put(m, "B", js_wi(-34516));
  s = js_wo(m);
  m2 = js_ro(s);
  assert(mchar_eq_str(m, m2));

  mchar_put(m, "C", js_wd(321.19));
  s = js_wo(m);
  m2 = js_ro(s);
  assert(mchar_eq_str(m, m2));

  mchar_put(m, "D", js_ws("caf"));
  s = js_wo(m);
  m2 = js_ro(s);
  assert(mchar_eq_str(m, m2));

  mchar_put(m, "F", js_wo(m));
  s = js_wo(m);
  m2 = js_ro(s);
  assert(mchar_eq_str(m, m2));

  puts("    Finished");
}


