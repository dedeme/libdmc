// Copyright 18-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Dec.h"
#include <math.h>
#include <locale.h>
#include "dmc/DEFS.h"
#include "dmc/str.h"
#include "dmc/Buf.h"
#include "dmc/js.h"

struct dec_Dec {
  double n;
  int scale;
  int mul;
};

double _dec_round(double dbl) {
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

Dec *dec_new(double n, int scale) {
  Dec *this = MALLOC(Dec);
  this->n = n >= 0 ? n + _dec_round(n) : n - _dec_round(n);
  this->scale = scale < 0 ? 0 : scale > 10 ? 10 : scale;
  int mul = 1;
  for (int i = 0; i < scale; ++i) {
    mul *= 10;
  }
  this->mul = mul;

  return this;
}

double dec_n(Dec *this) {
  return round(this->n * this->mul) / this->mul;
}

int dec_scale(Dec *this) {
  return this->scale;
}

char *dec_to_str(Dec *this) {
  double n = this->n;
  char *scale = str_f("%d", this->scale);
  char *r = str_f(str_cat("%.", scale, "f", NULL), n);
  struct lconv *lc = localeconv();
  r = str_replace(r, lc->decimal_point, ".");
  if (*r == '-' && dec_eq(n, 0.0)) {
    r = str_right(r, 1);
  }
  return r;
}

static char *int_to_str(char *s, char sep) {
  int sign = *s == '-' ? 1 : 0;
  s = str_reverse(s);

  Buf *bf = buf_new();

  int c = 0;
  char *send = s + strlen(s) - sign;
  while (s < send) {
    if (c == 3) {
      buf_cadd(bf, sep);
      c = 0;
    }
    buf_cadd(bf, *s++);
    ++c;
  }

  if (sign) buf_cadd(bf, '-');
  return str_reverse(buf_to_str(bf));
}

char *dec_int_to_iso(int n) {
  return int_to_str(str_f("%d", n), '.');
}

char *dec_int_to_us(int n) {
  return int_to_str(str_f("%d", n), ',');
}

static char *double_to_str(double n, int scale, char sep, char point) {
  if (scale < 1) scale = 0;
  if (n == -0) n = 0;
  char *r = str_f(str_cat("%.", str_f("%d", scale), "f", NULL), n);

  struct lconv *lc = localeconv();
  char *lcp = lc->decimal_point;
  if (scale) {
    int ix = str_index(r, lcp);
    return str_cat(
      int_to_str(str_left(r, ix), sep),
      str_c(point), str_right(r, ix + strlen(lcp)), NULL
    );
  } else {
    return int_to_str(r, sep);
  }
}

char *dec_double_to_iso(double n, int scale) {
  return double_to_str(n, scale, '.', ',');
}

char *dec_double_to_us(double n, int scale) {
  return double_to_str(n, scale, ',', '.');
}

int dec_eq(double d1, double d2) {
  double df1 = _dec_round(d1);
  double df2 = _dec_round(d2);
  double df = df1 > df2 ? df1 : df2;
  return d1 < d2 + df && d1 > d2 - df;
}

int dec_eq_gap(double d1, double d2, double gap) {
  return d1 < d2 + gap && d1 > d2 - gap;
}

int dec_eqf_gap(float d1, float d2, float gap) {
  return d1 < d2 + gap && d1 > d2 - gap;
}

int dec_digits(const char *s) {
  char ch;
  ch = *s++;
  while (ch) {
    if (ch < '0' || ch > '9')
      return 0;
    ch = *s++;
  }
  return 1;
}

char *dec_regularize_iso(char *s) {
  return str_creplace(str_replace(s, ".", ""), ',', '.');
}

char *dec_regularize_us(char *s) {
  return str_replace(s, ",", "");
}

int dec_number(char *s) {
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

char *dec_to_js(Dec *this) {
  Achar *a = achar_new();
  achar_push(a, js_wd(this->n));
  achar_push(a, js_wi(this->scale));
  char *r = js_wa(a);
  return r;
}

Dec *dec_from_js(char *js) {
  Dec *this = MALLOC(Dec);
  Achar *a = js_ra(js);
  this->n = js_rd(achar_get(a, 0));
  this->scale = js_ri(achar_get(a, 1));
  return this;
}

