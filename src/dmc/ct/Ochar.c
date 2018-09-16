// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/ct/Ochar.h"
#include "dmc/Json.h"

#define TY char
#define FN char
#include "dmc/tpl/topt.c"
#undef TY
#undef FN

Json *ochar_to_json(Ochar *this) {
  return ochar_to_jsonf(this, json_wstring);
}

Ochar *ochar_from_json(Json *js) {
  return ochar_from_jsonf(js, json_rstring);
}
