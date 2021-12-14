// Copyright 29-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/async.h"
#include <time.h>
#include "dmc/DEFS.h"
#include "dmc/Arr.h"
#include "dmc/sys.h"

static pthread_mutex_t async_mutex;

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

static void *async_thread_run0 (void (*fn)(void)) {
  fn();
  return NULL;
}

pthread_t *async_thread (void (*fn)(void)) {
  pthread_t *thr = MALLOC(pthread_t);
  pthread_create(thr, NULL, (void *(*)(void *))async_thread_run0, fn);
  return thr;
}

pthread_t *async_thread2 (void (*fn)(void *), void *value) {
  pthread_t *thr = MALLOC(pthread_t);
  struct async_Thread *data = async_thread_new(fn, value);
  pthread_create(thr, NULL, (void *(*)(void *))async_thread_run, data);
  return thr;
}

void async_thread_detached (void (*fn)(void *), void *value) {
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  pthread_t *thr = MALLOC(pthread_t);
  struct async_Thread *data = async_thread_new(fn, value);
  pthread_create(thr, &attr, (void *(*)(void *))async_thread_run, data);
}

void async_join (pthread_t *thr) {
  pthread_join(*thr, NULL);
}

void async_run (void (*fn)(void)) {
  pthread_mutex_lock(&async_mutex);
  fn();
  pthread_mutex_unlock(&async_mutex);
}

void async_run2 (void (*fn)(void *), void *value) {
  pthread_mutex_lock(&async_mutex);
  fn(value);
  pthread_mutex_unlock(&async_mutex);
}
