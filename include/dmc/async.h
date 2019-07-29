// Copyright 29-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Management of multithread programs.
///   NOTA: This file must be included instead of 'std.h'

#ifndef DMC_ASYNC_H
  #define DMC_ASYNC_H

/// Necesary definition for multithreading garbage collector.
#define GC_LINUX_THREADS
#include "gc.h"
#include "dmc/std.h"
#include <pthread.h>

/// Launchs 'fn' in a new joinable thread. Example of use:
///   void fn(char *tx) { puts(tx); }
///   pthread_t *thr = async_thread((FPROC)fn, "Hello");
///   async_join(thr); // Waits for thr.
/// NOTA: After calling 'async_thread' is mandatory to call 'async_join' to
/// free resources.
pthread_t *async_thread (void (*fn)(void *), void *value);

/// Launchs 'fn' in a new joinable thread. Example of use:
///   void fn() { puts("Here"); }
///   pthread_t *thr = async_thread0(fn);
///   async_join(thr); // Waits for thr.
/// NOTA: After calling 'async_thread0' is mandatory to call 'async_join' to
/// free resources.
pthread_t *async_thread0 (void (*fn)(void));

/// Launch 'fn' in a new thread. Example of use:
///   void fn(char *tx) { puts(tx); }
///   async_thread_detached((FPROC)fn, "Hello");
void async_thread_detached (void (*fn)(void *), void *value);

/// Launch 'fn' in a new thread. Example of use:
///   void fn(char *tx) { puts("Here"); }
///   async_thread_detached0(fn);
void async_thread_detached0 (void (*fn)(void));

/// Wait until thr finishes
void async_join (pthread_t *thr);

///
typedef struct async_AsyncActor AsyncActor;

/// 'millis' is the latence time.
AsyncActor *asyncActor_new (int millis);

/// Executes 'fn(value)' synchronicaly. This function returns immediatly.
void asyncActor_run (AsyncActor *this, void (*fn)(void *), void *value);

/// Executes 'fn(value)' synchronicaly. This function stops the program
/// until 'fn' is finished.
void asyncActor_wait (AsyncActor *this, void (*fn)(void *), void *value);

/// Finalizes 'this'. 'this' also will finish is pendant jobs.
void asyncActor_end (AsyncActor *this);

/// Waits until 'this' is finished.
void asyncActor_join (AsyncActor *this);

///
typedef struct async_AsyncTimer AsyncTimer;

/// Executes 'fn(value)' each 'millis' milliseconds
AsyncTimer *asyncTimer_new (void (*fn)(void *), void *value, int millis);

/// Finalizes 'this'
void asyncTimer_end (AsyncTimer *this);

#endif
