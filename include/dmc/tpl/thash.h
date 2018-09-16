// Copyright 04-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Header Hash template.<p>
/// Use:
///   #define TY char                    // Element type
///   #define FN char                    // Function prefix
///   #include "dmc/tpl/tmap.h"
///   #undef FN
///   #undef TY

#include "dmc/tpl/DEFS.h"
#include "dmc/Hash.h"

#define CT TPL_CAT(H, FN)
#define IT TPL_CAT(I, FN)
#define OP TPL_CAT(O, FN)
#define FUN(id) TPL_CAT_FUN(h, FN, id)

///
typedef struct FUN(CT) CT;
typedef struct TPL_CAT_FUN(i, FN, IT) IT;
typedef struct TPL_CAT_FUN(o, FN, OP) OP;
typedef struct ikv_Ikv Ikv;
typedef struct ichar_Ichar Ichar;

/// FUN(new) nitializates a Hash. Hash is an immutable struct which can be
/// cast to List.
CT *FUN(new)(void);

/// hash_count returns elements number of 'this' <u>including duplicates</u>.
size_t FUN(count)(CT *this);

/// FUN(put) puts 'value' with key 'key'. If key already exists its value
/// is changed.
///   this  : The map
///   key   : Entry key. It must not be NULL.
///   value : New value. It must not be NULL.
CT *FUN(put)(CT *this, char *key, TY *value);

/// FUN(get) returns the value pointed by key or opt_null() if 'key' does
/// not exist
OP *FUN(get)(CT *this, char *key);

/// FUN(opget) returns the value pointer by key or option if 'key' does not
/// exist
TY *FUN(oget)(CT *this, char *key, TY *option);

/// FUN(remove) removes value with key 'key' or does nothing if 'key' does
/// not exist
CT *FUN(remove)(CT *this, char *key);

/// FUN(compact) returns a new Hash without duplicated keys.
CT *FUN(compact)(CT *this);

/// FUN(has_key) returns 'true' if 'this' contains key 'key'.
bool FUN(has_key)(CT *this, char *key);

/// FUN(keys) returns keys of this
Ichar *FUN(keys)(CT *this);

/// FUN(values) returns values of this
IT *FUN(values)(CT *this);

/// FUN(to_it) returns an iterator of Kv's
Ikv *FUN(to_it)(CT *this);

/// FUN(to_it_sort) returns an iterator of Kv's sorted by key
Ikv *FUN(to_it_sort)(CT *this);

/// FUN(to_it_sort_locale) returns an iterator of Kv's sorted by key in locale
Ikv *FUN(to_it_sort_locale)(CT *this);

///
CT *FUN(from_it)(Ikv *it);

///
Json *FUN(to_jsonf)(CT *this, Json *(*to)(TY *));

///
CT *FUN(from_jsonf)(Json *js, TY *(*from)(Json *));

#undef CT
#undef IT
#undef OP
#undef FUN
