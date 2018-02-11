// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dm.h"
#include <execinfo.h>

void error_crash (int value, char *format, ...) {
  void *array[25];
  size_t size;
  char **strings;
  size_t i;

  size = backtrace(array, 25);
  strings = backtrace_symbols(array, size);

  printf("\n");

  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);

  printf("\nObtained %zd stack frames.\n", size);

  for (i = 0; i < size; i++)
    printf("%s\n", strings[i]);

  free(strings);

  exit(value);
}

inline
void error_generic(char *message, char *file, const char *func, int line) {
  error_crash(1, "--- Generic error in %s:%d:[%s]\n%s",
    path_name(file), line, func, message);
}

inline
void error_range(
  int begin,
  int end,
  int index,
  char *file,
  const char *func,
  int line
) {
  error_crash(1, "--- Index out of range in %s:%d:[%s]\n%d ouf of [%d - %d>",
    path_name(file), line, func, index, begin, end);
}

inline
void error_null_pointer(
  char *argument_name,
  char *file,
  const char *func,
  int line
) {
  error_crash(1, "--- NULL pointer in %s:%d:[%s]\nVariable name: '%s'",
    path_name(file), line, func, argument_name);
}

inline
void error_not_null_pointer(
  char *argument_name,
  char *file,
  const char *func,
  int line
) {
  error_crash(1, "--- Not NULL pointer in %s:%d:[%s]\nVariable name: '%s'",
    path_name(file), line, func, argument_name);
}

inline
void error_illegal_argument(
  char *argument_name,
  char *file,
  const char *func,
  int line
) {
  error_crash(1, "--- Illegal argument in %s:%d:[%s]\nVariable name: '%s'",
    path_name(file), line, func, argument_name);
}

inline
void error_illegal_state(
  char *cause,
  char *file,
  const char *func,
  int line
) {
  error_crash(1, "--- Illegal state in %s:%d:[%s]\n%s",
    path_name(file), line, func, cause);
}
