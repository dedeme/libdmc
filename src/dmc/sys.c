// Copyright 04-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/all.h"
#include <unistd.h>
#include <pwd.h>
#include <errno.h>
#include <locale.h>

static struct {
  char *home;
  char *uname;
} sys;

void sys_init (char *path) {
  exc_init();
  rnd_init();

  uid_t uid = getuid();
  struct passwd *udata = getpwuid(uid);
  sys.home = path_cat(udata->pw_dir, ".dmCApp", path, NULL);
  sys.uname = str_copy(udata->pw_name);
  file_mkdir(sys.home);
}

inline
char *sys_home () {
  return sys.home;
}

inline
char *sys_uname () {
  return sys.uname;
}

inline
void sys_locale (char *language) {
  setlocale (LC_ALL, language);
}

inline
void sys_random () {
  rnd_init();
}

Arr/*char*/ *sys_cmd(char *command) {
  Arr *r = arr_new();
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  fp = popen(str_printf("%s 2>&1", command), "r");
  if (!fp)
    return NULL;

  while (getline(&line, &len, fp) != -1) {
    arr_add(r, str_copy(line));
    free(line);
    line = NULL;
  }
  free(line);
  return r;
}

