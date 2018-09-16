// Copyright 16-Sep-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Kv of char *

#ifndef DM_CT_KCHAR_H
  #define DM_CT_KCHAR_H

#define TY char
#define FN char
#include "dmc/tpl/tkv.h"
#undef TY
#undef FN

///
Json *kchar_to_json(Kchar *this);

///
Kchar *kchar_from_json(Json *js);

#endif
