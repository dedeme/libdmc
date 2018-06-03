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

#undef CT
#undef FUN
