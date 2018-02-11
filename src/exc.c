// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dm.h"
#include <execinfo.h>

static List *buf = NULL;
static char *msg = NULL;
static Arr/*char*/ *stack = NULL;

void exc_init() {
  if (buf) {
    return;
  }
  buf = list_new();

  jmp_buf *bf = MALLOC(jmp_buf);
  buf = list_cons(buf, bf);
  int val = setjmp(*bf);
  if (val != 0) {
    puts(exc_msg());
    Arr/*char*/ *st = exc_stack();
    printf("\nObtained %zd stack frames.\n", arr_size(st));

    for (int i = 0; i < arr_size(st); ++i) {
      puts(arr_get(st, i));
    }
    exit(1);
  }
}

void exc_add(jmp_buf *jump) {
  if (buf) {
    buf = list_cons(buf, jump);
  } else {
    error_generic("'exc_init()' has not been called", ERROR_DATA);
  }
}

void exc_remove() {
  buf = list_tail(buf);
}

void *exc_throw(char *e, const char *file, const char *func, int line) {
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
    "%s:%d:[%s]: %s", path_name((char *)file), line, func, e
  );

  if (buf) {
    longjmp(*((jmp_buf *)list_head(buf)), 1);
  } else {
    error_generic("'exc_init()' has not been called", ERROR_DATA);
  }
  return NULL;
}

inline
char *exc_msg() {
  return msg ? msg : "";
}

inline
Arr/*char*/ *exc_stack() {
  return stack ? stack : arr_new();
}

