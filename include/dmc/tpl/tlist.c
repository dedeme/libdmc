// Copyright 1-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Code List template.<p>
/// Use:
///   #define TY char                    // Element type
///   #define FN char                    // Function prefix
///   #include "dmc/tpl/tlist.c"
///   #undef TY
///   #undef FN

#include "dmc/tpl/DEFS.h"
#include "dmc/DEFS.h"

#define CT TPL_CAT(L, FN)
#define IT TPL_CAT(I, FN)
#define FUN(id) TPL_CAT_FUN(l, FN, id)

inline
CT *FUN(new)(void) {
  return (CT *)list_new();
}

inline
CT *FUN(tail)(CT *this) {
  return (CT *)list_tail((List *)this);
}

inline
TY *FUN(head)(CT *this) {
  return list_head((List *)this);
}

inline
bool FUN(empty)(CT *this) {
  return list_empty((List *)this);
}

inline
CT *FUN(cons)(CT *this, TY *o) {
  return (CT *)list_cons((List *)this, o);
}

inline
CT *FUN(cat)(CT *this, CT *l) {
  return (CT *)list_cat((List *)this, (List *)l);
}

inline
CT *FUN(reverse)(CT *this) {
  return (CT *)list_reverse((List *)this);
}

inline
IT *FUN(to_it)(CT *this) {
  return (IT*)list_to_it((List *)this);
}

inline
CT *FUN(from_it)(IT *it) {
  return (CT *)list_from_it((It *)it);
}

inline
Arr *FUN(to_arr)(CT *this) {
  return list_to_arr((List *)this);
}

inline
CT *FUN(from_arr)(Arr *a) {
  return (CT *)list_from_arr(a);
}

inline
Ajson *FUN(to_json)(CT *this, Ajson *(*to)(TY *)) {
  return list_to_json((List *)this, (TO_JSON)to);
}

inline
CT *FUN(from_json)(Ajson *js, TY *(*from)(Ajson *)) {
  return (CT *)list_from_json(js, (FROM_JSON)from);
}

#undef CT
#undef IT
#undef FUN
