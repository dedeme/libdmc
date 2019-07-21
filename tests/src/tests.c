// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests.h"
#include "str_tests.h"
#include "arr_tests.h"
#include "exc_tests.h"
#include "js_tests.h"
#include "it_tests.h"
#include "map_tests.h"
#include "b64_tests.h"
#include "buf_tests.h"
#include "bytes_tests.h"
#include "cryp_tests.h"
#include "file_tests.h"

int main () {
  Gc *gc = sys_init("libdmc_tests");

  str_tests();
  arr_tests();
  exc_tests();
  js_tests();
  it_tests();
  map_tests();
  b64_tests();
  buf_tests();
  bytes_tests();
  cryp_tests();
  file_tests();

  gc_free(gc);
  return 0;
}
