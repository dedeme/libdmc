// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Kv/AKv.h"
#include <string.h>
#include <stdarg.h>

AKv *aKv_new (void) {
  return (AKv *)arr_new();
}

AKv *aKv_bf_new (int buffer) {
  return (AKv *)arr_bf_new(buffer);
}

AKv *aKv_new_from (Kv *e, ...) {
  va_list args;
  void *tmp;

  AKv *this = aKv_new();
  aKv_push(this, e);

  va_start(args, e);
  tmp = va_arg(args, Kv *);
  while (tmp) {
    aKv_push(this, tmp);
    tmp = va_arg(args, Kv *);
  }
  va_end(args);

  return this;
}

AKv *aKv_new_c (int size, Kv **es) {
  return (AKv *)arr_new_c(size, (void **)es);
}

AKv *aKv_copy (AKv *this) {
  return (AKv *)arr_copy((Arr *)this);
}

int aKv_size (AKv *this) {
  return arr_size((Arr *)this);
}

Kv *aKv_get (AKv *this, int ix) {
  return arr_get((Arr *)this, ix);
}

void aKv_push (AKv *this, Kv *e) {
  arr_push((Arr *)this, e);
}

Kv *aKv_pop (AKv *this) {
  return arr_pop((Arr *)this);
}

Kv *aKv_peek (AKv *this) {
  return arr_peek((Arr *)this);
}

void aKv_set (AKv *this, int ix, Kv *e) {
  arr_set((Arr *)this, ix, e);
}

void aKv_insert (AKv *this, int ix, Kv *e) {
  arr_insert((Arr *)this, ix, e);
}

void aKv_remove (AKv *this, int ix) {
  arr_remove((Arr *)this, ix);
}

void aKv_cat (AKv *this, AKv *other) {
  arr_cat((Arr *)this, (Arr *)other);
}

void aKv_insert_arr (AKv *this, int ix, AKv *other) {
  arr_insert_arr((Arr *)this, ix, (Arr *)other);
}

void aKv_remove_range (AKv *this, int begin, int end) {
  arr_remove_range((Arr *)this, begin, end);
}

void aKv_clear (AKv *this) {
  arr_clear((Arr *)this);
}

void aKv_reverse (AKv *this) {
  arr_reverse((Arr *)this);
}

void aKv_sort (AKv *this, int (*greater)(Kv *e1, Kv *e2)) {
  arr_sort((Arr *)this, (int(*)(void *, void *))greater);
}

void aKv_shuffle (AKv *this) {
  arr_shuffle((Arr *)this);
}

int aKv_all (AKv *this, int (*pred)(Kv *e)) {
  return arr_all((Arr *)this, (int(*)(void *))pred);
}

int aKv_any (AKv *this, int (*pred)(Kv *e)) {
  return arr_any((Arr *)this, (int(*)(void *))pred);
}

int aKv_index (AKv *this, int (*pred)(Kv *e)) {
  return arr_index((Arr *)this, (int(*)(void *))pred);
}

int aKv_last_index (AKv *this, int (*pred)(Kv *e)) {
  return arr_last_index((Arr *)this, (int(*)(void *))pred);
}

OKv *aKv_find(AKv *this, int (*pred)(Kv *e)) {
  return (OKv *)arr_find((Arr *)this, (int(*)(void *))pred);
}

OKv *aKv_find_last(AKv *this, int (*pred)(Kv *e)) {
  return (OKv *)arr_find_last((Arr *)this, (int(*)(void *))pred);
}

void aKv_filter_in (AKv *this, int (*pred)(Kv *e)) {
  arr_filter_in((Arr *)this, (int(*)(void *))pred);
}

AKv *aKv_take (AKv *this, int n) {
  return (AKv *)arr_take((Arr *)this, n);
}

AKv *aKv_takef (AKv *this, int (*pred)(Kv *e)) {
  return (AKv *)arr_takef((Arr *)this, (int(*)(void *))pred);
}

AKv *aKv_drop (AKv *this, int n) {
  return (AKv *)arr_drop((Arr *)this, n);
}

AKv *aKv_dropf (AKv *this, int (*pred)(Kv *e)) {
  return (AKv *)arr_dropf((Arr *)this, (int(*)(void *))pred);
}

AKv *aKv_filter_to (AKv *this, int (*pred)(Kv *e)) {
  return (AKv *)arr_filter_to((Arr *)this, (int(*)(void *))pred);
}

Arr *aKv_map (AKv *this, void *(*converter)(Kv *e)) {
  return arr_map((Arr *)this, (void *(*)(void *))converter);
}

Arr *aKv_map2 (
  AKv *this, void *(*conv1)(Kv *e), void *(*conv2)(Kv *e)
) {
  return arr_map2((Arr *)this, (void *(*)(void *))conv1, (void *(*)(void *))conv2);
}

Arr *aKv_zip (
  AKv *a1, AKv *a2, void *(*converter)(Kv *e1, Kv *e2)
) {
  return arr_zip((Arr *)a1, (Arr *)a2, (void *(*)(void *, void *))converter);
}

Arr*aKv_zip3 (
  AKv *a1, AKv *a2, AKv *a3,
  void*(*converter)(Kv*e1, Kv*e2, Kv*e3)
){
  return arr_zip3(
    (Arr *)a1, (Arr *)a2, (Arr *)a3,
    (void *(*)(void *, void *, void *))converter
  );
}

AKv *aKv_duplicates (
  AKv *this, int (feq)(Kv *e1, Kv *e2)
) {
  return (AKv *)arr_duplicates((Arr *)this, (int(*)(void *, void *))feq);
}

char *aKv_to_js (AKv *this, char *(*to)(Kv *e)) {
  return arr_to_js((Arr *)this, (char *(*)(void *))to);
}

AKv *aKv_from_js (char *js, Kv *(*from)(char *ejs)) {
  return (AKv *)arr_from_js(js, (void *(*)(char *))from);
}


//--// Not remove

