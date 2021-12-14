// Copyright 29-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Management of multithread programs.

#ifndef DMC_ASYNC_H
  #define DMC_ASYNC_H

#include <pthread.h>
/// Necesary definition for multithreading garbage collector.
#define GC_THREADS
#include "gc.h"

/// Launchs 'fn' in a new joinable thread. Example of use:
///   /**/void fn() { puts("Here"); }
///   pthread_t *thr = async_thread(fn);
///   async_join(thr); // Waits for thr.
/// NOTE: After calling 'async_thread' is mandatory to call 'async_join' to
///         free resources.
pthread_t *async_thread (void (*fn)(void));

/// Launchs 'fn' in a new joinable thread. Example of use:
///   /**/void fn(char *tx) { puts(tx); }
///   pthread_t *thr = async_thread2((void(*)(void *))fn, "Hello");
///   async_join(thr); // Waits for thr.
/// NOTE: After calling 'async_thread' is mandatory to call 'async_join' to
///         free resources.
pthread_t *async_thread2 (void (*fn)(void *), void *value);

/// Launch 'fn' in a new thread. Example of use:
///   /**/void fn(char *tx) { puts(tx); }
///   async_thread_detached((void(*)(void *))fn, "Hello");
void async_thread_detached (void (*fn)(void *), void *value);

/// Wait until thr finishes.
void async_join (pthread_t *thr);

/// Executes 'fn' synchronically. Example of use:
///   /**/void fn() { puts("Here"); }
///   async_run(fn);
void async_run (void (*fn)(void));

/// Executes 'fn' synchronically. Example of use:
///   /**/void fn(char *tx) { puts(tx); }
///   async_run2((void(*)(void *))fn, "Hello");
void async_run2 (void (*fn)(void *), void *value);

#endif
