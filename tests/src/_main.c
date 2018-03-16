// Copyright 04-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/all.h"

#include "bufTest.h"
#include "strTest.h"
#include "arrTest.h"
#include "listTest.h"
#include "itTest.h"
#include "sysTest.h"
#include "rndTest.h"
#include "mapTest.h"
#include "excTest.h"
#include "bytesTest.h"
#include "b64Test.h"
#include "crypTest.h"
#include "dateTest.h"
#include "decTest.h"
#include "jsonTest.h"
#include "fileTest.h"
#include "extTest.h"
#include "cgiTest.h"

int main (int argc, char **argv) {

  puts("dmTest. v201803\n");
  sys_init("dmTest");
  buf_test();
  str_test();
  arr_test();
  list_test();
  it_test();
  sys_test();
  rnd_test();
  map_test();
  exc_test();
  bytes_test();
  b64_test();
  cryp_test();
  date_test();
  dec_test();
  json_test();
  file_test();
  ext_test();
  cgi_test();

  return(0);
}
