// Copyright 23-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Hash.h"
#include "dmc/List.h"
#include "dmc/std.h"

struct hash_Hash {
  Hash *next;
  void *value;
};

Hash *hash_new(void) {
  Hash *this = MALLOC(Hash);
  this->next = NULL;
  this->value = NULL;
  return this;
}

int hash_count(Hash *this) {
  return list_count((List *)this);
}

Hash *hash_put(Hash *this, char *key, void *value) {
  return (Hash *)list_cons((List *)this, kv_new(key, value));
}

int hash_has_key(Hash *this, char *key) {
  EACHL(this, Kv, kv) {
    if (str_eq(kv_key(kv), key)) return 1;
  }_EACH
  return 0;
}
Opt *hash_get(Hash *this, char *key) {
  while (this->next && !str_eq(key, kv_key(this->value))) {
    this = this->next;
  }
  if (this->next) {
    return opt_new(kv_value(this->value));
  }
  return opt_empty();
}

Hash *hash_compact(Hash *this) {
  List *l = list_new();
  EACHL(this, Kv, kv) {
    // ---------------------------------------------------------------------- //
    int eq(Kv *ekv) { return str_eq(kv_key(kv), kv_key(ekv)); }               //
    // ---------------------------------------------------------------------- //
    if (!it_contains(list_to_it(l), (FPRED)eq)) {
      l = list_cons(l, kv);
    }
  }_EACH
  return (Hash *)list_reverse(l);
}

Hash *hash_remove(Hash *this, char *key) {
  List *l = list_new();
    EACHL(this, Kv, kv) {
      if (strcmp(kv_key(kv), key)) {
        l = list_cons(l, kv);
      }
    }_EACH
  return (Hash *)list_reverse(l);
}

Arr *hash_keys(Hash *this) {
  // Arr[char]
  Arr *r = arr_new();
  EACHL(hash_compact(this), Kv, e)
    arr_push(r, kv_key(e));
  _EACH
  return r;
}

Arr *hash_kvs(Hash *this) {
  // Arr[Kv]
  Arr *r = arr_new();
  EACHL(hash_compact(this), Kv, e)
    arr_push(r, e);
  _EACH
  return r;
}

It *hash_to_it(Hash *this) {
  return list_to_it((List *)hash_compact(this));
}

Hash *hash_from_it(It *it) {
  return (Hash *)list_from_it(it);
}

Js *hash_to_js(Hash *this, Js *(*to)(void *e)) {
  return map_to_js((Map *)hash_kvs(this), to);
}

Hash *hash_from_js(Js *js, void *(*from)(Js *jse)) {
  List *r = list_new();
  EACHR(js_ra(js), Js, j) {
    // Arr[Js]
    Arr *akv = js_ra(j);
    char *key = js_rs(arr_get(akv, 0));
    void *value = from(arr_get(akv, 1));
    r = list_cons(r, kv_new(key, value));
  }_EACH
  return (Hash *)r;
}
