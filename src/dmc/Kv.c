// Copyright 23-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Kv.h"
#include "dmc/err.h"
#include "dmc/js.h"

Kv *kv_new (char *k, void *v) {
  Kv *this = MALLOC(Kv);
  this->k = k;
  this->v = v;
  return this;
}

char *kv_to_js (Kv *this, char *(*to)(void *e)) {
  return js_wa(achar_new_from(
    js_ws(this->k),
    to(this->v),
    NULL
  ));
}

Kv *kv_from_js (char *js, void *(*from)(char *ejs)) {
  Achar *a = js_ra(js);
  return kv_new(
    js_rs(achar_get(a, 0)),
    from(achar_get(a, 1))
  );
}
