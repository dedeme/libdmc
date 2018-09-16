// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <string.h>
#include "dmc/ct/Lchar.h"
#include "dmc/Json.h"

#define TY char
#define FN char
#include "dmc/tpl/tlist.c"
#undef TY
#undef fn

Json *lchar_to_json(Lchar *this) {
  return lchar_to_jsonf(this, json_wstring);
}

Lchar *lchar_from_json(Json *js) {
  return lchar_from_jsonf(js, json_rstring);
}
