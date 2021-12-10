// Copyright 18-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Decimal number and numeric utilities.

#ifndef DMC_DEC_H
  #define DMC_DEC_H

///
typedef struct dec_Dec Dec;

/// Makes a new Dec. It requires link with -lm.
///   n    : Number which will be rounded to 'scale'.
///   scale: Decimal positions. Maximum scale is 10.
Dec *dec_new(double n, int scale);

/// Returns use '.' as decimal point.
char *dec_to_str(Dec *this);

/// Returns use '.' as thousands separator.
char *dec_int_to_iso(int n);

/// Returns use ',' as thousands separator.
char *dec_int_to_us(int n);

/// Returns use ',' as decimal point and '.' as thousands separator.
char *dec_double_to_iso(double n, int scale);

/// Returns use '.' as decimal point and ',' as thousands separator.
char *dec_double_to_us(double n, int scale);

/// Returns the rounded double value of 'this'.
double dec_n(Dec *this);

/// Returns the scale of 'this'.
int dec_scale(Dec *this);

/// Return has an error gap proportional to digits of 'd1' and 'd2'.
int dec_eq(double d1, double d2);

/// Returns true if d1 == d2 with an error margin of +- gap.
int dec_eq_gap(double d1, double d2, double gap);

/// Returns true if d1 == d2 with an error margin of +- gap.
int dec_eqf_gap(float d1, float d2, float gap);

/// Returns 'true' if all characters of 's' are digits.
/// ("" returns 'true').
int dec_digits(const char *s);

/// Returns a number without thousand separators and
/// with decimal point.
char *dec_regularize_iso(char *s);

/// Returns a number without thousand separators and with
/// decimal point.
char *dec_regularize_us(char *s);

/// Returns 'true' if "s" is a regularized number. If is "" returns 'true'.
/// "xxx.", "." or ".xxx" also return 'true'.
int dec_number(char *s);

///
char *dec_to_js(Dec *this);

///
Dec *dec_from_js(char *js);

#endif
