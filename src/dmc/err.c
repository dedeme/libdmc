// Copyright 21-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/err.h"
#include <string.h>
#include <execinfo.h>
#include "dmc/std.h"

static void(*err_func)(char *, char **, int) = NULL;

void err_func_default (char *msg, char **stack, int len) {
  puts(msg);
  for (size_t i = 0; i < len; i++) {
    printf("  %s\n", *stack);
    ++stack;
  }
  exit(1);
}

void err_fn (void(*fn)(char *, char **, int)) {
  err_func = fn;
}

void *err_fail (char *message, char *file, char *func, int line) {
  void *array[25];
  size_t size;
  char **strings;
  size = backtrace(array, 25);
  strings = backtrace_symbols(array, size);

  char *name = file + strlen(file);
  while (name > file && *name != '/') --name;
  if (*name == '/') ++name;

  char msg[2500];
  sprintf(msg, "%s:%d:[%s]:\n%s", name, line, func, message);

  if (err_func == NULL) err_func_default(msg, strings, size);
  else err_func(msg, strings, size);

  return NULL;
}
