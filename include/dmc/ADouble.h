// Copyright 04-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array of doubles.

#ifndef DMC_ADOUBLE_H
  #define DMC_ADOUBLE_H

struct aDouble_ADouble {
  double *es;
  double *end;
  double *endbf;
};

/// Array of doubles.
typedef struct aDouble_ADouble ADouble;

/// Array of doubles.
ADouble *aDouble_new(void);

/// buffer must be > 0.
ADouble *aDouble_bf_new(int buffer);

/// Creates a new array from a C array. For example:
///   ADouble *a = aDouble_new_c(3, (double[]){1.0, 1.8, 1.3});
/// If 'size' is less than C array length, result is ok (only will be
/// used 'size' first elements); but if 'size' is greater, the result is
/// undetermined.
ADouble *aDouble_new_c (int size, double *es);

/// Returns a new ADouble with elements from 0 to 'ix' (exclusive),
ADouble *aDouble_left(ADouble *this, int ix);

/// Returns a new ADouble with elements from 'ix' (inclusive) to end of 'this'.
ADouble *aDouble_right(ADouble *this, int ix);

/// Returns a new ADouble with elements from 'begin' (inclusive) to
/// to 'end' (exclusive),
ADouble *aDouble_sub(ADouble *this, int begin, int end);

///
ADouble *aDouble_copy(ADouble *this);

///
int aDouble_eq(ADouble *this, ADouble *other, double gap);

///
int aDouble_size(ADouble *this);

/// If ix is < 0 then is changed to 'aDouble_size - ix'.
double aDouble_get(ADouble *this, int ix);

///
double *aDouble_start(ADouble *this);

///
double *aDouble_end(ADouble *this);

///
void aDouble_push(ADouble *this, double e);

/// If ix is < 0 then is changed to 'aDouble_size - ix'.
void aDouble_set(ADouble *this, int ix, double e);

/// If ix is < 0 then is changed to 'aDouble_size - ix'.
void aDouble_insert(ADouble *this, int ix, double e);

/// If ix is < 0 then is changed to 'aDouble_size - ix'.
void aDouble_remove(ADouble *this, int ix);

///
void aDouble_cat(ADouble *this, ADouble *other);

/// If ix is < 0 then is changed to 'aDouble_size - ix'.
void aDouble_insert_arr(ADouble *this, int ix, ADouble *other);

/// If begin or end are < 0 then is changed to 'aDouble_size - itsValue'.
void aDouble_remove_range(ADouble *this, int begin, int end);

/// Removes every element of 'this'. Buffer size is equals to 15.
void aDouble_clear (ADouble *this);

/// Removes every element of 'this'.
void aDouble_bf_clear (ADouble *this, int buffer);

///
void aDouble_reverse(ADouble *this);

///
void aDouble_sort(ADouble *this);

///
char *aDouble_to_js(ADouble *this);

///
ADouble *aDouble_from_js(char *js);

#endif
