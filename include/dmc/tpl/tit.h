// Copyright 30-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Header It template.<p>
/// Use:
///   #define TY char                    // Element type
///   #define FN char                    // Function prefix
///   #include "dmc/tpl/tit.h"
///   #undef TY
///   #undef FN

#include <stdbool.h>
#include <stdlib.h>
#include "dmc/tpl/DEFS.h"

#define IT TPL_CAT(I, FN)
#define OP TPL_CAT(O, FN)
#define FUN(id) TPL_CAT_FUN(i, FN, id)

///
typedef struct FUN(IT) IT;
typedef struct TPL_CAT_FUN(o, FN, OP) OP;
typedef struct it_It It;
typedef struct itp_Itp Itp;
typedef struct itp3_Itp3 Itp3;



IT *FUN(new)(void *o, TY *(*next)(void *o));

///
IT *FUN(empty)(void);

///
IT *FUN(unary)(TY *e);

/// FUN(range is an iterator that returns values between begin (inclusive)
/// and end (exclusive)
IT *FUN(range)(int begin, int end);

/// FUN(range0 is equals to FUN(range(0, end);
IT *FUN(range0)(int end);

///
bool FUN(has_next)(IT *this);

///
TY *FUN(next)(IT *this);

// Show next element witout advancing.
TY *FUN(peek)(IT *this);

/// FUN(add adds an element at the end of 'this'
IT *FUN(add)(IT *this, TY *element);

/// FUN(add adds an element at the beginning of 'this'
IT *FUN(add0)(IT *this, TY *element);

///
IT *FUN(cat)(IT *this, IT *another);

///
IT *FUN(take)(IT *this, size_t n);

///
IT *FUN(takef)(IT *this, bool (*predicate)(TY *e));

///
IT *FUN(drop)(IT *this, size_t n);

///
IT *FUN(dropf)(IT *this, bool (*predicate)(TY *e));

///
IT *FUN(filter)(IT *this, bool (*predicate)(TY *e));

///
It *FUN(map)(IT *this, void *(*converter)(TY *e));

/// FUN(map2 applies conv1 to the first element and conv2 to the others.
It *FUN(map2)(IT *this, void *(*conv1)(TY *e), void *(*conv2)(TY *e));

///
Itp *FUN(zip) (IT *it1, IT *it2);

///
Itp3 *FUN(zip3) (IT *it1, IT *it2, IT *it3);

///
IT *FUN(reverse)(IT *this);

///
IT *FUN(sort)(IT *this, bool (*comparator)(TY *e1, TY *e2));

///
void FUN(each)(IT *this, void (*f)(TY *e));

///
void FUN(each_ix)(IT *this, void (*f)(TY *e, size_t ix));

///
size_t FUN(count)(IT *this);

///
bool FUN(eqf)(IT *it1, IT *it2, bool (*feq)(TY *e1, TY *e2));

///
int FUN(indexf)(IT *this, bool (*predicate)(TY *e));

///
bool FUN(containsf)(IT *this, bool (*predicate)(TY *e));

///
int FUN(last_indexf)(IT *this, bool (*predicate)(TY *e));

/// Returns the first element which satisfies 'predicate' or opt_null.
OP *FUN(find)(IT *this, bool (*predicate)(TY *e));

/// Returns the first element which satisfies 'predicate' or 'option'.
TY *FUN(ofind)(IT *this, TY *option, bool (*predicate)(TY *e));

#undef IT
#undef OP
#undef FUN
