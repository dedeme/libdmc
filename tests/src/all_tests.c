// Copyright 21-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/** Version 201904 */

#include "all_tests.h"
#include <time.h>

#include "dmc/std.h"
#include "b64_tests.h"
#include "bytes_tests.h"
#include "tp_tests.h"
#include "rnd_tests.h"
#include "buf_tests.h"
#include "arr_tests.h"
#include "str_tests.h"
#include "path_tests.h"
#include "cryp_tests.h"
#include "file_tests.h"
#include "map_tests.h"
#include "js_tests.h"
#include "list_tests.h"
#include "hash_tests.h"
#include "aInt_tests.h"
#include "aDouble_tests.h"
#include "dec_tests.h"
#include "date_tests.h"
#include "async_tests.h"
#include "ext_tests.h"
#include "iserver_tests.h"
#include "regex_tests.h"

int main(int argc, char *argv[]) {
  sys_init("libdmc_tests");

  b64_tests();
  bytes_tests();
  tp_tests();
  rnd_tests();
  buf_tests();
  arr_tests();
  str_tests();
  path_tests();
  cryp_tests();
  file_tests();
  map_tests();
  js_tests();
  list_tests();
  hash_tests();
  aInt_tests();
  aDouble_tests();
  dec_tests();
  date_tests();
  async_tests();
  ext_tests();
  iserver_tests(0);
  regex_tests();
}
