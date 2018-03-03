// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/all.h"

char *path_name (char *path) {
  int ix = str_last_cindex(path, '/');
  if (ix == -1)
    return str_copy(path);
  return str_sub(path, ix + 1, strlen(path));
}

char *path_extension (char *path) {
  char *name = path_name(path);

  int ix = str_last_cindex(name, '.');
  if (ix == -1)
    return "";
  return str_sub(name, ix, strlen(name));
}

char *path_only_name (char *path) {
  char *name = path_name(path);

  int ix = str_last_cindex(name, '.');
  if (ix == -1)
    return str_copy(name);
  return str_sub(name, 0, ix);
}

char *path_parent (char *path) {
  int ix = str_last_cindex(path, '/');
  if (ix == -1)
    return "";
  return str_sub(path, 0, ix);
}

char *path_cat (char *s, ...) {
  va_list args;
  char *tmp;
  int len;

  Buf *bf = buf_new();
  buf_add(bf, s);

  va_start(args, s);
  tmp = va_arg(args, char *);
  while (tmp) {
    len = buf_length(bf);
    if (len > 0 && buf_str(bf)[len - 1] != '/') {
      buf_cadd(bf, '/');
    }
    buf_add(bf, tmp);
    tmp = va_arg(args, char *);
  }
  va_end(args);
  return buf_to_str(bf);
}

