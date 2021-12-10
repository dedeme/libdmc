// Copyright 21-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/TpIntInt.h"
#include "dmc/DEFS.h"

TpIntInt *tpIntInt_new (int e1, int e2) {
  TpIntInt *this = MALLOC(TpIntInt);
  this->e1 = e1;
  this->e2 = e2;
  return this;
}
