// Copyright 18-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Dec.h"
#include <math.h>
#include "dmc/std.h"

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
  REPEAT(scale)
    mul *= 10;
  _REPEAT
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
  if (*r == '-' && dec_eq(n, 0.0)) {
    r = str_right(r, 1);
  }
  return r;
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

Js *dec_to_js(Dec *this) {
  Arr *a = arr_new();
  arr_push(a, js_wd(this->n));
  arr_push(a, js_wi(this->scale));
  Js *r = js_wa(a);
  return r;
}

Dec *dec_from_js(Js *js) {
  Dec *this = MALLOC(Dec);
  Arr *a = js_ra(js);
  this->n = js_rd(arr_get(a, 0));
  this->scale = js_ri(arr_get(a, 1));
  return this;
}

