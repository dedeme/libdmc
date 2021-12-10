// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Opt[Kchar*].

#ifndef DMC_CHAR_OKCHAR_H
  #define DMC_CHAR_OKCHAR_H

#include "dmc/char/Kchar.h"

/// Opt[Kchar*].
typedef struct oKchar_OKchar OKchar;

/// Returns a none option.
OKchar *oKchar_mk_none();

/// Returns an option with a value.
OKchar *oKchar_mk_some(Kchar *value);

/// Returns '1' if 'opt' is none and 0 otherwise.
int oKchar_none(OKchar *opt);

/// Returns the value of 'opt' or raise a FAIL if it is none.
Kchar *oKchar_some(OKchar *opt);

/// Raise a fail if 'opt' is empty with 'msg' as message.
Kchar *oKchar_esome (OKchar *opt, char *msg);

/// Returns 'value' if 'opt' is empty.
Kchar *oKchar_osome (OKchar *opt, Kchar *value);

/// Returns the value of 'opt' or NULL if 'this' is empty.
Kchar *oKchar_nsome (OKchar *opt);

/// Returns this JSONized.
///   this: Container.
///   to  : Converter of container element to JSON.
char *oKchar_to_js (OKchar *this, char *(*to)(Kchar *e));

/// Returns the container from its JSON representation.
///   js  : Container JSONized.
///   from: Converter from JSON to container element.
OKchar *oKchar_from_js (char *js, Kchar *(*from)(char *ejs));


//--// Not remove

#endif