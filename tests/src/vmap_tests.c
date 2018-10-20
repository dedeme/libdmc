// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "vmap_tests.h"
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

void vmap_tests(void) {
  puts("Vmap tests");

  // Vmap[char]
  Vmap *m = vmap_new();

  assert(vmap_size(m) == 0);

  vmap_put(m, "uno", "2");
  assert(str_eq("2", vmap_get_null(m, "uno")));
  vmap_put(m, "dos", "2");
  assert(str_eq("2", vmap_get_null(m, "dos")));
  vmap_put(m, "uno", "1");
  assert(str_eq("1", vmap_get_null(m, "uno")));
  assert(vmap_size(m) == 2);
  assert(!vmap_get_null(m, "tres"));
  vmap_put(m, "tres", "3");

  vmap_remove(m, "dos");
  assert(vmap_get_null(m, "uno"));
  assert(vmap_get_null(m, "tres"));
  assert(!vmap_get_null(m, "dos"));
  assert(vmap_size(m) == 2);
  assert(str_eq("3", vmap_get_null(m, "tres")));
  assert(str_eq("1", vmap_get_null(m, "uno")));
  assert(!vmap_get_null(m, "dos"));

  vmap_remove(m, "cero");
  assert(vmap_size(m) == 2);
  assert(str_eq("3", vmap_get_null(m, "tres")));
  assert(str_eq("1", vmap_get_null(m, "uno")));
  assert(!vmap_get_null(m, "dos"));

  // Varr[char]
  Varr *keys = vmap_keys_new(m);
  char *r = keys_to_s_new(keys);
  assert(str_eq("uno-tres-", r));
  free(r);

  varr_sort(keys, (FGREATER)str_greater);
  r = keys_to_s_new(keys);
  assert(str_eq("tres-uno-", r));
  free(r); varr_free(keys);

  arr_reverse((Arr *)m);
  keys = vmap_keys_new(m);
  r = keys_to_s_new(keys);
  assert(str_eq("tres-uno-", r));
  free(r); varr_free(keys);

  arr_reverse((Arr *)m);
  keys = vmap_keys_new(m);
  r = keys_to_s_new(keys);
  assert(str_eq("uno-tres-", r));
  free(r); varr_free(keys);

  vmap_sort(m);
  keys = vmap_keys_new(m);
  r = keys_to_s_new(keys);
  assert(str_eq("tres-uno-", r));
  free(r); varr_free(keys);

  arr_reverse((Arr *)m);
  vmap_sort_locale(m);
  keys = vmap_keys_new(m);
  r = keys_to_s_new(keys);
  assert(str_eq("tres-uno-", r));
  free(r); varr_free(keys);

  vmap_free(m);

  puts("    Finished");
}

