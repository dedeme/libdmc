// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Definitions.

#ifndef DMC_DEFS_H
  #define DMC_DEFS_H

typedef struct js_Js Js;

typedef struct schd_Task SchdTask;

///
#define MALLOC(type) (type *)GC_MALLOC(sizeof(type))

///
#define ATOMIC(size) GC_MALLOC_ATOMIC(size)

///
#define REPEAT(n) { \
  int __i = (n) + 1; \
  while (--__i) {

///
#define _REPEAT }}

///
#define RANGE0(i, end) { \
  int __end = end; \
  int i = -1; \
  while (++i < __end) {

///
#define RANGE(i, begin, end) { \
  int __end = end; \
  int i = (begin) - 1; \
  while (++i < __end) {

///
#define _RANGE }}

/// Iterates over an Iarr. See EACH.
#define IEACH(a, n) { \
  Iarr *__a = a; \
  int *__p = iarr_start(__a); \
  int *__pend = iarr_end(__a); \
  int n; \
  while(__p < __pend) { \
    n = *__p++;

/// Iterates over an Iarr. See EACH_IX.
#define IEACH_IX(a, n, ix) { \
  Iarr *__a = a; \
  int *__p = iarr_start(__a); \
  int *__pend = iarr_end(__a); \
  int n; \
  int ix = -1; \
  while(__p < __pend) { \
    ++ix; \
    n = *__p++;

/// Iterates over an Darr. See EACH.
#define DEACH(a, n) { \
  Darr *__a = a; \
  double *__p = darr_start(__a); \
  double *__pend = darr_end(__a); \
  double n; \
  while(__p < __pend) { \
    n = *__p++;

/// Iterates over an Darr. See EACH_IX
#define DEACH_IX(a, n, ix) { \
  Darr *__a = a; \
  double *__p = darr_start(__a); \
  double *__pend = darr_end(__a); \
  double n; \
  int ix = -1; \
  while(__p < __pend) { \
    ++ix; \
    n = *__p++;

/// Iterates over an 'Arr'. You can access to the 'element' index with _i.
///   a      : An Arr *
///   type   : Element type without pointer sign (*)
///   element: An element of type 'type'
/// For example:
///   EACH(a, char, s) {
///     printf("[%d] -> %s\n", _i, s);
///   } _EACH
#define EACH(a, type, element) { \
  Arr *__arr = (Arr *)a; \
  size_t __size = arr_size(__arr); \
  size_t _i; \
  type *element; \
  for (_i = 0; _i < __size; ++_i) { \
    element = arr_get(__arr, _i);

/// Iterates over an 'Arr'. You can access to the 'element' index with i.
///   a      : An Arr *
///   type   : Element type without pointer sign (*)
///   e: An element of type 'type'
///   i: Element index.
/// For example:
///   EACH(a, char, s, i) {
///     printf("[%d] -> %s\n", i, s);
///   } _EACH
#define EACH_IX(a, type, e, i) { \
  Arr *__a = (Arr *)a; \
  void **__p = arr_start(__a); \
  void **__pend = arr_end(__a); \
  type *e; \
  int i = -1; \
  while(__p < __pend) { \
    ++i; \
    e = *__p++;

/// Iterates over an 'It'. You can access to the 'element' index with _i.
///   a      : An It *
///   type   : Element type without pointer sign (*)
///   element: An element of type 'type'
/// For example:
///   EACH(it, char, s) {
///     printf("[%d] -> %s\n", _i, s);
///   } _EACH
#define EACHI(it, type, element) { \
  It *__it = (It *)it; \
  int _i = -1; \
  type *element; \
  while (it_has_next(__it)) { \
    ++_i; \
    element = it_next(__it);

/// Iterates over a 'List'.
///   list   : An List *
///   type   : Element type without pointer sign (*)
///   element: An element of type 'type'
/// For example:
///   EACHL(list, char, s) {
///     printf("%s\n", s);
///   } _EACH
#define EACHL(list, type, element) { \
  List *_EACHL_list = (List *)list; \
  type *element; \
  while (!list_empty(_EACHL_list)) { \
    element = list_head(_EACHL_list); \
    _EACHL_list = list_tail(_EACHL_list);

/// Iterates over an 'Arr' in reverse order. You can access to the 'element'
/// index with _i.
///   a      : An Arr *
///   fn
///   type   : Element type without pointer sign (*)
///   element: An element of type 'a'
/// For example:
///   EACHR(a, char, s) {
///     printf("[%d] -> %s\n", _i, s);
///   } _EACH
#define EACHR(a, type, element) { \
  Arr *__arr = (Arr *)a; \
  size_t _i = arr_size(__arr); \
  type *element; \
  while (_i) { \
    element = arr_get(__arr, --_i);

/// Finalizes an EACHL, EACH or a EACHR
#define _EACH }}

///
typedef void *(*FCOPY)(void *);

///
typedef int (*FCMP)(void *, void *);

///
typedef void (*FPROC)(void *);

///
typedef int (*FPRED)(void *);

///
typedef Js *(*FTO)(void *);

///
typedef void *(*FFROM)(Js *);

///
typedef void (*FLOOP)(void *, SchdTask *);

/// Macros to manage exceptions. Example:
///   TRY
///     ...
///   CATCH (e)
///     puts(exc_msg(e));
///   _TRY
/// NOTE: <i>CATCH block must return 'void'</i>
#define TRY { \
  jmp_buf *_TRY_buf = MALLOC(jmp_buf); \
  exc_add(_TRY_buf); \
  if (!setjmp(*_TRY_buf)) { \

/// See <a href="#hp:TRY">TRY</a>
#define CATCH(e) ;exc_remove();} else { Exc *e = exc_get();

/// See <a href="#hp:TRY">TRY</a>
#define _TRY ;exc_remove();}}

/// Example
///   THROW(exc_io_t) "Working directory not found: %s", strerror(errno) _THROW
#define THROW(type) exc_throw(type, str_f(

///
#define _THROW ), __FILE__, (char *)__func__, __LINE__);

/// Example:
///   EXC_GENERIC("Fail")
#define EXC_GENERIC(msg) \
  THROW(exc_generic_t) msg _THROW

/// Throw a range exception if v < 0 or v > 32.<br>
/// Example:
///   EXC_RANGE(v, 0, 23)
#define EXC_RANGE(value, min, max) { \
    int __v = value; \
    if (__v < (min) || __v > (max)) \
      THROW(exc_range_t) exc_range((min), (max), __v) _THROW \
  }

/// Example:
///   EXC_ILLEGAL_ARGUMENT("Fail", "a value", "another value")
#define EXC_ILLEGAL_ARGUMENT(msg, expected, actual) \
  THROW(exc_illegal_argument_t) \
    exc_illegal_argument(msg, expected, actual) \
  _THROW

/// Example:
///   EXC_ILLEGAL_STATE("Fail")
#define EXC_ILLEGAL_STATE(msg) \
  THROW(exc_illegal_state_t) exc_illegal_state(msg) _THROW

/// Example:
///   EXC_IO("Fail")
#define EXC_IO(msg) \
  THROW(exc_io_t) exc_io(msg) _THROW

/// CGI_GET read a 'field' of 'map'. If 'field' is not found produce a
/// ILLEGAL_ARGUMENT exception, otherwise returns its value in 'type var'
/// using 'fun'.
/// <table><tr><td>
///   type : type of var
///   var  : name of variable.
///   fun  : function to pass 'Js' to 'type'
///   map  : A Map[Js]
/// </table>
/// Examples:
/// <table><tr><td>
///   CGI_GET(int, index, js_ri, m)
///   CGI_GET(char *, value, js_rs, m)
///   CGI_GET(Arr *, values, js_ra, m)
/// </table>
#define CGI_GET(type, var, fun, map) \
  type var; \
  { \
    Opt *js = map_get(map, #var); \
    if (opt_is_empty(js))  \
      EXC_ILLEGAL_ARGUMENT(#var, "Map key", "Key not found") \
    var = fun(opt_get(js)); \
  }

/// Calls CGI_GET with 'var' as 'int'.
#define CGI_GET_BOOL(var, map) \
  CGI_GET(int, var, js_rb, map)

/// Calls CGI_GET with 'var' as 'int'.
#define CGI_GET_INT(var, map) \
  CGI_GET(int, var, js_ri, map)

/// Calls CGI_GET with 'var' as 'double'.
#define CGI_GET_DOUBLE(var, map) \
  CGI_GET(double, var, js_rd, map)

/// Calls CGI_GET with 'var' as 'char *'.
#define CGI_GET_STR(var, map) \
  CGI_GET(char *, var, js_rs, map)

/// Calls CGI_GET with 'var' as 'Arr[Js]'.
#define CGI_GET_ARR(var, map) \
  CGI_GET(Arr *, var, js_ra, map)

/// Calls CGI_GET with 'var' as 'Map[Js]'.
#define CGI_GET_MAP(var, map) \
  CGI_GET(Map *, var, js_ro, map)

#endif
