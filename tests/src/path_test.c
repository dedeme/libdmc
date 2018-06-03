// Copyright 02-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <assert.h>
#include "dmc/path.h"
#include "dmc/str.h"

void path_test() {
  printf("path test\n");

  char *r;

  r = path_name("");
  assert(str_eq(r, ""));
  r = path_name("/");
  assert(str_eq(r, ""));
  r = path_name("ab");
  assert(str_eq(r, "ab"));
  r = path_name("/ab.c");
  assert(str_eq(r, "ab.c"));
  r = path_name("cd/");
  assert(str_eq(r, ""));
  r = path_name("c/ab.c");
  assert(str_eq(r, "ab.c"));
  r = path_parent("");
  assert(str_eq(r, ""));
  r = path_parent("/");
  assert(str_eq(r, ""));
  r = path_parent("ab");
  assert(str_eq(r, ""));
  r = path_parent("/ab.c");
  assert(str_eq(r, ""));
  r = path_parent("cd/");
  assert(str_eq(r, "cd"));
  r = path_parent("cg/r/ab.c");
  assert(str_eq(r, "cg/r"));
  r = path_extension("");
  assert(str_eq(r, ""));
  r = path_extension("/");
  assert(str_eq(r, ""));
  r = path_extension("ab");
  assert(str_eq(r, ""));
  r = path_extension("/ab.c");
  assert(str_eq(r, ".c"));
  r = path_extension("cd/");
  assert(str_eq(r, ""));
  r = path_extension("cd/ab.c");
  assert(str_eq(r, ".c"));
  r = path_extension("cd/.");
  assert(str_eq(r, "."));
  r = path_extension("cd/f.");
  assert(str_eq(r, "."));
  r = path_only_name("");
  assert(str_eq(r, ""));
  r = path_only_name("/");
  assert(str_eq(r, ""));
  r = path_only_name("ab");
  assert(str_eq(r, "ab"));
  r = path_only_name("/ab.c");
  assert(str_eq(r, "ab"));
  r = path_only_name("cd/");
  assert(str_eq(r, ""));
  r = path_only_name("/cd/a.b");
  assert(str_eq(r, "a"));
  r = path_only_name("cd/.");
  assert(str_eq(r, ""));
  r = path_only_name("x/cd/f.");
  assert(str_eq(r, "f"));

  char *s0 = "";
  char *s1 = "1";

  r = path_cat(s0, NULL);
  assert(str_eq(r, ""));
  r = path_cat(s0, s0, NULL);
  assert(str_eq(r, ""));
  r = path_cat(s1, NULL);
  assert(str_eq(r, "1"));
  r = path_cat(s0, "ab", NULL);
  assert(str_eq(r, "ab"));
  r = path_cat(s1, "ab", NULL);
  assert(str_eq(r, "1/ab"));
  r = path_cat(s0, "ab", "", "cd", NULL);
  assert(str_eq(r, "ab/cd"));
  r = path_cat(s1, "ab", s1, "cd", NULL);
  assert(str_eq(r, "1/ab/1/cd"));

  printf( "    Finished\n");
}
