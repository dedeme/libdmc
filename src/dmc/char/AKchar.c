// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/char/AKchar.h"
#include <string.h>
#include <stdarg.h>

AKchar *aKchar_new (void) {
  return (AKchar *)arr_new();
}

AKchar *aKchar_bf_new (int buffer) {
  return (AKchar *)arr_bf_new(buffer);
}

AKchar *aKchar_new_from (Kchar *e, ...) {
  va_list args;
  void *tmp;

  AKchar *this = aKchar_new();
  aKchar_push(this, e);

  va_start(args, e);
  tmp = va_arg(args, Kchar *);
  while (tmp) {
    aKchar_push(this, tmp);
    tmp = va_arg(args, Kchar *);
  }
  va_end(args);

  return this;
}

AKchar *aKchar_new_c (int size, Kchar **es) {
  return (AKchar *)arr_new_c(size, (void **)es);
}

AKchar *aKchar_copy (AKchar *this) {
  return (AKchar *)arr_copy((Arr *)this);
}

int aKchar_size (AKchar *this) {
  return arr_size((Arr *)this);
}

Kchar *aKchar_get (AKchar *this, int ix) {
  return arr_get((Arr *)this, ix);
}

void aKchar_push (AKchar *this, Kchar *e) {
  arr_push((Arr *)this, e);
}

Kchar *aKchar_pop (AKchar *this) {
  return arr_pop((Arr *)this);
}

Kchar *aKchar_peek (AKchar *this) {
  return arr_peek((Arr *)this);
}

void aKchar_set (AKchar *this, int ix, Kchar *e) {
  arr_set((Arr *)this, ix, e);
}

void aKchar_insert (AKchar *this, int ix, Kchar *e) {
  arr_insert((Arr *)this, ix, e);
}

void aKchar_remove (AKchar *this, int ix) {
  arr_remove((Arr *)this, ix);
}

void aKchar_cat (AKchar *this, AKchar *other) {
  arr_cat((Arr *)this, (Arr *)other);
}

void aKchar_insert_arr (AKchar *this, int ix, AKchar *other) {
  arr_insert_arr((Arr *)this, ix, (Arr *)other);
}

void aKchar_remove_range (AKchar *this, int begin, int end) {
  arr_remove_range((Arr *)this, begin, end);
}

void aKchar_clear (AKchar *this) {
  arr_clear((Arr *)this);
}

void aKchar_reverse (AKchar *this) {
  arr_reverse((Arr *)this);
}

void aKchar_sortf (AKchar *this, int (*greater)(Kchar *e1, Kchar *e2)) {
  arr_sort((Arr *)this, (int(*)(void *, void *))greater);
}

void aKchar_shuffle (AKchar *this) {
  arr_shuffle((Arr *)this);
}

int aKchar_all (AKchar *this, int (*pred)(Kchar *e)) {
  return arr_all((Arr *)this, (int(*)(void *))pred);
}

int aKchar_any (AKchar *this, int (*pred)(Kchar *e)) {
  return arr_any((Arr *)this, (int(*)(void *))pred);
}

int aKchar_index (AKchar *this, int (*pred)(Kchar *e)) {
  return arr_index((Arr *)this, (int(*)(void *))pred);
}

int aKchar_last_index (AKchar *this, int (*pred)(Kchar *e)) {
  return arr_last_index((Arr *)this, (int(*)(void *))pred);
}

OKchar *aKchar_find(AKchar *this, int (*pred)(Kchar *e)) {
  return (OKchar *)arr_find((Arr *)this, (int(*)(void *))pred);
}

OKchar *aKchar_find_last(AKchar *this, int (*pred)(Kchar *e)) {
  return (OKchar *)arr_find_last((Arr *)this, (int(*)(void *))pred);
}

void aKchar_filter_in (AKchar *this, int (*pred)(Kchar *e)) {
  arr_filter_in((Arr *)this, (int(*)(void *))pred);
}

AKchar *aKchar_take (AKchar *this, int n) {
  return (AKchar *)arr_take((Arr *)this, n);
}

AKchar *aKchar_takef (AKchar *this, int (*pred)(Kchar *e)) {
  return (AKchar *)arr_takef((Arr *)this, (int(*)(void *))pred);
}

AKchar *aKchar_drop (AKchar *this, int n) {
  return (AKchar *)arr_drop((Arr *)this, n);
}

AKchar *aKchar_dropf (AKchar *this, int (*pred)(Kchar *e)) {
  return (AKchar *)arr_dropf((Arr *)this, (int(*)(void *))pred);
}

AKchar *aKchar_filter_to (AKchar *this, int (*pred)(Kchar *e)) {
  return (AKchar *)arr_filter_to((Arr *)this, (int(*)(void *))pred);
}

Arr *aKchar_map (AKchar *this, void *(*converter)(Kchar *e)) {
  return arr_map((Arr *)this, (void *(*)(void *))converter);
}

Arr *aKchar_map2 (
  AKchar *this, void *(*conv1)(Kchar *e), void *(*conv2)(Kchar *e)
) {
  return arr_map2((Arr *)this, (void *(*)(void *))conv1, (void *(*)(void *))conv2);
}

Arr *aKchar_zip (
  AKchar *a1, AKchar *a2, void *(*converter)(Kchar *e1, Kchar *e2)
) {
  return arr_zip((Arr *)a1, (Arr *)a2, (void *(*)(void *, void *))converter);
}

Arr*aKchar_zip3 (
  AKchar *a1, AKchar *a2, AKchar *a3,
  void*(*converter)(Kchar*e1, Kchar*e2, Kchar*e3)
){
  return arr_zip3(
    (Arr *)a1, (Arr *)a2, (Arr *)a3,
    (void *(*)(void *, void *, void *))converter
  );
}

AKchar *aKchar_duplicates (
  AKchar *this, int (feq)(Kchar *e1, Kchar *e2)
) {
  return (AKchar *)arr_duplicates((Arr *)this, (int(*)(void *, void *))feq);
}

char *aKchar_to_js (AKchar *this, char *(*to)(Kchar *e)) {
  return arr_to_js((Arr *)this, (char *(*)(void *))to);
}

AKchar *aKchar_from_js (char *js, Kchar *(*from)(char *ejs)) {
  return (AKchar *)arr_from_js(js, (void *(*)(char *))from);
}


//--// Not remove

