/*
 * Copyright 04-May-2017 ºDeme
 * GNU General Public License - V3 <http://www.gnu.org/licenses/>
 */

#include "dm.h"

void list_test() {

  char *paste (It *it) {
    Buf *bf = buf_new();
    while (it_has_next(it)) buf_add(bf, it_next(it));
    return str_copy(buf_str(bf));
  }

  puts("List test");

  List *s = list_new();
  assert(list_empty(s));
  s = list_cons(s, "a");
  assert(!list_empty(s));
  assert(!strcmp("a", list_head(s)));
  s = list_tail(s);
  assert(list_empty(s));
  s = list_cons(s, "a");
  s = list_cons(s, "b");

  assert(!strcmp("ba", paste(list_to_it(s))));
  assert(!strcmp("ba", paste(list_to_it(s))));

  List *s2 = list_from_it(list_to_it(s));
  assert(!strcmp("ab", paste(list_to_it(s2))));
  assert(!strcmp("ab", paste(list_to_it(s2))));

  assert(!strcmp("ba", paste(list_to_it(list_reverse(s2)))));
  assert(!strcmp("", paste(list_to_it(list_new()))));

  List *s3 = list_cons(s2, "c");
  assert(!strcmp("abcab", paste(list_to_it(list_cat(s2, s3)))));

  puts( "    Finished");
}

