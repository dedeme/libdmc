// Copyright 22-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef DMC_OPT_H
  #define DMC_OPT_H

typedef struct opt_Opt Opt;

///
Opt *opt_new (void *value);

///
Opt *opt_empty (void);

///
int opt_is_empty (Opt *this);

///
int opt_is_full (Opt *this);

/// Throws a illegal_state_exception_t if 'this' is empty
void *opt_get (Opt *this);

/// Throws a illegal_state_exception_t if 'this' is empty with 'msg' as message.
void *opt_eget (Opt *this, char *msg);

#endif
