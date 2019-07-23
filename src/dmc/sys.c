// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/sys.h"
#include <locale.h>
#include <unistd.h>
#include <pwd.h>
#include <execinfo.h>
#include "dmc/std.h"
#include "dmc/rnd.h"
#include "dmc/file.h"
#include "time.h"

static struct {
  char *home;
  char *uname;
} sys;


void sys_init (Gc *gc, char *path) {
  rnd_init();

  uid_t uid = getuid();
  struct passwd *udata = getpwuid(uid);
  sys.home = path_cat(gc, udata->pw_dir, ".dmCApp", path, NULL);
  sys.uname = str_new(gc, udata->pw_name);
  file_mkdir(sys.home);
}

char *sys_home (void) {
  return sys.home;
}

char *sys_uname (void) {
  return sys.uname;
}

void sys_locale (char *language) {
  setlocale (LC_ALL, language);
}

Opt *sys_cmd(Gc *gc, char *command) {
  Gc *gcl = gc_new();
  char *c = str_f(gcl, "%s 2>&1", command);
  FILE *fp = popen(c, "r");

  if (!fp) {
    gc_free(gcl);
    return opt_empty();
  }

  Buf *bf = buf_new(gcl);
  char *line = NULL;
  size_t len = 0;
  while (getline(&line, &len, fp) != -1) {
    buf_add(bf, line);
    free(line);
    line = NULL;
  }
  free(line);
  fclose(fp);
  Opt *r = opt_new(buf_to_str(gc, bf));
  gc_free(gcl);
  return r;
}

void sys_sleep (int millis) {
  struct timespec t;
  struct timespec rem;

  t.tv_sec = millis / 1000;
  t.tv_nsec = (millis % 1000) * 1000000;
  nanosleep(&t, &rem);
}

