// Copyright 21-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Decimal number and numeric utilities

#ifndef DMC_DEC_H
  #define DMC_DEC_H

#include "Js.h"

///
typedef struct dec_Dec Dec;

/// dec_new makes a new Dec. It requires link with -lm.
///   gc: Garbage collector
///   n : Number which will be rounded to 'scale'
///   scale: Decimal positions. Maximum scale is 10.
Dec *dec_new(Gc *gc, double n, int scale);

///
char *dec_to_str(Gc *gc, Dec *this);

/// dec_n returns the rounded double value of 'this'
double dec_n(Dec *this);

/// dec_scale returns the scale of 'this'
int dec_scale(Dec *this);

/// dec_eq returns has an error gap proportional to digits of 'd1' and 'd2'
int dec_eq(double d1, double d2);

/// dec_eq_gap return true if d1 == d2 with an error margin of +- gap
int dec_eq_gap(double d1, double d2, double gap);

/// dec_eqf_gap return true if d1 == d2 with an error margin of +- gap
int dec_eqf_gap(float d1, float d2, float gap);

/// dec_digits returns true if all characters of 's' are digits.
/// ("" returns 'true')
int dec_digits(const char *s);

/// dec_regularize_iso returns a number without thousand separators and
/// with decimal point.
char *dec_regularize_iso(Gc *gc, char *s);

/// dec_regularize_us returns a number without thousand separators and with
/// decimal point.
char *dec_regularize_us(Gc *gc, char *s);

/// dec_number Returns 'true' if "s" is a regularized number.<br>
/// "" returns 'true'. "xxx.", "." or ".xxx" also returns 'true'
int dec_number(char *s);

///
Js *dec_to_js(Gc *gc, Dec *this);

///
Dec *dec_from_js(Gc *gc, Js *js);

#endif
