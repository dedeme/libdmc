// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/List.h"
#include <gc.h>
#include "dmc/DEFS.h"
#include "dmc/str.h"
#include "dmc/exc.h"

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

List *list_tail(List *this) {
  if (this->next) {
    return this->next;
  }
  THROW exc_illegal_argument ("this", "not empty", "empty") _THROW
  return NULL;
}

/// If "this" is not empty, throws an error
void *list_head (List *this) {
  if (this->next) {
    return this->value;
  }
  THROW exc_illegal_argument ("this", "not empty", "empty") _THROW
  return NULL;
}

inline
bool list_empty(List *this) {
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


/**/typedef struct {
/**/    List *l;
/**/} PList;
/**/static FNP (has_next, PList, l) { return (bool)(l->l->next); }_FN
/**/static FNM (next, PList, l) {
/**/  void *r = l->l->value;
/**/  l->l = l->l->next;
/**/  return r;
/**/}_FN
It *list_to_it (List *this) {
  PList *pl = MALLOC(PList);
  pl->l = this;
  return it_new(pl, has_next, next);
}

List *list_from_it (It *it) {
  List *r = list_new();
  while (it_has_next(it)) {
    r = list_cons(r, it_next(it));
  }
  return r;
}

