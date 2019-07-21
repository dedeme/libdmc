// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Exc.h"
#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>
#include "dmc/str.h"
#include "dmc/Arr.h"
#include "dmc/path.h"
#include "dmc/DEFS.h"

#include <pthread.h>

// Arr[Exc]
static Arr *pool = NULL;
static pthread_mutex_t mutex;

struct exc_Exc {
  Gc *gc;
  pthread_t thread;
  // Arr[jmp_buf]
  Arr *buf;
  char *type;
  char *msg;
  // Arr[char]
  Arr *stack;
};

static Exc *exc_new (pthread_t thread, jmp_buf *bf) {
  Gc *gc = gc_new();
  Exc *this = gc_add(gc, malloc(sizeof(Exc)));
  this->gc = gc;
  this->thread = thread;
  this->buf = arr_new(gc);
  arr_push(this->buf, bf);
  this->type = NULL;
  this->msg = NULL;
  this->stack = arr_new(gc);
  return this;
}

Gc *exc_gc(Exc *this) {
  return this->gc;
}

char *exc_type (Exc *this) {
  if (!this->type) {
    puts("exc_type: 'type' is NULL");
    exit(1);
  }
  return this->type;
}

char *exc_msg (Exc *this) {
  if (!this->msg) {
    printf("exc_msg: 'msg' is NULL");
    exit(1);
  }
  return this->msg;
}

// Arr[char]
Arr *exc_stack (Exc *this) {
  return this->stack;
}

static void exc_exit (Exc *this) {
  puts(exc_msg(this));

  // Arr[char]
  Arr *st = exc_stack(this);
  printf("\nObtained %d stack frames.\n", arr_size(st));

  for (int i = 0; i < arr_size(st); ++i) {
    puts(arr_get(st, i));
  }

  gc_free(this->gc);
  exit(1);
}

Gc *exc_init () {
  if (pool) {
    puts("exc_init already has been called");
    exit(1);
  }
  pthread_mutex_init(&mutex, NULL);

  jmp_buf *bf = malloc(sizeof(jmp_buf));
  Exc *exc = exc_new(pthread_self(), bf);
  gc_add(exc->gc, bf);
  pool = arr_new(exc->gc);
  arr_push(pool, exc);

  int val = setjmp(*bf);
  if (val) {
    Exc *exc = exc_get();
    exc_exit(exc);
  }

  return exc->gc;
}

void exc_end () {

}

void exc_thread_init (void) {
  if (pool) {
    pthread_mutex_lock(&mutex);

    pthread_t self = pthread_self();
    jmp_buf *bf = malloc(sizeof(jmp_buf));
    Exc *exc = exc_new(self, bf);
    gc_add(exc->gc, bf);

    int i = -1;
    EACH_IX(pool, Exc, exc, ix)
      if (pthread_equal(exc->thread, self)) {
        i = ix;
        break;
      }
    _EACH

    if (i == -1) {
      arr_push(pool, exc);
    } else {
      arr_set(pool, i, exc);
    }

    pthread_mutex_unlock(&mutex);

    int val = setjmp(*bf);
    if (val) {
      Exc *exc = exc_get();
      exc_exit(exc);
    }
  } else {
    puts("'exc_init()' has not been called");
    exit(1);
  }
}

void exc_thread_end (void) {
  if (pool) {
    pthread_mutex_lock(&mutex);

    pthread_t th = pthread_self();
    int i = -1;
    EACH_IX(pool, Exc, exc, ix)
      if (pthread_equal(exc->thread, th)) {
        i = ix;
        break;
      }
    _EACH
    if (i != -1) {
      arr_remove(pool, i);
    }

    pthread_mutex_unlock(&mutex);
  } else {
    puts("'exc_init()' has not been called");
    exit(1);
  }
}

void exc_add (jmp_buf *jump) {
  Exc *exc = exc_get();
  arr_push(exc->buf, jump);
}

void exc_remove () {
  Exc *exc = exc_get();
  arr_pop(exc->buf);
}

Exc *exc_get (void) {
  Exc *r = NULL;
  if (pool) {
    pthread_mutex_lock(&mutex);

    pthread_t th = pthread_self();
    EACH(pool, Exc, exc)
      if (pthread_equal(exc->thread, th)) {
        r = exc;
        break;
      }
    _EACH

    pthread_mutex_unlock(&mutex);
  } else {
    puts("'exc_init()' has not been called");
    exit(1);
  }

  if (!r) {
    puts("exc_get: thread not found");
    exit(1);
  }
  return r;
}

void exc_throw (
  char *type, char *message, char *file, char *func, int line, Gc *gc
) {
  Exc *exc = exc_get();
  Gc *gcl = exc->gc;

  void *array[25];
  size_t size;
  char **strings;
  size = backtrace(array, 25);
  strings = backtrace_symbols(array, size);
  // Arr[char]
  Arr *stack = arr_new(gcl);
  RANGE0(i, size) {
    arr_push(stack, str_new(gcl, strings[i]));
  }_RANGE
  free(strings);

  exc->stack = stack;
  exc->msg = str_f(gcl,
    "%s:%d:[%s]:\n%s", path_name(gcl, file), line, func, message
  );
  exc->type = str_new(gcl, type);

  gc_free(gc); // gc was used to make 'message'
  longjmp(*((jmp_buf *)arr_peek(exc->buf)), 1);
}

char *exc_range(Gc *gc, int begin, int end, int index) {
  return str_f(gc,
    "--- Index out of range: %d out of [%d - %d]", index, begin, end
  );
}

char *exc_illegal_argument(
  Gc *gc, char *argument_name, char *expected, char *actual
) {
  return str_f(gc,
    "--- Illegal argument: Variable '%s'\nExpected: %s\nActual: %s",
    argument_name, expected, actual
  );
}

char *exc_illegal_state(Gc *gc, char *cause) {
  return str_f(gc, "--- Illegal state: %s", cause);
}

char *exc_io(Gc *gc, char *cause) {
  return str_f(gc, "--- Io error: %s", cause);
}
