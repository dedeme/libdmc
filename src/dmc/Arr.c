// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Arr.h"
#include <string.h>
#include "dmc/DEFS.h"
#include "dmc/std.h"
#include "dmc/Exc.h"
#include "dmc/rnd.h"
#include "dmc/It.h"

struct arr_Arr {
  void **es;
  void **end;
  void **endbf;
};

Arr *arr_new (Gc *gc) {
  return arr_new_bf(gc, 15);
}

Arr *arr_new_bf (Gc *gc, int sz) {
  Arr *this = gc_add_bf(gc, malloc(sizeof(Arr)));
  void **es = malloc(sz * sizeof(void *));
  this->es = es;
  this->end = es;
  this->endbf = es + sz;
  return this;
}

Arr *arr_copy (Gc *gc, Arr *this) {
  int len = this->end - this->es;
  int buffer = this->endbf - this->es;
  Arr *r = arr_new_bf(gc, buffer * sizeof(void *));
  r->end = r->es + len;
  memcpy(r->es, this->es, len * sizeof(void *));
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
    this->es = realloc(this->es, new_size * sizeof(void *));
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
    if (this_len + other_len > this_size){
      int new_size = this_size + other_len;
      this->es = realloc(this->es, new_size * sizeof(void *));
      this->end = this->es + this_len;
      this->endbf = this->es + new_size;
    }
    memcpy(this->end, other->es, other_len * sizeof(void *));
    this->end += other_len;
  }
}

void arr_insert_arr (Arr *this, int ix, Arr *other) {
  int this_len = this->end - this->es;
  int other_len = other->end - other->es;
  EXC_RANGE(ix, 0, this_len)
  if (other_len) {
    int this_size = this->endbf - this->es;
    if (this_len + other_len > this_size) {
      int new_size = this_size + other_len;
      this->es = realloc(this->es, new_size * sizeof(void *));
      this->end = this->es + this_len;
      this->endbf = this->es + new_size;
    }
    memcpy(
      this->es + ix + other_len, this->es + ix, (this_len - ix) * sizeof(void *)
    );
    memcpy(this->es + ix, other->es, other_len * sizeof(void *));
    this->end += other_len;
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
  arr_clear_bf(this, 15);
}

void arr_clear_bf (Arr *this, int sz) {
  this->es = realloc(this->es, sz * sizeof(void *));
  this->end = this->es;
  this->endbf = this->es + sz;
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

    Gc *gc = gc_new();
    int mid1 = size / 2;
    int mid2 = size - mid1;
    void **a1 = gc_add(gc, malloc(mid1 * sizeof(void *)));
    void **a2 = gc_add(gc, malloc(mid2 * sizeof(void *)));
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
    gc_free(gc);
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
  void **p = this->es;
  void **end = this->end;
  while (p < end) {
    if (pred(*p++)) return (p - this->es) - 1;
  }
  return -1;
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
It *arr_to_it (Gc *gc, Arr *this) {
  arr_to_it_O *o = gc_add(gc, malloc(sizeof(arr_to_it_O)));
  o->es = this->es;
  o->n = arr_size(this);
  o->i = 0;
  return it_new(gc, o, (it_Next)to_it_next);
}

Arr *arr_from_it (Gc *gc, It *it) {
  Arr *r = arr_new(gc);
  while (it_has_next(it)) {
    arr_push(r, it_next(it));
  }
  return r;
}

Js *arr_to_js (Gc *gc, Arr *this, Js *(*to)(Gc *gc, void *e)) {
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *a = arr_new(gcl);
  void **p = this->es;
  void **end = this->end;
  while (p < end) {
    arr_push(a, to(gcl, *p++));
  }
  Js *r = js_wa(gc, a);
  gc_free(gcl);
  return r;
}

Arr *arr_from_js (Gc *gc, Js *js, void *(*from)(Gc *gc, Js *jse)) {
  Arr *this = arr_new(gc);
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *a = js_ra(gcl, js);
  void **p = a->es;
  void **end = a->end;
  while (p < end) {
    arr_push(this, from(gc, *p++));
  }
  return this;
}
