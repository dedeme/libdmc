// Copyright 10-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dm.h"

void ext_test() {
  printf("ext test\n");
/*
  Arr *r = ext_wget("www.google.com");

  EACH(r, char, s) {
    puts(str_from_iso(s));
  }_EACH


  char *s = ext_zenity_entry("Title", "A text:\" --entry-text \"default");
  printf("%s\n%d\n", s, strlen(s));
*/
//  ext_zenity_msg("face-wink", "A <b>message</b>");

  char *target = str_printf("%s/doc.pdf", sys_home());
  ext_pdf("<html><head></head><body>He<b>llo</b></body></html>",
    target, "");
  file_del(target);

  char *f = str_printf("%s/source.txt", sys_home());
  char *fzip = str_printf("%s/target.zip", sys_home());

  file_write(f, "A test text");
  ext_zip(f, fzip);
  assert(file_exists(fzip));
  file_del(f);
  assert(!file_exists(f));
  ext_unzip(fzip, sys_home());
  assert(file_exists(f));
  char *tx = file_read(f);
  assert(!strcmp(tx, "A test text"));
  file_del(f);
  file_del(fzip);

  printf( "    Finished\n");
}
