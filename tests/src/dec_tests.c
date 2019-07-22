// Copyright 21-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dec_tests.h"
#include <assert.h>
#include <locale.h>
#include "dmc/Dec.h"

void dec_tests(void) {
  puts("Dec tests");

  int decEqstr(char *n, Dec *this) {
    Gc *gc = gc_new();
    char *r = dec_to_str(gc, this);
    char *lc = setlocale(LC_ALL, NULL);
    if (str_starts(lc, "es") || str_starts(lc, "ES")){
      r = dec_regularize_iso(gc, r);
    }
    int rt = str_eq(r, n);
    gc_free(gc);
    return rt;
  }

  int iso_eq(char *rs, char *dec) {
    Gc *gc = gc_new();
    int rt = str_eq(rs, dec_regularize_iso(gc, dec));
    gc_free(gc);
    return rt;
  }

  int us_eq(char *rs, char *dec) {
    Gc *gc = gc_new();
    int rt = str_eq(rs, dec_regularize_us(gc, dec));
    gc_free(gc);
    return rt;
  }

  double a2f (char *s) {
    Gc *gc = gc_new();
    char *lc = setlocale(LC_ALL, NULL);
    if (str_starts(lc, "es") || str_starts(lc, "ES")) {
      s = str_replace(gc, s, ".", ",");
    }
    double r = atof(s);
    gc_free(gc);
    return r;
  }

  Gc *gc = gc_new();

  Dec *d;
  Dec *djs;
  Js *js;

  d = dec_new(gc, 1234567890.125, 2);
  assert(decEqstr("1234567890.13", d));
  assert(1234567890.13 == dec_n(d));
  assert(dec_scale(d) == 2);
  js = dec_to_js(gc, d);
  djs = dec_from_js(gc, js);
  assert(decEqstr("1234567890.13", djs));

  d = dec_new(gc, 0.0, 0);
  assert(decEqstr("0", d));
  assert(0 == dec_n(d));
  assert(dec_scale(d) == 0);

  d = dec_new(gc, 0.1234, 3);
  assert(decEqstr("0.123", d));
  assert(0.123 == dec_n(d));
  assert(dec_scale(d) == 3);

  d = dec_new(gc, 12.21, 4);
  assert(decEqstr("12.2100", d));
  assert(12.2100 == dec_n(d));
  assert(dec_scale(d) == 4);

  d = dec_new(gc, -0.1226, 3);
  assert(decEqstr("-0.123", d));
  assert(-0.123 == dec_n(d));
  assert(dec_scale(d) == 3);

  d = dec_new(gc, -12.21, 4);
  assert(decEqstr("-12.2100", d));
  assert(-12.2100 == dec_n(d));
  assert(dec_scale(d) == 4);
  js = dec_to_js(gc, d);
  djs = dec_from_js(gc, js);
  assert(decEqstr("-12.2100", djs));

  assert(dec_eq(3.0, 3.0));
  assert(dec_eq_gap(3.0, 3.0, 0.01));
  assert(dec_eq(3.023456, 3.023456));
  assert(dec_eq_gap(3.023456, 3.023456, 0.01));
  assert(!dec_eq(3.024, 3.023456));
  assert(dec_eq_gap(3.024, 3.023456, 0.01));

  assert(iso_eq("", ""));
  assert(iso_eq(".0", ",0"));
  assert(iso_eq("0.", "0,"));
  assert(iso_eq("-0.", "-0,"));
  assert(iso_eq("-.0", "-,0"));
  assert(iso_eq("12.", "12,"));
  assert(iso_eq(".12", ",12"));
  assert(iso_eq("34.12", "34,12"));
  assert(iso_eq("123341.23", "123.341,23"));
  assert(iso_eq("-34.12", "-34,12"));
  assert(iso_eq("-123341.23", "-123.341,23"));
  assert(iso_eq("34", "34"));
  assert(iso_eq("123341", "123.341"));
  assert(iso_eq("-34", "-34"));
  assert(iso_eq("-123341", "-123.341"));

  assert(us_eq("", ""));
  assert(us_eq(".0", ".0"));
  assert(us_eq("0.", "0."));
  assert(us_eq("-0.", "-0."));
  assert(us_eq("-.0", "-.0"));
  assert(us_eq("12.", "12."));
  assert(us_eq(".12", ".12"));
  assert(us_eq("34.12", "34.12"));
  assert(us_eq("123341.23", "123,341.23"));
  assert(us_eq("-34.12", "-34.12"));
  assert(us_eq("-123341.23", "-123,341.23"));
  assert(us_eq("34", "34"));
  assert(us_eq("123341", "123,341"));
  assert(us_eq("-34", "-34"));
  assert(us_eq("-123341", "-123,341"));

  assert(dec_number(".0") && dec_eq(0, a2f(".0")));
  assert(dec_number("0.") && dec_eq(0, a2f("0.")));
  assert(dec_number("0") && dec_eq(0, a2f("0")));
  assert(dec_number("-.0") && dec_eq(0, a2f("-.0")));
  assert(dec_number("-0.") && dec_eq(0, a2f("-0.")));
  assert(dec_number("-0") && dec_eq(0, a2f("-0")));
  assert(dec_number("0.25") && dec_eq(0.25, a2f("0.25")));
  assert(dec_number("25.") && dec_eq(25, a2f("25.")));
  assert(dec_number("-0.25") && dec_eq(-0.25, a2f("-0.25")));
  assert(dec_number("-25.") && dec_eq(-25, a2f("-25.")));
  assert(dec_number("234.25") && dec_eq(234.25, a2f("234.25")));
  assert(dec_number("25.12") && dec_eq(25.12, a2f("25.12")));
  assert(dec_number("25") && dec_eq(25, a2f("25")));
  assert(dec_number("-234.25") && dec_eq(-234.25, a2f("-234.25")));
  assert(dec_number("-25.12") && dec_eq(-25.12, a2f("-25.12")));
  assert(dec_number("-25") && dec_eq(-25, a2f("-25")));

  assert(dec_number(""));
  assert(dec_number("3."));
  assert(dec_number(".3"));
  assert(dec_number("."));
  assert(!dec_number("a"));
  assert(!dec_number("2..3"));
  assert(!dec_number("--456"));
  assert(!dec_number("q56.12"));
  assert(!dec_number("465.s23"));
  assert(!dec_number("12b.eb"));
  assert(!dec_number("124.7b"));
  assert(!dec_number("12b"));

  gc_free(gc);
  puts("    Finished");
}

