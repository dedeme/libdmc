// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

///String utilities

#ifndef DMC_STR_H
  #define DMC_STR_H

#include <stdarg.h>
#include <string.h>
#include "Arr.h"

/// str_new returns a copy of 'str'.
char *str_new(const char *s);

/// str_new returns a string with the character 'ch'.
char *str_c_new(char ch);

/// str_cmp_locale reuturns strcoll(s1, s2)
int str_cmp_locale(const char *s1, const char *s2);

/// str_greater returns strcmp(s1, s2) > 0
int str_greater(const char *s1, const char *s2);

/// str_greater_locale reuturns strcoll(s1, s2) > 0
int str_greater_locale(const char *s1, const char *s2);

/// str_eq returns 'true' if str1 == str2
int str_eq(const char *str1, const char *str2);

/// str_starts indicates if str starts with substr
int str_starts(const char *str, const char *substr);

/// str_ends indicates if str ends with substr
int str_ends(const char *str, const char *substr);

/// str_cindex returns first position of 'ch' in 'str' or -1 if 'ch' is missing.
int str_cindex(const char *str, char ch);

/// str_cindex_from returns first position of 'ch' in 'str' from start or -1 if
/// 'ch' is missing.
int str_cindex_from(const char *str, char ch, int start);

/// str_index returns first position of 'substr' in 'str' or -1 if 'substr'
/// is missing.
int str_index(const char *str, const char *substr);

/// str_index_from returns first position of 'substr' in 'str' from start or -1
/// if 'substr'is missing.
int str_index_from(const char *str, const char *substr, int start);

/// str_last_cindex returns last position of 'ch' in 'str' or -1 if 'ch' is
/// missing.
int str_last_cindex(const char *str, char ch);

/// str_last_index returns last position of 'substr' in 'str' or -1 if 'substr'
/// is missing.
int str_last_index(const char *str, const char *substr);

/// str_cat_new is a string concatenation.<br>
/// Variable argumens must finish with NULL.<br>
char *str_cat_new(const char *s, ...);

/// str_sub_new returns a substring of itself.<br>
/// If 'begin' or 'end' are negatives, they are subtracted from 'strlen(s)'.
/// <br>
/// If 'begin' or 'end' are out of bounds, function returns "".
char *str_sub_new(const char *str, int begin, int end);

/// str_left_new is equals to str_sub_new(str, 0, end);
char *str_left_new(const char *str, int end);

/// str_right_new is equals to str_sub_new(str, begin, strlen(s))
char *str_right_new(const char *str, int begin);

/// str_ltrim_new returns a new string removing spaces (' ', '\\f', '\\n',
/// '\\r', '\\t' and '\\v') at left.
char *str_ltrim_new(const char *str);

/// str_rtrim_new returns a new string removing spaces (' ', '\\f', '\\n',
/// '\\r', '\\t' and '\\v') at right.
char *str_rtrim_new(const char *str);

/// str_trim_new returns a new string removing spaces (' ', '\\f',
/// '\\n', '\\r', '\\t' and '\\v') at left and right.
char *str_trim_new(const char *str);

/// str_csplit_new splits 'str' in an Arr[char].<br>
/// For example (using ';' as separator):
///   "" -> []
///   ";" -> [""]
///   "a;" -> [a]
///   "a;bc" -> ["a", "bc"]
/// Returns an Arr[char]
Arr *str_csplit_new(const char *str, char sep);

/// str_csplit_trim_new is similar to <tt>str_csplit</tt> but trimming elements<br>
/// Returns an Arr[char]
Arr *str_csplit_trim_new(const char *str, char sep);

/// str_split splits 'str' in an Arr[char].<br>
/// For example (using ";" as separator):
///   "" -> []
///   ";" -> [""]
///   "a;" -> [a]
///   "a;bc" -> ["a", "bc"]
/// If 'sep' is '' return will have only one string equals to 'str'.<br>
/// Returns an Arr[char]
Arr *str_split_new(const char *str, const char *sep);

/// str_split_trim_new is similar to <tt>str_split</tt> but trimming
/// elements<br>
/// Returns an Arr[char]
Arr *str_split_trim_new(const char *str, const char *sep);

/// str_cjoin joins elements of 'a', separated by 'sep'.<br>
/// 'a' is Arr[char]
char *str_cjoin_new(Arr *a, char sep);

/// str_join joins elements of 'a', separated by 'sep'.<br>
/// 'a' is Arr[char]
char *str_join_new(Arr *a, const char *sep);

/// str_creplace replace 'old' by 'new' in 's'.
void str_creplace(char **s, char old, char new);

/// str_replace_new replace 'old' by 'new' in 's'.<br>
/// If 'old' is "", it does nothing.
char *str_replace_new(const char *s, const char *old, const char *new);

/// str_vf_new returns a string with format similar to 'vprintf'
char *str_vf_new(const char *format, va_list args);

/// str_f_new returns a string with format similar to 'printf'
char *str_f_new(const char *format, ...);

/// str_runes returns utf8 caracter number or -1 if 's' is not a valid ut8
/// string.
int str_runes(const char *s);

/// str_next_rune reads next rune of 's'.<br>
/// If there are no more runes or there is some fail, it returns "".<br>
/// Example:
///   char *tx = "a text";
///   char *rune = str_new("");
///   char *tmp = str_next_rune(&rune, tx);
///   while (*rune) {
///     puts(rune);
///     tmp = str_next_rune(&rune, tmp);
///   }
///   free(rune);
///   free(tx);
const char *str_next_rune(char **rune, const char *s);

/// str_to_unicode_new_null codifies a string in Unicode. Returns NULL if
/// there is an error.
unsigned *str_to_unicode_new_null(const char *s);

/// str_from_unicode_new decodifies a string from Unicode. Returns NULL if
/// there is an error.
char *str_from_unicode_new_null(unsigned *u);

/// str_from_iso decodifies a string from ISO-8859-1
char *str_from_iso_new(const char *s);

/// str_to_upper_new returns 's' converted to uppercase. It is necessary set
/// language previously.
/// For example:
///   sys_locale("es_ES.utf8");
///   char *s = str_to_upper_new("cañón");
///   puts(s);
///   free(s);
/// This function can fail if s is not a valid utf-8 string.
char *str_to_upper_new (const char *s);

/// str_to_lower returns 's' converted to lowercase. It is necessary set
/// language previously.
/// For example:
///   sys_locale("es_ES.utf8");
///   char *s = str_to_lower_new("cañón");
///   puts(s);
///   free(s);
/// This function can fail if s is not a valid utf-8 string.
char *str_to_lower_new (const char *s);

/// str_to_escape_new replaces " by \" and \ by \\ and insert the result inside
/// quotes.
char *str_to_escape_new (const char *s);

/// str_from_escape_new restores the string escaped with 'escape'. If 's' does
/// not come from 'escape' the result is indefined.
char *str_from_escape_new (const char *s);

#endif
