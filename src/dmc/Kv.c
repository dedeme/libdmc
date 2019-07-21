// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Kv.h"

struct kv_Kv {
  char *key;
  void *value;
};

Kv *kv_new(Gc *gc, char *key, void *value) {
  Kv *this = gc_add(gc, malloc(sizeof(Kv)));
  this->key = key;
  this->value = value;
  return this;
}

char *kv_key(Kv *this) {
  return this->key;
}

void *kv_value(Kv *this) {
  return this->value;
}
