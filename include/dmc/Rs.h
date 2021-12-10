// Copyright 21-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Result data.

#ifndef DMC_RS_H
  #define DMC_RS_H

/// Rs structure
typedef struct rs_Rs Rs;

/// Returns a result of error.
Rs *rs_mk_error(char *error);

/// Returns a valid result.
Rs *rs_mk_ok(void *value);

/// Returns an error message if 'rs' is error or an empty string if 'rs' is
/// valid.
char *rs_error(Rs *rs);

/// Returns the value of 'rs' or raise a FAIL if it is an error.
void *rs_ok(Rs *rs);

/// Returns this JSONized.
///   this: Container.
///   to  : Converter of container element to JSON.
char *rs_to_js (Rs *this, char *(*to)(void *e));

/// Returns the container from its JSON representation.
///   js  : Container JSONized.
///   from: Converter from JSON to container element.
Rs *rs_from_js (char *js, void *(*from)(char *ejs));

#endif
