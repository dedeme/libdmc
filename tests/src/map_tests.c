// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "map_tests.h"
#include <assert.h>
#include "dmc/std.h"

static char *keys_to_s_new(Varr *keys) {
  Buf *bf = buf_new();
  EACH(keys, char, e)
    buf_add(bf, e);
    buf_cadd(bf, '-');
  _EACH
  char *r = buf_to_str_new(bf);
  buf_free(bf);
  return r;
}

void map_tests(void) {
  puts("Map tests");

  // Map[char]
  Map *m = map_new(free);

  assert(map_size(m) == 0);

  map_put(m, "uno", str_new("2"));
  assert(str_eq("2", map_get_null(m, "uno")));
  map_put(m, "dos", str_new("2"));
  assert(str_eq("2", map_get_null(m, "dos")));
  map_put(m, "uno", str_new("1"));
  assert(str_eq("1", map_get_null(m, "uno")));
  assert(map_size(m) == 2);
  assert(!map_get_null(m, "tres"));
  map_put(m, "tres", str_new("3"));

  map_remove(m, "dos");
  assert(map_get_null(m, "uno"));
  assert(map_get_null(m, "tres"));
  assert(!map_get_null(m, "dos"));
  assert(map_size(m) == 2);
  assert(str_eq("3", map_get_null(m, "tres")));
  assert(str_eq("1", map_get_null(m, "uno")));
  assert(!map_get_null(m, "dos"));

  map_remove(m, "cero");
  assert(map_size(m) == 2);
  assert(str_eq("3", map_get_null(m, "tres")));
  assert(str_eq("1", map_get_null(m, "uno")));
  assert(!map_get_null(m, "dos"));

  Js *js = map_to_js_new(m, (FTO)js_ws_new);
  // Map[char]
  Map *m2 = map_from_js_new(js, (FFROM)js_rs_new, free);
  assert(map_size(m) == 2);
  assert(str_eq("3", map_get_null(m, "tres")));
  assert(str_eq("1", map_get_null(m, "uno")));
  free(js);
  map_free(m2);

  // Varr[char]
  Varr *keys = map_keys_new(m);
  char *r = keys_to_s_new(keys);
  assert(str_eq("uno-tres-", r));
  free(r);

  varr_sort(keys, (FGREATER)str_greater);
  r = keys_to_s_new(keys);
  assert(str_eq("tres-uno-", r));
  free(r); varr_free(keys);

  arr_reverse((Arr *)m);
  keys = map_keys_new(m);
  r = keys_to_s_new(keys);
  assert(str_eq("tres-uno-", r));
  free(r); varr_free(keys);

  arr_reverse((Arr *)m);
  keys = map_keys_new(m);
  r = keys_to_s_new(keys);
  assert(str_eq("uno-tres-", r));
  free(r); varr_free(keys);

  map_sort(m);
  keys = map_keys_new(m);
  r = keys_to_s_new(keys);
  assert(str_eq("tres-uno-", r));
  free(r); varr_free(keys);

  arr_reverse((Arr *)m);
  map_sort_locale(m);
  keys = map_keys_new(m);
  r = keys_to_s_new(keys);
  assert(str_eq("tres-uno-", r));
  free(r); varr_free(keys);

  map_free(m);

  puts("    Finished");
}

