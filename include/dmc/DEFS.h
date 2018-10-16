// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef DMC_DEFS_H
  #define DMC_DEFS_H

#define REPEAT(n) { \
  int __i = (n) + 1; \
  while (--__i) {

#define _REPEAT }}

#define RANGE0(i, end) { \
  int __end = end; \
  int i = -1; \
  while (++i < __end) {

#define RANGE(i, begin, end) { \
  int __end = end; \
  int i = (begin) - 1; \
  while (++i < __end) {

#define _RANGE }}

#define IEACH(a, n) { \
  Iarr *__a = a; \
  int *__p = iarr_start(__a); \
  int *__pend = iarr_end(__a); \
  int n; \
  while(__p < __pend) { \
    n = *__p++;

#define IEACH_IX(a, n, ix) { \
  Iarr *__a = a; \
  int *__p = iarr_start(__a); \
  int *__pend = iarr_end(__a); \
  int n; \
  int ix = -1; \
  while(__p < __pend) { \
    ++ix; \
    n = *__p++;

#define DEACH(a, n) { \
  Darr *__a = a; \
  double *__p = darr_start(__a); \
  double *__pend = darr_end(__a); \
  double n; \
  while(__p < __pend) { \
    n = *__p++;

#define DEACH_IX(a, n, ix) { \
  Darr *__a = a; \
  double *__p = darr_start(__a); \
  double *__pend = darr_end(__a); \
  double n; \
  int ix = -1; \
  while(__p < __pend) { \
    ++ix; \
    n = *__p++;

#define EACH(a, type, e) { \
  Arr *__a = (Arr *)a; \
  void **__p = arr_start(__a); \
  void **__pend = arr_end(__a); \
  type *e; \
  while(__p < __pend) { \
    e = *__p++;

#define EACH_IX(a, type, e, i) { \
  Arr *__a = (Arr *)a; \
  void **__p = arr_start(__a); \
  void **__pend = arr_end(__a); \
  type *e; \
  int i = -1; \
  while(__p < __pend) { \
    ++i; \
    e = *__p++;

#define _EACH }}

#define FCOPY void *(*)(void *)

#define FGREATER int (*)(void *, void *)

#define FPROC void (*)(void *)

#endif
