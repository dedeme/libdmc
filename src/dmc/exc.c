// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/all.h"
#include <execinfo.h>

static List *buf = NULL;
static char *msg = NULL;
static Arr/*char*/ *stack = NULL;

static void exc_exit(char *message) {
  puts(message);

  Arr/*char*/ *st = exc_stack();
  printf("\nObtained %zd stack frames.\n", arr_size(st));

  for (int i = 0; i < arr_size(st); ++i) {
    puts(arr_get(st, i));
  }
  exit(1);
}

void exc_init() {
  if (buf) {
    return;
  }
  buf = list_new();

  jmp_buf *bf = MALLOC(jmp_buf);
  buf = list_cons(buf, bf);
  int val = setjmp(*bf);
  if (val != 0) {
    exc_exit(exc_msg());
  }
}

void exc_add(jmp_buf *jump) {
  if (buf) {
    buf = list_cons(buf, jump);
  } else {
    exc_exit("'exc_init()' has not been called");
  }
}

void exc_remove() {
  buf = list_tail(buf);
}

inline
char *exc_msg() {
  return msg ? msg : "";
}

inline
Arr/*char*/ *exc_stack() {
  return stack ? stack : arr_new();
}

void exc_throw(char *e, char *file, char *func, int line) {
  void *array[25];
  size_t size;
  char **strings;
  size = backtrace(array, 25);
  strings = backtrace_symbols(array, size);
  stack = arr_new();
  RANGE0(i, size) {
    arr_add(stack, str_copy(strings[i]));
  }_RANGE
  free(strings);

  msg = str_printf(
    "%s:%d:[%s]:\n%s", path_name(file), line, func, e
  );

  if (buf) {
    longjmp(*((jmp_buf *)list_head(buf)), 1);
  } else {
    exc_exit("'exc_init()' has not been called");
  }
}

inline
char *exc_range(int begin, int end, int index) {
  return str_printf(
    "--- Index out of range: %d out of [%d - %d>",
    index, begin, end
  );
}

inline
char *exc_null_pointer(char *argument_name) {
  return str_printf("--- NULL pointer: Variable '%s'", argument_name);
}

inline
char *exc_not_null_pointer(char *argument_name) {
  return str_printf("--- Not NULL pointer: Variable '%s'", argument_name);
}

inline
char *exc_illegal_argument(char *argument_name, char *expected, char *actual) {
  return str_printf(
    "--- Illegal argument:Variable '%s'\nExpected: %s\nActual: %s",
    argument_name, expected, actual
  );
}

inline
char *exc_illegal_state(char *cause) {
  return str_printf("--- Illegal state: %s", cause);
}

inline
char *exc_io(char *cause) {
  return str_printf("--- Io error: %s", cause);
}
