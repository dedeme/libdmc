// Copyright 25-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/char/Achar.h"
#include <string.h>
#include <stdarg.h>
#include "dmc/js.h"

Achar *achar_new (void) {
  return (Achar *)arr_new();
}

Achar *achar_bf_new (int buffer) {
  return (Achar *)arr_bf_new(buffer);
}

Achar *achar_new_from (char *e, ...) {
  va_list args;
  void *tmp;

  Achar *this = achar_new();
  achar_push(this, e);

  va_start(args, e);
  tmp = va_arg(args, char *);
  while (tmp) {
    achar_push(this, tmp);
    tmp = va_arg(args, char *);
  }
  va_end(args);

  return this;
}

Achar *achar_new_c (int size, char **es) {
  return (Achar *)arr_new_c(size, (void **)es);
}

Achar *achar_copy (Achar *this) {
  return (Achar *)arr_copy((Arr *)this);
}

int achar_size (Achar *this) {
  return arr_size((Arr *)this);
}

char *achar_get (Achar *this, int ix) {
  return arr_get((Arr *)this, ix);
}

void achar_push (Achar *this, char *e) {
  arr_push((Arr *)this, e);
}

char *achar_pop (Achar *this) {
  return arr_pop((Arr *)this);
}

char *achar_peek (Achar *this) {
  return arr_peek((Arr *)this);
}

void achar_set (Achar *this, int ix, char *e) {
  arr_set((Arr *)this, ix, e);
}

void achar_insert (Achar *this, int ix, char *e) {
  arr_insert((Arr *)this, ix, e);
}

void achar_remove (Achar *this, int ix) {
  arr_remove((Arr *)this, ix);
}

void achar_cat (Achar *this, Achar *other) {
  arr_cat((Arr *)this, (Arr *)other);
}

void achar_insert_arr (Achar *this, int ix, Achar *other) {
  arr_insert_arr((Arr *)this, ix, (Arr *)other);
}

void achar_remove_range (Achar *this, int begin, int end) {
  arr_remove_range((Arr *)this, begin, end);
}

void achar_clear (Achar *this) {
  arr_clear((Arr *)this);
}

void achar_reverse (Achar *this) {
  arr_reverse((Arr *)this);
}

void achar_sort (Achar *this, int (*greater)(char *e1, char *e2)) {
  arr_sort((Arr *)this, (int(*)(void *, void *))greater);
}

void achar_shuffle (Achar *this) {
  arr_shuffle((Arr *)this);
}

int achar_all (Achar *this, int (*pred)(char *e)) {
  return arr_all((Arr *)this, (int(*)(void *))pred);
}

int achar_any (Achar *this, int (*pred)(char *e)) {
  return arr_any((Arr *)this, (int(*)(void *))pred);
}

int achar_index (Achar *this, int (*pred)(char *e)) {
  return arr_index((Arr *)this, (int(*)(void *))pred);
}

int achar_last_index (Achar *this, int (*pred)(char *e)) {
  return arr_last_index((Arr *)this, (int(*)(void *))pred);
}

Ochar *achar_find(Achar *this, int (*pred)(char *e)) {
  return (Ochar *)arr_find((Arr *)this, (int(*)(void *))pred);
}

Ochar *achar_find_last(Achar *this, int (*pred)(char *e)) {
  return (Ochar *)arr_find_last((Arr *)this, (int(*)(void *))pred);
}

void achar_filter_in (Achar *this, int (*pred)(char *e)) {
  arr_filter_in((Arr *)this, (int(*)(void *))pred);
}

Achar *achar_take (Achar *this, int n) {
  return (Achar *)arr_take((Arr *)this, n);
}

Achar *achar_takef (Achar *this, int (*pred)(char *e)) {
  return (Achar *)arr_takef((Arr *)this, (int(*)(void *))pred);
}

Achar *achar_drop (Achar *this, int n) {
  return (Achar *)arr_drop((Arr *)this, n);
}

Achar *achar_dropf (Achar *this, int (*pred)(char *e)) {
  return (Achar *)arr_dropf((Arr *)this, (int(*)(void *))pred);
}

Achar *achar_filter_to (Achar *this, int (*pred)(char *e)) {
  return (Achar *)arr_filter_to((Arr *)this, (int(*)(void *))pred);
}

Arr *achar_map (Achar *this, void *(*converter)(char *e)) {
  return arr_map((Arr *)this, (void *(*)(void *))converter);
}

Arr *achar_map2 (
  Achar *this, void *(*conv1)(char *e), void *(*conv2)(char *e)
) {
  return arr_map2((Arr *)this, (void *(*)(void *))conv1, (void *(*)(void *))conv2);
}

Arr *achar_zip (
  Achar *a1, Achar *a2, void *(*converter)(char *e1, char *e2)
) {
  return arr_zip((Arr *)a1, (Arr *)a2, (void *(*)(void *, void *))converter);
}

Arr*achar_zip3 (
  Achar *a1, Achar *a2, Achar *a3,
  void*(*converter)(char*e1, char*e2, char*e3)
){
  return arr_zip3(
    (Arr *)a1, (Arr *)a2, (Arr *)a3,
    (void *(*)(void *, void *, void *))converter
  );
}

Achar *achar_duplicates (
  Achar *this, int (feq)(char *e1, char *e2)
) {
  return (Achar *)arr_duplicates((Arr *)this, (int(*)(void *, void *))feq);
}

char *achar_to_js (Achar *this) {
  return arr_to_js((Arr *)this, (char *(*)(void *))js_ws);
}

Achar *achar_from_js (char *js) {
  return (Achar *)arr_from_js(js, (void *(*)(char *))js_rs);
}


//--// Not remove

