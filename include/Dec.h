// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Decimal number and numeric utilities

#ifndef DM_DEC_H
# define DM_DEC_H

#include <stddef.h>

///
typedef struct dec_Dec Dec;

/// Maximum scale is 10. N is rounded to 'scale'.
Dec *dec_new (double n, size_t scale);

///
char *dec_to_str (Dec *this);

/// Returns the double value of 'this'
double dec_n (Dec *this);

/// Return the scale of 'this'
size_t dec_scale (Dec *this);

/// Return has an error gap proportional to digits of 'd1' and 'd2'
bool dec_eq (double d1, double d2);

/// Return has an error margin of +- gap
bool dec_eq_gap (double d1, double d2, double gap);

/// Return has an error margin of +- gap
bool dec_eqf_gap (float d1, float d2, float gap);

/// Returns true if all characters of 's' are digits. ("" returns 'true')
bool dec_digits (char *s);

/// Returns a number without thousand separators and with decimal point.
char *dec_regularize_iso (char *s);

/// Returns a number without thousand separators and with decimal point.
char *dec_regularize_us (char *s);

/// Retursn 'true' if "s" is a regularized number. ("" returns 'true',
/// "xxx.", "." or ".xxx" also returns 'true')
bool dec_number (char *s);

#endif

