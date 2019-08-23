// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array of ints.

#ifndef DMC_IARR_H
  #define DMC_IARR_H

#include "Js.h"

///
typedef struct iarr_Iarr Iarr;

///
Iarr *iarr_new(void);

/// buffer must be > 0
Iarr *iarr_bf_new(int buffer);

/// Creates a new array from a C array. For example:
///   Iarr *a = iarr_new_c(3, (int[]){2, 0, 4});
/// If 'size' is less than C array length, result is ok (only will be
/// used 'size' first elements); but if 'size' is greater, the result is
/// undetermined.
Iarr *iarr_new_c (int size, int *es);

/// If ix is < 0 then is changed to 'iarr_size - ix'.
Iarr *iarr_left(Iarr *this, int ix);

/// If ix is < 0 then is changed to 'iarr_size - ix'.
Iarr *iarr_right(Iarr *this, int ix);

/// If begin or end are < 0 then is changed to 'iarr_size - itsValue'.
Iarr *iarr_sub(Iarr *this, int begin, int end);

///
Iarr *iarr_copy(Iarr *this);

///
int iarr_size(Iarr *this);

///
int iarr_eq(Iarr *this, Iarr *other);

/// If ix is < 0 then is changed to 'iarr_size - ix'.
int iarr_get(Iarr *this, int ix);

///
int *iarr_start(Iarr *this);

///
int *iarr_end(Iarr *this);

///
void iarr_push(Iarr *this, int e);

/// If ix is < 0 then is changed to 'iarr_size - ix'.
void iarr_set(Iarr *this, int ix, int e);

/// If ix is < 0 then is changed to 'iarr_size - ix'.
void iarr_insert(Iarr *this, int ix, int e);

/// If ix is < 0 then is changed to 'iarr_size - ix'.
void iarr_remove(Iarr *this, int ix);

///
void iarr_cat(Iarr *this, Iarr *other);

/// If ix is < 0 then is changed to 'iarr_size - ix'.
void iarr_insert_arr(Iarr *this, int ix, Iarr *other);

/// If begin or end are < 0 then is changed to 'iarr_size - itsValue'.
void iarr_remove_range(Iarr *this, int begin, int end);

/// Removes every element of 'this'. Buffer size is equals to 15.
void iarr_clear (Iarr *this);

/// Removes every element of 'this'.
void iarr_bf_clear (Iarr *this, int buffer);

///
void iarr_reverse(Iarr *this);

///
void iarr_sort(Iarr *this);

///
Js *iarr_to_js(Iarr *this);

///
Iarr *iarr_from_js(Js *js);

#endif
