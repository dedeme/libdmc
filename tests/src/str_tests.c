// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "str_tests.h"
#include <assert.h>

void str_tests(void) {
  puts("Str tests");

  puts("    str-starts/ends\n");

  char *tmp;

  assert(str_starts("ab", ""));
  assert(str_starts("", ""));
  assert(str_starts("ab", "a"));
  assert(str_starts("ab", "ab"));
  assert(!str_starts("", "b"));
  assert(!str_starts("ab", "b"));
  assert(!str_starts("ab", "abc"));

  assert(str_ends("ab", ""));
  assert(str_ends("", ""));
  assert(str_ends("ab", "b"));
  assert(str_ends("ab", "ab"));
  assert(!str_ends("", "b"));
  assert(!str_ends("ab", "a"));
  assert(!str_ends("ab", "cab"));

  puts("    str-index/last_index\n");

  assert(str_cindex("ab", 'a') == 0);
  assert(str_cindex_from("ab", 'a', 1) == -1);
  assert(str_cindex("", 'a') == -1);
  assert(str_cindex("ab", 'b') == 1);
  assert(str_cindex_from("ab", 'b', 1) == 1);
  assert(str_cindex("ab", 'c') == -1);

  assert(str_index("ab", "a") == 0);
  assert(str_index("", "a") == -1);
  assert(str_index("ab", "b") == 1);
  assert(str_index("ab", "c") == -1);
  assert(str_index("ab", "") == 0);
  assert(str_index("", "") == 0);
  assert(str_index("ab", "abc") == -1);
  assert(str_index("abcd", "bc") == 1);
  assert(str_index("abcd", "bcd") == 1);
  assert(str_index("abcd", "d") == 3);
  assert(str_index_from("abcd", "d", 0) == 3);
  assert(str_index_from("abcd", "d", 2) == 3);
  assert(str_index_from("abcde", "d", 4) == -1);
  assert(str_index("abcd", "ad") == -1);

  assert(str_last_cindex("ab", 'a') == 0);
  assert(str_last_cindex("aba", 'a') == 2);
  assert(str_last_cindex("", 'a') == -1);
  assert(str_last_cindex("ab", 'b') == 1);
  assert(str_last_cindex("ab", 'c') == -1);

  assert(str_last_index("ab", "a") == 0);
  assert(str_last_index("abac", "a") == 2);
  assert(str_last_index("", "a") == -1);
  assert(str_last_index("ab", "b") == 1);
  assert(str_last_index("abcb", "b") == 3);
  assert(str_last_index("ab", "c") == -1);
  assert(str_last_index("ab", "") == 0);
  assert(str_last_index("", "") == 0);
  assert(str_last_index("ab", "abc") == -1);
  assert(str_last_index("abcd", "bc") == 1);
  assert(str_last_index("abcdbc", "bc") == 4);
  assert(str_last_index("abcd", "bcd") == 1);
  assert(str_last_index("abcd", "d") == 3);
  assert(str_last_index("abcddd", "d") == 5);
  assert(str_last_index("abcd", "ad") == -1);

  printf("    str-copy/cat\n");

  char *s0 = "";
  char *s1 = "1";

  char *r;

  r = str_new(s0);
  assert(str_eq(r, ""));
  free(r);
  r = str_new(s1);
  assert(str_eq(r, "1"));
  free(r);

  r = str_cat_new(s0, s1, NULL);
  assert(str_eq(r, "1"));

  char *r2 = str_cat_new(r, "ab", NULL);
  assert(str_eq(r2, "1ab"));
  free(r);
  r = str_cat_new(r2, "ab", "", "cd", "ab", s1, "cd", NULL);
  assert(str_eq(r, "1ababcdab1cd"));

  free(r2);
  free(r);

  printf("    str-sub\n");

  r = str_left_new("ab", 1);
  assert(str_eq(r, "a"));
  tmp = r;
  r = str_cat_new(tmp, "b", NULL);
  free(tmp);
  tmp = r;

  r = str_sub_new(tmp, -2, -1);
  assert(str_eq(r, "a"));
  free(tmp);
  tmp = r;
  r = str_cat_new(tmp, "b", NULL);
  free(tmp);
  tmp = r;
  r = str_sub_new(tmp, 0, 0);
  assert(str_eq(r, ""));
  free(tmp);
  tmp = r;
  r = str_cat_new(tmp, "ab", NULL);
  free(tmp);
  tmp = r;
  r = str_sub_new(tmp, -1, 0);
  assert(str_eq(r, ""));
  free(tmp);
  tmp = r;
  r = str_cat_new(tmp, "ab", NULL);
  free(tmp);
  tmp = r;
  r = str_sub_new(tmp, 0, 35);
  assert(str_eq(r, ""));
  free(tmp);
  tmp = r;
  r = str_cat_new(tmp, "ab", NULL);
  free(tmp);
  tmp = r;
  r = str_sub_new(tmp, 3, 3);
  assert(str_eq(r, ""));
  free(tmp);
  tmp = r;
  r = str_cat_new(tmp, "ab", NULL);
  free(tmp);
  tmp = r;
  r = str_sub_new(tmp, 2, 2);
  assert(str_eq(r, ""));
  free(tmp);
  tmp = r;
  r = str_cat_new(tmp, "ab", NULL);
  free(tmp);
  tmp = r;
  r = str_sub_new(tmp, 0, 2);
  assert(str_eq(r, "ab"));
  free(tmp);
  tmp = r;
  r = str_sub_new(tmp, 1, 2);
  assert(str_eq(r, "b"));
  free(tmp);
  tmp = r;
  r = str_right_new(tmp, 20);
  free(tmp);
  tmp = r;
  r = str_left_new(tmp, 0);
  assert(str_eq(r, ""));
  free(tmp);
  tmp = r;
  r = str_cat_new(tmp, "ab", NULL);
  free(tmp);
  tmp = r;
  r = str_right_new(tmp, 0);
  assert(str_eq(r, "ab"));
  free(tmp);
  tmp = r;
  r = str_right_new(tmp, -1);
  assert(str_eq(r, "b"));
  free(r);

  r = str_trim_new("nothing to trim");
  assert(str_eq(r, "nothing to trim"));
  free(r);
  r = str_trim_new("trim the back     ");
  assert(str_eq(r, "trim the back"));
  free(r);
  r = str_trim_new(" trim one char front and back ");
  assert(str_eq(r, "trim one char front and back"));
  free(r);
  r = str_trim_new(" trim one char front");
  assert(str_eq(r, "trim one char front"));
  free(r);
  r = str_trim_new("trim one char back ");
  assert(str_eq(r, "trim one char back"));
  free(r);
  r = str_trim_new("                   ");
  assert(str_eq(r, ""));
  free(r);
  r = str_trim_new(" ");
  assert(str_eq(r, ""));
  free(r);
  r = str_trim_new("a");
  assert(str_eq(r, "a"));
  free(r);
  r = str_trim_new("");
  assert(str_eq(r, ""));
  free(r);
  r = str_trim_new("nothing to trim");
  assert(str_eq(r, "nothing to trim"));
  free(r);
  r = str_ltrim_new("trim the back     ");
  assert(str_eq(r, "trim the back     "));
  free(r);
  r = str_ltrim_new(" trim one char front and back ");
  assert(str_eq(r, "trim one char front and back "));
  free(r);
  r = str_ltrim_new(" trim one char front");
  assert(str_eq(r, "trim one char front"));
  free(r);
  r = str_ltrim_new("trim one char back ");
  assert(str_eq(r, "trim one char back "));
  free(r);
  r = str_ltrim_new("                   ");
  assert(str_eq(r, ""));
  free(r);
  r = str_ltrim_new(" ");
  assert(str_eq(r, ""));
  free(r);
  r = str_ltrim_new("a");
  assert(str_eq(r, "a"));
  free(r);
  r = str_ltrim_new("");
  assert(str_eq(r, ""));
  free(r);
  r = str_rtrim_new("nothing to trim");
  assert(str_eq(r, "nothing to trim"));
  free(r);
  r = str_rtrim_new("trim the back     ");
  assert(str_eq(r, "trim the back"));
  free(r);
  r = str_rtrim_new(" trim one char front and back ");
  assert(str_eq(r, " trim one char front and back"));
  free(r);
  r = str_rtrim_new(" trim one char front");
  assert(str_eq(r, " trim one char front"));
  free(r);
  r = str_rtrim_new("trim one char back ");
  assert(str_eq(r, "trim one char back"));
  free(r);
  r = str_rtrim_new("                   ");
  assert(str_eq(r, ""));
  free(r);
  r = str_rtrim_new(" ");
  assert(str_eq(r, ""));
  free(r);
  r = str_rtrim_new("a");
  assert(str_eq(r, "a"));
  free(r);
  r = str_rtrim_new("");
  assert(str_eq(r, ""));

  free(r);

  puts("    str-split/join\n");

  Arr *a;
  char *tx0 = "";
  char *tx01 = ";";
  char *tx1 = "ab";
  char *tx2 = "ab;";
  char *tx3 = "ab;c;de";
  char *tx01b = ";--";
  char *tx2b = "ab;--";
  char *tx3b = "ab ;--  c \t ;--  de";
  char *tx3c = "ab;--c;--de";


  a = str_csplit_new(tx0, ';');
  r = str_cjoin_new(a, ';');
  assert(str_eq(r, tx0));
  arr_free(a); free(r);
  a = str_csplit_new(tx01, ';');
  r = str_cjoin_new(a, ';');
  assert(str_eq(r, ""));
  arr_free(a); free(r);
  a = str_csplit_new(tx1, ';');
  r = str_cjoin_new(a, ';');
  assert(str_eq(r, tx1));
  arr_free(a); free(r);
  a = str_csplit_new(tx2, ';');
  r = str_cjoin_new(a, ';');
  assert(str_eq(r, tx1));
  arr_free(a); free(r);
  a = str_csplit_new(tx3, ';');
  r = str_cjoin_new(a, ';');
  assert(str_eq(r, tx3));
  arr_free(a); free(r);
  a = str_split_new(tx0, ";");
  r = str_join_new(a, ";");
  assert(str_eq(r, tx0));
  arr_free(a); free(r);
  a = str_split_new(tx01, ";");
  r = str_join_new(a, ";");
  assert(str_eq(r, ""));
  arr_free(a); free(r);
  a = str_split_new(tx1, ";");
  r = str_join_new(a, ";");
  assert(str_eq(r, tx1));
  arr_free(a); free(r);
  a = str_split_new(tx2, ";");
  r = str_join_new(a, ";");
  assert(str_eq(r, tx1));
  arr_free(a); free(r);
  a = str_split_new(tx3, ";");
  r = str_join_new(a, ";");
  assert(str_eq(r, tx3));
  arr_free(a); free(r);
  a = str_split_new(tx0, ";--");
  r = str_join_new(a, ";--");
  assert(str_eq(r, tx0));
  arr_free(a); free(r);
  a = str_split_new(tx01b, ";--");
  r = str_join_new(a, ";--");
  assert(str_eq(r, ""));
  arr_free(a); free(r);
  a = str_split_new(tx1, ";--");
  r = str_join_new(a, ";--");
  assert(str_eq(r, tx1));
  arr_free(a); free(r);
  a = str_split_new(tx2b, ";--");
  r = str_join_new(a, ";--");
  assert(str_eq(r, tx1));
  arr_free(a); free(r);
  a = str_split_new(tx3b, ";--");
  r = str_join_new(a, ";--");
  assert(str_eq(r, tx3b));
  arr_free(a); free(r);
  a = str_split_trim_new(tx3b, ";--");
  r = str_join_new(a, ";--");
  assert(str_eq(r, tx3c));
  arr_free(a); free(r);

  puts("    str-replace\n");

  r = str_new("");
  str_creplace(&r, '.', ',');
  assert(str_eq(r, ""));
  free(r);
  r = str_new(".");
  str_creplace(&r, '.', ',');
  assert(str_eq(r, ","));
  free(r);
  r = str_new("f.j");
  str_creplace(&r, '.', ',');
  assert(str_eq(r, "f,j"));
  free(r);
  r = str_new("f.j.");
  str_creplace(&r, '.', ',');
  assert(str_eq(r, "f,j,"));
  free(r);

  r = str_replace_new("", ".", "");
  assert(str_eq(r, ""));
  free(r);
  r = str_replace_new(".", ".", "");
  assert(str_eq(r, ""));
  free(r);
  r = str_replace_new("f.j", ".", "");
  assert(str_eq(r, "fj"));
  free(r);
  r = str_replace_new("f.j.", ".", "");
  assert(str_eq(r, "fj"));
  free(r);

  r = str_replace_new("", "..", "---");
  assert(str_eq(r, ""));
  free(r);
  r = str_replace_new("..", "..", "---");
  assert(str_eq(r, "---"));
  free(r);
  r = str_replace_new("f..j", "..", "---");
  assert(str_eq(r, "f---j"));
  free(r);
  r = str_replace_new("f.j..", "..", "---");
  assert(str_eq(r, "f.j---"));
  free(r);

  r = str_f_new("");
  assert(str_eq(r, ""));
  free(r);
  r = str_f_new("ab");
  assert(str_eq(r, "ab"));
  free(r);
  r = str_f_new("Result is %d", 1);
  assert(str_eq(r, "Result is 1"));
  free(r);
  r = str_f_new("Your %s number %d", "book", 1);
  assert(str_eq(r, "Your book number 1"));
  free(r);
  r = str_f_new("%s%s%s", "1", "ab", "");
  assert(str_eq(r, "1ab"));
  free(r);
  r = str_f_new("%s%s%s", "1", "ab", "c");
  assert(str_eq(r, "1abc"));
  free(r);

  printf("    str-utf8\n");

  assert(0 == str_runes(""));
  assert(1 == str_runes("a"));
  assert(1 == str_runes("\n"));
  assert(1 == str_runes("ú"));
  assert(1 == str_runes("ñ"));
  assert(1 == str_runes("€"));
  assert(3 == str_runes("a b"));
  assert(3 == str_runes("a\nb"));
  assert(3 == str_runes("añb"));
  assert(3 == str_runes("a€b"));
  assert(7 == str_runes(" a\tb €\n"));

  char *s3 = "cañón | a\tb €\n";
  r = str_new("");
  const char *ctmp = str_next_rune(&r, s3);
  Buf *bf = buf_new();
  while (*r) {
    buf_add(bf, r);
    ctmp = str_next_rune(&r, ctmp);
  }
  free(r);
  r = buf_to_str_new(bf);
  buf_free(bf);

  assert(str_eq(r, s3));
  free(r);

  unsigned *u = NULL;
  u = str_to_unicode_new_null("");
  r = str_from_unicode_new_null(u);
  assert(str_eq("", r));
  free(u); free(r);
  u = str_to_unicode_new_null("a");
  r = str_from_unicode_new_null(u);
  assert(str_eq("a", r));
  free(u); free(r);
  u = str_to_unicode_new_null("\n");
  r = str_from_unicode_new_null(u);
  assert(str_eq("\n", r));
  free(u); free(r);
  u = str_to_unicode_new_null("ú");
  r = str_from_unicode_new_null(u);
  assert(str_eq("ú", r));
  free(u); free(r);
  u = str_to_unicode_new_null("ñ");
  r = str_from_unicode_new_null(u);
  assert(str_eq("ñ", r));
  free(u); free(r);
  u = str_to_unicode_new_null("€");
  r = str_from_unicode_new_null(u);
  assert(str_eq("€", r));
  free(u); free(r);
  u = str_to_unicode_new_null("a b");
  r = str_from_unicode_new_null(u);
  assert(str_eq("a b", r));
  free(u); free(r);
  u = str_to_unicode_new_null("a\nb");
  r = str_from_unicode_new_null(u);
  assert(str_eq("a\nb", r));
  free(u); free(r);
  u = str_to_unicode_new_null("a€b");
  r = str_from_unicode_new_null(u);
  assert(str_eq("a€b", r));
  free(u); free(r);
  u = str_to_unicode_new_null("añb");
  r = str_from_unicode_new_null(u);
  assert(str_eq("añb", r));
  free(u); free(r);
  u = str_to_unicode_new_null(" a\tb €\n");
  r = str_from_unicode_new_null(u);
  assert(str_eq(" a\tb €\n", r));
  free(u); free(r);

  printf("    str-to_upper/to_lower\n");

  sys_locale("es_ES.utf8");
  r = str_to_upper_new("cañón");
  assert(str_eq("CAÑÓN", r));
  free(tmp);
  tmp = r;
  r = str_to_lower_new(tmp);
  assert(str_eq("cañón", r));
  sys_locale("C");
  free(r);

  r = str_to_escape_new("cuña\\\"abc\"");
  assert(str_eq("\"cuña\\\\\\\"abc\\\"\"", r));
  free(tmp);
  tmp = r;
  r = str_from_escape_new(tmp);
  assert(str_eq("cuña\\\"abc\"", r));
  free(r);

  free(tmp);

  puts("    Finished");
}

