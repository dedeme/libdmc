// Copyright 04-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/ct/Hchar.h"
#include "dmc/Hash.h"
#include "dmc/Json.h"

#define TY char
#define FN char
#include "dmc/tpl/thash.c"
#undef TY
#undef FN

Json *hchar_to_json(Hchar *this) {
  return hchar_to_jsonf(this, json_wstring);
}

Hchar *hchar_from_json(Json *js) {
  return hchar_from_jsonf(js, json_rstring);
}
