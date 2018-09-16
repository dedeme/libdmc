// Copyright 04-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Hash of char *

#ifndef DM_CT_HCHAR_H
  #define DM_CT_HCHAR_H

#define TY char
#define FN char
#include "dmc/tpl/thash.h"
#undef TY
#undef FN

///
Json *hchar_to_json(Hchar *this);

///
Hchar *hchar_from_json(Json *js);

#endif
