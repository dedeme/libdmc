// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/It.h"
#include <stdio.h>
#include "dmc/It.h"
#include "dmc/Opt.h"
#include "dmc/str.h"
#include "dmc/Exc.h"
#include "dmc/Arr.h"
#include "dmc/Tp.h"
#include "dmc/DEFS.h"

struct it_It {
  void *o;
  Opt *e;
  Opt *(*next)(void *o);
};

It *it_new(Gc *gc, void *o, Opt *(*next)(void *o)) {
  It *this = gc_add(gc, malloc(sizeof(It)));
  this->o = o;
  this->next = next;
  this->e = next(o);
  return this;
}

// -------------------------------------------------------------------------- //
static Opt *empty_next(void *o) { return opt_empty(); }                       //
// -------------------------------------------------------------------------- //
It *it_empty (Gc *gc) {
  return it_new(gc, NULL, (it_Next)empty_next);
}

// -------------------------------------------------------------------------- //
typedef struct {                                                              //
  void *e;                                                                    //
  int is_first;                                                               //
} it_unary_O;                                                                 //
static Opt *unary_next(it_unary_O *o) {                                       //
  if (o->is_first) {                                                          //
    o->is_first = 0;                                                          //
    return opt_new(o->e);                                                     //                      //
  }                                                                           //
  return opt_empty();                                                         //        //
}                                                                             //
// -------------------------------------------------------------------------- //
It *it_unary (Gc *gc, void *e) {
  it_unary_O *o = gc_add(gc, malloc(sizeof(it_unary_O)));
  o->e = e;
  o->is_first = 1;
  return it_new(gc, o, (it_Next)unary_next);
}

// -------------------------------------------------------------------------- //
typedef struct {                                                              //
  Gc *gc;                                                                     //
  int i;                                                                      //
  int end;                                                                    //
} it_range_O;                                                                 //
static Opt *range_next (it_range_O *o) {                                      //
  int i = (o->i)++;                                                           //
  if (i >= o->end) {                                                          //
    return opt_empty();                                                       //
  }                                                                           //
  int *r = gc_add(o->gc, malloc(sizeof(int)));                                //
  *r = i;                                                                     //
  return opt_new(r);                                                          //
}                                                                             //
// -------------------------------------------------------------------------- //
It *it_range (Gc *gc, int begin, int end) {
  it_range_O *o = gc_add(gc, malloc(sizeof(it_range_O)));
  o->gc = gc;
  o->i = begin;
  o->end = end;
  return it_new(gc, o, (it_Next)range_next);
}

It *it_range0 (Gc *gc, int end) {
  return it_range(gc, 0, end);
}

int it_has_next (It *this) {
  return opt_is_full(this->e);
}

static Opt *it_onext (It *this) {
  Opt *r = this->e;
  this->e = this->next(this->o);
  return r;
}

void *it_next (It *this) {
  return opt_get(it_onext(this));
}

void *it_peek (It *this) {
  return opt_get(this->e);
}

It *it_add(Gc *gc, It *this, void *element) {
  return it_cat(gc, this, it_unary(gc, element));
}

It *it_add0(Gc *gc, It *this, void *element) {
  return it_cat(gc, it_unary(gc, element), this);
}

// -------------------------------------------------------------------------- //
typedef struct {                                                              //
  It *it1;                                                                    //
  It *it2;                                                                    //
} it_cat_O;                                                                   //
static Opt *cat_next (it_cat_O *o) {                                          //
  return (it_has_next(o->it1)) ? it_onext(o->it1)                             //
    : (it_has_next(o->it2)) ? it_onext(o->it2) : opt_empty();                 //
}                                                                             //
// -------------------------------------------------------------------------- //
It *it_cat (Gc *gc, It *this, It *another) {
  it_cat_O *o = gc_add(gc, malloc(sizeof(it_cat_O)));
  o->it1 = this;
  o->it2 = another;
  return it_new(gc, o, (it_Next)cat_next);
}

// -------------------------------------------------------------------------- //
typedef struct {                                                              //
  It *it;                                                                     //
  size_t i;                                                                   //
  size_t n;                                                                   //
} it_take_O;                                                                  //
static Opt *take_next (it_take_O *o) {                                        //
  if (o->i < o->n && it_has_next(o->it)) {                                    //
    o->i += 1;                                                                //
    return it_onext(o->it);                                                   //
  }                                                                           //
  else return opt_empty();                                                    //
}                                                                             //
// -------------------------------------------------------------------------- //
It *it_take (Gc *gc, It *this, size_t n) {
  it_take_O *o = gc_add(gc, malloc(sizeof(it_take_O)));
  o->it = this;
  o->n = n;
  o->i = 0;
  return it_new(gc, o, (it_Next)take_next);
}

// -------------------------------------------------------------------------- //
typedef struct {                                                              //
  It *it;                                                                     //
  int (*f)(void *e);                                                          //
} it_takef_O;                                                                 //
static Opt *takef_next (it_takef_O *o) {                                      //
  return it_has_next(o->it) && o->f(it_peek(o->it))                           //
    ? it_onext(o->it) : opt_empty();                                          //
}                                                                             //
// -------------------------------------------------------------------------- //
It *it_takef (Gc *gc, It *this, int (*predicate)(void *e)) {
  it_takef_O *o = gc_add(gc, malloc(sizeof(it_takef_O)));
  o->it = this;
  o->f = predicate;
  return it_new(gc, o, (it_Next)takef_next);
}

It *it_drop (Gc *gc, It *this, size_t n) {
  size_t i = 0;
  while (it_has_next(this) && i++ < n)
    it_next(this);
  return this;
}

It *it_dropf (Gc *gc, It *this, int (*predicate)(void *e)) {
  while (it_has_next(this) && predicate(it_peek(this)))
    it_next(this);
  return this;
}

// -------------------------------------------------------------------------- //
typedef struct {                                                              //
  It *it;                                                                     //
  int (*f)(void *e);                                                          //
} it_filter_O;                                                                //
static Opt *filter_next (it_filter_O *o) {                                    //
  for(;;) {                                                                   //
    if (!it_has_next(o->it)) return opt_empty();                              //
    Opt *onext = it_onext(o->it);                                             //
    void *next = opt_get(onext);                                              //
    if (o->f(next)) return onext;                                             //
  }                                                                           //
}                                                                             //
// -------------------------------------------------------------------------- //
It *it_filter (Gc *gc, It *this, int (*predicate)(void *e)) {
  it_filter_O *o = gc_add(gc, malloc(sizeof(it_filter_O)));
  o->it = this;
  o->f = predicate;
  return it_new(gc, o, (it_Next)filter_next);
}

// -------------------------------------------------------------------------- //
typedef struct {                                                              //
  Gc *gc;                                                                     //
  It *it;                                                                     //
  void *(*f)(Gc *gc, void *);                                                 //
} it_map_O;                                                                   //
static Opt *map_next (it_map_O *o) {                                          //
  return it_has_next(o->it)                                                   //
    ? opt_new(o->f(o->gc, it_next(o->it)))                                    //
    : opt_empty()                                                             //
  ;                                                                           //
}                                                                             //
// -------------------------------------------------------------------------- //
It *it_map (Gc *gc, It *this, void *(*converter)(Gc *gc, void *e)) {
  it_map_O *o = gc_add(gc, malloc(sizeof(it_map_O)));
  o->gc = gc;
  o->it = this;
  o->f = converter;
  return it_new(gc, o, (it_Next)map_next);
}

// -------------------------------------------------------------------------- //
typedef struct {                                                              //
  Gc *gc;                                                                     //
  It *it;                                                                     //
  int is_first;                                                               //
  void *(*f1)(Gc *gc, void *);                                                //
  void *(*f)(Gc *gc, void *);                                                 //
} it_map2_O;                                                                  //
static Opt *map2_next(it_map2_O *o) {                                         //
  if (!it_has_next(o->it)) return opt_empty();                                //
  if (o->is_first) {                                                          //
    o->is_first = 0;                                                          //
    return opt_new(o->f1(o->gc, it_next(o->it)));                             //
  }                                                                           //
  return opt_new(o->f(o->gc, it_next(o->it)));                                //
}                                                                             //
// -------------------------------------------------------------------------- //
It *it_map2 (
  Gc *gc,
  It *this,
  void *(*conv1)(Gc *gc, void *e),
  void *(*conv2)(Gc *gc, void *e)
) {
  it_map2_O *o = gc_add(gc, malloc(sizeof(it_map2_O)));
  o->gc = gc;
  o->it = this;
  o->is_first = 1;
  o->f1 = conv1;
  o->f = conv2;
  return it_new(o->gc, o, (it_Next)map2_next);
}

// -------------------------------------------------------------------------- //
typedef struct {                                                              //
  Gc *gc;                                                                     //
  It *it1;                                                                    //
  It *it2;                                                                    //
} it_zip_O;                                                                   //
static Opt *zip_next(it_zip_O *o) {                                           //
  return it_has_next(o->it1) && it_has_next(o->it2)                           //
    ? opt_new(tp_new(o->gc, it_next(o->it1), it_next(o->it2)))                //
    : opt_empty();                                                            //
}                                                                             //
// -------------------------------------------------------------------------- //
It *it_zip (Gc *gc, It *it1, It *it2) {
  it_zip_O *o = gc_add(gc, malloc(sizeof(it_zip_O)));
  o->gc = gc;
  o->it1 = it1;
  o->it2 = it2;
  return it_new(gc, o, (it_Next)zip_next);
}

// -------------------------------------------------------------------------- //
typedef struct {                                                              //
  Gc *gc;                                                                     //
  It *it1;                                                                    //
  It *it2;                                                                    //
  It *it3;                                                                    //
} it_zip3_O;                                                                  //
static Opt *zip3_next(it_zip3_O *o) {                                         //
  return it_has_next(o->it1) && it_has_next(o->it2) && it_has_next(o->it3)    //
    ? opt_new(tp3_new(o->gc,                                                  //
        it_next(o->it1),                                                      //
        it_next(o->it2),                                                      //
        it_next(o->it3)                                                       //
      ))                                                                      //
    : opt_empty();                                                            //
}                                                                             //
// -------------------------------------------------------------------------- //
It *it_zip3 (Gc *gc, It *it1, It *it2, It *it3) {
  it_zip3_O *o = gc_add(gc, malloc(sizeof(it_zip3_O)));
  o->gc = gc;
  o->it1 = it1;
  o->it2 = it2;
  o->it3 = it3;

  return it_new(gc, o, (it_Next)zip3_next);
}

It *it_reverse (Gc *gc, It *this) {
  Arr *a = arr_from_it(gc, this);
  arr_reverse(a);
  return arr_to_it(gc, a);
}

It *it_sort (Gc *gc, It *this, int (*comparator)(void *, void *)) {
  Arr *a = arr_from_it(gc, this);
  arr_sort(a, comparator);
  return arr_to_it(gc, a);
}

void it_each (It *this, void (*f)(void *e)) {
  while (it_has_next(this)) f(it_next(this));
}

void it_each_ix (It *this, void (*f)(void *e, size_t ix)) {
  size_t c = 0;
  while (it_has_next(this))
    f(it_next(this), c++);
}

size_t it_count (It *this) {
  size_t c = 0;
  while (it_has_next(this)) {
    it_next(this);
    ++c;
  }
  return c;
}

int it_eq (It *it1, It *it2, int (*feq)(void *e1, void *e2)) {
  while (it_has_next(it1) && it_has_next(it2)) {
    void *e1 = it_next(it1);
    void *e2 = it_next(it2);
    if (!feq(e1, e2))
      return 0;
  }
  if (it_has_next(it1) || it_has_next(it2))
    return 0;
  return 1;
}

int it_index (It *this, int (*predicate)(void *e)) {
  int ix = 0;
  while(it_has_next(this)) {
    if (predicate(it_next(this)))
      return ix;
    ++ix;
  }
  return -1;
}

int it_contains (It *this, int (*predicate)(void *e)) {
  while(it_has_next(this))
    if (predicate(it_next(this)))
      return 1;
  return 0;
}

int it_last_index (It *this, int (*predicate)(void *e)) {
  int r = -1;
  int ix = 0;
  while(it_has_next(this)) {
    if (predicate(it_next(this)))
      r = ix;
    ++ix;
  }
  return r;
}

Opt *it_find (It *this, int (*predicate)(void *e)) {
  while(it_has_next(this)) {
    void *next = it_next(this);
    if (predicate(next)) {
      return opt_new(next);
    }
  }
  return opt_empty();
}

Arr *it_to (Gc *gc, It *this) {
  return arr_from_it (gc, this);
}

It *it_from (Gc *gc, Arr *a) {
  return arr_to_it(gc, a);
}

