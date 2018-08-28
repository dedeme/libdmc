// Copyright 30-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <assert.h>
#include "dmc/It.h"
#include "dmc/ct/Ichar.h"
#include "dmc/ct/Achar.h"
#include "dmc/sys.h"
#include "dmc/str.h"
#include "dmc/DEFS.h"


void arr_test() {
  printf("Arr test\n");

  sys_locale("es_ES.utf8");

  Achar *a1, *a2, *a3, *a4;

  a1 = achar_new();
  a2 = achar_new();

  assert(achar_size(a1) == 0);
  assert(achar_size(a2) == 0);
  assert(ichar_eq(achar_to_it(a1), achar_to_it(a2)));
  assert(ichar_eq(achar_to_it(a1), achar_to_it(a2)));
  int i;
  for(i = 0; i < 17; ++i)
    achar_add(a1, str_printf("%d", i));

  for(i = 0; i < 17; ++i) {
    char *s = str_printf("%d", i);
    assert(str_eq(s, achar_get(a1, i)));
  }
  assert(!ichar_eq(achar_to_it(a1), achar_to_it(a2)));

  achar_reverse(a1);
  for(i = 0; i < 17; ++i) {
    char *s = str_printf("%d", 16 - i);
    if (str_len(s) == 1) s = str_cat("0", s, NULL);
    achar_set(a1, i, s);
  }
  achar_sort(a1);

  for(i = 0; i < 17; ++i) {
    char *s = str_printf("%d", i);
    if (str_len(s) == 1) s = str_cat("0", s, NULL);
    assert(str_eq(s, achar_get(a1, i)));
  }

  achar_add(a2, "aaa");
  achar_add(a2, "bbb");
  achar_add(a2, "ccc");
  achar_add(a2, "ddd");
  achar_add(a2, "eee");
  assert(str_eq(achar_get(a2, 0), "aaa"));
  assert(str_eq(achar_get(a2, 1), "bbb"));
  assert(str_eq(achar_get(a2, 2), "ccc"));
  assert(str_eq(achar_get(a2, 3), "ddd"));
  assert(str_eq(achar_get(a2, 4), "eee"));

  achar_reverse(a2);
  assert(str_eq(achar_get(a2, 0), "eee"));
  assert(str_eq(achar_get(a2, 1), "ddd"));
  assert(str_eq(achar_get(a2, 2), "ccc"));
  assert(str_eq(achar_get(a2, 3), "bbb"));
  assert(str_eq(achar_get(a2, 4), "aaa"));
  achar_sort(a2);
  assert(str_eq(achar_get(a2, 0), "aaa"));
  assert(str_eq(achar_get(a2, 1), "bbb"));
  assert(str_eq(achar_get(a2, 2), "ccc"));
  assert(str_eq(achar_get(a2, 3), "ddd"));
  assert(str_eq(achar_get(a2, 4), "eee"));

  char *lc = setlocale(LC_ALL, NULL);
  if (str_starts(lc, "es") || str_starts(lc, "ES")) {
    achar_set(a2, 0, "ááá");
    achar_reverse(a2);
    achar_sort_locale(a2);
    assert(str_eq(achar_get(a2, 0), "ááá"));
    assert(str_eq(achar_get(a2, 1), "bbb"));
    assert(str_eq(achar_get(a2, 2), "ccc"));
    assert(str_eq(achar_get(a2, 3), "ddd"));
    assert(str_eq(achar_get(a2, 4), "eee"));
    achar_set(a2, 0, "aaa");
  }

  achar_insert(a2, 2, "x");
  assert(achar_size(a2) == 6);
  assert(str_eq(achar_get(a2, 0), "aaa"));
  assert(str_eq(achar_get(a2, 1), "bbb"));
  assert(str_eq(achar_get(a2, 2), "x"));
  assert(str_eq(achar_get(a2, 3), "ccc"));
  assert(str_eq(achar_get(a2, 4), "ddd"));
  assert(str_eq(achar_get(a2, 5), "eee"));

  achar_set(a2, 2, "xxx");
  assert(str_eq(achar_get(a2, 0), "aaa"));
  assert(str_eq(achar_get(a2, 1), "bbb"));
  assert(str_eq(achar_get(a2, 2), "xxx"));
  assert(str_eq(achar_get(a2, 3), "ccc"));
  assert(str_eq(achar_get(a2, 4), "ddd"));
  assert(str_eq(achar_get(a2, 5), "eee"));

  achar_set(a2, 0, "a");
  achar_set(a2, 5, "e");
  assert(str_eq(achar_get(a2, 0), "a"));
  assert(str_eq(achar_get(a2, 1), "bbb"));
  assert(str_eq(achar_get(a2, 2), "xxx"));
  assert(str_eq(achar_get(a2, 3), "ccc"));
  assert(str_eq(achar_get(a2, 4), "ddd"));
  assert(str_eq(achar_get(a2, 5), "e"));

  achar_remove(a2, 0);
  achar_remove(a2, 4);
  assert(str_eq(achar_get(a2, 0), "bbb"));
  assert(str_eq(achar_get(a2, 1), "xxx"));
  assert(str_eq(achar_get(a2, 2), "ccc"));
  assert(str_eq(achar_get(a2, 3), "ddd"));

  achar_remove(a2, 1);
  assert(str_eq(achar_get(a2, 0), "bbb"));
  assert(str_eq(achar_get(a2, 1), "ccc"));
  assert(str_eq(achar_get(a2, 2), "ddd"));

  achar_insert(a2, 0, "aaa");
  achar_insert(a2, achar_size(a2), "eee");
  assert(str_eq(achar_get(a2, 0), "aaa"));
  assert(str_eq(achar_get(a2, 1), "bbb"));
  assert(str_eq(achar_get(a2, 2), "ccc"));
  assert(str_eq(achar_get(a2, 3), "ddd"));
  assert(str_eq(achar_get(a2, 4), "eee"));

  a3 = achar_from_it(
    ichar_drop(
      ichar_take(
        achar_to_it(a2),
        4),
      1)
    );

  achar_remove_range(a2, 1, 4);

  assert(str_eq(achar_get(a2, 0), "aaa"));
  assert(str_eq(achar_get(a2, 1), "eee"));
  assert(achar_size(a2) == 2);

  assert(str_eq(achar_get(a3, 0), "bbb"));
  assert(str_eq(achar_get(a3, 1), "ccc"));
  assert(str_eq(achar_get(a3, 2), "ddd"));
  assert(achar_size(a3) == 3);

  achar_add_arr(a2, a3);
  assert(achar_size(a2) == 5);
  assert(str_eq(achar_get(a2, 0), "aaa"));
  assert(str_eq(achar_get(a2, 1), "eee"));
  assert(str_eq(achar_get(a2, 2), "bbb"));
  assert(str_eq(achar_get(a2, 3), "ccc"));
  assert(str_eq(achar_get(a2, 4), "ddd"));

  achar_insert_arr(a2, 0, a3);
  assert(achar_size(a2) == 8);
  assert(str_eq(achar_get(a3, 0), "bbb"));
  assert(str_eq(achar_get(a3, 1), "ccc"));
  assert(str_eq(achar_get(a3, 2), "ddd"));
  assert(str_eq(achar_get(a2, 3), "aaa"));
  assert(str_eq(achar_get(a2, 4), "eee"));
  assert(str_eq(achar_get(a2, 5), "bbb"));
  assert(str_eq(achar_get(a2, 6), "ccc"));
  assert(str_eq(achar_get(a2, 7), "ddd"));

  achar_remove_range(a2, 0, 3);
  assert(achar_size(a2) == 5);
  achar_remove_range(a2, 2, 5);
  assert(achar_size(a2) == 2);
  assert(str_eq(achar_get(a2, 0), "aaa"));
  assert(str_eq(achar_get(a2, 1), "eee"));


  a4 = achar_new();
  for(i = 0; i < 100; ++i) {
    char *s = str_printf("%d", i);
    if (str_len(s) < 2) s = str_cat("0", s, NULL);
    achar_add(a4, s);
  }
  assert(!ichar_eq(achar_to_it(a1), achar_to_it(a4)));
  assert(!ichar_eq(achar_to_it(a1), achar_to_it(a4)));
  achar_insert_arr(a1, 11, a4);
  assert(achar_size(a1) == 117);
  assert(str_eq(achar_get(a1, 116), "16"));

  achar_remove(a1, 0);
  assert(achar_size(a1) == 116);
  assert(str_eq(achar_get(a1, 0), "01"));
  assert(str_eq(achar_get(a1,115), "16"));

  a4 = achar_from_it(
    ichar_drop(
      ichar_take(
        achar_to_it(a1),
        110),
      10)
    );

  achar_remove_range(a1, 10, 110);
  assert(achar_size(a1) == 16);
  assert(achar_size(a4) == 100);
  for(i = 0; i < 16; ++i) {
    char *s = str_printf("%d", i + 1);
    if (str_len(s) < 2) s = str_cat("0", s, NULL);
    assert(str_eq(s, achar_get(a1, i)));
  }

  achar_shuffle(a4);
  for(i = 0; i < achar_size(a4); ++i) {
//    printf("%s\n", (char *)achar_get(a4, i));
  }
  achar_sort(a4);
  for(i = 0; i < achar_size(a4); ++i) {
    assert(i == atoi(achar_get(a4, i)));
  }

  i = 0;
  EACH(a1, char, o) {
    assert(str_eq(o, achar_get(a1, i++)));
  }_EACH

  i = 0;
  EACHI(achar_to_it(a1), char, o) {
    assert(str_eq(o, achar_get(a1, i++)));
  }_EACH

  EACHR(a1, char, o) {
    assert(str_eq(o, achar_get(a1, --i)));
  }_EACH

  assert(ichar_eq(achar_to_it(a1), achar_to_it(a1)));

  assert(15 == ichar_index(achar_to_it(a1), "16"));

  printf( "    Finished\n");
}

