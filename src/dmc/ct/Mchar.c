// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/ct/Mchar.h"
#include "dmc/Map.h"
#include "dmc/Json.h"

#define TY char
#define FN char
#include "dmc/tpl/tmap.c"
#undef TY
#undef FN

Json *mchar_to_json(Mchar *this) {
  return mchar_to_jsonf(this, json_wstring);
}

Mchar *mchar_from_json(Json *js) {
  return mchar_from_jsonf(js, json_rstring);
}
