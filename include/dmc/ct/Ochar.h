// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Opt of char *

#ifndef DM_CT_OCHAR_H
  #define DM_CT_OCHAR_H

#define TY char
#define FN char
#include "dmc/tpl/topt.h"
#undef TY
#undef FN

///
Json *ochar_to_json(Ochar *this);

///
Ochar *ochar_from_json(Json *js);

#endif
