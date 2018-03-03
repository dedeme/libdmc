// Copyright 04-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

///String utilities

#ifndef DM_STR_H
  #define DM_STR_H

#include <stdarg.h>
#include "It.h"
#include "Arr.h"

/// Useful with second order functions
bool str_eq (void *str1, void  *str2);

/// Indicates if a str starts with a substr.<br>
/// Returns 0 (No) or 1 (Yes).
bool str_starts (char *str, char  *substr);

/// Indicates if a str ends with a substr.<br>
/// Returns 0 (No) or 1 (Yes).
bool str_ends (char *str, char  *substr);

/// Returns first position of 'ch' in 'str' or -1 if 'ch' is missing.
int str_cindex (char *str, char ch);

/// Returns first position of 'substr' in 'str' or -1 if 'substr' is missing.
int str_index (char *str, char *substr);

/// Returns last position of 'ch' in 'str' or -1 if 'ch' is missing.
int str_last_cindex (char *str, char ch);

/// Returns last position of 'substr' in 'str' or -1 if 'substr' is missing.
int str_last_index (char *str, char *substr);

/// Returns a copy of 'str'.
char *str_copy (char *source);

/// String concatenation.<br>
/// Variable argumens must finish with NULL.<br>
char *str_cat (char *s, ...);

/// Substring.<br>
/// If 'begin' or 'end' are negatives they are subtracted from 'strlen(s)'.
/// <br>
/// If 'begin' or 'end' are out of bounds function returns "".
char *str_sub (char *str, int begin, int end);

/// Equals to str_sub(s, begin, strlen(s))
char *str_sub_end(char *str, int begin);

/// Returns a new string removing spaces (' ', '\\f', '\\n', '\\r',
/// '\\t' and '\\v') at left.
char *str_ltrim (char *str);

/// Returns a new string removing spaces (' ', '\\f', '\\n', '\\r',
/// '\\t' and '\\v') at right.
char *str_rtrim (char *str);

/// Returns a new allocated string removing spaces (' ', '\\f', '\\n', '\\r',
/// '\\t' and '\\v') at left and right.
char *str_trim (char *str);

/// Splits 'str' in an Arr.<br>
/// For example (using ';' as separator):
///   "" -> []
///   ";" -> [""]
///   "a;" -> [a]
///   "a;bc" -> ["a", "bc"]
Arr/*char*/ *str_csplit (char *str, char sep);

/// Similar to <tt>str_csplit</tt> but trimming array elements
Arr/*char*/ *str_csplit_trim (char *str, char sep);

/// Splits 'str' in an Arr.<br>
/// For example (using ";" as separator):
///   "" -> []
///   ";" -> [""]
///   "a;" -> [a]
///   "a;bc" -> ["a", "bc"]
/// If 'sep' is '' return will have only one string equals to 'str'.
Arr/*char*/ *str_split (char *str, char *sep);

/// Similar to <tt>str_split</tt> but trimming array elements
Arr/*char*/ *str_split_trim (char *str, char *sep);

/// Joins elements of 'it', separated by 'sep'.
char *str_cjoin (It/*char*/ *it, char sep);

/// Joins elements of 'it', separated by 'sep'.
char *str_join (It/*char*/ *it, char *sep);

/// Replace 'old' by 'new' in 's'. 's' is unmodified
char *str_creplace(char *s, char old, char new);

/// Replace 'old' by 'new' in 's'. 's' is unmodified.<br>
/// If 'old' is "", returns a copy of 's'.
char *str_replace(char *s, char *old, char *new);

/// Returns a string with format similar to 'printf'
char *str_printf(char *format, ...);

/// Returns a string with format similar to 'vprintf'
char *str_vprintf(char *format, va_list args);

/// Returns utf8 caracter number or -1 if 's' is not a valid ut8 string.
int str_runes (char *s);

/// Reads first rune of 's'.
///   s      : Text to read
///   return : Next rune or "" if end of 's' is reached
char *str_next_rune (char *s);

/// Codifies a string in Unicode. Returns NULL if there is an error
unsigned *str_to_unicode (char *s);

/// Decodifies a string from Unicode. Returns NULL if there is an error
char *str_from_unicode (unsigned *u);

/// Decodifies a string from ISO-8859-1
char *str_from_iso (char *s);

/// Returns 's' converted to uppercase. It is necessary set language previously.
/// For example:
///   sys_locale("es_ES.utf8");
///   printf("->%s\n", str_to_upper ("cañón"));
char *str_to_upper (char *s);

/// Returns 's' converted to lowercase. It is necessary set language previously.
/// For example:
///   sys_locale("es_ES.utf8");
///   printf("->%s\n", str_to_lower ("CAÑON"));
char *str_to_lower (char *s);

/// Replaces " by \" and \ by \\ and insert the result inside quotes.
char *str_to_escape (char *s);

/// Restores the string escaped with 'escape'. If 's' does not come from
/// 'escape' the result is indefined.
char *str_from_escape (char *s);

#endif
