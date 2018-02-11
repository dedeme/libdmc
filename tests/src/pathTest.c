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

void path_test() {
  printf("path test\n");

  char *r;

  r = path_name("");
  assert(!strcmp(r, ""));
  r = path_name("/");
  assert(!strcmp(r, ""));
  r = path_name("ab");
  assert(!strcmp(r, "ab"));
  r = path_name("/ab.c");
  assert(!strcmp(r, "ab.c"));
  r = path_name("cd/");
  assert(!strcmp(r, ""));
  r = path_name("c/ab.c");
  assert(!strcmp(r, "ab.c"));
  r = path_parent("");
  assert(!strcmp(r, ""));
  r = path_parent("/");
  assert(!strcmp(r, ""));
  r = path_parent("ab");
  assert(!strcmp(r, ""));
  r = path_parent("/ab.c");
  assert(!strcmp(r, ""));
  r = path_parent("cd/");
  assert(!strcmp(r, "cd"));
  r = path_parent("cg/r/ab.c");
  assert(!strcmp(r, "cg/r"));
  r = path_extension("");
  assert(!strcmp(r, ""));
  r = path_extension("/");
  assert(!strcmp(r, ""));
  r = path_extension("ab");
  assert(!strcmp(r, ""));
  r = path_extension("/ab.c");
  assert(!strcmp(r, ".c"));
  r = path_extension("cd/");
  assert(!strcmp(r, ""));
  r = path_extension("cd/ab.c");
  assert(!strcmp(r, ".c"));
  r = path_extension("cd/.");
  assert(!strcmp(r, "."));
  r = path_extension("cd/f.");
  assert(!strcmp(r, "."));
  r = path_only_name("");
  assert(!strcmp(r, ""));
  r = path_only_name("/");
  assert(!strcmp(r, ""));
  r = path_only_name("ab");
  assert(!strcmp(r, "ab"));
  r = path_only_name("/ab.c");
  assert(!strcmp(r, "ab"));
  r = path_only_name("cd/");
  assert(!strcmp(r, ""));
  r = path_only_name("/cd/a.b");
  assert(!strcmp(r, "a"));
  r = path_only_name("cd/.");
  assert(!strcmp(r, ""));
  r = path_only_name("x/cd/f.");
  assert(!strcmp(r, "f"));

  char *s0 = "";
  char *s1 = "1";

  r = path_cat(s0, NULL);
  assert(!strcmp(r, ""));
  r = path_cat(s0, s0, NULL);
  assert(!strcmp(r, ""));
  r = path_cat(s1, NULL);
  assert(!strcmp(r, "1"));
  r = path_cat(s0, "ab", NULL);
  assert(!strcmp(r, "ab"));
  r = path_cat(s1, "ab", NULL);
  assert(!strcmp(r, "1/ab"));
  r = path_cat(s0, "ab", "", "cd", NULL);
  assert(!strcmp(r, "ab/cd"));
  r = path_cat(s1, "ab", s1, "cd", NULL);
  assert(!strcmp(r, "1/ab/1/cd"));

  printf( "    Finished\n");
}
