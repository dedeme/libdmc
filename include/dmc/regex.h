// Copyright 13-Dec-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Regular expressions management.

#ifndef DMC_REGEX_H
  #define DMC_REGEX_H

#include "dmc/std.h"

/// Returns the offsets where ereg is found in s.
///   rex   : Regular expression to find. It does not admit grouping (that is,
///           parenthesis).
///   s     : String to search in.
///   return: Opt[Arr[Itp]]. Offsets of 's' with elements which match. Each
///           'Itp' contains [ofsset start inclusive - ofsect - end exclusive].
///           If any error happens, it returns opt_empty.
Opt *regex_matches (char *rex, char *s);

/// Equals to 'regex_matches' but "ignoring case".
Opt *regex_matches_ic (char *rex, char *s);

/// Replace ocurrences of 'rex' in 's' by 'replacement'. Ocurrences are
/// find by regex_matches.
///   rex        : Regular expression to replace. It does not admit grouping (that
///                is, parenthesis).
///   s          : String to search in.
///   replacement: New string.
///   return     : Opt[char]. A new string with repacements done.
///                If any error happens, it returns opt_empty.
Opt *regex_replace (char *rex, char *s, char *replacement);

/// Equals to regex_replace, but "ignoring case".
Opt *regex_replace_ic (char *rex, char *s, char *replacement);

#endif
