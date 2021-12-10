// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Opt[ATpIntInt*].

#ifndef DMC_TPINTINT_OATPINTINT_H
  #define DMC_TPINTINT_OATPINTINT_H

#include "dmc/TpIntInt/ATpIntInt.h"

/// Opt[ATpIntInt*].
typedef struct oATpIntInt_OATpIntInt OATpIntInt;

/// Returns a none option.
OATpIntInt *oATpIntInt_mk_none();

/// Returns an option with a value.
OATpIntInt *oATpIntInt_mk_some(ATpIntInt *value);

/// Returns '1' if 'opt' is none and 0 otherwise.
int oATpIntInt_none(OATpIntInt *opt);

/// Returns the value of 'opt' or raise a FAIL if it is none.
ATpIntInt *oATpIntInt_some(OATpIntInt *opt);

/// Raise a fail if 'opt' is empty with 'msg' as message.
ATpIntInt *oATpIntInt_esome (OATpIntInt *opt, char *msg);

/// Returns 'value' if 'opt' is empty.
ATpIntInt *oATpIntInt_osome (OATpIntInt *opt, ATpIntInt *value);

/// Returns the value of 'opt' or NULL if 'this' is empty.
ATpIntInt *oATpIntInt_nsome (OATpIntInt *opt);

/// Returns this JSONized.
///   this: Container.
///   to  : Converter of container element to JSON.
char *oATpIntInt_to_js (OATpIntInt *this, char *(*to)(ATpIntInt *e));

/// Returns the container from its JSON representation.
///   js  : Container JSONized.
///   from: Converter from JSON to container element.
OATpIntInt *oATpIntInt_from_js (char *js, ATpIntInt *(*from)(char *ejs));


//--// Not remove

#endif