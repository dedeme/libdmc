/*
 * Copyright 25-Aug-2015 ºDeme
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

void bytes_test() {
  printf("Bytes test\n");

  char *s1 = "ab";
  char *s2 = "c";
  char *s3 = "";

  Bytes *b1;
  Bytes *b2;

  b1 = bytes_new();
  assert(bytes_length(b1) == 0);
  b2 = bytes_from_str(s2);
  assert(bytes_length(b2) == 1);

  bytes_add_str(b1, s1);
  assert(bytes_length(b1) == 2);
  assert(*bytes_bs(b1) == 'a');
  assert(bytes_bs(b1)[1] == 'b');

  bytes_add_str(b1, s3);
  assert(bytes_length(b1) == 2);
  assert(*bytes_bs(b1) == 'a');
  assert(bytes_bs(b1)[1] == 'b');

  bytes_add(b1, b2);
  assert(bytes_length(b1) == 3);
  assert(*bytes_bs(b1) == 'a');
  assert(bytes_bs(b1)[1] == 'b');
  assert(bytes_bs(b1)[2] == 'c');

  b1 = bytes_new();
  bytes_add(b1, b2);
  assert(bytes_length(b1) == 1);
  assert(*bytes_bs(b1) == 'c');

  printf( "    Finished\n");
}

