// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <string.h>
#include "dmc/ct/Achar.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

#define TY char
#define FN char
#include "dmc/tpl/tarr.c"
#undef TY
#undef FN

void achar_sort (Achar *this) {
  XNULL(this)

  bool cmp(char *e1, char *e2) { return str_cmp(e1, e2) > 0; }
  return achar_sortf(this, cmp);
}

void achar_sort_locale (Achar *this) {
  XNULL(this)

  bool cmp(char *e1, char *e2) { return strcoll(e1, e2) > 0; }
  return achar_sortf(this, cmp);
}
