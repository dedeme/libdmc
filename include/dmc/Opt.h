// Copyright 23-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Option data.

#ifndef DMC_OPT_H
  #define DMC_OPT_H

/// Option structure
typedef struct opt_Opt Opt;

/// Returns a none option.
Opt *opt_mk_none();

/// Returns an option with a value.
Opt *opt_mk_some(void *value);

/// Returns '1' if 'opt' is none and 0 otherwise.
int opt_none(Opt *opt);

/// Returns the value of 'opt' or raise a FAIL if it is none.
void *opt_some(Opt *opt);

/// Raise a fail if 'opt' is empty with 'msg' as message.
void *opt_esome (Opt *opt, char *msg);

/// Returns 'value' if 'opt' is empty.
void *opt_osome (Opt *opt, void *value);

/// Returns the value of 'opt' or NULL if 'this' is empty.
void *opt_nsome (Opt *opt);

/// Returns this JSONized.
///   this: Container.
///   to  : Converter of container element to JSON.
char *opt_to_js (Opt *this, char *(*to)(void *e));

/// Returns the container from its JSON representation.
///   js  : Container JSONized.
///   from: Converter from JSON to container element.
Opt *opt_from_js (char *js, void *(*from)(char *ejs));

#endif
