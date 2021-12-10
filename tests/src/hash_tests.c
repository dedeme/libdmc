// Copyright 02-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "hash_tests.h"
#include <assert.h>
#include "dmc/char/Hchar.h"

void hash_tests () {
  printf("Hash test\n");

  Hchar *m = hchar_new();

  assert(hchar_size(m) == 0);

  m = hchar_put(m, "uno", "2");
  assert(str_eq("2", ochar_some(hchar_get(m, "uno"))));
  m = hchar_put(m, "dos", "2");
  assert(str_eq("2", ochar_some(hchar_get(m, "dos"))));
  m = hchar_put(m, "uno", "1");
  assert(str_eq("1", ochar_some(hchar_get(m, "uno"))));
  assert(hchar_size(m) == 3);
  m = hchar_compact(m);
  assert(str_eq("2", ochar_some(hchar_get(m, "dos"))));
  assert(str_eq("1", ochar_some(hchar_get(m, "uno"))));
  assert(hchar_size(m) == 2);
  assert(!hchar_has_key(m, "tres"));
  m = hchar_put(m, "tres", "3");

  m = hchar_remove(m, "dos");
  assert(hchar_has_key(m, "uno"));
  assert(hchar_has_key(m, "tres"));
  assert(!hchar_has_key(m, "dos"));
  assert(hchar_size(m) == 2);
  assert(str_eq("3", ochar_some(hchar_get(m, "tres"))));
  assert(str_eq("1", ochar_some(hchar_get(m, "uno"))));
  assert(!hchar_has_key(m, "dos"));

  m = hchar_remove(m, "cero");
  assert(hchar_size(m) == 2);
  assert(str_eq("3", ochar_some(hchar_get(m, "tres"))));
  assert(str_eq("1", ochar_some(hchar_get(m, "uno"))));
  assert(!hchar_has_key(m, "dos"));

  assert(str_eq(hchar_to_js(m, js_ws), "{\"tres\":\"3\",\"uno\":\"1\"}"));
  assert(str_eq(
    hchar_to_js(hchar_from_js(hchar_to_js(m, js_ws), js_rs), js_ws),
    "{\"tres\":\"3\",\"uno\":\"1\"}"
  ));

  m = hchar_put(m, "uno", "-1-");
  assert(hchar_size(m) == 3);
  assert(str_eq("3", ochar_some(hchar_get(m, "tres"))));
  assert(str_eq("-1-", ochar_some(hchar_get(m, "uno"))));
  assert(str_eq(hchar_to_js(m, js_ws), "{\"uno\":\"-1-\",\"tres\":\"3\"}"));

  Hchar *m2 = hchar_sort(m);
  assert(str_eq(hchar_to_js(m2, js_ws), "{\"tres\":\"3\",\"uno\":\"-1-\"}"));
  m2 = hchar_sort_locale(m);
  assert(str_eq(hchar_to_js(m2, js_ws), "{\"tres\":\"3\",\"uno\":\"-1-\"}"));

  printf("    Finshed\n");
}

