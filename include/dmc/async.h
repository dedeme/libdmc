// Copyright 21-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Management of multithread programs.

#ifndef DMC_ASYNC_H
  #define DMC_ASYNC_H

#include <pthread.h>
#include "Gc.h"

/// Creates a new pthread_t
pthread_t *async_new(Gc *gc);

/// Launchs 'fn' in a new thread. Example of use:
///   pthread_t *thr = async_new(gc);
///   void fn(char *tx) { puts(tx); }
///   async_run((FPROC)fn, "Hello");
///   async_join(thr); // Waits for thr.
pthread_t *async_run (pthread_t *thr, void (*fn)(void *), void *value);

/// Launchs 'fn' in a new thread. Example of use:
///   void fn(char *tx) { puts(tx); }
///   async_run_detached((FPROC)fn, "Hello");
void async_run_detached (void (*fn)(void *), void *value);

/// Launchs 'fn' in a new thread. Example of use:
///   char *tx = "text";
///   void fn() { puts(tx); }
///   async_join(async_run0(async_new(gc), fn));
pthread_t *async_run0 (pthread_t *thr, void (*fn)());

/// Wait until thr finishes
void async_join (pthread_t *thr);

///
typedef struct async_AsyncActor AsyncActor;

/// 'millis' (milliseconds) is the latence time.
AsyncActor *asyncActor_new (Gc *gc, int millis);

/// Executes 'fn(value)' synchronicaly. This function returns immediatly
void asyncActor_run (AsyncActor *this, void (*fn)(void *), void *value);

/// Executes 'fn(value)' synchronicaly. This function stops the program
/// until 'fn' is finished
void asyncActor_wait (AsyncActor *this, void (*fn)(void *), void *value);

/// Executes 'fn(value)' synchronicaly. This function stops the program
/// until 'fn' is finished.
void asyncActor_wait0 (AsyncActor *this, void (*fn)());

/// Finalizes 'this'. 'this' also will finish is pendant jobs.
void asyncActor_end (AsyncActor *this);

/// Waits until 'this' is finished.
void asyncActor_join (AsyncActor *this);

///
typedef struct async_AsyncTimer AsyncTimer;

/// 'millis' (milliseconds) is the latence time.
AsyncTimer *asyncTimer_new (Gc *gc, int millis);

/// Executes 'fn(value)' each 'asyncTimer_millis'.
void asyncTimer_run (AsyncTimer *this, void (*fn)(void *), void *value);

/// Executes 'fn(value)' each 'asyncTimer_millis' after end 'fn'.
void asyncTimer_wait (
  AsyncTimer *this, void (*fn)(void *), void *value);

/// Finalizes 'this'
void asyncTimer_end (AsyncTimer *this);

#endif
