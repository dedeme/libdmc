// Copyright 1-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// List (immutable) structure.

#ifndef DM_LIST_H
  #define DM_LIST_H

///
typedef struct list_List List;
typedef struct opt_Opt Opt;
typedef struct it_It It;
typedef struct arr_Arr Arr;
typedef struct js_Js Js;

/// Creates a new List.
List *list_new(void);

/// Returns the number of elements. (O(n) operation).
int list_count(List *this);

/// Returns every element of 'this' less the first one. If "this" is
/// empty, throws an exception.
List *list_tail(List *this);

/// Returns the first element of 'this'. If "this" is empty.
/// throws an exception.
void *list_head (List *this);

/// Returns the element in 'ix' position -- head is in position 0.
/// (O(n) operation).
Opt *list_get (List *this, int ix);

/// Returns '1' if 'this' is empty.
int list_empty(List *this);

/// Adds 'o' at head. 'o' must be not NULL.
List *list_cons(List *this, void *o);

/// Returns 'this + l'.
List *list_cat(List *this, List *l);

/// list_reverse returns this in reverse order.
List *list_reverse(List *this);

/// list_to_it returns an iterator from top to bottom.
It *list_to_it (List *this);

/// list_from_it return a List with elements of 'it' in reverse order.
List *list_from_it (It *it);

/// list_to_arr returns an Arr with 'this' elements.
Arr *list_to_arr (List *this);

/// list_from_arr returns a list with 'a' elements.
List *list_from_arr (Arr *a);

/// list_to_json returns a serialization of 'this' using 'to' to.
/// convert elements.
Js *list_to_js(List *this, Js *(*to)(void *));

/// list_from_json restores a serialized List using 'from' to convert elements.
List *list_from_js(Js *js, void *(*from)(Js *));

#endif
