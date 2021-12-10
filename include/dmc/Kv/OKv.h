// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Opt[Kv*].

#ifndef DMC_KV_OKV_H
  #define DMC_KV_OKV_H

#include "dmc/Kv.h"

/// Opt[Kv*].
typedef struct oKv_OKv OKv;

/// Returns a none option.
OKv *oKv_mk_none();

/// Returns an option with a value.
OKv *oKv_mk_some(Kv *value);

/// Returns '1' if 'opt' is none and 0 otherwise.
int oKv_none(OKv *opt);

/// Returns the value of 'opt' or raise a FAIL if it is none.
Kv *oKv_some(OKv *opt);

/// Raise a fail if 'opt' is empty with 'msg' as message.
Kv *oKv_esome (OKv *opt, char *msg);

/// Returns 'value' if 'opt' is empty.
Kv *oKv_osome (OKv *opt, Kv *value);

/// Returns the value of 'opt' or NULL if 'this' is empty.
Kv *oKv_nsome (OKv *opt);

/// Returns this JSONized.
///   this: Container.
///   to  : Converter of container element to JSON.
char *oKv_to_js (OKv *this, char *(*to)(Kv *e));

/// Returns the container from its JSON representation.
///   js  : Container JSONized.
///   from: Converter from JSON to container element.
OKv *oKv_from_js (char *js, Kv *(*from)(char *ejs));


//--// Not remove

#endif