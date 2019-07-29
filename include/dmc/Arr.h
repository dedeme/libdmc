// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array structure.<p>
/// This structure is owner of its elements and frees them with 'arr_free".

#ifndef DMC_ARR_H
  #define DMC_ARR_H

#include "DEFS.h"

typedef struct js_Js Js;

typedef struct it_It It;

///
typedef struct arr_Arr Arr;

/// Creates a new Array with buffer size of 15 elements.
Arr *arr_new (void);

/// buffer must be > 0.
Arr *arr_bf_new (int buffer);

/// Returns a new array with elements of 'this'.
Arr *arr_copy (Arr *this);

///
int arr_size (Arr *this);

/// Resturn the element at position ix
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

/// Inserts an element at position ix
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

/// Reverses elements of 'this'
void arr_reverse (Arr *this);

/// Sorts 'this' ascendantly using the function 'greater' that returns 'true'
/// if 'e1' > 'e2'
void arr_sort (Arr *this, int (*greater)(void *e1, void *e2));

/// arr_shuflle remix 'this' elements. It should be used after calling
/// rnd_init() or sys_init()
void arr_shuffle (Arr *this);

/// arr_index returns the index of the first elements which returns 'true'
/// with 'pred', or -1 if such element does not exist.
int arr_index (Arr *this, int (*pred)(void *e));

/// arr_filter removes every element which returns 'false' with 'pred'.
void arr_filter (Arr *this, int (*pred)(void *e));

/// Creates an iterator over 'this'
It *arr_to_it (Arr *this);

/// Creates an Arr from 'it'
Arr *arr_from_it (It *it);

/// Returns a Js from an element of 'this'
Js *arr_to_js (Arr *this, Js *(*to)(void *e));

/// Parses a Js to an element of 'this'.<br>
Arr *arr_from_js (Js *js, void *(*from)(Js *jse));

#endif
