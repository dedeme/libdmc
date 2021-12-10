// Copyright 01-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/List.h"
#include "dmc/std.h"

List *list_new(void) {
  List *this = MALLOC(List);
  this->next = NULL;
  this->value = NULL;
  return this;
}

int list_size(List *this) {
  int c = 0;
  while (this->next) {
    this = this->next;
    ++c;
  }
  return c;
}

List *list_tail(List *this) {
  if (this->next) {
    return this->next;
  }
  return FAIL("this is empty");
}

/// If "this" is not empty, throws an error
void *list_head (List *this) {
  if (this->next) {
    return this->value;
  }
  return FAIL("this is empty");
}

Opt *list_get (List *this, int ix) {
  int c = 0;
  while (c < ix && this->next) {
    ++c;
    this = this->next;
  }
  if (this->next) {
    return opt_mk_some(this->value);
  }
  return opt_mk_none();
}

int list_empty(List *this) {
  return !this->next;
}

List *list_cons(List *old, void *o) {
  List *this = MALLOC(List);
  this->next = old;
  this->value = o;
  return this;
}

List *list_cat(List *this, List *l) {
  List *th = list_reverse(this);
  List *r = l;
  while (th->next) {
    r = list_cons(r, th->value);
    th = th->next;
  }
  return r;
}

List *list_reverse(List *this) {
  List *r = list_new();
  while(this->next) {
    r = list_cons(r, this->value);
    this = this->next;
  }
  return r;
}

List *list_sort (List *this, int (*greater)(void *e1, void *e2)){
  Arr *a = list_to_arr(this);
  arr_sort(a, greater);
  return list_from_arr(a);
}

int list_all (List *this, int (*pred)(void *e)) {
  while(this->next) {
    if (!pred(this->value)) return 0;
    this = this->next;
  }
  return 1;
}

int list_any (List *this, int (*pred)(void *e)) {
  while(this->next) {
    if (pred(this->value)) return 1;
    this = this->next;
  }
  return 0;
}

int list_index (List *this, int (*pred)(void *e)) {
  int c = 0;
  while (this->next) {
    if (pred(this->value)) return c;
    ++c;
    this = this->next;
  }
  return -1;
}

int list_last_index (List *this, int (*pred)(void *e)) {
  int r = -1;
  int c = 0;
  while (this->next) {
    if (pred(this->value)) r = c;
    ++c;
    this = this->next;
  }
  return r;
}

Opt *list_find(List *this, int (*pred)(void *e)) {
  while (this->next) {
    if (pred(this->value)) return opt_mk_some(this->value);
    this = this->next;
  }
  return opt_mk_none();
}

Opt *list_find_last(List *this, int (*pred)(void *e)) {
  Opt *r = opt_mk_none();
  while (this->next) {
    if (pred(this->value)) r = opt_mk_some(this->value);
    this = this->next;
  }
  return r;
}

List *list_filter (List *this, int (*pred)(void *e)) {
  List *r = list_new();
  while (this->next) {
    if (pred(this->value)) r = list_cons(r, this->value);
    this = this->next;
  }
  return list_reverse(r);
}

List *list_take (List *this, int n) {
  List *r = list_new();
  int c = 0;
  while (this->next && c < n) {
    r = list_cons(r, this->value);
    ++c;
    this = this->next;
  }
  return list_reverse(r);
}

List *list_takef (List *this, int (*pred)(void *e)) {
  List *r = list_new();
  while (this->next && pred(this->value)) {
    r = list_cons(r, this->value);
    this = this->next;
  }
  return list_reverse(r);
}

List *list_drop (List *this, int n) {
  int c = 0;
  while (this->next && c < n) {
    ++c;
    this = this->next;
  }
  List *r = list_new();
  while (this->next) {
    r = list_cons(r, this->value);
    this = this->next;
  }
  return list_reverse(r);
}

List *list_dropf (List *this, int (*pred)(void *e)) {
  while (this->next && pred(this->value)) {
    this = this->next;
  }
  List *r = list_new();
  while (this->next) {
    r = list_cons(r, this->value);
    this = this->next;
  }
  return list_reverse(r);
}

List *list_map (List *this, void *(*converter)(void *e)) {
  List *r = list_new();
  while(this->next) {
    r = list_cons(r, converter(this->value));
    this = this->next;
  }
  return list_reverse(r);
}

List *list_map2 (List *this, void *(*conv1)(void *e), void *(*conv2)(void *e)) {
  List *r = list_new();
  if (this->next) {
    r = list_cons(r, conv1(this->value));
    this = this->next;
  }
  while(this->next) {
    r = list_cons(r, conv2(this->value));
    this = this->next;
  }
  return list_reverse(r);
}

List *list_zip (List *l1, List *l2, void *(*converter)(void *e1, void *e2)) {
  List *r = list_new();
  while (l1->next && l2->next) {
    r = list_cons(r, converter(l1->value, l2->value));
    l1 = l1->next;
    l2 = l2->next;
  }
  return list_reverse(r);
}

List *list_zip3 (
  List *l1, List *l2, List *l3, void*(*converter)(void*e1, void*e2, void*e3)
) {
  List *r = list_new();
  while (l1->next && l2->next && l3->next) {
    r = list_cons(r, converter(l1->value, l2->value, l3->value));
    l1 = l1->next;
    l2 = l2->next;
    l3 = l3->next;
  }
  return list_reverse(r);
}

Arr *list_to_arr (List *this) {
  Arr *r = arr_new();
  while (this->next) {
    arr_push(r, this->value);
    this = this->next;
  }
  return r;
}

List *list_from_arr (Arr *a) {
  List *r = list_new();
  void **p = a->es;
  while (p < a->end) {
    r = list_cons(r, *p++);
  }
  return list_reverse(r);
}

char *list_to_js(List *this, char *(*to)(void *)) {
  Achar *r = achar_new();
  while (this->next) {
    achar_push(r, to(this->value));
    this = this->next;
  }
  return js_wa(r);
}

List *list_from_js(char *js, void *(*from)(char *)) {
  List *r = list_new();
  Achar *a = js_ra(js);
  char **p = a->es;
  while (p < a->end) {
    r = list_cons(r, from(*p++));
  }
  return list_reverse(r);
}

