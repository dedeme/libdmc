// Copyright 13-Dec-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "regex_tests.h"
#include <assert.h>
#include "dmc/regex.h"

void regex_tests(void) {
  puts("Regex tests");

  assert(str_eq(opt_get(regex_replace("a", "asasa", "e")), "esese"));
  assert(str_eq(opt_get(regex_replace(".", "as\nsa", "e")), "eeeee"));
  assert(str_eq(opt_get(regex_replace("e", "asasa", "e")), "asasa"));
  assert(str_eq(
    opt_get(regex_replace(
      "1-\\w*\n", "1-ab\n2-ab\n1-cde\n", "1-xx\n"
    )),
    "1-xx\n2-ab\n1-xx\n"
  ));

  assert(str_eq(opt_get(regex_replace_ic("a", "asAsa", "e")), "esese"));

  puts("    Finished");
}

