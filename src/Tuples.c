// Copyright 07-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dm.h"

Tp *tp_new(void *e1, void *e2) {
  Tp *this = MALLOC(Tp);
  this->e1 = e1;
  this->e2 = e2;
  return this;
}

Tp3 *tp3_new(void *e1, void *e2, void *e3) {
  Tp3 *this = MALLOC(Tp3);
  this->e1 = e1;
  this->e2 = e2;
  this->e3 = e3;
  return this;
}

Kv *kv_new(char *key, void *value) {
  Kv *this = MALLOC(Kv);
  this->key = key;
  this->value = value;
  return this;
}
