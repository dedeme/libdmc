// Copyright 02-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Hash.h"
#include "dmc/List.h"
#include "dmc/std.h"

Hash *hash_new(void) {
  LKv *this = MALLOC(LKv);
  this->next = NULL;
  this->value = NULL;
  return (Hash *)this;
}

int hash_size(Hash *this) {
  return list_size((List *)this);
}

Hash *hash_put(Hash *this, char *key, void *value) {
  return (Hash *)list_cons((List *)this, kv_new(key, value));
}

int hash_has_key(Hash *this, char *key) {
  LKv *l = (LKv *)this;
  while (l->next) {
    if (str_eq(l->value->k, key)) return 1;
    l = l->next;
  }
  return 0;
}

Opt *hash_get(Hash *this, char *key) {
  LKv *l = (LKv *)this;
  while (l->next) {
    if (str_eq(l->value->k, key)) return opt_mk_some(l->value->v);
    l = l->next;
  }
  return opt_mk_none();
}

Hash *hash_compact(Hash *this) {
  AKv *a = lKv_to_arr((LKv *)this);
  /**/int feq (Kv *e1, Kv *e2) { return str_eq(e1->k, e2->k); }
  aKv_duplicates(a, feq);
  return (Hash *)lKv_from_arr(a);
}

Hash *hash_remove(Hash *this, char *key) {
  LKv *l = (LKv *)this;
  LKv *r = lKv_new();
  while (l->next) {
    Kv *v = l->value;
    if (strcmp(v->k, key)) r = lKv_cons(r, v);
    l = l->next;
  }
  return (Hash *)lKv_reverse(r);
}

Lchar *hash_keys(Hash *this) {
  LKv *l = (LKv *)this;
  Lchar *r = lchar_new();
  while (l->next) {
    r = lchar_cons(r, l->value->k);
    l = l->next;
  }
  return lchar_reverse(r);
}

Hash *hash_sort(Hash *this) {
  /**/int greater(Kv *e1, Kv *e2) { return str_greater(e1->k, e2->k); }
  return (Hash *)lKv_sort(hash_to_list(this), greater);
}

Hash *hash_sort_locale(Hash *this) {
  /**/int greater(Kv *e1, Kv *e2) { return str_greater_locale(e1->k, e2->k); }
  return (Hash *)lKv_sort(hash_to_list(this), greater);
}

LKv *hash_to_list(Hash *this) {
  return (LKv *)hash_compact(this);
}

Hash *hash_from_list(LKv *this) {
  return (Hash *)this;
}

char *hash_to_js(Hash *this, char *(*to)(void *e)) {
  return map_to_js(map_from_array(lKv_to_arr(hash_to_list(this))), to);
}

Hash *hash_from_js(char *js, void *(*from)(char *jse)) {
  return hash_from_list(lKv_from_arr((AKv *)map_from_js(js, from)));
}
