// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/TpIntInt/OATpIntInt.h"
#include <string.h>
#include "dmc/DEFS.h"
#include "dmc/err.h"
#include "dmc/Opt.h"

struct oATpIntInt_OATpIntInt {
  ATpIntInt *value;
};

OATpIntInt *oATpIntInt_mk_none() {
  return (OATpIntInt *)opt_mk_none();
}

OATpIntInt *oATpIntInt_mk_some(ATpIntInt *value) {
  return (OATpIntInt *)opt_mk_some(value);
}

int oATpIntInt_none(OATpIntInt *opt) {
  return !opt->value;
}

ATpIntInt *oATpIntInt_some(OATpIntInt *opt) {
  if (opt->value) return opt->value;
  FAIL("Option is none");
  return NULL; // Unreachable
}

ATpIntInt *oATpIntInt_esome (OATpIntInt *opt, char *msg) {
  if (opt->value) return opt->value;
  FAIL(msg);
  return NULL; // Unreachable
}

ATpIntInt *oATpIntInt_osome (OATpIntInt *opt, ATpIntInt *value) {
  return opt->value ? opt->value : value;
}

ATpIntInt *oATpIntInt_nsome (OATpIntInt *opt) {
  return opt->value;
}

char *oATpIntInt_to_js (OATpIntInt *opt, char *(*to)(ATpIntInt *e)) {
  return opt->value ? to(opt->value) : "null";
}

OATpIntInt *oATpIntInt_from_js (char *js, ATpIntInt *(*from)(char *jse)) {
  return strcmp(js, "null") ? oATpIntInt_mk_some(from(js)) : oATpIntInt_mk_none();
}

//--// Not remove

