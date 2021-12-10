// Copyright 04-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array of ints.

#ifndef DMC_AINT_H
  #define DMC_AINT_H

/// Array of ints.
struct aInt_AInt {
  int *es;
  int *end;
  int *endbf;
};

/// Array of ints.
typedef struct aInt_AInt AInt;

///
AInt *aInt_new(void);

/// buffer must be > 0
AInt *aInt_bf_new(int buffer);

/// Creates a new array from a C array. For example:
///   AInt *a = aInt_new_c(3, (int[]){2, 0, 4});
/// If 'size' is less than C array length, result is ok (only will be
/// used 'size' first elements); but if 'size' is greater, the result is
/// undetermined.
AInt *aInt_new_c (int size, int *es);

/// Returns a new AInt with elements from 0 to 'ix' (exclusive),
AInt *aInt_left(AInt *this, int ix);

/// Returns a new AInt with elements from 'ix' (inclusive) to end of 'this'.
AInt *aInt_right(AInt *this, int ix);

/// Returns a new AInt with elements from 'begin' (inclusive) to
/// to 'end' (exclusive),
AInt *aInt_sub(AInt *this, int begin, int end);

///
AInt *aInt_copy(AInt *this);

///
int aInt_size(AInt *this);

///
int aInt_eq(AInt *this, AInt *other);

/// If ix is < 0 then is changed to 'aInt_size - ix'.
int aInt_get(AInt *this, int ix);

///
int *aInt_start(AInt *this);

///
int *aInt_end(AInt *this);

///
void aInt_push(AInt *this, int e);

/// If ix is < 0 then is changed to 'aInt_size - ix'.
void aInt_set(AInt *this, int ix, int e);

/// If ix is < 0 then is changed to 'aInt_size - ix'.
void aInt_insert(AInt *this, int ix, int e);

/// If ix is < 0 then is changed to 'aInt_size - ix'.
void aInt_remove(AInt *this, int ix);

///
void aInt_cat(AInt *this, AInt *other);

/// If ix is < 0 then is changed to 'aInt_size - ix'.
void aInt_insert_arr(AInt *this, int ix, AInt *other);

/// If begin or end are < 0 then is changed to 'aInt_size - itsValue'.
void aInt_remove_range(AInt *this, int begin, int end);

/// Removes every element of 'this'. Buffer size is equals to 15.
void aInt_clear (AInt *this);

/// Removes every element of 'this'.
void aInt_bf_clear (AInt *this, int buffer);

///
void aInt_reverse(AInt *this);

///
void aInt_sort(AInt *this);

///
char *aInt_to_js(AInt *this);

///
AInt *aInt_from_js(char *js);

#endif
