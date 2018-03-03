// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/all.h"

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
