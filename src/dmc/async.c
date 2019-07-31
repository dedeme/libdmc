// Copyright 29-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/async.h"
#include "time.h"

struct async_Thread {
  void (*fn) (void *);
  void *value;
};

static struct async_Thread *async_thread_new (
  void (*fn) (void *), void *value
) {
  struct async_Thread *this = MALLOC(struct async_Thread);
  this->fn = fn;
  this->value = value;
  return this;
}

static void *async_thread_run (struct async_Thread *data) {
  exc_thread_init();
  data->fn(data->value);
  exc_thread_end();
  return NULL;
}

static void *async_thread_run0 (void (*fn)(void)) {
  exc_thread_init();
  fn();
  exc_thread_end();
  return NULL;
}

pthread_t *async_thread (void (*fn)(void)) {
  pthread_t *thr = MALLOC(pthread_t);
  pthread_create(thr, NULL, (FCOPY)async_thread_run0, fn);
  return thr;
}

void async_thread_detached (void (*fn)(void *), void *value) {
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  pthread_t *thr = MALLOC(pthread_t);
  struct async_Thread *data = async_thread_new(fn, value);
  pthread_create(thr, &attr, (FCOPY)async_thread_run, data);
}

/// Wait until thr finishes
void async_join (pthread_t *thr) {
  pthread_join(*thr, NULL);
}

struct async_AThread {
  void (*fn) (void *);
  void *value;
  int waiting;
};

static struct async_AThread *async_athread_new (
  void (*fn) (void *), void *value
) {
  struct async_AThread *this = MALLOC(struct async_AThread);
  this->fn = fn;
  this->value = value;
  this->waiting = 1;
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
  /// Opt[struct async_AThread]
  Opt *task;
  int millis = this->millis;
  while (this->active) {
    task = get_task(this);
    if (opt_is_empty(task)) {
      sys_sleep(millis);
    } else {
      struct async_AThread *j = opt_get(task);
      j->fn(j->value);
      j->waiting = 0;
    }
  }
  task = get_task(this);
  while (opt_is_full(task)) {
    struct async_AThread *j = opt_get(task);
    j->fn(j->value);
    j->waiting = 1;
    task = get_task(this);
  }
  this->live = 0;
}

AsyncActor *asyncActor_new (int millis) {
  AsyncActor *this = MALLOC(AsyncActor);
  pthread_mutex_init(&this->mutex, NULL);
  this->tasks = arr_new();
  this->millis = millis;
  this->active = 1;
  this->live = 1;

  async_thread_detached((FPROC)actor_cycle, this);

  return this;
}

void asyncActor_run (AsyncActor *this, void (*fn)(void *), void *value) {
  if (this->active) {
    add_task(this, async_athread_new(fn, value));
  }
}

void asyncActor_wait (AsyncActor *this, void (*fn)(void)) {
  void fn2 (void *null) { fn(); }
  if (this->active) {
    struct async_AThread *task = async_athread_new(fn2, NULL);
    add_task(this, task);
    while (task->waiting) {
      sys_sleep(50);
    }
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
  int millis;
  int active;
};

static void timer_cycle(AsyncTimer *this) {
  struct async_Thread *task = this->task;
  while (this->active) {
    async_thread_detached(task->fn, task->value);
    sys_sleep(this->millis);
  }
}

AsyncTimer *asyncTimer_new (void (*fn)(void *), void *value, int millis) {
  AsyncTimer *this = MALLOC(AsyncTimer);
  this->task = async_thread_new(fn, value);
  this->millis = millis;
  this->active = 1;

  async_thread_detached((FPROC)timer_cycle, this);

  return this;
}

void asyncTimer_end (AsyncTimer *this) {
  this->active = 0;
}
