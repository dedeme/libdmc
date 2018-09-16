// Copyright 1-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Code Kv template.<p>
/// Use:
///   #define TY char                    // Element type
///   #define FN char                    // Function prefix
///   #include "dmc/tpl/tkv.c"
///   #undef TY
///   #undef FN

#include "dmc/tpl/DEFS.h"
#include "dmc/DEFS.h"
#include "dmc/Tuples.h"

#define CT TPL_CAT(K, FN)
#define FUN(id) TPL_CAT_FUN(k, FN, id)

inline
CT *FUN(new)(char *key, TY *value) {
  return (CT *)kv_new(key, value);
}

char *FUN(key)(CT *this) {
  return kv_key((Kv *)this);
}

TY *FUN(value)(CT *this) {
  return kv_value((Kv *)this);
}

inline
Json *FUN(to_jsonf)(CT *this, Json *(*to)(TY *)) {
  return kv_to_json((Kv *)this, (TO_JSON)to);
}

inline
CT *FUN(from_jsonf)(Json *js, TY *(*from)(Json *)) {
  return (CT *)kv_from_json(js, (FROM_JSON)from);
}

#undef CT
#undef FUN
