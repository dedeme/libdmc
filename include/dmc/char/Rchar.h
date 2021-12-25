// Copyright 25-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Rs[char *].

#ifndef DMC_CHAR_RCHAR_H
  #define DMC_CHAR_RCHAR_H

//// Rs[char *].
typedef struct rchar_Rchar Rchar;;

/// Returns a result of error.
Rchar *rchar_mk_error(char *error);

/// Returns a valid result.
Rchar *rchar_mk_ok(void *value);

/// Returns an error message if 'this' is error or an empty string if it is
/// valid.
char *rchar_error(Rchar *this);

/// Returns the value of 'this' or raise a FAIL if it is an error.
void *rchar_ok(Rchar *this);

/// Returns this JSONized.
///   this: Container.
char *rchar_to_js (Rchar *this);

/// Returns the container from its JSON representation.
///   js  : Container JSONized.
Rchar *rchar_from_js (char *js);


//--// Not remove

#endif