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
#include "dec_tests.h"
#include "darr_tests.h"
#include "iarr_tests.h"
#include "date_tests.h"
#include "ext_tests.h"
#include "async_tests.h"
#include "cgi_tests.h"

int main () {
  GC_NEW
  sys_init(gc, "libdmc_tests");

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
  dec_tests();
  dec_tests();
  darr_tests();
  iarr_tests();
  date_tests();
  ext_tests();
  async_tests();
  cgi_tests();

  GC_FREE
  return 0;
}
