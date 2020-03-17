// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Map.h"
#include "dmc/std.h"

struct map_Map {
  void **es;
  void **end;
  void **endbf;
};

Map *map_new(void) {
  Map *this = MALLOC(Map);
  void **es = GC_MALLOC(15 * sizeof(void *));
  this->es = es;
  this->end = es;
  this->endbf = es + 15;
  return this;
}

int map_size(Map *this) {
  return arr_size((Arr *) this);
}

void map_put(Map *this, char *key, void *value) {
  int no_added = 1;
  EACH_IX(this, Kv, e, i)
    if (str_eq(kv_key(e), key)) {
      this->es[i] = kv_new(key, value);
      no_added = 0;
      break;
    }
  _EACH
  if (no_added) {
    arr_push((Arr *)this, kv_new(key, value));
  }
}

int map_has_key(Map *this, char *key) {
  EACH(this, Kv, e)
    if (str_eq(kv_key(e), key)) return 1;
  _EACH
  return 0;
}

Opt *map_get(Map *this, char *key) {
  EACH(this, Kv, e)
    if (str_eq(kv_key(e), key)) {
      return opt_new(kv_value(e));
    }
  _EACH
  return opt_empty();
}

void map_remove(Map *this, char *key) {
  int ix = -1;
  EACH_IX(this, Kv, e, i)
    if (str_eq(kv_key(e), key)) {
      ix = i;
      break;
    }
  _EACH
  if (ix != -1) {
    arr_remove((Arr *)this, ix);
  }
}

// Arr[char]
Arr *map_keys(Map *this) {
  // Arr[char]
  Arr *r = arr_new();
  EACH(this, Kv, e)
    arr_push(r, kv_key(e));
  _EACH
  return r;
}

Arr *map_kvs(Map *this) {
  return (Arr *)this;
}

void map_sort(Map *this) {
  int greater(Kv *e1, Kv *e2) {
    return str_greater(kv_key(e1), kv_key(e2));
  }
  arr_sort((Arr *)this, (FCMP) greater);
}

void map_sort_locale(Map *this) {
  int greater(Kv *e1, Kv *e2) {
    return str_greater_locale(kv_key(e1), kv_key(e2));
  }
  arr_sort((Arr *)this, (FCMP) greater);
}

It *map_to_it(Map *this) {
  return arr_to_it((Arr *) this);
}

Map *map_from_it(It *it) {
  return (Map *)arr_from_it(it);
}

Js *map_to_js(Map *this, Js *(*to)(void *e)) {
  // Arr[Js]
  Arr *a = arr_new();
  Kv **p = (Kv **)this->es;
  Kv **end = (Kv **)this->end;
  while (p < end) {
    Kv *kv = *p++;
    // Arr[Js]
    Arr *akv = arr_new();
    arr_push(akv, js_ws(kv_key(kv)));
    arr_push(akv, to(kv_value(kv)));
    arr_push(a, js_wa(akv));
  }
  return js_wa(a);
}

Map *map_from_js(Js *js, void *(*from)(Js *jse)) {
  Arr *this = arr_new();
  // Arr[Js]
  Arr *a = js_ra(js);
  Js **p = (Js **)arr_start(a);
  Js **end = (Js **)arr_end(a);
  while (p < end) {
    // Arr[Js]
    Arr *akv = js_ra(*p++);
    char *key = js_rs(arr_get(akv, 0));
    void *value = from(arr_get(akv, 1));
    arr_push(this, kv_new(key, value));
  }
  return (Map *)this;
}
