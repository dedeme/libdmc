// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/** Version 201904 */

#include "all_tests.h"

#include "iarr_tests.h"
#include "darr_tests.h"
#include "bytes_tests.h"
#include "arr_tests.h"
#include "it_tests.h"
#include "buf_tests.h"
#include "str_tests.h"
#include "map_tests.h"
#include "list_tests.h"
#include "hash_tests.h"
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
#include "async_tests.h"
#include "schd_tests.h"
#include "iserver_tests.h"

int main(int argc, char *argv[]) {
  sys_init("libdmc_tests");
/*
  iarr_tests();
  darr_tests();
  bytes_tests();
  arr_tests();
  it_tests();
  buf_tests();
  str_tests();
  map_tests();
  list_tests();
  hash_tests();
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
*/  async_tests();
//  schd_tests();
//  iserver_tests();

}
