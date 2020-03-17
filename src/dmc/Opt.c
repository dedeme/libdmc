// Copyright 22-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Opt.h"
#include "dmc/DEFS.h"
#include "gc.h"
#include "dmc/sys.h"
#include "dmc/Exc.h"
#include "dmc/str.h"

struct opt_Opt {
  void *value;
};

Opt *opt_new (void *value) {
  Opt *this = MALLOC(Opt);
  this->value = value;
  return this;
}

Opt *opt_empty (void) {
  Opt *this = MALLOC(Opt);
  this->value = NULL;
  return this;
}

int opt_is_empty (Opt *this) {
  return !this->value;
}

int opt_is_full (Opt *this) {
  return this->value != NULL;
}

void *opt_get (Opt *this) {
  if (!this->value)
    EXC_ILLEGAL_STATE("Option is null")

  return this->value;
}

void *opt_eget (Opt *this, char *msg) {
  if (!this->value)
    EXC_ILLEGAL_STATE(msg)

  return this->value;
}

void *opt_oget (Opt *this, void *value) {
  if (!this->value) return value;
  return this->value;
}

void *opt_nget (Opt *this) {
  return this->value ? this->value : NULL;
}

Js *opt_to_js (Opt *this, Js *(*to)(void *e)) {
  // Arr[js]
  Arr *js = this->value
    ? arr_new_c(1, (void *[]) { to(this->value) })
    : arr_new()
  ;
  return js_wa(js);
}

Opt *opt_from_js (Js *js, void *(*from)(Js *jse)) {
  // Arr[js]
  Arr *a = js_ra(js);
  return arr_size(a) ? opt_new(from(*arr_start(a))) : opt_empty();
}
