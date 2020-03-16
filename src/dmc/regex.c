// Copyright 13-Dec-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/regex.h"
#include <regex.h>
#include "dmc/Itp.h"

// Returns Arr[Itp]
static Arr *matches (regex_t *rex, char *s) {
  // Arr[Itp]
  Arr *r = arr_new();

  regmatch_t match;

  int ix = 0;
  for (;;) {
    char *sub = str_right(s, ix);
    Itp *offset = NULL;
    int rs = regexec(rex, sub, 1, &match, 0);
    if (rs == REG_ESPACE) {
      EXC_GENERIC("Regular expression out of memory");
    }
    if (!rs) {
      int end = ix + match.rm_eo;
      offset = itp_new(ix + match.rm_so, end);
      ix = end;
    }

    if (offset) arr_push(r, offset);
    else break;
  }

  regfree(rex);
  return r;
}

Opt *regex_matches (char *rex, char *s) {
  if (!*rex)
    EXC_ILLEGAL_ARGUMENT("rex", "A non empty string", "An empty string")

  regex_t exp;
  if (regcomp(&exp, rex, 0)) return opt_empty();
  return opt_new(matches(&exp, s));
}

Opt *regex_matches_ic (char *rex, char *s) {
  if (!*rex)
    EXC_ILLEGAL_ARGUMENT("rex", "A non empty string", "An empty string")

  regex_t exp;
  if (regcomp(&exp, rex, REG_ICASE)) return opt_empty();
  return opt_new(matches(&exp, s));
}

// matches is Arr[Itp]
static char *replace (Arr *matches, char *s, char *replacement) {
  Buf *bf = buf_new();
  int ix = 0;
  EACH(matches, Itp, off) {
    buf_add(bf, str_sub(s, ix, itp_e1(off)));
    buf_add(bf, replacement);
    ix = itp_e2(off);
  }_EACH
  buf_add(bf, str_right(s, ix));
  return buf_to_str(bf);
}

Opt *regex_replace (char *rex, char *s, char *replacement) {
  // Arr[Itp]
  Arr *matches = opt_nget(regex_matches(rex, s));
  if (matches) return opt_new(replace(matches, s, replacement));
  return opt_empty();
}

Opt *regex_replace_ic (char *rex, char *s, char *replacement) {
  // Arr[Itp]
  Arr *matches = opt_nget(regex_matches_ic(rex, s));
  if (matches) return opt_new(replace(matches, s, replacement));
  return opt_empty();
}
