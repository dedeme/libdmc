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
      if (kv->value) {
        return kv->value;
      }
      THROW exc_null_pointer("kv->value") _THROW
    }
  }_EACH
  THROW "key '%s' does not exist", key _THROW
  return NULL;
}

void *map_nget(Map *this, char *key) {
  EACH(this, Kv, kv) {
    if (!strcmp(kv->key, key))
      return kv->value;
  }_EACH
  THROW "key '%s' does not exist", key _THROW
  return NULL;
}

void *map_oget(Map *this, char *key, void *option) {
  EACH(this, Kv, kv) {
    if (!strcmp(kv->key, key))
      return kv->value;
  }_EACH
  return option;
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
      return true;
  }_EACH
  return false;
}

Arr/*char*/ *map_keys(Map *this) {
  Arr/*char*/ *r = arr_new();
  EACH(this, Kv, kv) {
    arr_add(r, kv->key);
  }_EACH
  return r;
}

Arr/*void*/ *map_values(Map *this) {
  Arr/*void*/ *r = arr_new();
  EACH(this, Kv, kv) {
    arr_add(r, kv->value);
  }_EACH
  return r;
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
