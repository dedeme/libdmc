// Copyright 13-Dec-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Itp.h"
#include "gc.h"
#include "dmc/DEFS.h"

struct itp_Itp {
  int e1;
  int e2;
};

Itp *itp_new(int i1, int i2) {
  Itp *this = MALLOC(Itp);
  this->e1 = i1;
  this->e2 = i2;
  return this;
}

int itp_e1(Itp *this) {
  return this->e1;
}

int itp_e2(Itp *this) {
  return this->e2;
}
