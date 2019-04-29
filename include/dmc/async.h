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

/// Stops the current thread
void async_sleep (int millis);

/// Launchs 'fn' in a new thread. Example of use:
///   void fn(char *tx) { puts(tx); }
///   pthread_t *thr = async_thread((FPROC)fn, "Hello");
///   async_join(thr); // Waits for thr.
pthread_t *async_thread (void (*fn)(void *), void *value);

/// Wait until thr finishes
void async_join (pthread_t *thr);

///
typedef struct async_AsyncActor AsyncActor;

/// 'millis' is the latence time.
AsyncActor *asyncActor_new (int millis);

/// Executes 'fn(value)' synchronicaly.
void asyncActor_run (AsyncActor *this, void (*fn)(void *), void *value);

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
