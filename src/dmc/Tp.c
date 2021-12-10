// Copyright 21-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Tp.h"
#include "dmc/DEFS.h"

Tp *tp_new (void *e1, void *e2) {
  Tp *this = MALLOC(Tp);
  this->e1 = e1;
  this->e2 = e2;
  return this;
}
