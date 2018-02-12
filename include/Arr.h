// Copyright 04-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array structure

#ifndef DM_ARR_H
  # define DM_ARR_H

#include <stddef.h>
#include "It.h"

///
typedef struct arr_Arr Arr;

/// Initializates an array
Arr *arr_new (void);

/// Initializates an array with an intial size buffer.
///   this   : Arr object to create
///   buffer : Initial size buffer. Its default is 15.
Arr *arr_new_buf (size_t size_buf);

/// Returns 'this' size
size_t arr_size (Arr *this);

/// Returns elements of 'this'
void **arr_es (Arr *this);

/// Adds an element
void arr_add (Arr *this, void *element);

/// Adds an array
void arr_add_arr (Arr *this, Arr *another);

/// returns element at position 'index'. Tests limits.
void *arr_get (Arr *this, size_t index);

/// Inserts an element at 'index'
void arr_insert (Arr *this, size_t index, void *element);

/// Inserts an array at 'index'
void arr_insert_arr (Arr *this, size_t index, Arr *another);

/// Removes the elemente at 'index'
void arr_remove (Arr *this, size_t index);

/// Removes elements between 'begin' (inclusive) and 'end' (exclusive).
void arr_remove_range (Arr *this, size_t begin, size_t end);

/// Reverses elements of 'this'
void arr_reverse (Arr *this);

/// Replaces element at 'index' by a new 'element'. Tests limits.
void arr_set (Arr *this, size_t index, void *element);

/// Returns the index of 'e'. When there are several elements in such
/// situation, returns one of them without a particular order.
/// If the element does not exists function returns '-1'.<br>
/// This function make a binary search and requires that 'this' is ordered
/// with function 'f'
int arr_sindex (Arr *this, void *e, int(*f)(void *, void*));

/// Sorts elements of 'this' according 'f'
///   f: Function which returns 0 (equals), > 0 (greater) or < 0 (less)
void arr_sort (Arr *this, int (*f)(void *, void *));

/// Ascending natural sort of a string array ('this' can include null elements)
void arr_sort_str (Arr *this);

/// Ascending locale sort of a string array ('this' can include null elements)
void arr_sort_locale (Arr *this);

/// Should be used after calling <tt>cryp_random_init()</tt> or
/// <tt>sys_init()</tt>
void arr_shuffle (Arr *this);

///
It *arr_to_it (Arr *this);

///
Arr *arr_from_it (It *it);

#endif
