// Copyright 1-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// List structure
#ifndef DM_LIST_H
  #define DM_LIST_H

#include <stdbool.h>
#include <stddef.h>

///
typedef struct list_List List;
typedef struct it_It It;
typedef struct arr_Arr Arr;
typedef struct json_Json Json;

///
List *list_new(void);

///
size_t list_count(List *this);

/// list_head returns every element of 'this' less the first one. If "this" is
/// empty, throws an exception
List *list_tail(List *this);

/// list_head returns the first element of 'this'. If "this" is empty,
/// throws an exception.
void *list_head (List *this);

///
bool list_empty(List *this);

/// list_cons adds 'o' at head. 'o' must be not NULL.
List *list_cons(List *this, void *o);

/// list_cat returns this + l
List *list_cat(List *this, List *l);

/// list_reverse returns this in reverse order
List *list_reverse(List *this);

/// list_to_it returns an iterator from top to bottom
It *list_to_it (List *this);

/// list_from_it return a List with elements of 'it' in reverse order
List *list_from_it (It *it);

///
Arr *list_to_arr (List *this);

///
List *list_from_arr (Arr *a);

/// list_to_json returns a serialization of 'this' using 'to' to
/// convert elements.
Json *list_to_json(List *this, Json *(*to)(void *));

/// list_from_json restores a serialized List using 'from' to convert elements.
List *list_from_json(Json *js, void *(*from)(Json *));

#endif
