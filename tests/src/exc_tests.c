// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "exc_tests.h"
#include <assert.h>
#include "dmc/async.h"

static void fn () {
  //THROW(exc_generic_t) "FAIL fn" _THROW
}

void exc_tests(void) {
  puts("Exc tests");

  //THROW(exc_generic_t) "FAIL" _THROW

  Gc *gc = gc_new();
  async_join(async_run0(async_new(gc), fn));
  gc_free(gc);

  puts("    Finished");
}

