// Copyright 18-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/ext.h"
#include "dmc/std.h"

char *ext_wget(char *url) {
  char *cmd = str_f("wget -q --no-cache -O - %s", url);
  // Opt[char]
  Opt *r = sys_cmd(cmd);
  if (opt_is_empty(r)) {
    return "";
  }
  return opt_get(r);
}

char *ext_puppeteer (char *url) {
  char *fmt = "node -e \""
    "const puppeteer = require('puppeteer');"
    "(async () => {"
    "  try {"
    "    const browser = await puppeteer.launch();"
    "    const page = await browser.newPage();"
    "    page.setDefaultNavigationTimeout(180000);"
    "    await page.goto('%s');"
    "    const ct = await page.content();"
    "    console.log(ct);"
    "    await browser.close();"
    "  } catch (e) {"
    "    console.error(e.toString());"
    "    process.exit(1);"
    "  }"
    "})();"
    "\" 2>/dev/null"
  ;
  char *cmd = str_f(fmt, url);
  // Opt[char]
  Opt *r = sys_cmd(cmd);
  if (opt_is_empty(r)) {
    return "";
  }
  return opt_get(r);
}

char *ext_zenity_entry(char *title, char *prompt) {
  char *cmd = str_f(
    "zenity --entry --title=\"%s\" --text=\"%s\" 2>/dev/null", title, prompt
  );
  return str_trim(opt_eget(sys_cmd(cmd), "Fail running zenity."));
}

void ext_zenity_msg(char *icon, char *text) {
  char *cmd = str_f(
    "zenity --info --icon-name=\"%s\" --text=\"%s\" 2>/dev/null", icon, text
  );
  opt_eget(sys_cmd(cmd), "Fail running zenity.");
}

void ext_pdf(
  char *tx_source,
  char *file_target,
  char *options
) {
  char *tsource = str_f("%s.html", file_tmp("libdm"));
  file_write(tsource, tx_source);

  char *ttarget = str_f("%s.pdf", file_tmp("libdm"));

  char *cmd = str_f(
    "pdfPrinter -s %s -t %s %s 2>&1", tsource, ttarget, options
  );

  // Opt[char]
  Opt *ort = sys_cmd(cmd);
  if (opt_is_empty(ort)) {
    file_del(tsource);
    file_del(ttarget);
    EXC_GENERIC("Fail running pdfPrinter.")
  }
  char *rt = opt_get(ort);

  file_del(tsource);

  if (!file_exists(ttarget))
    EXC_IO(str_f("Target file '%s' not found\n%s", ttarget, rt))

  file_copy(ttarget, file_target);
  file_del(ttarget);
}

void ext_zip(char *source, char *target) {
  char *cd = file_cwd();
  char *tg = str_new(target);
  if (*tg != '/') {
    char *t = path_cat(cd, tg, NULL);
    tg = t;
  }
  char *parent = path_parent(source);
  char *name = path_name(source);
  file_cd(parent);

  char *cmd = str_f("zip -q %s %s 2>&1", tg, name);
  if (file_is_directory(name)) {
    cmd = str_f("zip -q -r %s %s 2>&1", tg, name);
  }

  char *rt = opt_eget(sys_cmd(cmd), "Fail running zip.");

  file_cd(cd);

  if (!file_exists(tg))
    EXC_IO(str_f("Target file '%s' not found\n%s", tg, rt))
}

void ext_unzip(char *source, char *target) {
  if (!file_is_directory(target))
    EXC_IO(str_f("'%s' is not a directory", target))

  char *cmd = str_f("unzip -q -o %s -d %s 2>&1", source, target);
  char *rt = opt_eget(sys_cmd(cmd), "Fail running unzip.");

  if (*rt)
    EXC_IO(rt)
}

