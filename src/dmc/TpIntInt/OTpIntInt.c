// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/TpIntInt/OTpIntInt.h"
#include <string.h>
#include "dmc/DEFS.h"
#include "dmc/err.h"
#include "dmc/Opt.h"

struct oTpIntInt_OTpIntInt {
  TpIntInt *value;
};

OTpIntInt *oTpIntInt_mk_none() {
  return (OTpIntInt *)opt_mk_none();
}

OTpIntInt *oTpIntInt_mk_some(TpIntInt *value) {
  return (OTpIntInt *)opt_mk_some(value);
}

int oTpIntInt_none(OTpIntInt *opt) {
  return !opt->value;
}

TpIntInt *oTpIntInt_some(OTpIntInt *opt) {
  if (opt->value) return opt->value;
  FAIL("Option is none");
  return NULL; // Unreachable
}

TpIntInt *oTpIntInt_esome (OTpIntInt *opt, char *msg) {
  if (opt->value) return opt->value;
  FAIL(msg);
  return NULL; // Unreachable
}

TpIntInt *oTpIntInt_osome (OTpIntInt *opt, TpIntInt *value) {
  return opt->value ? opt->value : value;
}

TpIntInt *oTpIntInt_nsome (OTpIntInt *opt) {
  return opt->value;
}

char *oTpIntInt_to_js (OTpIntInt *opt, char *(*to)(TpIntInt *e)) {
  return opt->value ? to(opt->value) : "null";
}

OTpIntInt *oTpIntInt_from_js (char *js, TpIntInt *(*from)(char *jse)) {
  return strcmp(js, "null") ? oTpIntInt_mk_some(from(js)) : oTpIntInt_mk_none();
}

//--// Not remove

