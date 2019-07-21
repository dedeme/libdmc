// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Global definitions

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

/// Macros to manage exceptions. Example:
///   TRY
///     ...
///   CATCH (e)
///     puts(exc_msg(e));
///   _TRY
/// NOTE: <i>CATCH block must return 'void'</i>
#define TRY { \
  jmp_buf *_TRY_buf = malloc(sizeof(jmp_buf)); \
  exc_add(_TRY_buf); \
  if (!setjmp(*_TRY_buf)) { \

/// See <a href="#hp:TRY">TRY</a>
#define CATCH(e) ;exc_remove();} else { Exc *e = exc_get();

/// See <a href="#hp:TRY">TRY</a>
#define _TRY ;exc_remove();} free (_TRY_buf);}

/// Throws an Exception. Example
///   THROW(exc_io_t, gc)
///     "Working directory not found: %s", strerror(errno)
///   _THROW
/// Parameters:
///   type: char *. Identifier of exception type
///   gc: Gc *. This Gc will be freed by 'exc_throw'
#define THROW(type, gc) { \
  Gc *_THROW_gc = (gc); \
  exc_throw(str_new(_THROW_gc, (type)), str_f(_THROW_gc,

///
#define _THROW ), __FILE__, (char *)__func__, __LINE__, _THROW_gc);}

/// Example:
///   EXC_GENERIC("Fail", gc_new())
#define EXC_GENERIC(msg, gc) \
  THROW(exc_generic_t, gc) msg _THROW

/// Throw a range exception if v < 0 or v > 32.<br>
/// Example:
///   EXC_RANGE(v, 0, 23, gc_new())
#define EXC_RANGE(value, min, max, gc) { \
    int __v = value; \
    if (__v < (min) || __v > (max)) \
      THROW(exc_range_t, gc) exc_range(_THROW_gc, (min), (max), __v) _THROW \
  }

/// Example:
///   EXC_ILLEGAL_ARGUMENT("Fail", "a value", "another value", gc_new())
#define EXC_ILLEGAL_ARGUMENT(msg, expected, actual, gc) \
  THROW(exc_illegal_argument_t, gc) \
    exc_illegal_argument(_THROW_gc, msg, expected, actual) \
  _THROW

/// Example:
///   EXC_ILLEGAL_STATE("Fail", gc_new())
#define EXC_ILLEGAL_STATE(msg, gc) \
  THROW(exc_illegal_state_t, gc) exc_illegal_state(_THROW_gc, msg) _THROW

/// Example:
///   EXC_IO("Fail", gc_new())
#define EXC_IO(msg, gc) \
  THROW(exc_io_t, gc) exc_io(_THROW_gc, msg) _THROW

#endif
