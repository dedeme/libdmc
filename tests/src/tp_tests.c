// Copyright 21-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tp_tests.h"
#include <assert.h>
#include <string.h>

void tp_tests(void) {
  puts("tp tests");

  Tp *tp = tp_new("a", "b");
  assert(!strcmp(tp->e1, "a"));
  assert(!strcmp(tp->e2, "b"));

  Kv *kv = kv_new("a", "b");
  assert(!strcmp(kv->k, "a"));
  assert(!strcmp(kv->v, "b"));

  puts("    Finished");
}
