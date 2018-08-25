// Copyright 1-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "dmc/List.h"
#include "dmc/exc.h"
#include "dmc/str.h"
#include "dmc/It.h"
#include "dmc/Arr.h"
#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"
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

size_t list_count(List *this) {
  XNULL(this)

  size_t c = 0;
  while (this->next) {
    this = this->next;
    ++c;
  }
  return c;
}

List *list_tail(List *this) {
  XNULL(this)
  if (this->next) {
    return this->next;
  }
  THROW(exc_illegal_argument_t)
    exc_illegal_argument ("this", "not empty", "empty")
  _THROW
  return NULL;
}

/// If "this" is not empty, throws an error
void *list_head (List *this) {
  XNULL(this)
  if (this->next) {
    return this->value;
  }
  THROW(exc_illegal_argument_t)
    exc_illegal_argument ("this", "not empty", "empty")
  _THROW
  return NULL;
}

inline
bool list_empty(List *this) {
  XNULL(this)
  return !this->next;
}

List *list_cons(List *old, void *o) {
  XNULL(old)
  XNULL(o)

  List *this = MALLOC(List);
  this->next = old;
  this->value = o;
  return this;
}

List *list_cat(List *this, List *l) {
  XNULL(this)
  XNULL(l)

  List *th = list_reverse(this);
  List *r = l;
  while (th->next) {
    r = list_cons(r, th->value);
    th = th->next;
  }
  return r;
}

List *list_reverse(List *this) {
  XNULL(this)

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
/**/} list_PList;
/**/static FNM (next, list_PList, pl) {
/**/  if (list_empty(pl->l)) return NULL;
/**/  void *r = pl->l->value;
/**/  pl->l = pl->l->next;
/**/  return r;
/**/}_FN
It *list_to_it (List *this) {
  XNULL(this)

  list_PList *pl = MALLOC(list_PList);
  pl->l = this;
  return it_new(pl, next);
}

List *list_from_it (It *it) {
  XNULL(it)

  List *r = list_new();
  while (it_has_next(it)) {
    r = list_cons(r, it_next(it));
  }
  return r;
}

Arr *list_to_arr (List *this) {
  XNULL(this)

  Arr *r = arr_new();
  EACHL(this, void, e) {
    arr_add(r, e);
  }_EACH
  return r;
}

List *list_from_arr (Arr *a) {
  XNULL(a)

  List *r = list_new();
  EACHR(a, void, e) {
    r = list_cons(r, e);
  }_EACH
  return r;
}

Json *list_to_json(List *this, Json *(*to)(void *)) {
  XNULL(this)

  Ajson *r = ajson_new();
  EACHL(this, void, e) {
    ajson_add(r, to(e));
  }_EACH
  return json_warray(r);
}

List *list_from_json(Json *js, void *(*from)(Json *)) {
  XNULL(js)

  List *r = list_new();
  EACHR(json_rarray(js), Json, j) {
    r = list_cons(r, from(j));
  }_EACH
  return r;
}

