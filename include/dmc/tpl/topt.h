// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Header Opt template.<p>
/// Use:
///   #define TY char                    // Element type
///   #define FN char                    // Function prefix
///   #include "dmc/tpl/topt.h"
///   #undef TY
///   #undef FN

#include "dmc/tpl/DEFS.h"
#include "dmc/Opt.h"

#define CT TPL_CAT(O, FN)
#define FUN(id) TPL_CAT_FUN(o, FN, id)

///
typedef struct FUN(CT) CT;

CT *FUN(null)();

/// opt_new throws exception if 'v' is null.
/// Throws exception
CT *FUN(new)(TY *v);

///
bool FUN(is_null)(CT *this);

/// opt_value trhows exception if 'this' value is null.
/// Throws exception.
TY *FUN(value)(CT *this);

///
Json *FUN(to_jsonf)(CT *this, Json *(*to)(TY *));

///
CT *FUN(from_jsonf)(Json *js, TY *(*from)(Json *));

#undef CT
#undef FUN
