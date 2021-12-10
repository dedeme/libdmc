// Copyright 21-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Option[double] data.

#ifndef DMC_ODOUBLE_H
  #define DMC_ODOUBLE_H

/// Option structure
typedef struct oDouble_ODouble ODouble;

/// Returns a none option.
ODouble *oDouble_mk_none();

/// Returns an option with a value.
ODouble *oDouble_mk_some(double value);

/// Returns '1' if 'opt' is none and 0 otherwise.
int oDouble_none(ODouble *opt);

/// Returns the value of 'opt' or raise a FAIL if it is none.
double oDouble_some(ODouble *opt);

#endif
