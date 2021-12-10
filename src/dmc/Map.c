// Copyright 22-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Map.h"
#include "dmc/std.h"

Map *map_new(void) {
  AKv *this = MALLOC(AKv);
  Kv **es = GC_MALLOC(15 * sizeof(Kv *));
  this->es = es;
  this->end = es;
  this->endbf = es + 15;
  return (Map *)this;
}

int map_size(Map *this) {
  return arr_size((Arr *) this);
}

void map_put(Map *this, char *key, void *value) {
  AKv *th = (AKv *)this;
  int no_added = 1;
  Kv **p = th->es;
  while (p < th->end) {
    Kv *e = *p;
    if (str_eq(e->k, key)) {
      *p = kv_new(key, value);
      no_added = 0;
      break;
    }
    ++p;
  }
  if (no_added) {
    aKv_push(th, kv_new(key, value));
  }
}

int map_has_key(Map *this, char *key) {
  AKv *th = (AKv *)this;
  Kv **p = th->es;
  while (p < th->end) if (str_eq((*p++)->k, key)) return 1;
  return 0;
}

Opt *map_get(Map *this, char *key) {
  AKv *th = (AKv *)this;
  Kv **p = th->es;
  while (p < th->end) {
    if (str_eq((*p)->k, key)) return opt_mk_some((*p)->v);
    ++p;
  }
  return opt_mk_none();
}

void map_remove(Map *this, char *key) {
  AKv *th = (AKv *)this;
  /**/int fn (Kv *e) { return str_eq(e->k, key); }
  int ix = aKv_index(th, fn);
  if (ix != -1) aKv_remove(th, ix);
}

Achar *map_keys(Map *this) {
  /**/void *fmap (Kv *e) { return e->k; }
  return (Achar *)aKv_map((AKv *)this, fmap);
}

AKv *map_to_array(Map *this) {
  return (AKv *)this;
}

Map *map_from_array(AKv *this) {
  return (Map *)this;
}

void map_sort(Map *this) {
  /**/int greater(Kv *e1, Kv *e2) { return str_greater(e1->k, e2->k); }
  aKv_sort((AKv *)this, greater);
}

void map_sort_locale(Map *this) {
  /**/int greater(Kv *e1, Kv *e2) { return str_greater_locale(e1->k, e2->k); }
  aKv_sort((AKv *)this, greater);
}

char *map_to_js (Map *this, char *(*to)(void *e)) {
  /**/void *to_Kchar (Kv *kv) { return kchar_new(kv->k, to(kv->v)); }
  return js_wo(mchar_from_array((AKchar *)aKv_map(
    map_to_array(this), to_Kchar
  )));
}

Map *map_from_js (char *js, void *(*from)(char *ejs)) {
  /**/void *to_Kv (Kchar *kv) { return kv_new(kv->k, from(kv->v)); }
  return map_from_array((AKv *)aKchar_map(
    mchar_to_array(js_ro(js)), to_Kv
  ));
}
