// Copyright 21-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array of ints.

#ifndef DMC_IARR_H
  #define DMC_IARR_H

#include "Js.h"

///
typedef struct iarr_Iarr Iarr;

///
Iarr *iarr_new(Gc *gc);

/// 'sz' must be > 0
Iarr *iarr_new_bf(Gc *gc, int sz);

/// If ix is < 0 then is changed to 'iarr_size - ix'
Iarr *iarr_left(Gc *gc, Iarr *this, int ix);

/// If ix is < 0 then is changed to 'iarr_size - ix'
Iarr *iarr_right(Gc *gc, Iarr *this, int ix);

/// If begin or end are < 0 then is changed to 'iarr_size - itsValue'
Iarr *iarr_sub(Gc *gc, Iarr *this, int begin, int end);

/// Returns a new array with elements of 'this'.
Iarr *iarr_copy(Gc *gc, Iarr *this);

///
int iarr_eq(Iarr *this, Iarr *other);

///
int iarr_size(Iarr *this);

/// Resturn the element at position ix.
int iarr_get(Iarr *this, int ix);

/// Return a pointer to the first element of 'this'.
int *iarr_start(Iarr *this);

/// Returns a pointer to the next element after the last element of 'this'.
/// 'arr_end' does not point to a valid element.
int *iarr_end(Iarr *this);

/// Adds an element at the end of 'this'.
void iarr_push(Iarr *this, int e);

/// Returns and removes the last element. Buffer size of 'this' does not change.
int iarr_pop(Iarr *this);

/// Returns the las element.
int iarr_peek(Iarr *this);

/// Sets the element at position ix.
void iarr_set(Iarr *this, int ix, int e);

/// Inserts an element at position ix.
void iarr_insert(Iarr *this, int ix, int e);

/// Removes elements between [begin-end). Buffer size of 'this' does not change.
void iarr_remove(Iarr *this, int ix);

/// Adds elements of 'other' to 'this'.
void iarr_cat(Iarr *this, Iarr *other);

/// Inserts elements of 'other' at 'ix'
void iarr_insert_arr(Iarr *this, int ix, Iarr *other);

/// Removes elements between [begin-end). Buffer size of 'this' does not change.
void iarr_remove_range(Iarr *this, int begin, int end);

/// Removes every element of 'this'. Buffer size is equals to 15.
void iarr_clear (Iarr *this);

/// Removes every element of 'this'.
void iarr_clear_bf (Iarr *this, int sz);

/// Reverses elements of 'this'.
void iarr_reverse(Iarr *this);

/// Sorts 'this' ascendantly.
void iarr_sort(Iarr *this);

/// Remixes 'this' elements. It should be used after calling
/// rnd_init() or sys_init()
void iarr_shuffle (Iarr *this);

/// Returns a Js from an element of 'this'.
Js *iarr_to_js(Gc *gc, Iarr *this);

/// Parses a Js to an element of 'this'.
Iarr *iarr_from_js(Gc *gc, Js *js);

#endif
