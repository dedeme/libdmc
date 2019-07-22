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
