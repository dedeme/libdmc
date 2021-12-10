// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/TpIntInt/ATpIntInt.h"
#include <string.h>
#include <stdarg.h>

ATpIntInt *aTpIntInt_new (void) {
  return (ATpIntInt *)arr_new();
}

ATpIntInt *aTpIntInt_bf_new (int buffer) {
  return (ATpIntInt *)arr_bf_new(buffer);
}

ATpIntInt *aTpIntInt_new_from (TpIntInt *e, ...) {
  va_list args;
  void *tmp;

  ATpIntInt *this = aTpIntInt_new();
  aTpIntInt_push(this, e);

  va_start(args, e);
  tmp = va_arg(args, TpIntInt *);
  while (tmp) {
    aTpIntInt_push(this, tmp);
    tmp = va_arg(args, TpIntInt *);
  }
  va_end(args);

  return this;
}

ATpIntInt *aTpIntInt_new_c (int size, TpIntInt **es) {
  return (ATpIntInt *)arr_new_c(size, (void **)es);
}

ATpIntInt *aTpIntInt_copy (ATpIntInt *this) {
  return (ATpIntInt *)arr_copy((Arr *)this);
}

int aTpIntInt_size (ATpIntInt *this) {
  return arr_size((Arr *)this);
}

TpIntInt *aTpIntInt_get (ATpIntInt *this, int ix) {
  return arr_get((Arr *)this, ix);
}

void aTpIntInt_push (ATpIntInt *this, TpIntInt *e) {
  arr_push((Arr *)this, e);
}

TpIntInt *aTpIntInt_pop (ATpIntInt *this) {
  return arr_pop((Arr *)this);
}

TpIntInt *aTpIntInt_peek (ATpIntInt *this) {
  return arr_peek((Arr *)this);
}

void aTpIntInt_set (ATpIntInt *this, int ix, TpIntInt *e) {
  arr_set((Arr *)this, ix, e);
}

void aTpIntInt_insert (ATpIntInt *this, int ix, TpIntInt *e) {
  arr_insert((Arr *)this, ix, e);
}

void aTpIntInt_remove (ATpIntInt *this, int ix) {
  arr_remove((Arr *)this, ix);
}

void aTpIntInt_cat (ATpIntInt *this, ATpIntInt *other) {
  arr_cat((Arr *)this, (Arr *)other);
}

void aTpIntInt_insert_arr (ATpIntInt *this, int ix, ATpIntInt *other) {
  arr_insert_arr((Arr *)this, ix, (Arr *)other);
}

void aTpIntInt_remove_range (ATpIntInt *this, int begin, int end) {
  arr_remove_range((Arr *)this, begin, end);
}

void aTpIntInt_clear (ATpIntInt *this) {
  arr_clear((Arr *)this);
}

void aTpIntInt_reverse (ATpIntInt *this) {
  arr_reverse((Arr *)this);
}

void aTpIntInt_sort (ATpIntInt *this, int (*greater)(TpIntInt *e1, TpIntInt *e2)) {
  arr_sort((Arr *)this, (int(*)(void *, void *))greater);
}

void aTpIntInt_shuffle (ATpIntInt *this) {
  arr_shuffle((Arr *)this);
}

int aTpIntInt_all (ATpIntInt *this, int (*pred)(TpIntInt *e)) {
  return arr_all((Arr *)this, (int(*)(void *))pred);
}

int aTpIntInt_any (ATpIntInt *this, int (*pred)(TpIntInt *e)) {
  return arr_any((Arr *)this, (int(*)(void *))pred);
}

int aTpIntInt_index (ATpIntInt *this, int (*pred)(TpIntInt *e)) {
  return arr_index((Arr *)this, (int(*)(void *))pred);
}

int aTpIntInt_last_index (ATpIntInt *this, int (*pred)(TpIntInt *e)) {
  return arr_last_index((Arr *)this, (int(*)(void *))pred);
}

OTpIntInt *aTpIntInt_find(ATpIntInt *this, int (*pred)(TpIntInt *e)) {
  return (OTpIntInt *)arr_find((Arr *)this, (int(*)(void *))pred);
}

OTpIntInt *aTpIntInt_find_last(ATpIntInt *this, int (*pred)(TpIntInt *e)) {
  return (OTpIntInt *)arr_find_last((Arr *)this, (int(*)(void *))pred);
}

void aTpIntInt_filter_in (ATpIntInt *this, int (*pred)(TpIntInt *e)) {
  arr_filter_in((Arr *)this, (int(*)(void *))pred);
}

ATpIntInt *aTpIntInt_take (ATpIntInt *this, int n) {
  return (ATpIntInt *)arr_take((Arr *)this, n);
}

ATpIntInt *aTpIntInt_takef (ATpIntInt *this, int (*pred)(TpIntInt *e)) {
  return (ATpIntInt *)arr_takef((Arr *)this, (int(*)(void *))pred);
}

ATpIntInt *aTpIntInt_drop (ATpIntInt *this, int n) {
  return (ATpIntInt *)arr_drop((Arr *)this, n);
}

ATpIntInt *aTpIntInt_dropf (ATpIntInt *this, int (*pred)(TpIntInt *e)) {
  return (ATpIntInt *)arr_dropf((Arr *)this, (int(*)(void *))pred);
}

ATpIntInt *aTpIntInt_filter_to (ATpIntInt *this, int (*pred)(TpIntInt *e)) {
  return (ATpIntInt *)arr_filter_to((Arr *)this, (int(*)(void *))pred);
}

Arr *aTpIntInt_map (ATpIntInt *this, void *(*converter)(TpIntInt *e)) {
  return arr_map((Arr *)this, (void *(*)(void *))converter);
}

Arr *aTpIntInt_map2 (
  ATpIntInt *this, void *(*conv1)(TpIntInt *e), void *(*conv2)(TpIntInt *e)
) {
  return arr_map2((Arr *)this, (void *(*)(void *))conv1, (void *(*)(void *))conv2);
}

Arr *aTpIntInt_zip (
  ATpIntInt *a1, ATpIntInt *a2, void *(*converter)(TpIntInt *e1, TpIntInt *e2)
) {
  return arr_zip((Arr *)a1, (Arr *)a2, (void *(*)(void *, void *))converter);
}

Arr*aTpIntInt_zip3 (
  ATpIntInt *a1, ATpIntInt *a2, ATpIntInt *a3,
  void*(*converter)(TpIntInt*e1, TpIntInt*e2, TpIntInt*e3)
){
  return arr_zip3(
    (Arr *)a1, (Arr *)a2, (Arr *)a3,
    (void *(*)(void *, void *, void *))converter
  );
}

ATpIntInt *aTpIntInt_duplicates (
  ATpIntInt *this, int (feq)(TpIntInt *e1, TpIntInt *e2)
) {
  return (ATpIntInt *)arr_duplicates((Arr *)this, (int(*)(void *, void *))feq);
}

char *aTpIntInt_to_js (ATpIntInt *this, char *(*to)(TpIntInt *e)) {
  return arr_to_js((Arr *)this, (char *(*)(void *))to);
}

ATpIntInt *aTpIntInt_from_js (char *js, TpIntInt *(*from)(char *ejs)) {
  return (ATpIntInt *)arr_from_js(js, (void *(*)(char *))from);
}


//--// Not remove

