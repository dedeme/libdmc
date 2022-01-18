// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Arr[Kv *].

#ifndef DMC_KV_AKV_H
  #define DMC_KV_AKV_H

#include "dmc/Arr.h"
#include "dmc/Kv/OKv.h"

#include "dmc/Kv.h"

/// Arr[Kv *].
struct aKv_AKv {
  Kv **es; // Start elements.
  Kv **end; // End elements. (Elements are between 'es' (inclusive) and 'end'
              // exclusive.
  Kv **endbf; // End buffer.
};

/// Arr[Kv *].
typedef struct aKv_AKv AKv;

/// Creates a new Array with buffer size of 15 elements.
AKv *aKv_new (void);

/// 'buffer' must be > 0.
AKv *aKv_bf_new (int buffer);

/// Creates a new array from several elements.
/// Elements list must finish with NULL.
AKv *aKv_new_from (Kv *e, ...);

/// Creates a new array from a C array. For example:
///   Arr *a = arr_new_c(3, (void *[]){"c", "d", "e"});
/// If 'size' is less than C array length, result is ok (only will be
/// used 'size' first elements); but if 'size' is greater, the result is
/// undetermined.
AKv *aKv_new_c (int size, Kv **es);

/// Returns a new array with elements of 'this'.
AKv *aKv_copy (AKv *this);

///
int aKv_size (AKv *this);

/// Resturn the element at position ix.
Kv *aKv_get (AKv *this, int ix);

/// Adds an element at the end of 'this'. 'e' will be freed by 'this'.
void aKv_push (AKv *this, Kv *e);

/// Returns and removes the last element.
Kv *aKv_pop (AKv *this);

/// Returns the las element.
Kv *aKv_peek (AKv *this);

/// Sets the element at position ix.
void aKv_set (AKv *this, int ix, Kv *e);

/// Inserts an element at position ix.
void aKv_insert (AKv *this, int ix, Kv *e);

/// Removes an element at position ix. Buffer size of 'this' does not change.
void aKv_remove (AKv *this, int ix);

/// Adds pointer to elements of 'other' to 'this'.
void aKv_cat (AKv *this, AKv *other);

/// Inserts pointer to elements of 'other' at 'ix'
void aKv_insert_arr (AKv *this, int ix, AKv *other);

/// Removes elements between [begin-end). Buffer size of 'this' does not change.
void aKv_remove_range (AKv *this, int begin, int end);

/// Removes every element of 'this'.
void aKv_clear (AKv *this);

/// Reverses elements of 'this'.
void aKv_reverse (AKv *this);

/// Sorts 'this' ascendantly using the function 'greater' that returns '1'
/// if 'e1' > 'e2'.
void aKv_sort (AKv *this, int (*greater)(Kv *e1, Kv *e2));

/// aKv_shuflle remix 'this' elements. It should be used after calling
/// rnd_init() or sys_init().
void aKv_shuffle (AKv *this);

/// Returns '1' if every element of 'this' yields '1' with 'pred'.
int aKv_all (AKv *this, int (*pred)(Kv *e));

/// Returns '1' if some element of 'this' yields '1' with 'pred'.
int aKv_any (AKv *this, int (*pred)(Kv *e));

/// Returns the index of the first elements which returns '1'
/// with 'pred', or -1 if such element does not exist.
int aKv_index (AKv *this, int (*pred)(Kv *e));

/// Returns the index of the last elements which returns '1'
/// with 'pred', or -1 if such element does not exist.
int aKv_last_index (AKv *this, int (*pred)(Kv *e));

/// Returns the first element which pruduces '1' with 'pred' or 'tp_none'.
OKv *aKv_find(AKv *this, int (*pred)(Kv *e));

/// Returns the last element which pruduces '1' with 'pred' or 'tp_none'.
OKv *aKv_find_last(AKv *this, int (*pred)(Kv *e));

/// aKv_filter_in removes every element which returns '0' with 'pred'.
void aKv_filter_in (AKv *this, int (*pred)(Kv *e));

/// Returns a new Arr with the n first elements of this.
/// If this has less elements than n, returs a copy of this.
AKv *aKv_take (AKv *this, int n);

/// Returns a new Arr with the first elements which return '1' with 'predicate'.
AKv *aKv_takef (AKv *this, int (*predicate)(Kv *e));

/// Returns a new Arr with elements left after aKv_take.
AKv *aKv_drop (AKv *this, int n);

/// Returns a new Arr with elements left after aKv_takef.
AKv *aKv_dropf (AKv *this, int (*predicate)(Kv *e));

/// Returns a new Arr with every element which returns '1' with 'pred'.
AKv *aKv_filter_to (AKv *this, int (*predicate)(Kv *e));

/// Returns a new Arr with elements generated by converter.
Arr *aKv_map (AKv *this, void *(*converter)(Kv *e));

/// Returns a new Arr whit the first element generated by conv1 and the rest
/// by conv2.
Arr *aKv_map2 (AKv *this, void *(*conv1)(Kv *e), void *(*conv2)(Kv *e));

/// Returns a new Arr mixing values of 'a1' and 'a2'. The size of the resultant
/// array is the less of 'a1' size and 'a2' size.
Arr *aKv_zip (AKv *a1, AKv *a2, void *(*converter)(Kv *e1, Kv *e2));

/// Returns a new Arr mixing values of 'a1', 'a2' and 'a3'. The size of the
/// resultant array is the less of 'a1' size, 'a2' size and 'a3' size.
Arr *aKv_zip3 (
  AKv *a1, AKv *a2, AKv *a3,
  void *(*conveter)(Kv*e1, Kv*e2, Kv*e3)
);

/// Removes duplicates with function 'feq=1' and returns them in a new array.
/// It returns only the first duplicated element.
AKv *aKv_duplicates (AKv *this, int (feq)(Kv *e1, Kv *e2));

/// Returns this JSONized.
///   this: Container.
///   to  : Converter of container element to JSON.
char *aKv_to_js (AKv *this, char *(*to)(Kv *e));

/// Returns the container from its JSON representation.
///   js  : Container JSONized.
///   from: Converter from JSON to container element.
AKv *aKv_from_js (char *js, Kv *(*from)(char *ejs));


//--// Not remove

#endif