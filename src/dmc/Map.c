// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Map.h"

#include "dmc/std.h"

struct map_Kv {
  char *key;
  void *value;
  void (*ffree)(void *);
};

static Kv *kv_new(char *key, void *value, void (*ffree)(void *)) {
  Kv *this = malloc(sizeof(Kv));
  this->key = str_new(key);
  this->value = value;
  this->ffree = ffree;
  return this;
}

static void kv_free(Kv *this) {
  free(this->key);
  this->ffree(this->value);
  free(this);
}

static void free_es(void **es, int len, void(*ffree)(void *)) {
  void **p = es;
  REPEAT(len)
    ffree(*p++);
  _REPEAT
  free(es);
}

struct map_Map {
  void **es;
  void **end;
  void **endbf;
  void (*ffree)(void *);
  void (*vfree)(void *);
};

Map *map_new(void (*ffree)(void *)) {
  Map *this = malloc(sizeof(Map));
  void **es = malloc(15 * sizeof(void *));
  this->es = es;
  this->end = es;
  this->endbf = es + 15;
  this->ffree = (FPROC)kv_free;
  this->vfree = ffree;
  return this;
}

void map_free(Map *this) {
  free_es(this->es, this->end - this->es, this->ffree);
  free(this);
}

int map_size(Map *this) {
  return arr_size((Arr *) this);
}

void map_put(Map *this, char *key, void *value) {
  int no_added = 1;
  EACH(this, Kv, e)
    if (str_eq(e->key, key)) {
      this->vfree(e->value);
      e->value = value;
      no_added = 0;
      break;
    }
  _EACH
  if (no_added) {
    arr_push((Arr *)this, kv_new(key, value, this->vfree));
  }
}

void *map_get_null(Map *this, char *key) {
  EACH(this, Kv, e)
    if (str_eq(e->key, key)) {
      return e->value;
    }
  _EACH
  return NULL;
}

void map_remove(Map *this, char *key) {
  int ix = -1;
  EACH_IX(this, Kv, e, i)
    if (str_eq(e->key, key)) {
      ix = i;
      break;
    }
  _EACH
  if (ix != -1) {
    arr_remove((Arr *)this, ix);
  }
}

// Varr[char]
Varr *map_keys_new(Map *this) {
  // Varr[char]
  Varr *r = varr_new();
  EACH(this, Kv, e)
    varr_push(r, e->key);
  _EACH
  return r;
}

Arr *map_kvs(Map *this) {
  return (Arr *)this;
}

char *map_key(Kv *entry) {
  return entry->key;
}

void *map_value(Kv *entry) {
  return entry->value;
}

void map_sort(Map *this) {
  int greater(Kv *e1, Kv *e2) {
    return str_greater(e1->key, e2->key);
  }
  varr_sort((Varr *)this, (FGREATER) greater);
}

void map_sort_locale(Map *this) {
  int greater(Kv *e1, Kv *e2) {
    return str_greater_locale(e1->key, e2->key);
  }
  varr_sort((Varr *)this, (FGREATER) greater);
}

