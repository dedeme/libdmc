// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/TpIntInt/OOTpIntInt.h"
#include <string.h>
#include "dmc/DEFS.h"
#include "dmc/err.h"
#include "dmc/Opt.h"

struct oOTpIntInt_OOTpIntInt {
  OTpIntInt *value;
};

OOTpIntInt *oOTpIntInt_mk_none() {
  return (OOTpIntInt *)opt_mk_none();
}

OOTpIntInt *oOTpIntInt_mk_some(OTpIntInt *value) {
  return (OOTpIntInt *)opt_mk_some(value);
}

int oOTpIntInt_none(OOTpIntInt *opt) {
  return !opt->value;
}

OTpIntInt *oOTpIntInt_some(OOTpIntInt *opt) {
  if (opt->value) return opt->value;
  FAIL("Option is none");
  return NULL; // Unreachable
}

OTpIntInt *oOTpIntInt_esome (OOTpIntInt *opt, char *msg) {
  if (opt->value) return opt->value;
  FAIL(msg);
  return NULL; // Unreachable
}

OTpIntInt *oOTpIntInt_osome (OOTpIntInt *opt, OTpIntInt *value) {
  return opt->value ? opt->value : value;
}

OTpIntInt *oOTpIntInt_nsome (OOTpIntInt *opt) {
  return opt->value;
}

char *oOTpIntInt_to_js (OOTpIntInt *opt, char *(*to)(OTpIntInt *e)) {
  return opt->value ? to(opt->value) : "null";
}

OOTpIntInt *oOTpIntInt_from_js (char *js, OTpIntInt *(*from)(char *jse)) {
  return strcmp(js, "null") ? oOTpIntInt_mk_some(from(js)) : oOTpIntInt_mk_none();
}

//--// Not remove

