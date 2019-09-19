// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/sys.h"
#include <locale.h>
#include <unistd.h>
#include <pwd.h>
#include <execinfo.h>
#include "dmc/std.h"
#include "dmc/rnd.h"
#include "time.h"

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
  sys.uname = str_new(udata->pw_name);
  file_mkdir(sys.home);
}

char *sys_home (void) {
  return sys.home;
}

char *sys_uname (void) {
  return sys.uname;
}

void sys_set_locale (char *language) {
  setlocale (LC_ALL, language);
}

char *sys_locale (void) {
  return setlocale (LC_ALL, NULL);
}

Opt *sys_cmd(char *command) {
//  char *c = str_f("%s 2>&1", command);
  FILE *fp = popen(command, "r");

  if (!fp)
    return opt_empty();

  Buf *bf = buf_new();
  char *line = NULL;
  size_t len = 0;
  while (getline(&line, &len, fp) != -1) {
    buf_add(bf, line);
    free(line);
    line = NULL;
  }
  free(line);
  fclose(fp);
  return opt_new(buf_to_str(bf));
}

void sys_sleep (int millis) {
  struct timespec t;
  struct timespec rem;

  t.tv_sec = millis / 1000;
  t.tv_nsec = (millis % 1000) * 1000000;
  nanosleep(&t, &rem);
}
