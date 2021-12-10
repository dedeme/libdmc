// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Kv/OKv.h"
#include <string.h>
#include "dmc/DEFS.h"
#include "dmc/err.h"
#include "dmc/Opt.h"

struct oKv_OKv {
  Kv *value;
};

OKv *oKv_mk_none() {
  return (OKv *)opt_mk_none();
}

OKv *oKv_mk_some(Kv *value) {
  return (OKv *)opt_mk_some(value);
}

int oKv_none(OKv *opt) {
  return !opt->value;
}

Kv *oKv_some(OKv *opt) {
  if (opt->value) return opt->value;
  FAIL("Option is none");
  return NULL; // Unreachable
}

Kv *oKv_esome (OKv *opt, char *msg) {
  if (opt->value) return opt->value;
  FAIL(msg);
  return NULL; // Unreachable
}

Kv *oKv_osome (OKv *opt, Kv *value) {
  return opt->value ? opt->value : value;
}

Kv *oKv_nsome (OKv *opt) {
  return opt->value;
}

char *oKv_to_js (OKv *opt, char *(*to)(Kv *e)) {
  return opt->value ? to(opt->value) : "null";
}

OKv *oKv_from_js (char *js, Kv *(*from)(char *jse)) {
  return strcmp(js, "null") ? oKv_mk_some(from(js)) : oKv_mk_none();
}

//--// Not remove

