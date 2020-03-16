// Copyright 17-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "path_tests.h"
#include <assert.h>

void path_tests(void) {
  puts("Path tests");

  int test_path(char *(*f)(char *), char *path, char *rs) {
    return str_eq(f(path), rs);
  }

  assert(test_path(path_name, "", ""));
  assert(test_path(path_name, "/", ""));
  assert(test_path(path_name, "ab", "ab"));
  assert(test_path(path_name, "/ab.c", "ab.c"));
  assert(test_path(path_name, "cd/", ""));
  assert(test_path(path_name, "c/ab.c", "ab.c"));

  assert(test_path(path_parent, "", ""));
  assert(test_path(path_parent, "/", ""));
  assert(test_path(path_parent, "ab", ""));
  assert(test_path(path_parent, "/ab.c", ""));
  assert(test_path(path_parent, "cd/", "cd"));
  assert(test_path(path_parent, "cg/r/ab.c", "cg/r"));

  assert(test_path(path_extension, "", ""));
  assert(test_path(path_extension, "/", ""));
  assert(test_path(path_extension, "ab", ""));
  assert(test_path(path_extension, "/ab.c", ".c"));
  assert(test_path(path_extension, "cd/", ""));
  assert(test_path(path_extension, "cd/ab.c", ".c"));
  assert(test_path(path_extension, "cd/.", "."));
  assert(test_path(path_extension, "cd/f.", "."));

  assert(test_path(path_only_name, "", ""));
  assert(test_path(path_only_name, "/", ""));
  assert(test_path(path_only_name, "ab", "ab"));
  assert(test_path(path_only_name, "/ab.c", "ab"));
  assert(test_path(path_only_name, "cd/", ""));
  assert(test_path(path_only_name, "/cd/a.b", "a"));
  assert(test_path(path_only_name, "cd/.", ""));
  assert(test_path(path_only_name, "cd/f.", "f"));


  char *s0 = "";
  char *s1 = "1";

  char *r = path_cat(s0, s1, "ab", "ab", "", "cd", NULL);
  assert(str_eq(r, "1/ab/ab/cd"));

  assert(str_eq(opt_oget(path_canonical("/hom/dem"), "EE"), "EE"));
  assert(str_eq(
    opt_oget(path_canonical("/bin/xx/.//..//../bin"), "EE"), "EE"
  ));
  assert(str_eq(
    opt_oget(path_canonical("/etc/apt/.//..//../etc"), "EE"), "/etc"
  ));

  puts("    Finished");
}

