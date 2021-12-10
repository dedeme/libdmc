// Copyright 18-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/ext.h"
#include "dmc/std.h"

char *ext_wget(char *url, int is_mozilla) {
  char *moz = is_mozilla
    ? "--user-agent Mozilla "
      "--load-cookies=/home/deme/"
        ".mozilla/firefox/bfrqeymk.default/cookies.sqlite "
    : ""
  ;
  char *cmd = str_f("wget %s-q --no-cache -O - %s", moz, url);
  Ochar *r = sys_cmd(cmd);
  return ochar_osome(r, "");
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
  Ochar *r = sys_cmd(cmd);
  return ochar_osome(r, "");
}

char *ext_zenity_entry(char *title, char *prompt) {
  char *cmd = str_f(
    "zenity --entry --title=\"%s\" --text=\"%s\" 2>/dev/null", title, prompt
  );
  return str_trim(ochar_esome(sys_cmd(cmd), "Fail running zenity."));
}

void ext_zenity_msg(char *icon, char *text) {
  char *cmd = str_f(
    "zenity --info --icon-name=\"%s\" --text=\"%s\" 2>/dev/null", icon, text
  );
  ochar_esome(sys_cmd(cmd), "Fail running zenity.");
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

  Ochar *ort = sys_cmd(cmd);
  if (ochar_none(ort)) {
    file_del(tsource);
    file_del(ttarget);
    FAIL("Fail running pdfPrinter.");
  }
  char *rt = ochar_some(ort);

  file_del(tsource);

  if (!file_exists(ttarget))
    FAIL(str_f("Target file '%s' not found\n%s", ttarget, rt));

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

  char *rt = ochar_esome(sys_cmd(cmd), "Fail running zip.");

  file_cd(cd);

  if (!file_exists(tg))
    FAIL(str_f("Target file '%s' not found\n%s", tg, rt));
}

void ext_unzip(char *source, char *target) {
  if (!file_is_directory(target))
    FAIL(str_f("'%s' is not a directory", target));

  char *cmd = str_f("unzip -q -o %s -d %s 2>&1", source, target);
  char *rt = ochar_esome(sys_cmd(cmd), "Fail running unzip.");

  if (*rt) FAIL(rt);
}

