// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Code array template.<p>
/// Use:
///   #define TY char                    // Element type
///   #define FN char                    // Function prefix
///   #include "dmc/tpl/tarr.c"
///   #undef TY
///   #undef FN

#include "dmc/tpl/DEFS.h"
#include "dmc/DEFS.h"

#define CT TPL_CAT(A, FN)
#define IT TPL_CAT(I, FN)
#define FUN(id) TPL_CAT_FUN(a, FN, id)

inline
CT *FUN(new)(void) {
  return (CT *)arr_new();
}

inline
size_t FUN(size) (CT *this) {
  return arr_size((Arr *)this);
}

inline
void FUN(add) (CT *this, TY *element) {
  arr_add((Arr *)this, element);
}

inline
void FUN(add_arr)(CT *this, CT *another) {
  arr_add_arr((Arr *)this, (Arr *)another);
}

inline
TY *FUN(get) (CT *this, size_t index) {
  return arr_get((Arr *)this, index);
}

inline
void FUN(set)(CT *this, size_t index, TY *element) {
  arr_set((Arr *)this, index, element);
}

inline
void FUN(insert)(CT *this, size_t index, TY *element) {
  arr_insert((Arr *)this, index, element);
}

inline
void FUN(insert_arr)(CT *this, size_t index, CT *another) {
  arr_insert_arr((Arr *)this, index, (Arr *)another);
}

inline
void FUN(remove)(CT *this, size_t index) {
  arr_remove((Arr *)this, index);
}

inline
void FUN(remove_range)(CT *this, size_t begin, size_t end) {
  arr_remove_range((Arr *)this, begin, end);
}

inline
void FUN(reverse)(CT *this) {
  arr_reverse((Arr *)this);
}

inline
void FUN(sortf)(CT *this, bool (*f)(TY *e1, TY *e2)) {
  arr_sort((Arr*)this, (bool(*)(void *, void*))f);
}

inline
void FUN(shuffle)(CT *this) {
  arr_shuffle((Arr *)this);
}

inline
IT *FUN(to_it)(CT *this) {
  return (IT*) arr_to_it((Arr *)this);
}

inline
CT *FUN(from_it)(IT *it) {
  return (CT *)arr_from_it((It *)it);
}

inline
Ajson *FUN(to_json)(CT *this, Ajson *(*to)(TY *)) {
  return arr_to_json((Arr *)this, (TO_JSON)to);
}

inline
CT *FUN(from_json)(Ajson *js, TY *(*from)(Ajson *)) {
  return (CT *)arr_from_json(js, (FROM_JSON)from);
}

#undef CT
#undef IT
#undef FUN
