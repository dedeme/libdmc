// Copyright 04-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "dmc/Hash.h"
#include "dmc/List.h"
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

Hash *hash_new(void) {
  return (Hash *)list_new();
}

size_t hash_count(Hash *this) {
  return list_count((List *) this);
}

Hash *hash_put(Hash *this, char *key, void *value) {
  XNULL(this)
  XNULL(key)
  XNULL(value)

  return (Hash *)list_cons((List *)this, kv_new(key, value));
}

Opt *hash_get(Hash *this, char *key) {
  XNULL(this)
  XNULL(key)

  EACHL(this, Kv, kv) {
    if (str_eq(kv_key(kv), key)) {
      return opt_new(kv_value(kv));
    }
  }_EACH
  return opt_null();
}

void *hash_oget(Hash *this, char *key, void *option) {
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

Hash *hash_remove(Hash *this, char *key) {
  XNULL(this)
  XNULL(key)

  List *l = list_new();
  EACHL(this, Kv, kv) {
    if (str_cmp(kv_key(kv), key)) {
      l = list_cons(l, kv);
    }
  }_EACH
  return (Hash *)list_reverse(l);
}

/// hash_compact returns a new Hash without duplicated keys.
Hash *hash_compact(Hash *this) {
  XNULL(this)

  List *l = list_new();
  EACHL(this, Kv, kv) {
    /**/ bool eq(Kv *ekv) { return str_eq(kv_key(kv), kv_key(ekv)); }
    if (!ikv_containsf((Ikv *)list_to_it(l), eq)) {
      l = list_cons(l, kv);
    }
  }_EACH
  return (Hash *)list_reverse(l);
}

bool hash_has_key(Hash *this, char *key) {
  XNULL(this)
  XNULL(key)

  EACHL(this, Kv, kv) {
    if (str_eq(kv_key(kv), key)) return true;
  }_EACH
  return false;
}

/**/static FNM(key, Kv, kv) { return kv_key(kv); } _FN
Ichar *hash_keys(Hash *this) {
  XNULL(this)
  return (Ichar *)it_map(list_to_it((List *)this), key);
}

/**/static FNM(value, Kv, kv) { return kv_value(kv); } _FN
It *hash_values(Hash *this) {
  XNULL(this)
  return it_map(list_to_it((List *)this), value);
}

Ikv *hash_to_it(Hash *this) {
  XNULL(this)
  return (Ikv *)list_to_it((List *)this);
}

/**/static FNE (cmp, Kv, e1, e2) {
/**/  return str_cmp(kv_key(e1), kv_key(e2)) > 0;
/**/}_FN
Ikv *hash_to_it_sort(Hash *this) {
  XNULL(this)
  return (Ikv *)it_sort(list_to_it((List *)this), cmp);
}

/**/static FNE (cmp_locale, Kv, e1, e2) {
/**/  return str_cmp_locale(kv_key(e1), kv_key(e2)) > 0;
/**/}_FN
Ikv *hash_to_it_sort_locale(Hash *this) {
  XNULL(this)
  return (Ikv *)it_sort(list_to_it((List *)this), cmp_locale);
}

Hash *hash_from_it(Ikv *it) {
  XNULL(it)
  return (Hash *)list_from_it((It *)it);
}

Ajson *hash_to_json(Hash *this, Ajson *(*to)(void *)) {
  XNULL(this)

  Ajson *r = ajson_new();
  ajson_add(r, json_warray(achar_to_json(
    achar_from_it(hash_keys(this)), str_to_json
  )));
  ajson_add(r, json_warray(arr_to_json(
    arr_from_it(hash_values(this)), to)
  ));
  return r;
}

/// hash_from_json restores a serialized Hash using 'from' to convert elements.
Hash *hash_from_json(Ajson *js, void *(*from)(Ajson *)) {
  XNULL(js);

  Ajson *jks = json_rarray(ajson_get(js, 0));
  Ajson *jvs = json_rarray(ajson_get(js, 1));
  size_t size = ajson_size(jks);
  if (ajson_size(jvs) != size)
    THROW(exc_range_t) exc_range(size, size + 1, ajson_size(jvs)) _THROW
  Achar *ks = achar_from_json(jks, str_from_json);
  Arr *vs = arr_from_json(jvs, from);

  List *l = list_new();
  Kv *kv;
  for (int i = size - 1; i >= 0; --i) {
    kv = kv_new(achar_get(ks, i), arr_get(vs, i));
    list_cons(l, kv);
  }
  return (Hash *) l;
}
