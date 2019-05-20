// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>
#include <setjmp.h>
#include <gc.h>
#include "dmc/Exc.h"
#include "dmc/str.h"
#include "dmc/Arr.h"
#include "dmc/List.h"
#include "dmc/path.h"
#include "dmc/DEFS.h"

#include <pthread.h>

// Arr[Exc]
static Arr *pool = NULL;

struct exc_Exc {
  pthread_t thread;
  // List[jmp_buf]
  List *buf;
  char *type;
  char *msg;
  // Arr[char]
  Arr *stack;
};

static Exc *exc_new (pthread_t thread, jmp_buf *bf) {
  Exc *this = MALLOC(Exc);
  this->thread = thread;
  this->buf = list_cons(list_new(), bf);
  this->type = NULL;
  this->msg = NULL;
  this->stack = arr_new();
  return this;
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

  exit(1);
}

void exc_init () {
  if (pool) {
    return;
  }
  pool = arr_new();

  jmp_buf *bf = MALLOC(jmp_buf);
  Exc *exc = exc_new(pthread_self(), bf);
  arr_push(pool, exc);

  int val = setjmp(*bf);
  if (val) {
    Exc *exc = exc_get();
    exc_exit(exc);
  }
}

void exc_thread_init (void) {
  if (pool) {
    pthread_t self = pthread_self();
    jmp_buf *bf = MALLOC(jmp_buf);
    Exc *exc = exc_new(self, bf);

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
  } else {
    puts("'exc_init()' has not been called");
    exit(1);
  }
}

void exc_add (jmp_buf *jump) {
  Exc *exc = exc_get();
  exc->buf = list_cons(exc->buf, jump);
}

void exc_remove () {
  Exc *exc = exc_get();
  exc->buf = list_tail(exc->buf);
}

Exc *exc_get (void) {
  Exc *r = NULL;
  if (pool) {
    pthread_t th = pthread_self();
    EACH(pool, Exc, exc)
      if (pthread_equal(exc->thread, th)) {
        r = exc;
        break;
      }
    _EACH
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

void exc_throw (char *type, char *message, char *file, char *func, int line) {
  Exc *exc = exc_get();

  void *array[25];
  size_t size;
  char **strings;
  size = backtrace(array, 25);
  strings = backtrace_symbols(array, size);
  // Arr[char]
  Arr *stack = arr_new();
  RANGE0(i, size) {
    arr_push(stack, str_new(strings[i]));
  }_RANGE
  free(strings);

  exc->stack = stack;
  exc->msg = str_f(
    "%s:%d:[%s]:\n%s", path_name(file), line, func, message
  );
  exc->type = type;

  longjmp(*((jmp_buf *)list_head(exc->buf)), 1);
}

char *exc_range(int begin, int end, int index) {
  return str_f(
    "--- Index out of range: %d out of [%d - %d]", index, begin, end
  );
}

char *exc_illegal_argument(char *argument_name, char *expected, char *actual) {
  return str_f(
    "--- Illegal argument: Variable '%s'\nExpected: %s\nActual: %s",
    argument_name, expected, actual
  );
}

char *exc_illegal_state(char *cause) {
  return str_f("--- Illegal state: %s", cause);
}

char *exc_io(char *cause) {
  return str_f("--- Io error: %s", cause);
}
