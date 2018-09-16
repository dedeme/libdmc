// Copyright 16-Sep-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/ct/Kchar.h"
#include "dmc/Json.h"

#define TY char
#define FN char
#include "dmc/tpl/tkv.c"
#undef TY
#undef FN

Json *kchar_to_json(Kchar *this) {
  return kchar_to_jsonf(this, json_wstring);
}

Kchar *kchar_from_json(Json *js) {
  return kchar_from_jsonf(js, json_rstring);
}
