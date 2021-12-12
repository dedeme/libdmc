// Copyright 21-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/ODouble.h"
#include "dmc/err.h"

struct oDouble_ODouble {
  int isNone;
  double value;
};

ODouble *oDouble_mk_none() {
  ODouble *this = MALLOC(ODouble);
  this->isNone = 1;
  this->value = 0;
  return this;
}

ODouble *oDouble_mk_some(double value) {
  ODouble *this = MALLOC(ODouble);
  this->isNone = 0;
  this->value = value;
  return this;
}

int oDouble_none(ODouble *opt) {
  return opt->isNone;
}

double oDouble_some(ODouble *opt) {
  if (opt->isNone) FAIL("ODoubleion is none");
  return opt->value;
}
