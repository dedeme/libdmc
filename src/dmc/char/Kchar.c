// Copyright 25-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/char/Kchar.h"
#include "dmc/DEFS.h"
#include "dmc/Kv.h"
#include "dmc/js.h"

Kchar *kchar_new (char *k, char *v) {
  return (Kchar *)kv_new(k, v);
}

char *kchar_to_js (Kchar *this) {
  return kv_to_js((Kv *)this, (char *(*)(void *))js_ws);
}

Kchar *kchar_from_js (char *js) {
  return (Kchar *)kv_from_js(js, (void *(*)(char *))js_rs);
}

//--// Not remove

