// Copyright 25-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/char/OKchar.h"
#include <string.h>
#include "dmc/DEFS.h"
#include "dmc/err.h"
#include "dmc/Opt.h"

struct oKchar_OKchar {
  Kchar *value;
};

OKchar *oKchar_mk_none() {
  return (OKchar *)opt_mk_none();
}

OKchar *oKchar_mk_some(Kchar *value) {
  return (OKchar *)opt_mk_some(value);
}

int oKchar_none(OKchar *opt) {
  return !opt->value;
}

Kchar *oKchar_some(OKchar *opt) {
  if (opt->value) return opt->value;
  FAIL("Option is none");
  return NULL; // Unreachable
}

Kchar *oKchar_esome (OKchar *opt, char *msg) {
  if (opt->value) return opt->value;
  FAIL(msg);
  return NULL; // Unreachable
}

Kchar *oKchar_osome (OKchar *opt, Kchar *value) {
  return opt->value ? opt->value : value;
}

Kchar *oKchar_nsome (OKchar *opt) {
  return opt->value;
}

char *oKchar_to_js (OKchar *opt, char *(*to)(Kchar *e)) {
  return opt->value ? to(opt->value) : "null";
}

OKchar *oKchar_from_js (char *js, Kchar *(*from)(char *jse)) {
  return strcmp(js, "null") ? oKchar_mk_some(from(js)) : oKchar_mk_none();
}

//--// Not remove

