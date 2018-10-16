// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "vmap_tests.h"
#include <assert.h>
#include "dmc/std.h"

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

  Buf *bf = buf_new();
  // Varr[char]
  Varr *keys = vmap_keys_new(m);
  EACH(keys, char, e)
    buf_add(bf, e);
    buf_cadd(bf, '-');
  _EACH
  char *r = buf_to_str_new(bf);
  assert(str_eq("uno-tres-", r));
  free(r);

  varr_sort(keys, (FGREATER)str_greater);

  buf_reset(bf);
  EACH(keys, char, e)
    buf_add(bf, e);
    buf_cadd(bf, '-');
  _EACH
  r = buf_to_str_new(bf);
  assert(str_eq("tres-uno-", r));
  free(r);

  buf_free(bf);
  varr_free(keys);

  vmap_free(m);

  puts("    Finished");
}

