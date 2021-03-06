// Copyright 23-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Tp.h"
#include "gc.h"
#include "dmc/DEFS.h"

struct tp_Tp {
  void *e1;
  void *e2;
};

Tp *tp_new(void *e1, void *e2) {
  Tp *this = MALLOC(Tp);
  this->e1 = e1;
  this->e2 = e2;
  return this;
}

void *tp_e1(Tp *this) {
  return this->e1;
}

void *tp_e2(Tp *this) {
  return this->e2;
}
