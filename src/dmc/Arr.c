// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Arr.h"
#include "string.h"

#include "dmc/std.h"
#include "dmc/rnd.h"

struct arr_Arr {
  void **es;
  void **end;
  void **endbf;
};

Arr *arr_new () {
  return arr_bf_new(15);
}

Arr *arr_bf_new (int buffer) {
  Arr *this = MALLOC(Arr);
  void **es = GC_MALLOC(buffer * sizeof(void *));
  this->es = es;
  this->end = es;
  this->endbf = es + buffer;
  return this;
}

Arr *arr_new_from (void *e, ...) {
  va_list args;
  void *tmp;

  Arr *this = arr_new();
  arr_push(this, e);

  va_start(args, e);
  tmp = va_arg(args, void *);
  while (tmp) {
    arr_push(this, tmp);
    tmp = va_arg(args, void *);
  }
  va_end(args);

  return this;
}

Arr *arr_new_c (int size, void **es) {
  int buffer = size + size;
  int bf_size = buffer * sizeof(void *);
  Arr *this = MALLOC(Arr);
  this->es = GC_MALLOC(bf_size);
  this->end = this->es + size;
  this->endbf = this->es + buffer;
  memcpy(this->es, es, bf_size);
  return this;
}

Arr *arr_copy (Arr *this) {
  int buffer = this->endbf - this->es;
  int bf_size = buffer * sizeof(void *);
  Arr *r = MALLOC(Arr);
  void **es = GC_MALLOC(bf_size);
  r->es = es;
  r->end = es + (this->end - this->es);
  r->endbf = es + buffer;
  memcpy(es, this->es, bf_size);
  return r;
}

int arr_size (Arr *this) {
  return this->end - this->es;
}

void *arr_get (Arr *this, int ix) {
  EXC_RANGE(ix, 0, arr_size(this) - 1)

  return *(this->es + ix);
}

void **arr_start (Arr *this) {
  return this->es;
}

void **arr_end (Arr *this) {
  return this->end;
}

void arr_push (Arr *this, void *e) {
  if (this->end == this->endbf) {
    int size = this->endbf - this->es;
    int new_size = size + size;
    this->es = GC_REALLOC(this->es, new_size * sizeof(void *));
    this->end = this->es + size;
    this->endbf = this->es + new_size;
  }
  *this->end++ = e;
}

void *arr_pop (Arr *this) {
  if (this->es >= this->end)
    EXC_ILLEGAL_STATE("Array is empty")

  --this->end;
  return *this->end;
}

void *arr_peek (Arr *this) {
  return *(this->end - 1);
}

void arr_set (Arr *this, int ix, void *e) {
  EXC_RANGE(ix, 0, arr_size(this) - 1)

  void **p = this->es + ix;
  *p = e;
}

void arr_insert (Arr *this, int ix, void *e) {
  EXC_RANGE(ix, 0, arr_size(this))

  arr_push(this, e);
  void **p = this->end - 1;
  void **pix = this->es + ix;
  while (p > pix) {
    *p = *(p - 1);
    --p;
  }
  *p = e;
}

void arr_remove (Arr *this, int ix) {
  EXC_RANGE(ix, 0, arr_size(this) - 1)

  void **p = this->es + ix;
  void **p1 = p + 1;
  void **pend = this->end;
  while (p1 < pend) {
    *p++ = *p1++;
  }
  --this->end;
}

void arr_cat (Arr *this, Arr *other) {
  int other_len = other->end - other->es;
  if (other_len) {
    int this_len = this->end - this->es;
    int this_size = this->endbf - this->es;
    if (this_len + other_len >= this_size){
      int new_size = this_size + other_len;
      this->es = GC_REALLOC(this->es, new_size * sizeof(void *));
      this->end = this->es + this_len;
      this->endbf = this->es + new_size;
    }
    void **s = other->es;
    void **t = this->end;
    REPEAT(other_len)
      *t++ = *s++;
    _REPEAT
    this->end = t;
  }
}

void arr_insert_arr (Arr *this, int ix, Arr *other) {
  const int this_len = this->end - this->es;
  EXC_RANGE(ix, 0, this_len)

  int other_len = other->end - other->es;
  if (other_len) {
    int this_size = this->endbf - this->es;
    int new_size = this_size;
    if (this_len + other_len >= this_size){
      new_size = this_size + other_len;

      void **es = GC_MALLOC(new_size * sizeof(void *));
      void **end = es;

      void **p = this->es;
      void **pend = p + ix;
      while (p < pend) {
        *end++ = *p++;
      }
      void **p2 = other->es;
      void **p2end = other->end;
      while (p2 < p2end) {
        *end++ = *p2++;
      }
      pend = this->end;
      while (p < pend) {
        *end++ = *p++;
      }

      this->es = es;
      this->end = end;
      this->endbf = es + new_size;
    } else {
      void **s = this->end - 1;
      void **t = s + other_len;
      void **limit = this->es + ix;
      while (s >= limit) {
        *t-- = *s--;
      }
      s = other->es;
      t = this->es + ix;
      limit = other->end;
      while (s < limit) {
        *t++ = *s++;
      }
    }
  }
}

void arr_remove_range (Arr *this, int begin, int end) {
  int sz = arr_size(this);
  EXC_RANGE(begin, 0, sz)
  EXC_RANGE(end, begin, sz)

  int df = end - begin;
  if (df == 0) {
    return;
  }

  void **pb = this->es + begin;
  void **pe = this->es + end;
  void **pend = this->end;
  while (pe < pend) {
    *pb++ = *pe++;
  }
  this->end -= df;
}

void arr_clear (Arr *this) {
  arr_bf_clear(this, 15);
}

void arr_bf_clear (Arr *this, int buffer) {
  void **es = GC_MALLOC(buffer * sizeof(void *));
  this->es = es;
  this->end = es;
  this->endbf = es + buffer;
}

void arr_reverse (Arr *this) {
  void **p = this->es;
  void **end = this->end - 1;
  void *tmp;
  REPEAT ((this->end - this->es) / 2) {
    tmp = *p;
    *p++ = *end;
    *end-- = tmp;
  }_REPEAT
}

void arr_sort (Arr *this, int (*greater)(void *, void *)) {
  void sort(void **a, int size) {
    if (size < 2) {
      return;
    }
    int mid1 = size / 2;
    int mid2 = size - mid1;
    void **a1 = ATOMIC(mid1 * sizeof(void *));
    void **a2 = ATOMIC(mid2 * sizeof(void *));
    void **pa = a;
    void **pa1 = a1;
    void **pa2 = a2;
    REPEAT(mid1) {
      *pa1++ = *pa++;
    }_REPEAT
    REPEAT(mid2) {
      *pa2++ = *pa++;
    }_REPEAT
    sort(a1, mid1);
    sort(a2, mid2);

    pa = a;
    int ia1 = 0;
    pa1 = a1;
    int ia2 = 0;
    pa2 = a2;

    for(;;) {
      if (ia1 == mid1) {
        for(;;) {
          if (ia2++ == mid2) {
            break;
          }
          *pa++ = *pa2++;
        }
        break;
      }
      if (ia2 == mid2) {
        for (;;) {
          if (ia1++ == mid1) {
            break;
          }
          *pa++ = *pa1++;
        }
        break;
      }
      if (greater(*pa1, *pa2)) {
        *pa++ = *pa2++;
        ++ia2;
      } else {
        *pa++ = *pa1++;
        ++ia1;
      }
    }
  }
  sort(this->es, this->end - this->es);
}

void arr_shuffle (Arr *this) {
  void **es = this->es;
  int size = this->end - es;
  void **p = this->end - 1;
  void **pix, *tmp;
  while (p > es) {
    pix = es + rnd_i(size--);
    tmp = *p;
    *p-- = *(pix);
    *(pix) = tmp;
  }
}

int arr_index (Arr *this, int (*pred)(void *e)) {
  int ix = -1;
  EACH_IX(this, void, e, i)
    if (pred(e)) {
      ix = i;
      break;
    }
  _EACH
  return ix;
}

void arr_filter (Arr *this, int (*pred)(void *e)) {
  void **p = this->es;
  void **end = this->end;
  void **new_end = p;
  while (p < end) {
    if (pred(*p)) {
      *new_end++ = *p;
    }
    ++p;
  }
  this->end = new_end;
}

// -------------------------------------------------------------------------- //
typedef struct {                                                              //
  void **es;                                                                  //
  size_t n;                                                                   //
  size_t i;                                                                   //
} arr_to_it_O;                                                                //
static Opt *to_it_next(arr_to_it_O *o) {                                      //
  size_t i = o->i;                                                            //
  if (i < o->n) {                                                             //
    o->i += 1;                                                                //
    return opt_new(o->es[i]);                                                 //
  } else {                                                                    //
    return opt_empty();                                                       //
  }                                                                           //
}                                                                             //
// -------------------------------------------------------------------------- //
It *arr_to_it (Arr *this) {
  arr_to_it_O *o = MALLOC(arr_to_it_O);
  o->es = this->es;
  o->n = arr_size(this);
  o->i = 0;
  return it_new(o, (it_Next)to_it_next);
}

Arr *arr_from_it (It *it) {
  Arr *r = arr_new();
  while (it_has_next(it)) {
    arr_push(r, it_next(it));
  }
  return r;
}

Js *arr_to_js (Arr *this, Js *(*to)(void *e)) {
  // Arr[Js]
  Arr *a = arr_new();
  void **p = this->es;
  void **end = this->end;
  while (p < end) {
    arr_push(a, to(*p++));
  }
  Js *r = js_wa(a);
  return r;
}

Arr *arr_from_js (Js *js, void *(*from)(Js *jse)) {
  Arr *this = arr_new();
  // Arr[Js]
  Arr *a = js_ra(js);
  void **p = a->es;
  void **end = a->end;
  while (p < end) {
    arr_push(this, from(*p++));
  }
  return this;
}
