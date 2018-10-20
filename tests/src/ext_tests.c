// Copyright 18-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ext_tests.h"
#include <assert.h>
#include "dmc/ext.h"

void ext_tests(void) {
  puts("Ext tests");


/*
  char *r;

  r = ext_wget_new("www.google.com");
  puts(r);
  free(r);

  r = ext_zenity_entry_new("Title", "A text:\" --entry-text \"default");
  printf("%s\n%zu\n", r, strlen(r));
  free(r);
*/
//  ext_zenity_msg("face-wink", "A <b>message</b>");

  char *target = str_f_new("%s/doc.pdf", sys_home());
  ext_pdf("<html><head></head><body>He<b>llo</b></body></html>",
    target, "");
  file_del(target);
  free(target);

  char *f = str_f_new("%s/source.txt", sys_home());
  char *fzip = str_f_new("%s/target.zip", sys_home());

  file_write(f, "A test text");
  ext_zip(f, fzip);
  assert(file_exists(fzip));
  file_del(f);
  assert(!file_exists(f));
  ext_unzip(fzip, sys_home());
  assert(file_exists(f));
  char *tx = file_read_new(f);
  assert(str_eq(tx, "A test text"));
  file_del(f);
  file_del(fzip);

  free(tx);
  free(f);
  free(fzip);

  puts("    Finished");
}


