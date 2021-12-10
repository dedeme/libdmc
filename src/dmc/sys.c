// Copyright 22-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/sys.h"
#include <locale.h>
#include <unistd.h>
#include <pwd.h>
#include <execinfo.h>
#include "dmc/std.h"
#include "time.h"

static struct {
  char *home;
  char *uname;
  char *udir;
} sys = {.home = NULL, .uname = NULL, .udir = NULL};

void sys_init (char *path) {
  if (sys.home) FAIL("'sys_init' already has been called");
  rnd_init();

  uid_t uid = getuid();
  struct passwd *udata = getpwuid(uid);
  sys.home = path_cat(udata->pw_dir, ".dmCApp", path, NULL);
  sys.uname = str_new(udata->pw_name);
  sys.udir = str_new(udata->pw_dir);
  file_mkdir(sys.home);
}

char *sys_home (void) {
  if (!sys.home) FAIL("'sys_init' has not been called");
  return sys.home;
}

char *sys_uname (void) {
  if (!sys.home) FAIL("'sys_init' has not been called");
  return sys.uname;
}

char *sys_udir (void) {
  if (!sys.home) FAIL("'sys_init' has not been called");
  return sys.udir;
}

void sys_set_locale (char *language) {
  setlocale (LC_ALL, language);
}

char *sys_locale (void) {
  return setlocale (LC_ALL, NULL);
}

Ochar *sys_cmd(char *command) {
//  char *c = str_f("%s 2>&1", command);
  FILE *fp = popen(command, "r");

  if (!fp)
    return ochar_mk_none();

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
  return ochar_mk_some(buf_to_str(bf));
}

void sys_sleep (int millis) {
  struct timespec t;
  struct timespec rem;

  t.tv_sec = millis / 1000;
  t.tv_nsec = (millis % 1000) * 1000000;
  nanosleep(&t, &rem);
}
