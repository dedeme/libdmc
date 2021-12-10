// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Opt[OTpIntInt*].

#ifndef DMC_TPINTINT_OOTPINTINT_H
  #define DMC_TPINTINT_OOTPINTINT_H

#include "dmc/TpIntInt/OTpIntInt.h"

/// Opt[OTpIntInt*].
typedef struct oOTpIntInt_OOTpIntInt OOTpIntInt;

/// Returns a none option.
OOTpIntInt *oOTpIntInt_mk_none();

/// Returns an option with a value.
OOTpIntInt *oOTpIntInt_mk_some(OTpIntInt *value);

/// Returns '1' if 'opt' is none and 0 otherwise.
int oOTpIntInt_none(OOTpIntInt *opt);

/// Returns the value of 'opt' or raise a FAIL if it is none.
OTpIntInt *oOTpIntInt_some(OOTpIntInt *opt);

/// Raise a fail if 'opt' is empty with 'msg' as message.
OTpIntInt *oOTpIntInt_esome (OOTpIntInt *opt, char *msg);

/// Returns 'value' if 'opt' is empty.
OTpIntInt *oOTpIntInt_osome (OOTpIntInt *opt, OTpIntInt *value);

/// Returns the value of 'opt' or NULL if 'this' is empty.
OTpIntInt *oOTpIntInt_nsome (OOTpIntInt *opt);

/// Returns this JSONized.
///   this: Container.
///   to  : Converter of container element to JSON.
char *oOTpIntInt_to_js (OOTpIntInt *this, char *(*to)(OTpIntInt *e));

/// Returns the container from its JSON representation.
///   js  : Container JSONized.
///   from: Converter from JSON to container element.
OOTpIntInt *oOTpIntInt_from_js (char *js, OTpIntInt *(*from)(char *ejs));


//--// Not remove

#endif