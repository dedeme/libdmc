// Copyright 21-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/async.h"
#include <time.h>
#include "dmc/Exc.h"
#include "dmc/Arr.h"
#include "dmc/Opt.h"
#include "dmc/sys.h"
#include "dmc/DEFS.h"
#include "dmc/std.h"

pthread_t *async_new(Gc *gc) {
  return gc_add(gc, malloc(sizeof(pthread_t)));
}

struct async_Thread {
  void (*fn) (void *);
  void *value;
  Gc *gc;
};

static struct async_Thread *thread_new (
  Gc *gc, void (*fn) (void *), void *value
) {
  struct async_Thread *this = gc_add(gc, malloc(sizeof(struct async_Thread)));
  this->fn = fn;
  this->value = value;
  this->gc = gc;
  return this;
}

static void *thread_run (struct async_Thread *data) {
  exc_thread_init();
  data->fn(data->value);
  gc_free(data->gc);
  exc_thread_end();
  return NULL;
}

void async_run (pthread_t *thr, void (*fn)(void *), void *value) {
  Gc *gc = gc_new();
  struct async_Thread *data = thread_new(gc, fn, value);
  pthread_create(thr, NULL, (void *(*)(void *))thread_run, data);
}

void async_run_detached (void (*fn)(void *), void *value) {
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  Gc *gc = gc_new();
  struct async_Thread *data = thread_new(gc, fn, value);
  pthread_create(async_new(gc), &attr, (void *(*)(void *))thread_run, data);
}

static void *thread_run0 (void (*fn)(void)) {
  exc_thread_init();
  fn();
  exc_thread_end();
  return NULL;
}

void async_run0 (pthread_t *thr, void (*fn)()) {
  pthread_create(thr, NULL, (void *(*)(void *))thread_run0, fn);
}

/// Wait until thr finishes
void async_join (pthread_t *thr) {
  pthread_join(*thr, NULL);
}

struct async_AThread {
  void (*fn) (void *);
  void *value;
  Gc *gc;
  int waiting;
};

static struct async_AThread *async_athread_new (
  Gc *gc, void (*fn) (void *), void *value, int waiting
) {
  struct async_AThread *this = gc_add(gc, malloc(sizeof(struct async_AThread)));
  this->fn = fn;
  this->value = value;
  this->gc = gc;
  this->waiting = waiting;
  return this;
}

struct async_AsyncActor {
  pthread_mutex_t mutex;
  // Arr[async_AThread]
  Arr *tasks;
  int millis;
  int active;
  int live;
};

static void add_task (AsyncActor *this, struct async_AThread *data) {
  pthread_mutex_lock(&this->mutex);
  arr_push(this->tasks, data);
  pthread_mutex_unlock(&this->mutex);
}

// Returns Opt[struct async_AThread]
static Opt *get_task (AsyncActor *this) {
  Opt *r = opt_empty();
  Arr *tasks = this->tasks;
  if (arr_size(tasks)) {
    pthread_mutex_lock(&this->mutex);
    r = opt_new(*arr_start(tasks));
    arr_remove(tasks, 0);
    pthread_mutex_unlock(&this->mutex);
  }
  return r;
}

static void actor_cycle(AsyncActor *this) {
  struct async_AThread *task;
  int millis = this->millis;
  while (this->active) {
    task = opt_nget(get_task(this));
    if (task) {
      task->fn(task->value);
      if (task->waiting) task->waiting = 0;
      else gc_free(task->gc);
    } else {
      sys_sleep(millis);
    }
  }
  task = opt_nget(get_task(this));
  while (task) {
    task->fn(task->value);
    if (task->waiting) task->waiting = 0;
    else gc_free(task->gc);
    task = opt_nget(get_task(this));
  }
  sys_sleep(2 * this->millis);
  this->live = 0;
}

AsyncActor *asyncActor_new (Gc*gc, int millis) {
  AsyncActor *this = gc_add(gc, malloc(sizeof(AsyncActor)));
  pthread_mutex_init(&this->mutex, NULL);
  this->tasks = arr_new(gc);
  this->millis = millis;
  this->active = 1;
  this->live = 1;

  async_run_detached((FPROC)actor_cycle, this);

  return this;
}

void asyncActor_run (AsyncActor *this, void (*fn)(void *), void *value) {
  if (this->active) {
    Gc *gc = gc_new();
    add_task(this, async_athread_new(gc, fn, value, 0));
  }
}

void asyncActor_wait (AsyncActor *this, void (*fn)(void *), void *value) {
  if (this->active) {
    Gc *gc = gc_new();
    struct async_AThread *task = async_athread_new(gc, fn, value, 1);
    add_task(this, task);
    while (task->waiting) {
      sys_sleep(this->millis);
    }
    gc_free(gc);
  }
}

void asyncActor_end (AsyncActor *this) {
  this->active = 0;
}

void asyncActor_join (AsyncActor *this) {
  while (this->live) {
    sys_sleep(this->millis);
  }
}

struct async_AsyncTimer {
  struct async_Thread *task;
  int detached;
  int millis;
  int active;
};

static void timer_cycle(AsyncTimer *this) {
  struct async_Thread *task = this->task;
  while (this->active) {
    Gc *gc = gc_new();
    pthread_t *thr = async_new(gc);
    struct async_Thread *data = thread_new(gc, task->fn, task->value);
    if (this->detached) {
      pthread_attr_t attr;
      pthread_attr_init(&attr);
      pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
      pthread_create(thr, &attr, (void *(*)(void *))thread_run, data);
    } else {
      pthread_create(thr, NULL, (void *(*)(void *))thread_run, data);
    }
    sys_sleep(this->millis);
  }
  gc_free(task->gc);
}

AsyncTimer *asyncTimer_new (Gc *gc, int millis) {
  AsyncTimer *this = gc_add(gc, malloc(sizeof(AsyncTimer)));
  this->task = NULL;
  this->detached = 1;
  this->millis = millis;
  this->active = 1;
  return this;
}

void asyncTimer_run (AsyncTimer *this, void (*fn)(void *), void *value) {
  Gc *gc = gc_new();
  this->task = thread_new(gc, fn, value);
  async_run_detached((void (*)(void *))timer_cycle, this);
}

void asyncTimer_wait (AsyncTimer *this, void (*fn)(void *), void *value) {
  Gc *gc = gc_new();
  this->task = thread_new(gc, fn, value);
  this->detached = 0;
  async_run_detached((void (*)(void *))timer_cycle, this);
}

void asyncTimer_end (AsyncTimer *this) {
  this->active = 0;
}

