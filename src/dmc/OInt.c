// Copyright 21-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/OInt.h"
#include "dmc/DEFS.h"
#include "dmc/err.h"

struct oInt_OInt {
  int isNone;
  int value;
};

OInt *oInt_mk_none() {
  OInt *this = MALLOC(OInt);
  this->isNone = 1;
  this->value = 0;
  return this;
}

OInt *oInt_mk_some(int value) {
  OInt *this = MALLOC(OInt);
  this->isNone = 0;
  this->value = value;
  return this;
}

int oInt_none(OInt *opt) {
  return opt->isNone;
}

int oInt_some(OInt *opt) {
  if (opt->isNone) FAIL("OIntion is none");
  return opt->value;
}
