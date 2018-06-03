// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array of char *

#ifndef DM_ACHAR_H
  #define DM_ACHAR_H

#define TY char
#define FN char
#include "dmc/tpl/tarr.h"
#undef TY
#undef FN

/// Ascending natural sort
void achar_sort (Achar *this);

/// Ascending local sort
void achar_sort_locale (Achar *this);


#endif