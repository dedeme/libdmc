// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Code Opt template.<p>
/// Use:
///   #define TY char                    // Element type
///   #define FN char                    // Function prefix
///   #include "dmc/tpl/topt.c"
///   #undef TY
///   #undef FN

#include "dmc/tpl/DEFS.h"
#include "dmc/DEFS.h"

#define CT TPL_CAT(O, FN)
#define FUN(id) TPL_CAT_FUN(o, FN, id)

inline
CT *FUN(null)() {
  return (CT *)opt_null();
}

inline
CT *FUN(new)(TY *v) {
  return (CT *)opt_new(v);
}

inline
bool FUN(is_null)(CT *this) {
  return opt_is_null((Opt *)this);
}

inline
TY *FUN(value)(CT *this) {
  return (TY *)opt_value((Opt *)this);
}

inline
Json *FUN(to_json)(CT *this, Json *(*to)(TY *)) {
  return opt_to_json((Opt *)this, (TO_JSON)to);
}

inline
CT *FUN(from_json)(Json *js, TY *(*from)(Json *)) {
  return (CT *)opt_from_json(js, (FROM_JSON)from);
}

#undef CT
#undef FUN
