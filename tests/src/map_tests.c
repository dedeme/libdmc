// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "map_tests.h"
#include <assert.h>

static char *keys_to_s(Gc *gc, Arr *keys) {
  Gc *gcl = gc_new();
  Buf *bf = buf_new(gcl);
  EACH(keys, char, e)
    buf_add(bf, e);
    buf_cadd(bf, '-');
  _EACH
  char *r = buf_to_str(gc, bf);
  gc_free(gcl);
  return r;
}

void map_tests(void) {
  puts("Map tests");
  Gc *gc = gc_new();

  // Map[char]
  Map *m = map_new(gc);

  assert(map_size(m) == 0);

  map_put(m, "uno", str_new(gc, "2"));
  assert(str_eq("2", opt_get(map_get(m, "uno"))));
  map_put(m, "dos", str_new(gc, "2"));
  assert(str_eq("2", opt_get(map_get(m, "dos"))));
  map_put(m, "uno", str_new(gc, "1"));
  assert(str_eq("1", opt_get(map_get(m, "uno"))));
  assert(map_size(m) == 2);
  assert(opt_is_empty(map_get(m, "tres")));
  map_put(m, "tres", str_new(gc, "3"));

  map_remove(m, "dos");
  assert(opt_get(map_get(m, "uno")));
  assert(opt_get(map_get(m, "tres")));
  assert(opt_is_empty(map_get(m, "dos")));
  assert(map_size(m) == 2);
  assert(str_eq("3", opt_get(map_get(m, "tres"))));
  assert(str_eq("1", opt_get(map_get(m, "uno"))));
  assert(opt_is_empty(map_get(m, "dos")));

  map_remove(m, "cero");
  assert(map_size(m) == 2);
  assert(str_eq("3", opt_get(map_get(m, "tres"))));
  assert(str_eq("1", opt_get(map_get(m, "uno"))));
  assert(opt_is_empty(map_get(m, "dos")));

  Js *js = map_to_js(gc, m, (FTO)js_ws);
  // Map[char]
  Map *m2 = map_from_js(gc, js, (FFROM)js_rs);
  assert(map_size(m2) == 2);
  assert(str_eq("3", opt_get(map_get(m2, "tres"))));
  assert(str_eq("1", opt_get(map_get(m2, "uno"))));

  // Varr[char]
  Arr *keys = map_keys(gc, m);
  char *r = keys_to_s(gc, keys);
  assert(str_eq("uno-tres-", r));

  arr_sort(keys, (FCMP)str_greater);
  r = keys_to_s(gc, keys);
  assert(str_eq("tres-uno-", r));

  arr_reverse((Arr *)m);
  keys = map_keys(gc, m);
  r = keys_to_s(gc, keys);
  assert(str_eq("tres-uno-", r));

  arr_reverse((Arr *)m);
  keys = map_keys(gc, m);
  r = keys_to_s(gc, keys);
  assert(str_eq("uno-tres-", r));

  map_sort(m);
  keys = map_keys(gc, m);
  r = keys_to_s(gc, keys);
  assert(str_eq("tres-uno-", r));

  arr_reverse((Arr *)m);
  map_sort_locale(m);
  keys = map_keys(gc, m);
  r = keys_to_s(gc, keys);
  assert(str_eq("tres-uno-", r));

  gc_free(gc);
  puts("    Finished");
}


