// Copyright 1-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <assert.h>
#include "dmc/ct/Mchar.h"
#include "dmc/ct/Ochar.h"
#include "dmc/str.h"
#include "dmc/Buf.h"
#include "dmc/Tuples.h"
#include "dmc/ct/Ikv.h"
#include "dmc/DEFS.h"

void map_test() {
  printf("Map test\n");

  Mchar *m = mchar_new();

  assert(mchar_size(m) == 0);

  mchar_put(m, "uno", "2");
  assert(str_eq("2", ochar_value(mchar_get(m, "uno"))));
  mchar_put(m, "dos", "2");
  assert(str_eq("2", ochar_value(mchar_get(m, "dos"))));
  mchar_put(m, "uno", "1");
  assert(str_eq("1", ochar_value(mchar_get(m, "uno"))));
  assert(mchar_size(m) == 2);
  assert(!mchar_has_key(m, "tres"));
  mchar_put(m, "tres", "3");

  mchar_remove(m, "dos");
  assert(mchar_has_key(m, "uno"));
  assert(mchar_has_key(m, "tres"));
  assert(!mchar_has_key(m, "dos"));
  assert(mchar_size(m) == 2);
  assert(str_eq("3", ochar_value(mchar_get(m, "tres"))));
  assert(str_eq("1", ochar_value(mchar_get(m, "uno"))));
  assert(!mchar_has_key(m, "dos"));

  mchar_remove(m, "cero");
  assert(mchar_size(m) == 2);
  assert(str_eq("3", ochar_value(mchar_get(m, "tres"))));
  assert(str_eq("1", ochar_value(mchar_get(m, "uno"))));
  assert(!mchar_has_key(m, "dos"));

  Buf *bf = buf_new();
  void tostr(Kv *kv) { buf_add(bf, kv_key(kv)); buf_cadd(bf,'-'); }
  ikv_each(mchar_to_it(m), tostr);
  assert(str_eq("uno-tres-", buf_str(bf)));
  bf = buf_new();
  ikv_each(mchar_to_it_sort(m), tostr);
  assert(str_eq("tres-uno-", buf_str(bf)));

  printf("    Finshed\n");
}
