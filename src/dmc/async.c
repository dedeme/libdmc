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
  data->fn(data->value);
  return NULL;
}

pthread_t *async_thread (void (*fn)(void *), void *value) {
  pthread_t *thr = MALLOC(pthread_t);
  struct async_Thread *data = async_thread_new(fn, value);
  pthread_create(thr, NULL, (FCOPY)async_thread_run, data);
  return thr;
}

/// Wait until thr finishes
void async_join (pthread_t *thr) {
  pthread_join(*thr, NULL);
}

struct async_AsyncActor {
  pthread_mutex_t mutex;
  // Arr[async_Thread]
  Arr *jobs;
  int millis;
  int active;
  int live;
};

static void add_job (AsyncActor *this, struct async_Thread *data) {
  pthread_mutex_lock(&this->mutex);
  arr_push(this->jobs, data);
  pthread_mutex_unlock(&this->mutex);
}

// Returns Opt[struct async_Thread]
static Opt *get_job (AsyncActor *this) {
  Opt *r = opt_empty();
  Arr *jobs = this->jobs;
  if (arr_size(jobs)) {
    pthread_mutex_lock(&this->mutex);
    r = opt_new(*arr_start(jobs));
    arr_remove(jobs, 0);
    pthread_mutex_unlock(&this->mutex);
  }
  return r;
}

static void actor_cycle(AsyncActor *this) {
  /// Opt[struct async_Thread]
  Opt *job;
  int millis = this->millis;
  while (this->active) {
    job = get_job(this);
    if (opt_is_empty(job)) {
      sys_sleep(millis);
    } else {
      struct async_Thread *j = opt_get(job);
      j->fn(j->value);
    }
  }
  job = get_job(this);
  while (opt_is_full(job)) {
    struct async_Thread *j = opt_get(job);
    j->fn(j->value);
    job = get_job(this);
  }
  this->live = 0;
}

AsyncActor *asyncActor_new (int millis) {
  AsyncActor *this = MALLOC(AsyncActor);
  pthread_mutex_init(&this->mutex, NULL);
  this->jobs = arr_new();
  this->millis = millis;
  this->active = 1;
  this->live = 1;

  async_thread((FPROC)actor_cycle, this);

  return this;
}

void asyncActor_run (AsyncActor *this, void (*fn)(void *), void *value) {
  if (this->active) {
    add_job(this, async_thread_new(fn, value));
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
  struct async_Thread *job;
  int millis;
  int active;
};

static void timer_cycle(AsyncTimer *this) {
  struct async_Thread *job = this->job;
  while (this->active) {
    async_thread(job->fn, job->value);
    sys_sleep(this->millis);
  }
}

AsyncTimer *asyncTimer_new (void (*fn)(void *), void *value, int millis) {
  AsyncTimer *this = MALLOC(AsyncTimer);
  this->job = async_thread_new(fn, value);
  this->millis = millis;
  this->active = 1;

  async_thread((FPROC)timer_cycle, this);

  return this;
}

void asyncTimer_end (AsyncTimer *this) {
  this->active = 0;
}
