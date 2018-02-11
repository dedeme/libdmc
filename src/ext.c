// Copyright 07-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dm.h"

// Frees 'a' and does not return.
static void throw_first_line(Arr/*char*/ *a, char *header) {
    char *msg = "Unknown error";
    if (arr_size(a) > 1) {
      msg = arr_get(a, 0);
    }
    THROW "%s\n%s", header, msg _THROW
}

inline
Arr/*char*/ *ext_wget(char *url) {
  return sys_cmd(str_printf(
    "wget -q --no-cache -O - %s", url
  ));
}

char *ext_zenity_entry(char *title, char *prompt) {
  Arr *a = sys_cmd(str_printf(
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

inline
void ext_zenity_msg(char *icon, char *text) {
  sys_cmd(str_printf(
    "zenity --info --icon-name=\"%s\" --text=\"%s\"", icon, text
  ));
}

void ext_pdf(char *tx_source, char *file_target, char *options) {
  char *tsource0 = file_tmp("libdm");
  char *tsource = str_printf("%s.html", tsource0);
  file_write(tsource, tx_source);

  char *ttarget0 = file_tmp("libdm");
  char *ttarget = str_printf("%s.pdf", ttarget0);

  Arr/*char*/ *a = sys_cmd(str_printf(
    "pdfPrinter -s %s -t %s %s 2>&1", tsource, ttarget, options
  ));

  file_del(tsource);

  if (!file_exists(ttarget)) {
      THROW str_join(it_from(a), "") _THROW
  }

  file_copy(ttarget, file_target);
  file_del(ttarget);
}

void ext_zip(char *source, char *target) {
  char *cd = file_cwd();
  char *parent = path_parent(source);
  char *name = path_name(source);
  file_cd(parent);

  char *cmd = str_printf("zip -q %s %s 2>&1", target, name);
  if (file_is_directory(name)) {
    cmd = str_printf("zip -q -r %s %s 2>&1", target, name);
  }

  Arr/*char*/ *a = sys_cmd(cmd);
  int exists = file_exists(target);
  file_cd(cd);
  if (!exists) {
    throw_first_line(a, "Zip error:");
  }
}

void ext_unzip(char *source, char *target) {
  if (!file_is_directory(target)) {
    THROW "'%s' is not a directory", target _THROW
  }

  char *cmd = str_printf("unzip -q -o %s -d %s 2>&1", source, target);

  Arr/*char*/ *a = sys_cmd(cmd);
  if (arr_size(a)) {
    throw_first_line(a, "Unzip error:");
  }
}
