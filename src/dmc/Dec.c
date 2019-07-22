// Copyright 21-Jul-2019 ºDeme
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

Dec *dec_new(Gc *gc, double n, int scale) {
  Dec *this = gc_add(gc, malloc(sizeof(Dec)));
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

char *dec_to_str(Gc *gc, Dec *this) {
  Gc *gcl = gc_new();
  double n = this->n;
  char *scale = str_f(gcl, "%d", this->scale);
  char *r = str_f(gcl, str_cat(gcl, "%.", scale, "f", NULL), n);
  if (*r == '-' && dec_eq(n, 0.0)) {
    r = str_right(gcl, r, 1);
  }
  r = str_new(gc, r);
  gc_free(gcl);
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

char *dec_regularize_iso(Gc *gc, char *s) {
  Gc *gcl = gc_new();
  char *r = str_creplace(gc, str_replace(gcl, s, ".", ""), ',', '.');
  gc_free(gcl);
  return r;
}

char *dec_regularize_us(Gc *gc, char *s) {
  return str_replace(gc, s, ",", "");
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

Js *dec_to_js(Gc *gc, Dec *this) {
  Gc *gcl = gc_new();
  Arr *a = arr_new(gcl);
  arr_push(a, js_wd(gcl, this->n));
  arr_push(a, js_wi(gcl, this->scale));
  Js *r = js_wa(gc, a);
  gc_free(gcl);
  return r;
}

Dec *dec_from_js(Gc *gc, Js *js) {
  Gc *gcl = gc_new();
  Dec *this = gc_add(gc, malloc(sizeof(Dec)));
  Arr *a = js_ra(gcl, js);
  this->n = js_rd(arr_get(a, 0));
  this->scale = js_ri(arr_get(a, 1));
  gc_free(gcl);
  return this;
}

