// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>


#include "dmc/std.h"
#include "dmc/sys.h"
#include "opt_test.h"
#include "sys_test.h"
#include "str_test.h"
#include "buf_test.h"
#include "it_test.h"
#include "arr_test.h"
#include "list_test.h"
#include "map_test.h"
#include "hash_test.h"
#include "bytes_test.h"
#include "b64_test.h"
#include "cryp_test.h"
#include "file_test.h"
#include "path_test.h"
#include "rnd_test.h"
#include "json_test.h"
#include "ext_test.h"
#include "dec_test.h"
#include "date_test.h"
#include "cgi_test.h"

int main (int argc, char **argv) {

  puts("dmTest. v201805\n");
  sys_init("dmTest");

  opt_test();
  sys_test();
  str_test();
  buf_test();
  it_test();
  arr_test();
  list_test();
  map_test();
  hash_test();
  bytes_test();
  b64_test();
  cryp_test();
  file_test();
  path_test();
  rnd_test();
  json_test();
  ext_test();
  dec_test();
  date_test();
  cgi_test();

  return(0);
}
