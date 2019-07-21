// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Map.h"
#include "dmc/std.h"
#include "dmc/Kv.h"
#include "dmc/It.h"

struct map_Map {
  void **es;
  void **end;
  void **endbf;
  Gc *gc;
};

Map *map_new(Gc *gc) {
  Map *this = gc_add_bf(gc, malloc(sizeof(Map)));
  void **es = malloc(15 * sizeof(void *));
  this->es = es;
  this->end = es;
  this->endbf = es + 15;
  this->gc = gc;
  return this;
}

int map_size(Map *this) {
  return arr_size((Arr *) this);
}

void map_put(Map *this, char *key, void *value) {
  int no_added = 1;
  EACH_IX(this, Kv, e, i)
    if (str_eq(kv_key(e), key)) {
      this->es[i] = kv_new(this->gc, key, value);
      no_added = 0;
      break;
    }
  _EACH
  if (no_added) {
    arr_push((Arr *)this, kv_new(this->gc, key, value));
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
Arr *map_keys(Gc *gc, Map *this) {
  // Arr[char]
  Arr *r = arr_new(gc);
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

It *map_to_it(Gc *gc, Map *this) {
  return arr_to_it(gc, (Arr *) this);
}

Map *map_from_it(Gc *gc, It *it) {
  Map *this = map_new(gc);
  this->gc = gc;
  while (it_has_next(it)) {
    arr_push((Arr *)this, it_next(it));
  }
  return this;
}

Js *map_to_js(Gc *gc, Map *this, Js *(*to)(Gc *gc, void *e)) {
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *a = arr_new(gcl);
  Kv **p = (Kv **)this->es;
  Kv **end = (Kv **)this->end;
  while (p < end) {
    Kv *kv = *p++;
    // Arr[Js]
    Arr *akv = arr_new(gcl);
    arr_push(akv, js_ws(gcl, kv_key(kv)));
    arr_push(akv, to(gcl, kv_value(kv)));
    arr_push(a, js_wa(gcl, akv));
  }
  Js *r = js_wa(gc, a);
  gc_free(gcl);
  return r;
}

Map *map_from_js(Gc *gc, Js *js, void *(*from)(Gc *gc, Js *jse)) {
  Map *this = map_new(gc);
  this->gc = gc;
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *a = js_ra(gcl, js);
  Js **p = (Js **)arr_start(a);
  Js **end = (Js **)arr_end(a);
  while (p < end) {
    // Arr[Js]
    Arr *akv = js_ra(gcl, *p++);
    char *key = js_rs(gc, arr_get(akv, 0));
    void *value = from(gc, arr_get(akv, 1));
    arr_push((Arr *)this, kv_new(gc, key, value));
  }
  gc_free(gcl);
  return this;
}
