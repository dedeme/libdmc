// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <string.h>
#include "dmc/ct/Ichar.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

#define TY char
#define FN char
#include "dmc/tpl/tit.c"
#undef TY
#undef FN

inline
bool ichar_eq (Ichar *i1, Ichar *i2) {
  return ichar_eqf(i1, i2, str_eq);
}


int ichar_index(Ichar *this, char *s) {
  XNULL(s)
  /**/bool eq(char *e) { return str_eq(e, s); }
  return ichar_indexf(this, eq);
}

bool ichar_contains(Ichar *this, char *s) {
  XNULL(s)
  /**/bool eq(char *e) { return str_eq(e, s); }
  return ichar_containsf(this, eq);
}

int ichar_last_index(Ichar *this, char *s) {
  XNULL(s)
  /**/bool eq(char *e) { return str_eq(e, s); }
  return ichar_last_indexf(this, eq);
}
