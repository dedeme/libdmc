// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/char/Ochar.h"
#include <string.h>
#include "dmc/DEFS.h"
#include "dmc/err.h"
#include "dmc/Opt.h"

struct ochar_Ochar {
  char *value;
};

Ochar *ochar_mk_none() {
  return (Ochar *)opt_mk_none();
}

Ochar *ochar_mk_some(char *value) {
  return (Ochar *)opt_mk_some(value);
}

int ochar_none(Ochar *opt) {
  return !opt->value;
}

char *ochar_some(Ochar *opt) {
  if (opt->value) return opt->value;
  FAIL("Option is none");
  return NULL; // Unreachable
}

char *ochar_esome (Ochar *opt, char *msg) {
  if (opt->value) return opt->value;
  FAIL(msg);
  return NULL; // Unreachable
}

char *ochar_osome (Ochar *opt, char *value) {
  return opt->value ? opt->value : value;
}

char *ochar_nsome (Ochar *opt) {
  return opt->value;
}

char *ochar_to_js (Ochar *opt, char *(*to)(char *e)) {
  return opt->value ? to(opt->value) : "null";
}

Ochar *ochar_from_js (char *js, char *(*from)(char *jse)) {
  return strcmp(js, "null") ? ochar_mk_some(from(js)) : ochar_mk_none();
}

//--// Not remove

