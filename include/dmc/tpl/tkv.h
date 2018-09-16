// Copyright 1-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Header Kv template.<p>
/// Use:
///   #define TY char                    // Element type
///   #define FN char                    // Function prefix
///   #include "dmc/tpl/tkv.h"
///   #undef TY
///   #undef FN

#include "dmc/tpl/DEFS.h"
#include "dmc/Json.h"

#define CT TPL_CAT(K, FN)
#define FUN(id) TPL_CAT_FUN(k, FN, id)

///
typedef struct FUN(CT) CT;

CT *FUN(new)(char *key, TY *value);

///
char *FUN(key)(CT *this);

///
TY *FUN(value)(CT *this);

///
Json *FUN(to_jsonf)(CT *this, Json *(*to)(TY *));

///
CT *FUN(from_jsonf)(Json *js, TY *(*from)(Json *));

#undef CT
#undef FUN
