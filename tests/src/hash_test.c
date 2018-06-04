// Copyright 04-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <assert.h>
#include "dmc/ct/Hchar.h"
#include "dmc/ct/Ochar.h"
#include "dmc/str.h"
#include "dmc/Buf.h"
#include "dmc/ct/Ikv.h"
#include "dmc/Tuples.h"
#include "dmc/DEFS.h"

void hash_test() {
  printf("Hash test\n");

  Hchar *m = hchar_new();

  assert(hchar_count(m) == 0);

  m = hchar_put(m, "uno", "2");
  assert(str_eq("2", ochar_value(hchar_get(m, "uno"))));
  m = hchar_put(m, "dos", "2");
  assert(str_eq("2", ochar_value(hchar_get(m, "dos"))));
  m = hchar_put(m, "uno", "1");
  assert(str_eq("1", ochar_value(hchar_get(m, "uno"))));
  assert(hchar_count(m) == 3);
  m = hchar_compact(m);
  assert(str_eq("2", ochar_value(hchar_get(m, "dos"))));
  assert(str_eq("1", ochar_value(hchar_get(m, "uno"))));
  assert(hchar_count(m) == 2);
  assert(!hchar_has_key(m, "tres"));
  m = hchar_put(m, "tres", "3");

  m = hchar_remove(m, "dos");
  assert(hchar_has_key(m, "uno"));
  assert(hchar_has_key(m, "tres"));
  assert(!hchar_has_key(m, "dos"));
  assert(hchar_count(m) == 2);
  assert(str_eq("3", ochar_value(hchar_get(m, "tres"))));
  assert(str_eq("1", ochar_value(hchar_get(m, "uno"))));
  assert(!hchar_has_key(m, "dos"));

  m = hchar_remove(m, "cero");
  assert(hchar_count(m) == 2);
  assert(str_eq("3", ochar_value(hchar_get(m, "tres"))));
  assert(str_eq("1", ochar_value(hchar_get(m, "uno"))));
  assert(!hchar_has_key(m, "dos"));

  Buf *bf = buf_new();
  void tostr(Kv *kv) { buf_add(bf, kv_key(kv)); buf_cadd(bf,'-'); }
  ikv_each(hchar_to_it(m), tostr);
  assert(str_eq("tres-uno-", buf_str(bf)));
  m = hchar_compact(hchar_put(m, "uno", "1"));
  bf = buf_new();
  ikv_each(hchar_to_it(m), tostr);
  assert(str_eq("uno-tres-", buf_str(bf)));
  bf = buf_new();
  ikv_each(hchar_to_it_sort(m), tostr);
  assert(str_eq("tres-uno-", buf_str(bf)));

  printf("    Finshed\n");
}
