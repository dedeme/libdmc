// Copyright 21-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Definitions.

#ifndef DMC_DEFS_H
  #define DMC_DEFS_H

#include <gc.h>

///
#define MALLOC(type) (type *)GC_MALLOC(sizeof(type))

///
#define ATOMIC(size) GC_MALLOC_ATOMIC(size)

///
#define FAIL(msg) err_fail (msg, __FILE__, (char *)__func__, __LINE__)

///
typedef void (*FPROC)(void *);

///
typedef void *(*FMAP)(void *);

///
typedef int (*FPRED)(void *);

///
typedef int (*FCMP)(void *, void *);

///
typedef char *(*TO_STR)(void *);

///
typedef void *(*FROM_STR)(char *);

#endif
