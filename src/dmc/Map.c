// Copyright 1-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "dmc/Map.h"
#include "dmc/Arr.h"
#include "dmc/exc.h"
#include "dmc/str.h"
#include "dmc/It.h"
#include "dmc/Opt.h"
#include "dmc/Tuples.h"
#include "dmc/Json.h"
#include "dmc/ct/Ikv.h"
#include "dmc/ct/Ajson.h"
#include "dmc/ct/Achar.h"
#include "dmc/DEFS.h"

inline
Map *map_new(void) {
  return (Map *)arr_new();
}

size_t map_size(Map *this) {
  XNULL(this)
  return arr_size((Arr *)this);
}

void map_put(Map *this, char *key, void *value) {
  XNULL(this)
  XNULL(key)
  XNULL(value)

  EACH(this, Kv, kv) {
    if (str_eq(kv_key(kv), key)) {
      arr_set((Arr*)this, _i, kv_new(key, value));
      return;
    }
  }_EACH
  arr_add((Arr*)this, kv_new(key, value));
}

Opt *map_get(Map *this, char *key) {
  XNULL(this)
  XNULL(key)

  EACH(this, Kv, kv) {
    if (str_eq(kv_key(kv), key)) {
      return opt_new(kv_value(kv));
    }
  }_EACH
  return opt_null();
}

void *map_oget(Map *this, char *key, void *option) {
  XNULL(this)
  XNULL(key)
  XNULL(option)

  EACH(this, Kv, kv) {
    if (str_eq(kv_key(kv), key)) {
      return opt_new(kv_value(kv));
    }
  }_EACH
  return option;
}

void map_remove(Map *this, char *key) {
  XNULL(this)
  XNULL(key)

  EACH(this, Kv, kv) {
    if (str_eq(kv_key(kv), key)) {
      arr_remove((Arr*)this, _i);
      break;
    }
  }_EACH
}

bool map_has_key(Map *this, char *key) {
  XNULL(this)
  XNULL(key)

  EACH(this, Kv, kv) {
    if (str_eq(kv_key(kv), key)) return true;
  }_EACH
  return false;
}

/**/static FNM(key, Kv, kv) { return kv_key(kv); } _FN
Ichar *map_keys(Map *this) {
  XNULL(this)
  return (Ichar *)it_map(arr_to_it((Arr *)this), key);
}

/**/static FNM(value, Kv, kv) { return kv_value(kv); } _FN
It *map_values(Map *this) {
  XNULL(this)
  return it_map(arr_to_it((Arr *)this), value);
}

Ikv *map_to_it (Map *this) {
  XNULL(this)
  return (Ikv *)arr_to_it((Arr *)this);
}

/**/static FNE (cmp, Kv, e1, e2) {
/**/  return str_cmp(kv_key(e1), kv_key(e2)) > 0;
/**/}_FN
Ikv *map_to_it_sort (Map *this) {
  XNULL(this)
  return (Ikv *)it_sort(arr_to_it((Arr *)this), cmp);
}

/**/static FNE (cmp_locale, Kv, e1, e2) {
/**/  return str_cmp_locale(kv_key(e1), kv_key(e2)) > 0;
/**/}_FN
Ikv *map_to_it_sort_locale (Map *this) {
  XNULL(this)
  return (Ikv *)it_sort(arr_to_it((Arr *)this), cmp_locale);
}

Ajson *map_to_json(Map *this, Ajson *(*to)(void *)) {
  XNULL(this)

  Ajson *r = ajson_new();
  ajson_add(r, json_warray(achar_to_json(
    achar_from_it(map_keys(this)), str_to_json
  )));
  ajson_add(r, json_warray(arr_to_json(
    arr_from_it(map_values(this)), to)
  ));
  return r;
}

Map *map_from_json(Ajson *js, void *(*from)(Ajson *)) {
  XNULL(js);

  Ajson *jks = json_rarray(ajson_get(js, 0));
  Ajson *jvs = json_rarray(ajson_get(js, 1));
  size_t size = ajson_size(jks);
  if (ajson_size(jvs) != size)
    THROW(exc_range_t) exc_range(size, size + 1, ajson_size(jvs)) _THROW
  Achar *ks = achar_from_json(jks, str_from_json);
  Arr *vs = arr_from_json(jvs, from);

  Arr *r = arr_new();
  Kv *kv;
  RANGE0(i, size) {
    kv = kv_new(achar_get(ks, i), arr_get(vs, i));
    arr_add(r, kv);
  }_RANGE
  return (Map *) r;
}
