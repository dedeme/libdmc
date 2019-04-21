// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "all_tests.h"
#include "iarr_tests.h"
#include "darr_tests.h"
#include "bytes_tests.h"
#include "varr_tests.h"
#include "arr_tests.h"
#include "buf_tests.h"
#include "str_tests.h"
#include "vmap_tests.h"
#include "map_tests.h"
#include "rnd_tests.h"
#include "b64_tests.h"
#include "cryp_tests.h"
#include "path_tests.h"
#include "file_tests.h"
#include "js_tests.h"
#include "dec_tests.h"
#include "date_tests.h"
#include "ext_tests.h"
#include "cgi_tests.h"

int main(int argc, char *argv[]) {
  sys_init("libdmc_tests");

  iarr_tests();
  darr_tests();
  bytes_tests();
  varr_tests();
  arr_tests();
  buf_tests();
  str_tests();
  vmap_tests();
  map_tests();
  rnd_tests();
  b64_tests();
  cryp_tests();
  path_tests();
  file_tests();
  js_tests();
  dec_tests();
  date_tests();
  ext_tests();
  cgi_tests();

  sys_end();
}
