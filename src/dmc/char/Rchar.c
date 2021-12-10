// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/char/Rchar.h"
#include "dmc/DEFS.h"
#include "dmc/err.h"
#include "dmc/Rs.h"

struct rchar_Rchar {
  char *err;
  Rchar *value;
};

Rchar *rchar_mk_error(char *error) {
  return (Rchar *)rs_mk_error(error);
}

Rchar *rchar_mk_ok(void *value) {
  return (Rchar *)rs_mk_ok(value);
}

char *rchar_error(Rchar *this) {
  return rs_error((Rs *)this);
}

void *rchar_ok(Rchar *this) {
  return rs_ok((Rs *)this);
}

char *rchar_to_js (Rchar *this, char *(*to)(char *e)) {
  return rs_to_js((Rs *)this, (char *(*)(void *))to);
}

Rchar *rchar_from_js (char *js, char *(*from)(char *jse)) {
  return (Rchar *)rs_from_js(js, (void *(*)(char *))from);
}

//--// Not remove

