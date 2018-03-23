/*
 * Copyright 26-Aug-2015 ºDeme
 *
 * This file is part of 'dm'
 * 'dm' is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 *
 * 'dm' is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 'dm'.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dmc/all.h"

void map_test() {
  printf("Map test\n");

  Map *m = map_new();

  assert(arr_size(m) == 0);

  map_put(m, "uno", "2");
  assert(!strcmp("2", map_get(m, "uno")));
  map_put(m, "dos", "2");
  assert(!strcmp("2", map_get(m, "dos")));
  map_put(m, "uno", "1");
  assert(!strcmp("1", map_get(m, "uno")));
  assert(arr_size(m) == 2);
  assert(!map_has_key(m, "tres"));
  map_put(m, "tres", "3");

  map_remove(m, "dos");
  assert(map_has_key(m, "uno"));
  assert(map_has_key(m, "tres"));
  assert(!map_has_key(m, "dos"));
  assert(arr_size(m) == 2);
  assert(!strcmp("3", map_get(m, "tres")));
  assert(!strcmp("1", map_get(m, "uno")));
  assert(!map_has_key(m, "dos"));

  map_remove(m, "cero");
  assert(arr_size(m) == 2);
  assert(!strcmp("3", map_get(m, "tres")));
  assert(!strcmp("1", map_get(m, "uno")));
  assert(!map_has_key(m, "dos"));

  Buf *bf = buf_new();
  FNX (tostr, Kv, kv) { buf_add(bf, kv->key); buf_cadd(bf,'-'); }_FN
  it_each(map_to_it(m), tostr);
  assert(!strcmp("uno-tres-", buf_str(bf)));
  bf = buf_new();
  it_each(map_to_it_sort(m), tostr);
  assert(!strcmp("tres-uno-", buf_str(bf)));

  printf("    Finshed\n");
}
