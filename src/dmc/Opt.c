// Copyright 23-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Opt.h"
#include <string.h>
#include "dmc/DEFS.h"
#include "dmc/err.h"

struct opt_Opt {
  void *value;
};

Opt *opt_mk_none() {
  Opt *this = MALLOC(Opt);
  this->value = NULL;
  return this;
}

Opt *opt_mk_some(void *value) {
  Opt *this = MALLOC(Opt);
  this->value = value;
  return this;
}

int opt_none(Opt *opt) {
  return !opt->value;
}

void *opt_some(Opt *opt) {
  if (opt->value) return opt->value;
  return FAIL("Option is none");
}

void *opt_esome (Opt *opt, char *msg) {
  if (opt->value) return opt->value;
  return FAIL(msg);
}

void *opt_osome (Opt *opt, void *value) {
  return opt->value ? opt->value : value;
}

void *opt_nsome (Opt *opt) {
  return opt->value;
}

char *opt_to_js (Opt *opt, char *(*to)(void *e)) {
  return opt->value ? to(opt->value) : "null";
}

Opt *opt_from_js (char *js, void *(*from)(char *jse)) {
  return strcmp(js, "null") ? opt_mk_some(from(js)) : opt_mk_none();
}

