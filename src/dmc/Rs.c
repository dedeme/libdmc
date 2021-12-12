// Copyright 21-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Rs.h"
#include "dmc/err.h"
#include "dmc/js.h"

struct rs_Rs {
  char *err;
  void *value;
};

Rs *rs_mk_error(char *error) {
  Rs *this = MALLOC(Rs);
  this->err = error;
  this->value = NULL;
  return this;
}

Rs *rs_mk_ok(void *value) {
  Rs *this = MALLOC(Rs);
  this->value = value;
  return this;
}

char *rs_error(Rs *rs) {
  if (rs->value) return "";
  return rs->err;
}

void *rs_ok(Rs *rs) {
  if (rs->value) return rs->value;
  return FAIL("Result is error");
}

char *rs_to_js (Rs *this, char *(*to)(void *e)) {
  return js_wa(achar_new_from(
    js_wb(this->value ? 1 : 0),
    this->value ? to(this->value) : js_ws(this->err),
    NULL
  ));
}

Rs *rs_from_js (char *js, void *(*from)(char *ejs)) {
  Achar *a = js_ra(js);
  if (js_rb(achar_get(a, 0))) {
    return rs_mk_ok(from(achar_get(a, 1)));
  }
  return rs_mk_error(js_rs(achar_get(a, 1)));
}
