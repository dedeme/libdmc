// Copyright 23-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "hash_tests.h"
#include <stdio.h>
#include <assert.h>
#include "dmc/Hash.h"

void hash_tests () {
  printf("Hash test\n");

  Hash *m = hash_new();

  assert(hash_count(m) == 0);

  m = hash_put(m, "uno", "2");
  assert(str_eq("2", opt_get(hash_get(m, "uno"))));
  m = hash_put(m, "dos", "2");
  assert(str_eq("2", opt_get(hash_get(m, "dos"))));
  m = hash_put(m, "uno", "1");
  assert(str_eq("1", opt_get(hash_get(m, "uno"))));
  assert(hash_count(m) == 3);
  m = hash_compact(m);
  assert(str_eq("2", opt_get(hash_get(m, "dos"))));
  assert(str_eq("1", opt_get(hash_get(m, "uno"))));
  assert(hash_count(m) == 2);
  assert(!hash_has_key(m, "tres"));
  m = hash_put(m, "tres", "3");

  m = hash_remove(m, "dos");
  assert(hash_has_key(m, "uno"));
  assert(hash_has_key(m, "tres"));
  assert(!hash_has_key(m, "dos"));
  assert(hash_count(m) == 2);
  assert(str_eq("3", opt_get(hash_get(m, "tres"))));
  assert(str_eq("1", opt_get(hash_get(m, "uno"))));
  assert(!hash_has_key(m, "dos"));

  m = hash_remove(m, "cero");
  assert(hash_count(m) == 2);
  assert(str_eq("3", opt_get(hash_get(m, "tres"))));
  assert(str_eq("1", opt_get(hash_get(m, "uno"))));
  assert(!hash_has_key(m, "dos"));

  Buf *bf = buf_new();
  void tostr(Kv *kv) { buf_add(bf, kv_key(kv)); buf_cadd(bf,'-'); }
  it_each(hash_to_it(m), (FPROC)tostr);
  assert(str_eq("tres-uno-", buf_str(bf)));
  m = hash_compact(hash_put(m, "uno", "1"));
  bf = buf_new();
  it_each(hash_to_it(m), (FPROC)tostr);
  assert(str_eq("uno-tres-", buf_str(bf)));
  bf = buf_new();
  Map *tmp = map_from_it(hash_to_it(m));
  map_sort(tmp);
  it_each(map_to_it(tmp), (FPROC)tostr);
  assert(str_eq("tres-uno-", buf_str(bf)));

  printf("    Finshed\n");
}

