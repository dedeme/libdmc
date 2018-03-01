// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dm/dm.h"

void sys_test() {
  printf("sys\n");
  char *s = (char *)GC_malloc(22);
  assert(str_eq(s, s));
  printf("    Finshed\n");
}
