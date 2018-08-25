// Copyright 1-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Header List template.<p>
/// Use:
///   #define TY char                    // Element type
///   #define FN char                    // Function prefix
///   #include "dmc/tpl/tlist.h"
///   #undef TY
///   #undef FN

#include "dmc/tpl/DEFS.h"
#include "dmc/List.h"

#define CT TPL_CAT(L, FN)
#define IT TPL_CAT(I, FN)
#define AR TPL_CAT(A, FN)
#define FUN(id) TPL_CAT_FUN(l, FN, id)

///
typedef struct FUN(CT) CT;
typedef struct TPL_CAT_FUN(i, FN, IT) IT;
typedef struct TPL_CAT_FUN(a, FN, AR) AR;

///
CT *FUN(new)(void);

///
size_t FUN(count)(CT *this);

/// FUN(head returns every element of 'this' less the first one. If "this" is
/// empty, throws an exception
CT *FUN(tail)(CT *this);

/// FUN(head returns the first element of 'this'. If "this" is empty,
/// throws an exception.
TY *FUN(head)(CT *this);

///
bool FUN(empty)(CT *this);

/// FUN(cons adds 'o' at head. 'o' must be not NULL.
CT *FUN(cons)(CT *this, TY *o);

/// FUN(cat returns this + l
CT *FUN(cat)(CT *this, CT *l);

/// FUN(reverse returns this in reverse order
CT *FUN(reverse)(CT *this);

/// FUN(to_it returns an iterator from top to bottom
IT *FUN(to_it)(CT *this);

/// FUN(from_it return a CT with elements of 'it' in reverse order
CT *FUN(from_it)(IT *it);

///
AR *FUN(to_arr)(CT *this);

///
CT *FUN(from_arr)(AR *a);

///
Json *FUN(to_json)(CT *this, Json *(*to)(TY *));

///
CT *FUN(from_json)(Json *js, TY *(*from)(Json *));

#undef CT
#undef IT
#undef AR
#undef FUN
