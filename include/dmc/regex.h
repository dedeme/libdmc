// Copyright 05-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Regular expressions management.

#ifndef DMC_REGEX_H
  #define DMC_REGEX_H

#include "dmc/TpIntInt/OATpIntInt.h"
#include "dmc/char/Ochar.h"

/// Returns the offsets where 'rex' is found in s.
///   rex   : Regular expression to find. It does not admit grouping (that is,
///           parenthesis).
///   s     : String to search in.
///   return: Offsets of 's' with elements which match. Each
///           'TpIntInt' contains [ofsset start inclusive - ofset end exclusive].
///           If any error happens, it returns oATIntInt_none.
OATpIntInt *regex_matches (char *rex, char *s);

/// Equals to 'regex_matches' but "ignoring case".
OATpIntInt *regex_matches_ic (char *rex, char *s);

/// Replace ocurrences of 'rex' in 's' by 'replacement'. Ocurrences are
/// searched by regex_matches.
///   rex        : Regular expression to replace. It does not admit grouping
///                (that is, parenthesis).
///   s          : String to search in.
///   replacement: New string.
///   return     : A new string with repacements done.
///                If any error happens, it returns ochar_none.
Ochar *regex_replace (char *rex, char *s, char *replacement);

/// Equals to regex_replace, but "ignoring case".
Ochar *regex_replace_ic (char *rex, char *s, char *replacement);

#endif
