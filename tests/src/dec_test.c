// Copyright 02-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <limits.h>
#include <float.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dmc/Dec.h"
#include "dmc/str.h"

void dec_test() {
  char *dec2str (Dec *this) {
    char *r = dec_to_str(this);
    char *lc = setlocale(LC_ALL, NULL);
    return str_starts(lc, "es") || str_starts(lc, "ES")
      ? dec_regularize_iso(r)
      : r;
  }
  double a2f (char *s) {
    char *lc = setlocale(LC_ALL, NULL);
    if (str_starts(lc, "es") || str_starts(lc, "ES"))
      s = str_replace(s, ".", ",");
    return atof(s);
  }

  printf("Dec test\n");

  assert(str_eq("1234567890.13",
    dec2str(dec_new(1234567890.125, 2))));
  assert(str_eq("0",
    dec2str(dec_new(0.0, 0))));
  assert(str_eq("0.123",
    dec2str(dec_new(0.1234, 3))));
  assert(str_eq("12.2100",
    dec2str(dec_new(12.21, 4))));
  assert(str_eq("-0.123",
    dec2str(dec_new(-0.1226, 3))));
  assert(str_eq("-12.2100",
    dec2str(dec_new(-12.21, 4))));

  assert(dec_eq(3.0, 3.0));
  assert(dec_eq_gap(3.0, 3.0, 0.01));
  assert(dec_eq(3.023456, 3.023456));
  assert(dec_eq_gap(3.023456, 3.023456, 0.01));
  assert(!dec_eq(3.024, 3.023456));
  assert(dec_eq_gap(3.024, 3.023456, 0.01));

  assert(str_eq("", dec_regularize_iso("")));
  assert(str_eq(".0", dec_regularize_iso(",0")));
  assert(str_eq("0.", dec_regularize_iso("0,")));
  assert(str_eq("-0.", dec_regularize_iso("-0,")));
  assert(str_eq("-.0", dec_regularize_iso("-,0")));
  assert(str_eq("12.", dec_regularize_iso("12,")));
  assert(str_eq(".12", dec_regularize_iso(",12")));
  assert(str_eq("34.12", dec_regularize_iso("34,12")));
  assert(str_eq("123341.23", dec_regularize_iso("123.341,23")));
  assert(str_eq("-34.12", dec_regularize_iso("-34,12")));
  assert(str_eq("-123341.23", dec_regularize_iso("-123.341,23")));
  assert(str_eq("34", dec_regularize_iso("34")));
  assert(str_eq("123341", dec_regularize_iso("123.341")));
  assert(str_eq("-34", dec_regularize_iso("-34")));
  assert(str_eq("-123341", dec_regularize_iso("-123.341")));

  assert(str_eq("", dec_regularize_us("")));
  assert(str_eq(".0", dec_regularize_us(".0")));
  assert(str_eq("0.", dec_regularize_us("0.")));
  assert(str_eq("-0.", dec_regularize_us("-0.")));
  assert(str_eq("-.0", dec_regularize_us("-.0")));
  assert(str_eq("12.", dec_regularize_us("12.")));
  assert(str_eq(".12", dec_regularize_us(".12")));
  assert(str_eq("34.12", dec_regularize_us("34.12")));
  assert(str_eq("123341.23", dec_regularize_us("123,341.23")));
  assert(str_eq("-34.12", dec_regularize_us("-34.12")));
  assert(str_eq("-123341.23", dec_regularize_us("-123,341.23")));
  assert(str_eq("34", dec_regularize_us("34")));
  assert(str_eq("123341", dec_regularize_us("123,341")));
  assert(str_eq("-34", dec_regularize_us("-34")));
  assert(str_eq("-123341", dec_regularize_us("-123,341")));

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

  printf( "    Finished\n");
}

