// Copyright 22-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Option

#ifndef DMC_OPT_H
  #define DMC_OPT_H

typedef struct opt_Opt Opt;

/// Creates a full option
Opt *opt_new (void *value);

/// Creates an empty option
Opt *opt_empty (void);

/// Returns 1 if option has no value.
int opt_is_empty (Opt *this);

/// Returns 1 if option has value.
int opt_is_full (Opt *this);

/// Throws a illegal_state_exception_t if 'this' is empty
void *opt_get (Opt *this);

/// Throws a illegal_state_exception_t if 'this' is empty with 'msg' as message.
void *opt_eget (Opt *this, char *msg);

#endif
