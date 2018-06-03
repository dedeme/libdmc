// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Iterator of char *

#ifndef DM_CT_ICHAR_H
  #define DM_CT_ICHAR_H

#include <stdbool.h>

#define TY char
#define FN char
#include "dmc/tpl/tit.h"
#undef TY
#undef FN

/// ichar_normal_eq returns true using str_eq()
bool ichar_eq(Ichar *i1, Ichar *i2);

///
int ichar_index(Ichar *this, char *s);

///
bool ichar_contains(Ichar *this, char *s);

///
int ichar_last_index(Ichar *this, char *s);

#endif
