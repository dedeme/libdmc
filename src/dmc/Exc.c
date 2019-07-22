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

static void exc_exitf (Exc *this) {
  puts(exc_msg(this));

  // Arr[char]
  Arr *st = exc_stack(this);
  printf("\nObtained %d stack frames.\n", arr_size(st));

  for (int i = 0; i < arr_size(st); ++i) {
    puts(arr_get(st, i));
  }

  exit(1);
}

static int exc_exit_reasigned = 0;
static void (*exc_exit) (Exc *) = exc_exitf;

struct exc_Exc {
  char *type;
  char *msg;
  // Arr[char]
  Arr *stack;
};

// stack is Arr[char]
static Exc *exc_new (Gc *gc, char *type, char *msg, Arr *stack) {
  Exc *this = gc_add(gc, malloc(sizeof(Exc)));
  this->type = type;
  this->msg = msg;
  this->stack = stack;
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

void exc_init (void (*exitf) (Exc *)) {
  if (exc_exit_reasigned)
    EXC_ILLEGAL_STATE("Exception exit function already defined");

  exc_exit_reasigned = 1;
  exc_exit = exitf;
}

void exc_thread_init (void) {
}

void exc_thread_end (void) {
}

void exc_throw (
  char *type, char *message, char *file, char *func, int line
) {
  Gc *gc = gc_new();

  void *array[25];
  size_t size;
  char **strings;
  size = backtrace(array, 25);
  strings = backtrace_symbols(array, size);
  // Arr[char]
  Arr *stack = arr_new(gc);
  RANGE0(i, size) {
    arr_push(stack, str_new(gc, strings[i]));
  }_RANGE
  free(strings);

  message = str_f(gc,
    "%s:%d:[%s]:\n%s", path_name(gc, file), line, func, message
  );

  exc_exit(exc_new(gc, type, message, stack));
}

char *exc_range(int begin, int end, int index) {
  return str_f(gc_new(),
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
