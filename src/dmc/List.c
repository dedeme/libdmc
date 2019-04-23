// Copyright 1-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "dmc/List.h"
#include "dmc/exc.h"
#include "dmc/str.h"
#include "dmc/It.h"
#include "dmc/Arr.h"
#include "dmc/Js.h"
#include "dmc/DEFS.h"

struct list_List {
  List *next;
  void *value;
};

List *list_new(void) {
  List *this = MALLOC(List);
  this->next = NULL;
  this->value = NULL;
  return this;
}

int list_count(List *this) {
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
  EXC_ILLEGAL_ARGUMENT("this", "not empty", "empty")
  return NULL;
}

/// If "this" is not empty, throws an error
void *list_head (List *this) {
  if (this->next) {
    return this->value;
  }
  EXC_ILLEGAL_ARGUMENT("this", "not empty", "empty")
  return NULL;
}

Opt *list_get (List *this, int ix) {
  int c = 0;
  while (c < ix && this->next) {
    ++c;
    this = this->next;
  }
  if (this->next) {
    return opt_new(this->value);
  }
  return opt_empty();
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
  List *l = this;
  List *r = list_new();
  while(l->next) {
    r = list_cons(r, l->value);
    l = l->next;
  }
  return r;
}

// -------------------------------------------------------------------------- //
typedef struct {                                                              //
    List *l;                                                                  //
} list_PList;                                                                 //
static Opt *next (list_PList *pl) {                                           //
  if (list_empty(pl->l)) return opt_empty();                                  //
  Opt *r = opt_new(pl->l->value);                                             //
  pl->l = pl->l->next;                                                        //
  return r;                                                                   //
}                                                                             //
// -------------------------------------------------------------------------- //
It *list_to_it (List *this) {
  list_PList *pl = MALLOC(list_PList);
  pl->l = this;
  return it_new(pl, (it_Next)next);
}

List *list_from_it (It *it) {
  List *r = list_new();
  while (it_has_next(it)) {
    r = list_cons(r, it_next(it));
  }
  return r;
}

Arr *list_to_arr (List *this) {
  Arr *r = arr_new();
  EACHL(this, void, e) {
    arr_push(r, e);
  }_EACH
  return r;
}

List *list_from_arr (Arr *a) {
  List *r = list_new();
  EACHR(a, void, e) {
    r = list_cons(r, e);
  }_EACH
  return r;
}

Js *list_to_js(List *this, Js *(*to)(void *)) {
  // Arr[Js]
  Arr *r = arr_new();
  EACHL(this, void, e) {
    arr_push(r, to(e));
  }_EACH
  return js_wa(r);
}

List *list_from_js(Js *js, void *(*from)(Js *)) {
  List *r = list_new();
  EACHR(js_ra(js), Js, j) {
    r = list_cons(r, from(j));
  }_EACH
  return r;
}

