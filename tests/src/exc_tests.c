// Copyright 19-May-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "exc_tests.h"

#include <assert.h>
#include "dmc/Exc.h"
#include "dmc/async.h"

static void fn (void *null) {
  int v = -1;

  TRY
    v = 0;
    THROW(exc_generic_t) "Value is %d", v _THROW
    v = 1;
  CATCH(e)
    assert (v == 0);
    assert(str_eq(exc_type(e), exc_generic_t));
    assert(str_ends(exc_msg(e), "Value is 0"));
    v = 2;
  _TRY
  assert(v == 2);

//  THROW(exc_generic_t) "FAIL fn" _THROW
}

void exc_tests(void) {
  puts("Exc tests");

  int v = -1;

  TRY
    v = 0;
    THROW(exc_generic_t) "Value is %d", v _THROW
    v = 1;
  CATCH(e)
    assert (v == 0);
    assert(str_eq(exc_type(e), exc_generic_t));
    assert(str_ends(exc_msg(e), "Value is 0"));
    v = 2;
  _TRY
  assert(v == 2);

//  THROW(exc_generic_t) "FAIL" _THROW

  async_join(async_thread(fn, NULL));

  puts("    Finished");
}
