// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Opt[TpIntInt*].

#ifndef DMC_TPINTINT_OTPINTINT_H
  #define DMC_TPINTINT_OTPINTINT_H

#include "dmc/TpIntInt.h"

/// Opt[TpIntInt*].
typedef struct oTpIntInt_OTpIntInt OTpIntInt;

/// Returns a none option.
OTpIntInt *oTpIntInt_mk_none();

/// Returns an option with a value.
OTpIntInt *oTpIntInt_mk_some(TpIntInt *value);

/// Returns '1' if 'opt' is none and 0 otherwise.
int oTpIntInt_none(OTpIntInt *opt);

/// Returns the value of 'opt' or raise a FAIL if it is none.
TpIntInt *oTpIntInt_some(OTpIntInt *opt);

/// Raise a fail if 'opt' is empty with 'msg' as message.
TpIntInt *oTpIntInt_esome (OTpIntInt *opt, char *msg);

/// Returns 'value' if 'opt' is empty.
TpIntInt *oTpIntInt_osome (OTpIntInt *opt, TpIntInt *value);

/// Returns the value of 'opt' or NULL if 'this' is empty.
TpIntInt *oTpIntInt_nsome (OTpIntInt *opt);

/// Returns this JSONized.
///   this: Container.
///   to  : Converter of container element to JSON.
char *oTpIntInt_to_js (OTpIntInt *this, char *(*to)(TpIntInt *e));

/// Returns the container from its JSON representation.
///   js  : Container JSONized.
///   from: Converter from JSON to container element.
OTpIntInt *oTpIntInt_from_js (char *js, TpIntInt *(*from)(char *ejs));


//--// Not remove

#endif