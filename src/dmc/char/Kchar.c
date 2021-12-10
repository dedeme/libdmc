// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/char/Kchar.h"
#include "dmc/DEFS.h"
#include "dmc/Kv.h"

Kchar *kchar_new (char *k, char *v) {
  return (Kchar *)kv_new(k, v);
}

char *kchar_to_js (Kchar *this, char *(*to)(char *e)) {
  return kv_to_js((Kv *)this, (char *(*)(void *))to);
}

Kchar *kchar_from_js (char *js, char *(*from)(char *jse)) {
  return (Kchar *)kv_from_js(js, (void *(*)(char *))from);
}

//--// Not remove

