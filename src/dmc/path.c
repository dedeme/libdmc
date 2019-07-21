// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/path.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "dmc/str.h"
#include "dmc/Buf.h"

char *path_name (Gc *gc, char *path) {
  int ix = str_last_cindex(path, '/');
  if (ix != -1) {
    return str_right(gc, path, ix + 1);
  }
  return str_new(gc, path);
}

char *path_parent (Gc *gc, char *path) {
  int ix = str_last_cindex(path, '/');
  if (ix == -1) {
    ix = 0;
  }
  return str_left(gc, path, ix);
}

char *path_extension (Gc *gc, char *path) {
  path = path_name(gc, path);
  int ix = str_last_cindex(path, '.');
  if (ix == -1) {
    ix = strlen(path);
  }
  return str_right(gc, path, ix);
}

char *path_only_name (Gc *gc, char *path) {
  path = path_name(gc, path);
  int ix = str_last_cindex(path, '.');
  if (ix != -1) {
    return str_left(gc, path, ix);
  }
  return str_new(gc, path);
}

char *path_cat (Gc *gc, char *s, char *more, ...) {
  Gc *gcl = gc_new();

  va_list args;
  char *tmp;

  Buf *bf = buf_new(gcl);
  buf_add(bf, s);
  if (*s) {
    buf_cadd(bf, '/');
  }
  buf_add(bf, more);

  va_start(args, more);
  tmp = va_arg(args, char *);
  while (tmp) {
    if (*tmp) {
      buf_cadd(bf, '/');
      buf_add(bf, tmp);
    }
    tmp = va_arg(args, char *);
  }
  va_end(args);

  char *r = buf_to_str(gc, bf);
  gc_free(gcl);
  return r;
}

