// Copyright 22-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Opt.h"
#include "dmc/DEFS.h"
#include "gc.h"
#include "dmc/sys.h"
#include "dmc/exc.h"
#include "dmc/str.h"

struct opt_Opt {
  void *value;
};

Opt *opt_new (void *value) {
  Opt *this = MALLOC(Opt);
  this->value = value;
  return this;
}

Opt *opt_empty (void) {
  Opt *this = MALLOC(Opt);
  this->value = NULL;
  return this;
}

int opt_is_empty (Opt *this) {
  return !this->value;
}

int opt_is_full (Opt *this) {
  return this->value != NULL;
}

void *opt_get (Opt *this) {
  if (!this->value)
    EXC_ILLEGAL_STATE("Option is null")

  return this->value;
}

void *opt_eget (Opt *this, char *msg) {
  if (!this->value)
    EXC_ILLEGAL_STATE(msg)

  return this->value;
}
