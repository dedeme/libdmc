// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/all.h"

inline
Map *map_new() {
  return arr_new();
}

void map_put(Map *this, char *key, void *value) {
  if (!key) THROW exc_null_pointer("key") _THROW

  EACH(this, Kv, kv) {
    if (!strcmp(kv->key, key)) {
      kv->value = value;
      return;
    }
  }_EACH
  arr_add(this, kv_new(key, value));
}

void *map_get(Map *this, char *key) {
  EACH(this, Kv, kv) {
    if (!strcmp(kv->key, key)) {
      return kv->value;
    }
  }_EACH
  return NULL;
}

void map_remove(Map *this, char *key) {
  int ix = -1;
  int c = 0;
  EACH(this, Kv, kv) {
    if (!strcmp(kv->key, key)) {
      ix = c;
      break;
    }
    ++c;
  }_EACH
  if (ix != -1)
    arr_remove(this, ix);
}

bool map_has_key(Map *this, char *key) {
  EACH(this, Kv, kv) {
    if (!strcmp(kv->key, key))
      return 1;
  }_EACH
  return 0;
}

inline
It/*Kv*/ *map_to_it (Map *this) {
  return arr_to_it(this);
}

It/*Kv*/ *map_to_it_sort (Map *this) {
  FNE (cmp, Kv, e1, e2) { return strcmp(e1->key, e2->key) > 0; }_FN
  return it_sort(arr_to_it(this), cmp);
}

It/*Kv*/ *map_to_it_sort_locale (Map *this) {
  FNE (cmp, Kv, e1, e2) { return strcoll(e1->key, e2->key) > 0; }_FN
  return it_sort(arr_to_it(this), cmp);
}
