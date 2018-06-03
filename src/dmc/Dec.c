// Copyright 02-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <math.h>
#include <gc.h>
#include "dmc/Dec.h"
#include "dmc/exc.h"
#include "dmc/str.h"
#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"
#include "dmc/DEFS.h"


struct dec_Dec {
  double n;      // R
  int scale;  // R
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

Dec *dec_new (double n, int scale) {
  Dec *this = MALLOC(Dec);
  this->n = n >= 0 ? n + _dec_round(n) : n - _dec_round(n);
  this->scale = scale < 0 ? 0 : scale > 10 ? 10 : scale;

  return this;
}

char *dec_to_str (Dec *this) {
  XNULL(this)

  double n = this->n;
  char *r = str_printf(
    str_cat("%.", str_printf("%d", this->scale), "f",  NULL), n);
  return *r == '-' && dec_eq(n, 0.0) ? str_sub(r, 1, str_len(r)) : r;
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
  XNULL(s)

  char ch;
  ch = *s++;
  while (ch) {
    if (ch < '0' || ch > '9')
      return 0;
    ch = *s++;
  }
  return 1;
}

char *dec_regularize_iso (char *s) {
  XNULL(s)
  return str_creplace(str_replace(s, ".", ""), ',', '.');
}

char *dec_regularize_us (char *s) {
  XNULL(s)
  return str_replace(s, ",", "");
}

bool dec_number (char *s) {
  XNULL(s)

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

Ajson *dec_to_json(Dec *this) {
  XNULL(this)

  Ajson *r = ajson_new();
  ajson_add(r, json_wdouble(this->n, this->scale));
  ajson_add(r, json_wint(this->scale));
  return r;
}

Dec *dec_from_json(Ajson *js) {
  XNULL(js)
  size_t size = ajson_size(js);
  if (size != 2)
    THROW(exc_range_t) exc_range(2, 3, size) _THROW

  return dec_new(json_rdouble(ajson_get(js, 0)), json_rint(ajson_get(js, 1)));
}
