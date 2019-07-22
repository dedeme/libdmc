// Copyright 21-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/ext.h"
#include "dmc/std.h"

char *ext_wget(Gc *gc, char *url) {
  Gc *gcl = gc_new();
  char *cmd = str_f(gcl, "wget -q --no-cache -O - %s", url);
  char *r = opt_nget(sys_cmd(gc, cmd));
  gc_free(gcl);
  return r ? r : "";
}

char *ext_zenity_entry(Gc *gc, char *title, char *prompt) {
  Gc *gcl = gc_new();
  char *cmd = str_f(gcl,
    "zenity --entry --title=\"%s\" --text=\"%s\"", title, prompt
  );
  char *rt = opt_nget(sys_cmd(gcl, cmd));
  if (!rt) EXC_ILLEGAL_STATE("Fail running zenity.");

  // Arr[char]
  Arr *parts = str_csplit_trim(gcl, rt, '\n');
  char *s = "";
  EACH(parts, char, l) {
    if (!str_starts(l, "Gtk-")) {
      s = str_new(gc, l);
      break;
    }
  }_EACH
  gc_free(gcl);
  return s;
}

void ext_zenity_msg(char *icon, char *text) {
  Gc *gcl = gc_new();
  char *cmd = str_f(gcl,
    "zenity --info --icon-name=\"%s\" --text=\"%s\"", icon, text
  );
  if (!opt_nget(sys_cmd(gcl, cmd)))
    EXC_ILLEGAL_STATE("Fail running zenity.")

  gc_free(gcl);
}

void ext_pdf(
  char *tx_source,
  char *file_target,
  char *options
) {
  Gc *gc = gc_new();

  char *tsource = str_f(gc, "%s.html", file_tmp(gc, "libdm"));
  file_write(tsource, tx_source);

  char *ttarget = str_f(gc, "%s.pdf", file_tmp(gc, "libdm"));

  char *cmd = str_f(gc,
    "pdfPrinter -s %s -t %s %s 2>&1", tsource, ttarget, options
  );

  char *rt = opt_nget(sys_cmd(gc, cmd));
  if (!rt) {
    file_del(tsource);
    file_del(ttarget);
    EXC_GENERIC("Fail running pdfPrinter.")
  }

  file_del(tsource);

  if (!file_exists(ttarget))
    EXC_IO(str_f(gc_new(), "Target file '%s' not found\n%s", ttarget, rt))

  file_copy(ttarget, file_target);
  file_del(ttarget);

  gc_free(gc);
}

void ext_zip(char *source, char *target) {
  Gc *gc = gc_new();

  char *cd = file_cwd(gc);
  char *tg = str_new(gc, target);
  if (*tg != '/') {
    char *t = path_cat(gc, cd, tg, NULL);
    tg = t;
  }
  char *parent = path_parent(gc, source);
  char *name = path_name(gc, source);
  file_cd(parent);

  char *cmd = str_f(gc, "zip -q %s %s 2>&1", tg, name);
  if (file_is_directory(name)) {
    cmd = str_f(gc, "zip -q -r %s %s 2>&1", tg, name);
  }

  char *rt = opt_nget(sys_cmd(gc, cmd));
  if (!rt) EXC_ILLEGAL_STATE("Fail running zip.")

  file_cd(cd);

  if (!file_exists(tg))
    EXC_IO(str_f(gc, "Target file '%s' not found\n%s", tg, rt))

  gc_free(gc);
}

void ext_unzip(char *source, char *target) {
  Gc *gc = gc_new();

  if (!file_is_directory(target))
    EXC_IO(str_f(gc_new(), "'%s' is not a directory", target))

  char *cmd = str_f(gc, "unzip -q -o %s -d %s 2>&1", source, target);
  char *rt = opt_nget(sys_cmd(gc, cmd));
  if (!rt) EXC_ILLEGAL_STATE("Fail running unzip.")

  if (*rt)
    EXC_IO(rt)

  gc_free(gc);
}


