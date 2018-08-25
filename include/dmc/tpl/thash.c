// Copyright 04-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Code Hash template.<p>
/// Use:
///   #define TY char                    // Element type
///   #define FN char                    // Function prefix
///   #include "dmc/tpl/topt.c"
///   #undef TY
///   #undef FN

#include "dmc/tpl/DEFS.h"
#include "dmc/DEFS.h"

#define CT TPL_CAT(H, FN)
#define IT TPL_CAT(I, FN)
#define OP TPL_CAT(O, FN)
#define FUN(id) TPL_CAT_FUN(h, FN, id)

inline
CT *FUN(new)(void) {
  return (CT *)hash_new();
}

inline
size_t FUN(count)(CT *this) {
  return hash_count((Hash *)this);
}

inline
CT *FUN(put)(CT *this, char *key, TY *value) {
  return (CT *)hash_put((Hash *)this, key, value);
}

inline
OP *FUN(get)(CT *this, char *key) {
  return (OP *)hash_get((Hash *)this, key);
}

inline
TY *FUN(oget)(CT *this, char *key, TY *option) {
  return (TY *)hash_get((Hash *)this, key);
}

inline
CT *FUN(remove)(CT *this, char *key) {
  return (CT *)hash_remove((Hash *)this, key);
}

inline
CT *FUN(compact)(CT *this) {
  return (CT *)hash_compact((Hash *)this);
}

inline
bool FUN(has_key)(CT *this, char *key) {
  return hash_has_key((Hash *)this, key);
}

inline
Ichar *FUN(keys)(CT *this) {
  return hash_keys((Hash *)this);
}

inline
IT *FUN(values)(CT *this) {
  return (IT *)hash_values((Hash *)this);
}

inline
Ikv *FUN(to_it)(CT *this) {
  return hash_to_it((Hash *)this);
}

inline
Ikv *FUN(to_it_sort)(CT *this) {
  return hash_to_it_sort((Hash *)this);
}

inline
Ikv *FUN(to_it_sort_locale)(CT *this) {
  return hash_to_it_sort_locale((Hash *)this);
}

inline
CT *FUN(from_it)(Ikv *it) {
  return (CT *)hash_from_it(it);
}

inline
Json *FUN(to_json)(CT *this, Json *(*to)(TY *)) {
  return hash_to_json((Hash *)this, (TO_JSON)to);
}

inline
CT *FUN(from_json)(Json *js, TY *(*from)(Json *)) {
  return (CT *)hash_from_json(js, (FROM_JSON)from);
}

#undef CT
#undef IT
#undef OP
#undef FUN
