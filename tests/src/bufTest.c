// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dm/dm.h"

void buf_test() {
  printf("Buf test\n");

  char *s0 = "";
  char *s1 = "1";
  char *s2 = "abc";
  Buf *bf = buf_new();

  buf_add(bf, s0);
  assert(!strcmp(buf_str(bf), ""));
  buf_cadd(bf, '+');
  assert(!strcmp(buf_str(bf), "+"));
  buf_add(bf, "0");
  assert(!strcmp(buf_str(bf), "+0"));
  buf_add(bf, s1);
  assert(!strcmp(buf_str(bf), "+01"));
  buf_add(bf, s2);
  assert(!strcmp(buf_str(bf), "+01abc"));
  buf_add(bf, s0);
  assert(!strcmp(buf_str(bf), "+01abc"));
  buf_add(bf, s2);
  assert(!strcmp(buf_str(bf), "+01abcabc"));
  buf_add(bf, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  assert(!strcmp(buf_str(bf), "+01abcabc"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));

  printf("    Finished\n");
}
