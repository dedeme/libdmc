// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities to manage exceptions.<p>
/// Only functions 'init'  and 'throw' is intended to be use directly. The
/// rest must be used through the macros TRY-CATCH-FINALLY-_TRY. (see
/// <a href="?libdm@dm/defs#hp:TRY">defs</a>)
#ifndef DM_LIST_H
  #define DM_LIST_H

#include "It.h"


///
typedef struct list_List List;

///
List *list_new(void);

/// If "this" is not empty, throws an error
List *list_tail(List *this);

/// If "this" is not empty, throws an error
void *list_head (List *this);

///
bool list_empty(List *this);

/// Adds 'o' at head.
List *list_cons(List *this, void *o);

/// Returns this + l
List *list_cat(List *this, List *l);

/// Returns this in reverse order
List *list_reverse(List *this);

/// Returs an iterator from top to bottom
It *list_to_it (List *this);

/// 'it' goes from bottom to top (reverse)
List *list_from_it (It *it);

#endif
