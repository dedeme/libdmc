// Copyright 21-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array of doubles.

#ifndef DMC_DARR_H
  #define DMC_DARR_H

#include "Js.h"

///
typedef struct darr_Darr Darr;

///
Darr *darr_new(Gc *gc);

/// 'sz' must be > 0
Darr *darr_new_bf(Gc *gc, int sz);

/// If ix is < 0 then is changed to 'darr_size + ix'
Darr *darr_left(Gc *gc, Darr *this, int ix);

/// If ix is < 0 then is changed to 'darr_size + ix'
Darr *darr_right(Gc *gc, Darr *this, int ix);

/// If 'begin' or 'end' are < 0 then is changed to 'darr_size + itsValue'
Darr *darr_sub(Gc *gc, Darr *this, int begin, int end);

/// Returns a new array with elements of 'this'.
Darr *darr_copy(Gc *gc, Darr *this);

///
int darr_eq(Darr *this, Darr *other, double gap);

///
int darr_size(Darr *this);

/// Resturn the element at position ix.
double darr_get(Darr *this, int ix);

/// Return a pointer to the first element of 'this'.
double *darr_start(Darr *this);

/// Returns a pointer to the next element after the last element of 'this'.
/// 'arr_end' does not point to a valid element.
double *darr_end(Darr *this);

/// Adds an element at the end of 'this'.
void darr_push(Darr *this, double e);

/// Returns and removes the last element. Buffer size of 'this' does not change.
double darr_pop(Darr *this);

/// Returns the las element.
double darr_peek(Darr *this);

/// Sets the element at position ix.
void darr_set(Darr *this, int ix, double e);

/// Inserts an element at position ix.
void darr_insert(Darr *this, int ix, double e);

/// Removes elements between [begin-end). Buffer size of 'this' does not change.
void darr_remove(Darr *this, int ix);

/// Adds elements of 'other' to 'this'.
void darr_cat(Darr *this, Darr *other);

/// Inserts elements of 'other' at 'ix'
void darr_insert_arr(Darr *this, int ix, Darr *other);

/// Removes elements between [begin-end). Buffer size of 'this' does not change.
void darr_remove_range(Darr *this, int begin, int end);

/// Removes every element of 'this'. Buffer size is equals to 15.
void darr_clear (Darr *this);

/// Removes every element of 'this'.
void darr_clear_bf (Darr *this, int sz);

/// Reverses elements of 'this'.
void darr_reverse(Darr *this);

/// Sorts 'this' ascendantly.
void darr_sort(Darr *this);

/// Remixes 'this' elements. It should be used after calling
/// rnd_init() or sys_init()
void darr_shuffle (Darr *this);

/// Returns a Js from an element of 'this'.
Js *darr_to_js(Gc *gc, Darr *this);

/// Parses a Js to an element of 'this'.
Darr *darr_from_js(Gc *gc, Js *js);

#endif
