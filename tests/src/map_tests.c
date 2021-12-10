// Copyright 22-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "map_tests.h"
#include <assert.h>
#include "dmc/std.h"

static char *keys_to_s(Achar *keys) {
  Buf *bf = buf_new();
  char **p = keys->es;
  while (p < keys->end) {
    buf_add(bf, *p++);
    buf_cadd(bf, '-');
  }
  return bf->str;
}

void map_tests(void) {
  puts("Map tests");

  Mchar *m = mchar_new();

  assert(mchar_size(m) == 0);

  mchar_put(m, "uno", str_new("2"));
  assert(str_eq("2", ochar_some(mchar_get(m, "uno"))));
  mchar_put(m, "dos", "2");
  assert(str_eq("2", ochar_some(mchar_get(m, "dos"))));
  mchar_put(m, "uno", str_new("1"));
  assert(str_eq("1", ochar_some(mchar_get(m, "uno"))));
  assert(mchar_size(m) == 2);
  assert(ochar_none(mchar_get(m, "tres")));
  mchar_put(m, "tres", "3");

  mchar_remove(m, "dos");
  assert(ochar_some(mchar_get(m, "uno")));
  assert(ochar_some(mchar_get(m, "tres")));
  assert(ochar_none(mchar_get(m, "dos")));
  assert(mchar_size(m) == 2);
  assert(str_eq("3", ochar_some(mchar_get(m, "tres"))));
  assert(str_eq("1", ochar_some(mchar_get(m, "uno"))));
  assert(ochar_none(mchar_get(m, "dos")));

  mchar_remove(m, "cero");
  assert(mchar_size(m) == 2);
  assert(str_eq("3", ochar_some(mchar_get(m, "tres"))));
  assert(str_eq("1", ochar_some(mchar_get(m, "uno"))));
  assert(ochar_none(mchar_get(m, "dos")));

  Achar *keys = mchar_keys(m);
  char *r = keys_to_s(keys);
  assert(str_eq("uno-tres-", r));

  achar_sort(keys, str_greater);
  r = keys_to_s(keys);
  assert(str_eq("tres-uno-", r));

  aKchar_reverse(mchar_to_array(m));
  keys = mchar_keys(m);
  r = keys_to_s(keys);
  assert(str_eq("tres-uno-", r));

  aKchar_reverse(mchar_to_array(m));
  keys = mchar_keys(m);
  r = keys_to_s(keys);
  assert(str_eq("uno-tres-", r));

  mchar_sort(m);
  keys = mchar_keys(m);
  r = keys_to_s(keys);
  assert(str_eq("tres-uno-", r));

  arr_reverse((Arr *)m);
  mchar_sort_locale(m);
  keys = mchar_keys(m);
  r = keys_to_s(keys);
  assert(str_eq("tres-uno-", r));

  puts("    Finished");
}

