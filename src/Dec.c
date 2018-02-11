// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dm.h"
#include "math.h"

struct dec_Dec {
  double n;      // R
  size_t scale;  // R
};

double _dec_round (double dbl) {
  double d = fabs(dbl);
  return
    d > 1000000000 ? 0.000001
  : d > 100000000 ? 0.0000001
  : d > 10000000 ? 0.00000001
  : d > 1000000 ? 0.000000001
  : d > 100000 ? 0.0000000001
  : d > 10000 ? 0.00000000001
  : d > 1000 ? 0.000000000001
  : d > 100 ? 0.0000000000001
  : d > 10 ? 0.00000000000001
  :         0.000000000000001;
}

Dec *dec_new (double n, size_t scale) {
  Dec *this = MALLOC(Dec);
  this->n = n >= 0 ? n + _dec_round(n) : n - _dec_round(n);
  this->scale = scale;

  return this;
}

char *dec_to_str (Dec *this) {
  double n = this->n;
  char *r = str_printf(
    str_cat("%.", str_printf("%d", this->scale), "f",  NULL), n);
  return *r == '-' && dec_eq(n, 0.0) ? str_sub(r, 1, strlen(r)) : r;
}

bool dec_eq (double d1, double d2) {
  double df1 = _dec_round(d1);
  double df2 = _dec_round(d2);
  double df = df1 > df2 ? df1 : df2;
  return d1 < d2 + df && d1 > d2 - df;
}

inline
bool dec_eq_gap (double d1, double d2, double gap) {
  return d1 < d2 + gap && d1 > d2 - gap;
}

inline
bool dec_eqf_gap (float d1, float d2, float gap) {
  return d1 < d2 + gap && d1 > d2 - gap;
}

bool dec_digits (char *s) {
  char ch;
  ch = *s++;
  while (ch) {
    if (ch < '0' || ch > '9')
      return 0;
    ch = *s++;
  }
  return 1;
}

inline
char *dec_regularize_iso (char *s) {
  return str_creplace(str_replace(s, ".", ""), ',', '.');
}

inline
char *dec_regularize_us (char *s) {
  return str_replace(s, ",", "");
}

bool dec_number (char *s) {
  char ch;
  ch = *s++;
  if (ch == '-')
    ch = *s++;

  while (ch) {
    if (ch == '.') {
      ch = *s++;
      while (ch) {
        if (ch < '0' || ch > '9')
          return 0;
        ch = *s++;
      }
      return 1;
    }
    if (ch < '0' || ch > '9')
      return 0;
    ch = *s++;
  }
  return 1;
}
