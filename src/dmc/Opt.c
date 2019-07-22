// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Opt.h"
#include "dmc/DEFS.h"
#include "dmc/Exc.h"
#include "dmc/str.h"

Opt *opt_new (void *value) {
  return (Opt *)value;
}

Opt *opt_empty (void) {
  return (Opt *)NULL;
}

int opt_is_empty (Opt *this) {
  return !this;
}

int opt_is_full (Opt *this) {
  return !!this;
}

void *opt_get (Opt *this) {
  if (!this)
    EXC_ILLEGAL_STATE("Option is null")

  return this;
}

void *opt_oget (Opt *this, void *value) {
  return this ? this : value;
}

void *opt_nget (Opt *this) {
  return this ? this : NULL;
}

