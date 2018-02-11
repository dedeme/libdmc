/*
 * Copyright 6-Apr-2015 ºDeme
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

#include "dm.h"

void sys_test() {
  printf("sys\n");
  char *s = (char *)GC_malloc(22);
  assert(str_eq(s, s));
  printf("    Finshed\n");
}
