// Copyright 1-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Code Map template.<p>
/// Use:
///   #define TY char                    // Element type
///   #define FN char                    // Function prefix
///   #include "dmc/tpl/topt.c"
///   #undef TY
///   #undef FN

#include "dmc/tpl/DEFS.h"

#define CT TPL_CAT(M, FN)
#define IT TPL_CAT(I, FN)
#define OP TPL_CAT(O, FN)
#define FUN(id) TPL_CAT_FUN(m, FN, id)

inline
CT *FUN(new)(void) {
  return (CT *)map_new();
}

inline
size_t FUN(size)(CT *this) {
  return map_size((Map *)this);
}

inline
void FUN(put)(CT *this, char *key, TY *value) {
  map_put((Map *)this, key, value);
}

inline
OP *FUN(get)(CT *this, char *key) {
  return (OP *)map_get((Map *)this, key);
}

inline
TY *FUN(oget)(CT *this, char *key, TY *option) {
  return (TY *)map_get((Map *)this, key);
}

inline
void FUN(remove)(CT *this, char *key) {
  map_remove((Map *)this, key);
}

inline
bool FUN(has_key)(CT *this, char *key) {
  return map_has_key((Map *)this, key);
}

inline
Ichar *FUN(keys)(CT *this) {
  return map_keys((Map *)this);
}

inline
IT *FUN(values)(CT *this) {
  return (IT *)map_values((Map *)this);
}

inline
Ikv *FUN(to_it)(CT *this) {
  return map_to_it((Map *)this);
}

inline
Ikv *FUN(to_it_sort)(CT *this) {
  return map_to_it_sort((Map *)this);
}

inline
Ikv *FUN(to_it_sort_locale)(CT *this) {
  return map_to_it_sort_locale((Map *)this);
}

#undef CT
#undef IT
#undef OP
#undef FUN
