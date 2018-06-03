// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "gc.h"
#include "dmc/Tuples.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

struct tp_Tp {
  void *e1;
  void *e2;
};

Tp *tp_new(void *e1, void *e2) {
  XNULL(e1)
  XNULL(e2)
  Tp *this = MALLOC(Tp);
  this->e1 = e1;
  this->e2 = e2;
  return this;
}

void *tp_e1(Tp *this) {
  XNULL(this);
  return this->e1;
}

void *tp_e2(Tp *this) {
  XNULL(this);
  return this->e2;
}

struct tp3_Tp3 {
  void *e1;
  void *e2;
  void *e3;
};

Tp3 *tp3_new(void *e1, void *e2, void *e3) {
  XNULL(e1)
  XNULL(e2)
  XNULL(e3)
  Tp3 *this = MALLOC(Tp3);
  this->e1 = e1;
  this->e2 = e2;
  this->e3 = e3;
  return this;
}

void *tp3_e1(Tp3 *this) {
  XNULL(this);
  return this->e1;
}

void *tp3_e2(Tp3 *this) {
  XNULL(this);
  return this->e2;
}

void *tp3_e3(Tp3 *this) {
  XNULL(this);
  return this->e3;
}

struct kv_Kv {
  char *key;
  void *value;
};

Kv *kv_new(char *key, void *value) {
  XNULL(key)
  XNULL(value)
  Kv *this = MALLOC(Kv);
  this->key = key;
  this->value = value;
  return this;
}

char *kv_key(Kv *this) {
  XNULL(this);
  return this->key;
}

void *kv_value(Kv *this) {
  XNULL(this);
  return this->value;
}
