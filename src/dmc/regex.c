// Copyright 05-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/regex.h"
#include <regex.h>

// Returns Arr[Itp]
static ATpIntInt *matches (regex_t *rex, char *s) {
  ATpIntInt *r = aTpIntInt_new();

  regmatch_t match;

  int ix = 0;
  for (;;) {
    char *sub = str_right(s, ix);
    TpIntInt *offset = NULL;
    int rs = regexec(rex, sub, 1, &match, 0);
    if (rs == REG_ESPACE) {
      FAIL("Regular expression out of memory");
    }
    if (!rs) {
      int end = ix + match.rm_eo;
      offset = tpIntInt_new(ix + match.rm_so, end);
      ix = end;
    }

    if (offset) aTpIntInt_push(r, offset);
    else break;
  }

  regfree(rex);
  return r;
}

OATpIntInt *regex_matches (char *rex, char *s) {
  if (!*rex)
    FAIL("'rex' is an empty string");

  regex_t exp;
  if (regcomp(&exp, rex, 0)) return oATpIntInt_mk_none();
  return oATpIntInt_mk_some(matches(&exp, s));
}

OATpIntInt *regex_matches_ic (char *rex, char *s) {
  if (!*rex)
    FAIL("'rex' is an empty string");

  regex_t exp;
  if (regcomp(&exp, rex, REG_ICASE)) return oATpIntInt_mk_none();
  return oATpIntInt_mk_some(matches(&exp, s));
}

// matches is Arr[Itp]
static char *replace (ATpIntInt *matches, char *s, char *replacement) {
  Buf *bf = buf_new();
  int ix = 0;
  TpIntInt **p = matches->es;
  while (p < matches->end) {
    TpIntInt *start_end = *p++;
    buf_add(bf, str_sub(s, ix, start_end->e1));
    buf_add(bf, replacement);
    ix = start_end->e2;
  }
  buf_add(bf, str_right(s, ix));
  return buf_to_str(bf);
}

Ochar *regex_replace (char *rex, char *s, char *replacement) {
  ATpIntInt *matches = oATpIntInt_nsome(regex_matches(rex, s));
  if (matches) return ochar_mk_some(replace(matches, s, replacement));
  return ochar_mk_none();
/*
  ATpIntInt *matches = opt_nget(regex_matches(rex, s));
  if (matches) return opt_new(replace(matches, s, replacement));
  return opt_empty();*/
}

Ochar *regex_replace_ic (char *rex, char *s, char *replacement) {
  ATpIntInt *matches = oATpIntInt_nsome(regex_matches_ic(rex, s));
  if (matches) return ochar_mk_some(replace(matches, s, replacement));
  return ochar_mk_none();
}
