// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <gc.h>
#include "dmc/Opt.h"
#include "dmc/DEFS.h"
#include "dmc/exc.h"
#include "dmc/str.h"
#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"
#include "dmc/ct/Ojson.h"

struct opt_Opt {
  void *value;
};

inline
Opt *opt_null() {
  Opt *this = MALLOC(Opt);
  this->value = NULL;
  return this;
}

Opt *opt_new(void *v) {
  XNULL(v)
  Opt *this = MALLOC(Opt);
  this->value = v;
  return this;
}

inline
bool opt_is_null(Opt *this) {
  return !this->value;
}

void *opt_value(Opt *this) {
  XNULL(this)
  return this->value;
}

Json *opt_to_json(Opt *this, Json *(*to)(void *)) {
  XNULL(this)

  Ajson *r = ajson_new();
  if (!opt_is_null(this)) {
    ajson_add(r, to(this->value));
  }
  return json_warray(r);
}

Opt *opt_from_json(Json *js, void *(*from)(Json *)) {
  XNULL(js)
  Ajson *ajs = json_rarray(js);
  size_t size = ajson_size(ajs);
  if (size > 1)
    THROW(exc_range_t) exc_range(1, 2, size) _THROW

  if (size) {
    return opt_new(from(ajson_get(ajs, 0)));
  }
  return opt_null();
}
