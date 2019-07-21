// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Garbage collector

#ifndef DMC_GC_H
  #define DMC_GC_H

#include <stdlib.h>

///
typedef struct gc_Gc Gc;

///
Gc *gc_new (void);

/// 'sz' is the normal elements number of Gc buffer and 'sz_bf' the buffered
/// ones.
Gc *gc_new_bf (size_t sz, size_t sz_bf);

/// Adds and returns a normal object
void *gc_add (Gc *this, void *object);

/// Adds and returns and struct with an array (buffer) which uses 'realloc'.
/// his array is the first object in the struct (see Arr or Buf)
void *gc_add_bf (Gc *this, void *object);

///
void gc_free (Gc *this);

#endif
