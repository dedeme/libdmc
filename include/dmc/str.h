// Copyright 22-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

///String utilities.

#ifndef DMC_STR_H
  #define DMC_STR_H

#include <stdarg.h>
#include <string.h>
#include "dmc/Opt.h"
#include "dmc/char/Ochar.h"
#include "dmc/char/Achar.h"

/// Returns a copy of 'str'.
char *str_new(char *s);

/// Returns a string with the character 'ch'.
char *str_c(char ch);

/// Returns 'strcoll(s1, s2)'.
int str_cmp_locale(char *s1, char *s2);

/// Returns 'strcmp(s1, s2) > 0'.
int str_greater(char *s1, char *s2);

/// Returns 'strcoll(s1, s2) > 0'.
int str_greater_locale(char *s1, char *s2);

/// Returns 'true' if 'str1 == str2'.
int str_eq(char *str1, char *str2);

/// Indicates if 'str' starts with 'substr'.
int str_starts(char *str, char *substr);

/// Indicates if 'str' ends with 'substr'.
int str_ends(char *str, char *substr);

/// Returns first position of 'ch' in 'str' or -1 if 'ch' is missing.
int str_cindex(char *str, char ch);

/// Returns first position of 'ch' in 'str' from start or -1 if 'ch' is missing.
int str_cindex_from(char *str, char ch, int start);

/// Returns first position of 'substr' in 'str' or -1 if 'substr' is missing.
int str_index(char *str, char *substr);

/// Returns first position of 'substr' in 'str' from start or -1 if 'substr' is
/// missing.
int str_index_from(char *str, char *substr, int start);

/// Returns last position of 'ch' in 'str' or -1 if 'ch' is missing.
int str_last_cindex(char *str, char ch);

/// Returns last position of 'substr' in 'str' or -1 if 'substr' is missing.
int str_last_index(char *str, char *substr);

/// str_cat is a string concatenation.
/// Variable argumens must finish with NULL.
char *str_cat(char *s, ...);

/// Returns a substring.
///   - If 'begin' or 'end' are negatives, they are subtracted from 'strlen(s)'.
///   - If 'begin' or 'end' are out of bounds, they are reset to them.
char *str_sub(char *str, int begin, int end);

/// str_left is equals to 'str_sub(str, 0, end)'.
char *str_left(char *str, int end);

/// str_right is equals to 'str_sub(str, begin, strlen(s))'.
char *str_right(char *str, int begin);

/// Returns a new string in reverse order
char *str_reverse(char *str);

/// Returns a new string removing spaces (ch <= ' ') at left.
char *str_ltrim(char *str);

/// Returns a new string removing spaces (ch <= ' ') at right.
char *str_rtrim(char *str);

/// Returns a new string removing spaces (ch <= ' ') at left and right.
char *str_trim(char *str);

/// Splits 'str' in an AStr.
/// For example (using ';' as separator):
///   "" -> [""]
///   ";" -> ["", ""]
///   "a;" -> [a, ""]
///   "a;bc" -> ["a", "bc"]
Achar *str_csplit(char *str, char sep);

/// str_csplit_trim is similar to <tt>str_csplit</tt> but trimming elements.
Achar *str_csplit_trim(char *str, char sep);

/// Splits 'str' in an AStr.
/// For example (using ";" as separator):
///   "" -> [""]
///   ";" -> ["", ""]
///   "a;" -> [a, ""]
///   "a;bc" -> ["a", "bc"]
/// If 'sep' is "" return all runes of 'str'.
Achar *str_split(char *str, char *sep);

/// str_split_trim is similar to <tt>str_split</tt> but trimming elements.
Achar *str_split_trim(char *str, char *sep);

/// Joins elements of 'a', separated by 'sep'.
/// 'a' is Arr<char>.
char *str_cjoin(Achar *a, char sep);

/// Joins elements of 'a', separated by 'sep'.
/// 'a' is Arr<char>.
char *str_join(Achar *a, char *sep);

/// Returns a new string replacing 'old' by 'new' in 's'.
char *str_creplace(char *s, char old, char new);

/// Returns a new string replacing 'old' by 'new' in 's'.
/// If 'old' is "", it does nothing.
char *str_replace(char *s, char *old, char *new);

/// Returns a string with format similar to 'vprintf'.
char *str_vf(char *format, va_list args);

/// Returns a string with format similar to 'printf'.
char *str_f(char *format, ...);

/// Returns utf8 caracter number or -1 if 's' is not a valid utf8 string.
int str_runes(char *s);

/// Reads next rune of 's'.
/// If there are no more runes or it fails, 'rune' is equals to "".
/// Example:
///   char *tx = "a text";
///   char *rune;
///   char *rest = str_next_rune(&rune, tx);
///   while (*rune) {
///     puts(rune);
///     rest = str_next_rune(&rune, rest);
///   }
char *str_next_rune(char **rune, char *s);

/// Codifies a string in Unicode. Returns an Opt[unicode *]
Opt *str_to_unicode(char *s);

/// Decodifies a string from Unicode.
Ochar *str_from_unicode(unsigned *u);

/// Decodifies a string from ISO-8859-1.
char *str_from_iso(char *s);

/// Returns 's' converted to uppercase. It is necessary set language previously.
/// For example:
///   sys_locale("es_ES.utf8");
///   char *s = str_to_upper_new("cañón");
///   puts(s);
/// This function can fail if s is not a valid utf-8 string.
char *str_to_upper (char *s);

/// Returns 's' converted to lowercase. It is necessary set language previously.
/// For example:
///   sys_locale("es_ES.utf8");
///   char *s = str_to_lower_new("cañón");
///   puts(s);
/// This function can fail if s is not a valid utf-8 string.
char *str_to_lower (char *s);

/// Replaces " by \" and \ by \\ and insert the result inside quotes.
char *str_to_escape (char *s);

/// Restores the string escaped with 'escape'. If 's' does not come from
/// 'escape' the result is indefined.
char *str_from_escape (char *s);

#endif
