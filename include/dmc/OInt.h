// Copyright 21-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Option[int] data.

#ifndef DMC_OINT_H
  #define DMC_OINT_H

/// Option structure
typedef struct oInt_OInt OInt;

/// Returns a none option.
OInt *oInt_mk_none();

/// Returns an option with a value.
OInt *oInt_mk_some(int value);

/// Returns '1' if 'opt' is none and 0 otherwise.
int oInt_none(OInt *opt);

/// Returns the value of 'opt' or raise a FAIL if it is none.
int oInt_some(OInt *opt);

#endif
