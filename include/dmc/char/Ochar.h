// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Opt[char*].

#ifndef DMC_CHAR_OCHAR_H
  #define DMC_CHAR_OCHAR_H

/// Opt[char*].
typedef struct ochar_Ochar Ochar;

/// Returns a none option.
Ochar *ochar_mk_none();

/// Returns an option with a value.
Ochar *ochar_mk_some(char *value);

/// Returns '1' if 'opt' is none and 0 otherwise.
int ochar_none(Ochar *opt);

/// Returns the value of 'opt' or raise a FAIL if it is none.
char *ochar_some(Ochar *opt);

/// Raise a fail if 'opt' is empty with 'msg' as message.
char *ochar_esome (Ochar *opt, char *msg);

/// Returns 'value' if 'opt' is empty.
char *ochar_osome (Ochar *opt, char *value);

/// Returns the value of 'opt' or NULL if 'this' is empty.
char *ochar_nsome (Ochar *opt);

/// Returns this JSONized.
///   this: Container.
///   to  : Converter of container element to JSON.
char *ochar_to_js (Ochar *this, char *(*to)(char *e));

/// Returns the container from its JSON representation.
///   js  : Container JSONized.
///   from: Converter from JSON to container element.
Ochar *ochar_from_js (char *js, char *(*from)(char *ejs));


//--// Not remove

#endif