// Copyright 02-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Decimal number and numeric utilities

#ifndef DM_DEC_H
# define DM_DEC_H

#include <stdbool.h>

///
typedef struct dec_Dec Dec;
typedef struct json_Json Json;
typedef struct ajson_Ajson Ajson;

/// dec_new makes a new Dec
///   n : Number which will be rounded to 'scale'
///   scale: Decimal positions. Maximum scale is 10.
Dec *dec_new (double n, int scale);

///
char *dec_to_str (Dec *this);

/// dec_n returns the double value of 'this'
double dec_n (Dec *this);

/// dec_scale returns the scale of 'this'
int dec_scale (Dec *this);

/// dec_eq returns has an error gap proportional to digits of 'd1' and 'd2'
bool dec_eq (double d1, double d2);

/// dec_eq_gap return true if d1 == d2 with an error margin of +- gap
bool dec_eq_gap (double d1, double d2, double gap);

/// dec_eqf_gap return true if d1 == d2 with an error margin of +- gap
bool dec_eqf_gap (float d1, float d2, float gap);

/// dec_digits returns true if all characters of 's' are digits.
/// ("" returns 'true')
bool dec_digits (char *s);

/// dec_regularize_iso returns a number without thousand separators and
/// with decimal point.
char *dec_regularize_iso (char *s);

/// dec_regularize_us returns a number without thousand separators and with
/// decimal point.
char *dec_regularize_us (char *s);

/// dec_number Returns 'true' if "s" is a regularized number.
/// ("" returns 'true', "xxx.", "." or ".xxx" also returns 'true')
bool dec_number (char *s);

/// dec_to_json returns a serialization of 'this' using 'to' to
/// convert elements.
Ajson *dec_to_json(Dec *this);

/// dec_from_json restores a serialized Dec using 'from' to convert elements.
Dec *dec_from_json(Ajson *js);

#endif

