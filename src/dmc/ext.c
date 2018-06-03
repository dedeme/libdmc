// Copyright 02-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include "dmc/ext.h"
#include "dmc/sys.h"
#include "dmc/str.h"
#include "dmc/ct/Achar.h"
#include "dmc/DEFS.h"
#include "dmc/file.h"
#include "dmc/exc.h"
#include "dmc/path.h"

Achar *ext_wget(char *url) {
  XNULL(url)

  return sys_cmd(str_printf(
    "wget -q --no-cache -O - %s", url
  ));
}

char *ext_zenity_entry(char *title, char *prompt) {
  XNULL(title)
  XNULL(prompt)

  Achar *a = sys_cmd(str_printf(
    "zenity --entry --title=\"%s\" --text=\"%s\"", title, prompt
  ));
  char *s = "";
  EACH(a, char, l) {
    if (!str_starts(l, "Gtk-")) {
      s = str_trim(l);
      break;
    }
  }_EACH
  return s;
}

void ext_zenity_msg(char *icon, char *text) {
  XNULL(icon)
  XNULL(text)

  sys_cmd(str_printf(
    "zenity --info --icon-name=\"%s\" --text=\"%s\"", icon, text
  ));
}

void ext_pdf(char *tx_source, char *file_target, char *options) {
  XNULL(tx_source)
  XNULL(file_target)
  XNULL(options)

  char *tsource0 = file_tmp("libdm");
  char *tsource = str_printf("%s.html", tsource0);
  file_write(tsource, tx_source);

  char *ttarget0 = file_tmp("libdm");
  char *ttarget = str_printf("%s.pdf", ttarget0);

  Achar *a = sys_cmd(str_printf(
    "pdfPrinter -s %s -t %s %s 2>&1", tsource, ttarget, options
  ));

  file_del(tsource);

  if (!file_exists(ttarget))
    THROW(exc_io_t) str_join(achar_to_it(a), "") _THROW

  file_copy(ttarget, file_target);
  file_del(ttarget);
}

void ext_zip(char *source, char *target) {
  XNULL(source)
  XNULL(target)

  char *cd = file_cwd();
  if (*target != '/') {
    target = path_cat(cd, target, NULL);
  }
  char *parent = path_parent(source);
  char *name = path_name(source);
  file_cd(parent);

  char *cmd = str_printf("zip -q %s %s 2>&1", target, name);
  if (file_is_directory(name)) {
    cmd = str_printf("zip -q -r %s %s 2>&1", target, name);
  }

  Achar *a = sys_cmd(cmd);

  file_cd(cd);

  if (!file_exists(target)) {
    char *msg = "Unknown error";
    if (achar_size(a) > 1) {
      msg = achar_get(a, 0);
    }
    THROW(exc_io_t) "%s\n%s", cmd, msg _THROW
  }
}

void ext_unzip(char *source, char *target) {
  XNULL(source)
  XNULL(target)

  if (!file_is_directory(target))
    THROW(exc_io_t) "'%s' is not a directory", target _THROW

  char *cmd = str_printf("unzip -q -o %s -d %s 2>&1", source, target);

  Achar *a = sys_cmd(cmd);
  if (achar_size(a))
    THROW(exc_generic_t) "%s\n%s", cmd, achar_get(a, 0) _THROW
}
