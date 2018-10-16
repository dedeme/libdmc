// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "all_tests.h"
#include "iarr_tests.h"
#include "darr_tests.h"
#include "varr_tests.h"
#include "arr_tests.h"
#include "buf_tests.h"
#include "str_tests.h"
#include "vmap_tests.h"
#include "map_tests.h"

int main(int argc, char *argv[]) {

  iarr_tests();
  darr_tests();
  varr_tests();
  arr_tests();
  buf_tests();
  str_tests();
  vmap_tests();

  map_tests();
}
