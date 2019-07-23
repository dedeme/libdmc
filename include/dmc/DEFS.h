// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Global definitions.

#ifndef DMC_DEFS_H
  #define DMC_DEFS_H

typedef struct js_Js Js;

typedef struct gc_Gc Gc;

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

/// Iterates over an 'Arr'.
///   a   : An Arr *
///   type: Element type without pointer sign (*)
///   e   : An element of type 'type'
/// For example:
///   EACH(a, char, s) {
///     printf("%s\n", s);
///   } _EACH
#define EACH(a, type, e) { \
  Arr *__a = (Arr *)a; \
  void **__p = arr_start(__a); \
  void **__pend = arr_end(__a); \
  type *e; \
  while(__p < __pend) { \
    e = *__p++;

/// Iterates over an 'Arr'. You can access to element index with i.
///   a   : An Arr *
///   type: Element type without pointer sign (*)
///   e   : An element of type 'type'
///   i   : Element index.
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

/// Iterates over an Iarr. See EACH.
#define IEACH(a, n) { \
  Iarr *__a = a; \
  int *__p = iarr_start(__a); \
  int *__pend = iarr_end(__a); \
  int n; \
  while(__p < __pend) { \
    n = *__p++;

/// Iterates over an Iarr. See EACH_IX.
#define IEACH_IX(a, n, i) { \
  Iarr *__a = a; \
  int *__p = iarr_start(__a); \
  int *__pend = iarr_end(__a); \
  int n; \
  int i = -1; \
  while(__p < __pend) { \
    ++i; \
    n = *__p++;

/// Iterates over a Darr. See EACH.
#define DEACH(a, n) { \
  Darr *__a = a; \
  double *__p = darr_start(__a); \
  double *__pend = darr_end(__a); \
  double n; \
  while(__p < __pend) { \
    n = *__p++;

/// Iterates over a Darr. See EACH_IX
#define DEACH_IX(a, n, i) { \
  Darr *__a = a; \
  double *__p = darr_start(__a); \
  double *__pend = darr_end(__a); \
  double n; \
  int i = -1; \
  while(__p < __pend) { \
    ++i; \
    n = *__p++;

/// Iterates over an It. See EACH.
#define EACHI(it, type, e) { \
  It *__it = (It *)it; \
  type *e; \
  while (it_has_next(__it)) { \
    e = it_next(__it);

/// Iterates over an It. See EACH_IX.
#define EACHI_IX(it, type, e, i) { \
  It *__it = (It *)it; \
  int i = -1; \
  type *e; \
  while (it_has_next(__it)) { \
    ++i; \
    e = it_next(__it);

/// Iterates over an 'Arr' in reverse order.
///   a   : An Arr *
///   type: Element type without pointer sign (*)
///   e   : An element of type 'a'
/// For example:
///   EACHR(a, char, s) {
///     printf("%s\n", s);
///   } _EACH
#define EACHR(a, type, e) { \
  Arr *__a = (Arr *)a; \
  void **__pstart = arr_start(__a); \
  void **__p = arr_end(__a); \
  type *e; \
  while(__p > __pstart) { \
    e = *(--__p);

/// Iterates over an 'Arr' in reverse order. You can access to the 'element'
/// index with i.
///   a   : An Arr *
///   type: Element type without pointer sign (*)
///   e   : An element of type 'a'
///   i   : Index of element 'e'
/// For example:
///   EACHR(a, char, s) {
///     printf("[%d] -> %s\n", i, s);
///   } _EACH
#define EACHR_IX(a, type, e) { \
  Arr *__a = (Arr *)a; \
  void **__pstart = arr_start(__a); \
  void **__p = arr_end(__a); \
  int i = __p - __pstart; \
  type *e; \
  while(__p > __pstart) { \
    --i; \
    e = *(--__p);

/// Iterates over an Iarr. See EACH.
#define IEACHR(a, n) { \
  Iarr *__a = (Iarr *)a; \
  int *__pstart = iarr_start(__a); \
  int *__p = iarr_end(__a); \
  int n; \
  while(__p > __pstart) { \
    n = *(--__p);

/// Iterates over an Iarr. See EACH_IX.
#define IEACHR_IX(a, n, i) { \
  Iarr *__a = (Iarr *)a; \
  int *__pstart = iarr_start(__a); \
  int *__p = iarr_end(__a); \
  int i = __p - __pstart; \
  int n; \
  while(__p > __pstart) { \
    --i; \
    n = *(--__p);

/// Iterates over a Darr. See EACH.
#define DEACHR(a, n) { \
  Darr *__a = (Darr *)a; \
  double *__pstart = darr_start(__a); \
  double *__p = darr_end(__a); \
  double n; \
  while(__p > __pstart) { \
    n = *(--__p);

/// Iterates over a Darr. See EACH_IX
#define DEACHR_IX(a, n, i) { \
  Darr *__a = (Darr *)a; \
  double *__pstart = darr_start(__a); \
  double *__p = darr_end(__a); \
  int i = __p - __pstart; \
  double n; \
  while(__p > __pstart) { \
    --i; \
    n = *(--__p);

/// Finalizes an EACHL, EACH or a EACHR
#define _EACH }}

///
typedef void *(*FCOPY)(Gc *gc, void *);

///
typedef int (*FCMP)(void *, void *);

///
typedef void (*FPROC)(void *);

///
typedef int (*FPRED)(void *);

///
typedef Js *(*FTO)(Gc *gc, void *);

///
typedef void *(*FFROM)(Gc *gc, Js *);

/// Throws an Exception. Example
///   THROW(exc_io_t)
///     "Working directory not found: %s", strerror(errno)
///   _THROW
/// Parameters:
///   type: char *. Identifier of exception type
#define THROW(type) { \
  exc_throw((type), str_f(gc_new(),

///
#define _THROW ), __FILE__, (char *)__func__, __LINE__);}

/// Example:
///   EXC_GENERIC("Fail")
#define EXC_GENERIC(msg) \
  THROW(exc_generic_t) msg _THROW

/// Throw a range exception if v < 0 or v > 32.
///   value: The index out of range (< begin and > end)
///   min  : Lower limit inclusive
///   max  : Upper limit inclusive
/// Example:
///   EXC_RANGE(v, 0, 23)
#define EXC_RANGE(value, min, max) { \
    int __v = value; \
    if (__v < (min) || __v > (max)) \
      THROW(exc_range_t) \
        "--- Index out of range: %d out of [%d - %d]", __v, (min), (max) \
      _THROW \
  }

/// Illegal argument of variable.
///   var     : Name of wrong argument
///   expected: Value expected
///   actual  : Actual value
/// Example:
///   EXC_ILLEGAL_ARGUMENT("my_var", "a value", "another value")
#define EXC_ILLEGAL_ARGUMENT(var, expected, actual) \
  THROW(exc_illegal_argument_t) \
    "--- Illegal argument: Variable '%s'\nExpected: %s\nActual: %s", \
    (var), (expected), (actual) \
  _THROW

/// Example:
///   EXC_ILLEGAL_STATE("Variable not set")
#define EXC_ILLEGAL_STATE(cause) \
  THROW(exc_illegal_state_t) "--- Illegal state: %s", (cause) _THROW

/// Example:
///   EXC_IO("File not found")
#define EXC_IO(cause) \
  THROW(exc_io_t) "--- Io error: %s", (cause) _THROW

#define GC_NEW Gc *gc = gc_new();

#define GCL_NEW Gc *gcl = gc_new();

#define GC_FREE gc_free(gc);

#define GCL_FREE gc_free(gcl);

#define GC_MALLOC (type) gc_add(gc, malloc(sizeof(type)))

#define GC_RET (value) gc_clean(gc, (value));

#define GCL_RET (value) gc_clean(gcl, (value));


/// CGI_GET read a 'field' of 'map'. If 'field' is not found produce a
/// ILLEGAL_ARGUMENT exception, otherwise returns its value in 'type var'
/// using 'fun'.
/// <table><tr><td>
///   type : type of var
///   var  : name of new variable. It shuld be free.
///   fun  : function [type fn (Js *)] to translate 'Js' to 'type'
///   map  : A Map[Js]
///   field: field key. Its value must be const char *
/// </table>
/// Examples:
/// <table><tr><td>
///   CGI_GET(int, i, js_ri, m, "index")
///   CGI_GET(char *, a, js_rs, m, "value")
///   CGI_GET(Arr *, a, js_ra, m, "values")
/// </table>
#define CGI_GET(type, var, fun, map, field) \
  type var; \
  { \
    Opt *js = map_get((map), (field)); \
    if (opt_is_empty(js))  \
      EXC_ILLEGAL_ARGUMENT(field, "Map key", "Key not found") \
    var = fun(opt_get(js)); \
  }

/// CGI_GET_GC read a 'field' of 'map'. If 'field' is not found produce a
/// ILLEGAL_ARGUMENT exception, otherwise returns its value in 'type var'
/// using 'fun'.
/// <table><tr><td>
///   gc   : Garbage collector to use with fun
///   type : type of var
///   var  : name of new variable. It shuld be free.
///   fun  : function [type fn (Gc *, Js *)] to translate 'Js' to 'type'
///   map  : A Map[Js]
///   field: field key. Its value must be const char *
/// </table>
/// Examples:
/// <table><tr><td>
///   CGI_GET(int, i, js_ri, m, "index")
///   CGI_GET(char *, a, js_rs, m, "value")
///   CGI_GET(Arr *, a, js_ra, m, "values")
/// </table>
#define CGI_GET_GC(gc, type, var, fun, map, field) \
  type var; \
  { \
    Opt *js = map_get((map), (field)); \
    if (opt_is_empty(js))  \
      EXC_ILLEGAL_ARGUMENT(field, "Map key", "Key not found") \
    var = fun((gc), opt_get(js)); \
  }

/// Calls CGI_GET with 'var' as 'int'.
#define CGI_GET_BOOL(var, map, field) \
  CGI_GET(int, var, js_rb, map, field)

/// Calls CGI_GET with 'var' as 'int'.
#define CGI_GET_INT(var, map, field) \
  CGI_GET(int, var, js_ri, map, field)

/// Calls CGI_GET with 'var' as 'double'.
#define CGI_GET_DOUBLE(var, map, field) \
  CGI_GET(double, var, js_rd, map, field)

/// Calls CGI_GET with 'var' as 'char *'.
#define CGI_GET_STR(gc, var, map, field) \
  CGI_GET_GC(gc, char *, var, js_rs, map, field)

/// Calls CGI_GET with 'var' as 'Arr[Js]'.
#define CGI_GET_ARR(gc, var, map, field) \
  CGI_GET_GC(gc, Arr *, var, js_ra, map, field)

/// Calls CGI_GET with 'var' as 'Map[Js]'.
#define CGI_GET_MAP(var, map, field) \
  CGI_GET_GC(gc, Map *, var, js_ro, map, field)

#endif
