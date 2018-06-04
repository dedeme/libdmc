// Copyright 1-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Header Map template.<p>
/// Use:
///   #define TY char                    // Element type
///   #define FN char                    // Function prefix
///   #include "dmc/tpl/tmap.h"
///   #undef FN
///   #undef TY

#include "dmc/tpl/DEFS.h"
#include "dmc/Map.h"

#define CT TPL_CAT(M, FN)
#define IT TPL_CAT(I, FN)
#define OP TPL_CAT(O, FN)
#define FUN(id) TPL_CAT_FUN(m, FN, id)

///
typedef struct FUN(CT) CT;
typedef struct TPL_CAT_FUN(i, FN, IT) IT;
typedef struct TPL_CAT_FUN(o, FN, OP) OP;
typedef struct ikv_Ikv Ikv;
typedef struct ichar_Ichar Ichar;

/// Initializates a map. Map can be cast to Arr.
CT *FUN(new)(void);

///
size_t FUN(size)(CT *this);

/// FUN(put puts 'value' with key 'key'. If key already exists its value
/// is changed.
///   this  : The map
///   key   : Entry key. It must not be NULL.
///   value : New value. It must not be NULL.
void FUN(put)(CT *this, char *key, TY *value);

/// FUN(get returns the value pointed by key or opt_null() if 'key' does
/// not exist
OP *FUN(get)(CT *this, char *key);

/// FUN(opget returns the value pointer by key or option if 'key' does not
/// exist
TY *FUN(oget)(CT *this, char *key, TY *option);

/// FUN(remove removes value with key 'key' or does nothing if 'key' does
/// not exist
void FUN(remove)(CT *this, char *key);

/// FUN(has_key returns 'true' if 'this' contains key 'key'.
bool FUN(has_key)(CT *this, char *key);

/// FUN(keys returns keys of this
Ichar *FUN(keys)(CT *this);

/// FUN(values returns values of this
IT *FUN(values)(CT *this);

/// FUN(to_it returns an iterator of Kv's
Ikv *FUN(to_it)(CT *this);

/// FUN(to_it_sort returns an iterator of Kv's sorted by key
Ikv *FUN(to_it_sort)(CT *this);

/// mapt_to_it_sort_locale returns an iterator of Kv's sorted by key in locale
Ikv *FUN(to_it_sort_locale)(CT *this);

///
Ajson *FUN(to_json)(CT *this, Ajson *(*to)(TY *));

///
CT *FUN(from_json)(Ajson *js, TY *(*from)(Ajson *));

#undef CT
#undef IT
#undef OP
#undef FUN
