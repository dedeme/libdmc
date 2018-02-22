/*
 * Copyright 24-Aug-2015 ºDeme
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
 * along with 'dm'. If not, see <http://www.gnu.org/licenses/>.
 */

#include "dm.h"

void rnd_test() {
  printf("rnd test\n");

  assert(rnd_d() < 1);
  assert(rnd_d() >= 0);
  assert(rnd_i(3) < 3);
  assert(rnd_i(3) >= 0);

  Arr *a = arr_new();
  It *box = rnd_box(a);
  assert(!it_has_next(box));

  arr_add(a, "0");
  arr_add(a, "1");
  arr_add(a, "2");

  FNX(each, char, s) {
//    printf("%s\n", s);
    assert(!strcmp(s, "0") ||!strcmp(s, "1") ||!strcmp(s, "2"));
  }_FN

  it_each(it_take(rnd_box(a), 7), each);

  printf("    Finished\n");
}
