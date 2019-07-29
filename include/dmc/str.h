// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

///String utilities

#ifndef DMC_STR_H
  #define DMC_STR_H

#include <stdarg.h>
#include <string.h>
#include "Opt.h"
#include "Arr.h"

/// str_new returns a copy of 'str'.
char *str_new(char *s);

/// str_new returns a string with the character 'ch'.
char *str_c(char ch);

/// str_cmp_locale reuturns strcoll(s1, s2)
int str_cmp_locale(char *s1, char *s2);

/// str_greater returns strcmp(s1, s2) > 0
int str_greater(char *s1, char *s2);

/// str_greater_locale reuturns strcoll(s1, s2) > 0
int str_greater_locale(char *s1, char *s2);

/// str_eq returns 'true' if str1 == str2
int str_eq(char *str1, char *str2);

/// str_starts indicates if str starts with substr
int str_starts(char *str, char *substr);

/// str_ends indicates if str ends with substr
int str_ends(char *str, char *substr);

/// str_cindex returns first position of 'ch' in 'str' or -1 if 'ch' is missing.
int str_cindex(char *str, char ch);

/// str_cindex_from returns first position of 'ch' in 'str' from start or -1 if
/// 'ch' is missing.
int str_cindex_from(char *str, char ch, int start);

/// str_index returns first position of 'substr' in 'str' or -1 if 'substr'
/// is missing.
int str_index(char *str, char *substr);

/// str_index_from returns first position of 'substr' in 'str' from start or -1
/// if 'substr'is missing.
int str_index_from(char *str, char *substr, int start);

/// str_last_cindex returns last position of 'ch' in 'str' or -1 if 'ch' is
/// missing.
int str_last_cindex(char *str, char ch);

/// str_last_index returns last position of 'substr' in 'str' or -1 if 'substr'
/// is missing.
int str_last_index(char *str, char *substr);

/// str_cat is a string concatenation.<br>
/// Variable argumens must finish with NULL.<br>
char *str_cat(char *s, ...);

/// str_sub returns a substring of itself.<br>
/// If 'begin' or 'end' are negatives, they are subtracted from 'strlen(s)'.
/// <br>
/// If 'begin' or 'end' are out of bounds, function returns "".
char *str_sub(char *str, int begin, int end);

/// str_left is equals to str_sub(str, 0, end);
char *str_left(char *str, int end);

/// str_right is equals to str_sub(str, begin, strlen(s))
char *str_right(char *str, int begin);

/// str_ltrim returns a new string removing spaces (' ', '\\f', '\\n',
/// '\\r', '\\t' and '\\v') at left.
char *str_ltrim(char *str);

/// str_rtrim returns a new string removing spaces (' ', '\\f', '\\n',
/// '\\r', '\\t' and '\\v') at right.
char *str_rtrim(char *str);

/// str_trim returns a new string removing spaces (' ', '\\f',
/// '\\n', '\\r', '\\t' and '\\v') at left and right.
char *str_trim(char *str);

/// str_csplit splits 'str' in an Arr[char].<br>
/// For example (using ';' as separator):
///   "" -> []
///   ";" -> [""]
///   "a;" -> [a]
///   "a;bc" -> ["a", "bc"]
/// Returns an Arr[char]
Arr *str_csplit(char *str, char sep);

/// str_csplit_trim is similar to <tt>str_csplit</tt> but trimming elements<br>
/// Returns an Arr[char]
Arr *str_csplit_trim(char *str, char sep);

/// str_split splits 'str' in an Arr[char].<br>
/// For example (using ";" as separator):
///   "" -> []
///   ";" -> [""]
///   "a;" -> [a]
///   "a;bc" -> ["a", "bc"]
/// If 'sep' is '' return will have only one string equals to 'str'.<br>
/// Returns an Arr[char]
Arr *str_split(char *str, char *sep);

/// str_split_trim is similar to <tt>str_split</tt> but trimming
/// elements<br>
/// Returns an Arr[char]
Arr *str_split_trim(char *str, char *sep);

/// str_cjoin joins elements of 'a', separated by 'sep'.<br>
/// 'a' is Arr[char]
char *str_cjoin(Arr *a, char sep);

/// str_join joins elements of 'a', separated by 'sep'.<br>
/// 'a' is Arr[char]
char *str_join(Arr *a, char *sep);

/// str_creplace returns a new string replacing 'old' by 'new' in 's'.
char *str_creplace(char *s, char old, char new);

/// str_replace returns a new string replacing 'old' by 'new' in 's'.<br>
/// If 'old' is "", it does nothing.
char *str_replace(char *s, char *old, char *new);

/// str_vf returns a string with format similar to 'vprintf'
char *str_vf(char *format, va_list args);

/// str_f returns a string with format similar to 'printf'
char *str_f(char *format, ...);

/// str_runes returns utf8 caracter number or -1 if 's' is not a valid ut8
/// string.
int str_runes(char *s);

/// str_next_rune reads next rune of 's'.<br>
/// If there are no more runes or it fails, 'rune' is equals to "".<br>
/// Example:
///   char *tx = "a text";
///   char *rune;
///   char *rest = str_next_rune(&rune, tx);
///   while (*rune) {
///     puts(rune);
///     rest = str_next_rune(&rune, rest);
///   }
char *str_next_rune(char **rune, char *s);

/// str_to_unicode codifies a string in Unicode. Returns an Opt_empty()
/// if there are errors.
///   return: Opt[unsigned]
Opt *str_to_unicode(char *s);

/// str_from_unicode_new decodifies a string from Unicode. Returns an Opt_empty()
/// if there are errors.
///   return: Opt[char]
Opt *str_from_unicode(unsigned *u);

/// str_from decodifies a string from ISO-8859-1
char *str_from_iso(char *s);

/// str_to_upper returns 's' converted to uppercase. It is necessary set
/// language previously.
/// For example:
///   sys_locale("es_ES.utf8");
///   char *s = str_to_upper_new("cañón");
///   puts(s);
/// This function can fail if s is not a valid utf-8 string.
char *str_to_upper (char *s);

/// str_to_lower returns 's' converted to lowercase. It is necessary set
/// language previously.
/// For example:
///   sys_locale("es_ES.utf8");
///   char *s = str_to_lower_new("cañón");
///   puts(s);
/// This function can fail if s is not a valid utf-8 string.
char *str_to_lower (char *s);

/// str_to_escape replaces " by \" and \ by \\ and insert the result inside
/// quotes.
char *str_to_escape (char *s);

/// str_from_escape restores the string escaped with 'escape'. If 's' does
/// not come from 'escape' the result is indefined.
char *str_from_escape (char *s);

#endif
