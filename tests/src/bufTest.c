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

void buf_test() {
  printf("Buf test\n");

  char *s0 = "";
  char *s1 = "1";
  char *s2 = "abc";
  Buf *bf = buf_new();

  buf_add(bf, s0);
  assert(!strcmp(buf_str(bf), ""));
  buf_cadd(bf, '+');
  assert(!strcmp(buf_str(bf), "+"));
  buf_add(bf, "0");
  assert(!strcmp(buf_str(bf), "+0"));
  buf_add(bf, s1);
  assert(!strcmp(buf_str(bf), "+01"));
  buf_add(bf, s2);
  assert(!strcmp(buf_str(bf), "+01abc"));
  buf_add(bf, s0);
  assert(!strcmp(buf_str(bf), "+01abc"));
  buf_add(bf, s2);
  assert(!strcmp(buf_str(bf), "+01abcabc"));
  buf_add(bf, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  assert(!strcmp(buf_str(bf), "+01abcabc"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));

  printf("    Finished\n");
}
