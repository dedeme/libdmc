// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array structure.

#ifndef DMC_ARR_H
  #define DMC_ARR_H

#include "DEFS.h"
#include "Tp.h"
#include "Tp3.h"

typedef struct js_Js Js;

typedef struct it_It It;

///
typedef struct arr_Arr Arr;

/// Creates a new Array with buffer size of 15 elements.
Arr *arr_new (void);

/// 'buffer' must be > 0.
Arr *arr_bf_new (int buffer);

/// Creates a new array from several elements.
Arr *arr_new_from (void *e, ...);

/// Creates a new array from a C array. For example:
///   Arr *a = arr_new_c(3, (void *[]){"c", "d", "e"});
/// If 'size' is less than C array length, result is ok (only will be
/// used 'size' first elements); but if 'size' is greater, the result is
/// undetermined.
Arr *arr_new_c (int size, void **es);

/// Returns a new array with elements of 'this'.
Arr *arr_copy (Arr *this);

///
int arr_size (Arr *this);

/// Resturn the element at position ix.
void *arr_get (Arr *this, int ix);

/// Return a pointer to the first element of 'this'
void **arr_start (Arr *this);

/// Returns a pointer to the next element after the last element of 'this'.
/// 'arr_end' does not point to a valid element.
void **arr_end (Arr *this);

/// Adds an element at the end of 'this'. 'e' will be freed by 'this'.
void arr_push (Arr *this, void *e);

/// Returns and removes the last element.
void *arr_pop (Arr *this);

/// Returns the las element.
void *arr_peek (Arr *this);

/// Sets the element at position ix.
void arr_set (Arr *this, int ix, void *e);

/// Inserts an element at position ix.
void arr_insert (Arr *this, int ix, void *e);

/// Removes an element at position ix. Buffer size of 'this' does not change.
void arr_remove (Arr *this, int ix);

/// Adds pointer to elements of 'other' to 'this'.
void arr_cat (Arr *this, Arr *other);

/// Inserts pointer to elements of 'other' at 'ix'
void arr_insert_arr (Arr *this, int ix, Arr *other);

/// Removes elements between [begin-end). Buffer size of 'this' does not change.
void arr_remove_range (Arr *this, int begin, int end);

/// Removes every element of 'this'. Buffer size is equals to 15.
void arr_clear (Arr *this);

/// Removes every element of 'this'.
void arr_bf_clear (Arr *this, int buffer);

/// Reverses elements of 'this'.
void arr_reverse (Arr *this);

/// Sorts 'this' ascendantly using the function 'greater' that returns 'true'
/// if 'e1' > 'e2'.
void arr_sort (Arr *this, int (*greater)(void *e1, void *e2));

/// arr_shuflle remix 'this' elements. It should be used after calling
/// rnd_init() or sys_init().
void arr_shuffle (Arr *this);

/// Returns '1' if every element of 'this' yields '1' with 'pred'.
int arr_all (Arr *this, int (*pred)(void *e));

/// Returns '1' if some element of 'this' yields '1' with 'pred'.
int arr_any (Arr *this, int (*pred)(void *e));

/// arr_index returns the index of the first elements which returns 'true'
/// with 'pred', or -1 if such element does not exist.
int arr_index (Arr *this, int (*pred)(void *e));

/// arr_filter_in removes every element which returns 'false' with 'pred'.
void arr_filter_in (Arr *this, int (*pred)(void *e));

/// Returns a new Arr. See it_take.
Arr *arr_take (Arr *this, int n);

/// Returns a new Arr. See it_takef.
Arr *arr_takef (Arr *this, int (*predicate)(void *e));

/// Returns a new Arr. See it_drop.
Arr *arr_drop (Arr *this, int n);

/// Returns a new Arr. See it_dropf.
Arr *arr_dropf (Arr *this, int (*predicate)(void *e));

/// Returns a new Arr. See it_filter.
Arr *arr_filter_to (Arr *this, int (*predicate)(void *e));

/// Returns a new Arr. See it_map.
Arr *arr_map (Arr *this, void *(*converter)(void *e));

/// Returns a new Arr. See it_map2.
Arr *arr_map2 (Arr *this, void *(*conv1)(void *e), void *(*conv2)(void *e));

/// Returns a new Arr. Returns Arr[Tp]. See it_zip.
Arr *arr_zip (Arr *a1, Arr *a2);

/// Returns a new Arr. Returns Arr[Tp3]. See it_zip3.
Arr *arr_zip3 (Arr *a1, Arr *a2, Arr *a3);

/// Returns Tp[Arr, Arr] from an Arr[Tp]. 'Return_e1' contains elements of
/// source 'Tp_e1' and 'return_e2' elementso of 'Tp_e2'.
Tp *arr_unzip (Arr *this);

/// Returns Tp[Arr, Arr, Arr] from an Arr[Tp2]. 'Return_e1' contains elements
/// of source 'Tp_e1', 'return_e2' elements of 'Tp_e2' and 'return_e3'
/// elements of 'Tp_e3'.
Tp3 *arr_unzip3 (Arr *this);

/// Returns Tp[Arr, Arr] (duplicates, rest) See it_duplicates.
Tp *arr_duplicates (Arr *this, int (feq)(void *e1, void *e2));

/// Creates an iterator over 'this'.
It *arr_to_it (Arr *this);

/// Creates an Arr from 'it'.
Arr *arr_from_it (It *it);

/// Returns a Js from an element of 'this'
Js *arr_to_js (Arr *this, Js *(*to)(void *e));

/// Parses a Js to an element of 'this'.
Arr *arr_from_js (Js *js, void *(*from)(Js *jse));

#endif
