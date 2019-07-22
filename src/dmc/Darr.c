// Copyright 21-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Darr.h"
#include "string.h"
#include "dmc/std.h"
#include "dmc/Dec.h"

struct darr_Darr {
  double *es;
  double *end;
  double *endbf;
};

Darr *darr_new(Gc *gc) {
  return darr_new_bf(gc, 15);
}

Darr *darr_new_bf(Gc *gc, int buffer) {
  Darr *this = gc_add_bf(gc, malloc(sizeof(Darr)));
  double *es = malloc(buffer * sizeof(double));
  this->es = es;
  this->end = es;
  this->endbf = es + buffer;
  return this;
}


Darr *darr_left(Gc *gc, Darr *this, int ix) {
  int size = darr_size(this);
  if (ix < 0) ix = size + ix;
  EXC_RANGE(ix, 0, size)

  double *source = this->es;
  Darr *r = darr_new_bf(gc, ix);
  double *target = r->es;
  double *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}


Darr *darr_right(Gc *gc, Darr *this, int ix) {
  int size = darr_size(this);
  if (ix < 0) ix = size + ix;
  EXC_RANGE(ix, 0, size)

  double *source = this->es + ix;
  Darr *r = darr_new_bf(gc, this->end - source);
  double *target = r->es;
  double *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}

Darr *darr_sub(Gc *gc, Darr *this, int begin, int end) {
  int size = darr_size(this);
  if (begin < 0) begin = size + begin;
  if (end < 0) end = size + end;

  EXC_RANGE(begin, 0, size);
  EXC_RANGE(end, begin, size);

  double *source = this->es + begin;
  Darr *r = darr_new_bf(gc, (this->es + end) - source);
  double *target = r->es;
  double *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}

Darr *darr_copy(Gc *gc, Darr *this) {
  double *source = this->es;
  Darr *r = darr_new_bf(gc, this->end - source);
  double *target = r->es;
  double *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}

int darr_eq(Darr *this, Darr *other, double gap) {
  double *p1 = this->es;
  double len = this->end - p1;
  double *p2 = other->es;
  if (len == other->end - p2) {
    REPEAT(len)
      if (!dec_eq_gap(*p1++, *p2++, gap)) {
        return 0;
      }
    _REPEAT
    return 1;
  }
  return 0;
}

int darr_size(Darr *this) {
  return this->end - this->es;
}

double darr_get(Darr *this, int ix) {
  EXC_RANGE(ix, 0, darr_size(this) - 1)

  return *(this->es + ix);
}

double *darr_start(Darr *this) {
  return this->es;
}

double *darr_end(Darr *this) {
  return this->end;
}

void darr_push(Darr *this, double e) {
  if (this->end == this->endbf) {
    int size = this->endbf - this->es;
    int new_size = size + size;
    this->es = realloc(this->es, new_size * sizeof(double));
    this->end = this->es + size;
    this->endbf = this->es + new_size;
  }
  *this->end++ = e;
}

double darr_pop(Darr *this) {
  if (this->es >= this->end)
    EXC_ILLEGAL_STATE("Array is empty")

  --this->end;
  return *this->end;
}

double darr_peek(Darr *this) {
  return *(this->end - 1);
}

void darr_set(Darr *this, int ix, double e) {
  EXC_RANGE(ix, 0, darr_size(this) - 1)

  *(this->es + ix) = e;
}

void darr_insert(Darr *this, int ix, double e) {
  EXC_RANGE(ix, 0, darr_size(this))

  darr_push(this, e);
  double *p = this->end - 1;
  double *pix = this->es + ix;
  while (p > pix) {
    *p = *(p - 1);
    --p;
  }
  *p = e;
}

void darr_remove(Darr *this, int ix) {
  EXC_RANGE(ix, 0, darr_size(this) - 1)

  double *p = this->es + ix;
  double *p1 = p + 1;
  double *pend = this->end;
  while (p1 < pend) {
    *p++ = *p1++;
  }
  --this->end;
}

void darr_cat(Darr *this, Darr *other) {
  int other_len = other->end - other->es;
  if (other_len) {
    int this_len = this->end - this->es;
    int this_size = this->endbf - this->es;
    if (this_len + other_len >= this_size){
      int new_size = this_size + other_len;
      this->es = realloc(this->es, new_size * sizeof(double));
      this->end = this->es + this_len;
      this->endbf = this->es + new_size;
    }
    memcpy(this->end, other->es, other_len * sizeof(double));
    this->end += other_len;
  }
}

void darr_insert_arr(Darr *this, int ix, Darr *other) {
  int this_len = this->end - this->es;
  int other_len = other->end - other->es;
  EXC_RANGE(ix, 0, this_len)
  if (other_len) {
    int this_size = this->endbf - this->es;
    if (this_len + other_len > this_size) {
      int new_size = this_size + other_len;
      this->es = realloc(this->es, new_size * sizeof(double));
      this->end = this->es + this_len;
      this->endbf = this->es + new_size;
    }
    memcpy(
      this->es + ix + other_len, this->es + ix, (this_len - ix) * sizeof(double)
    );
    memcpy(this->es + ix, other->es, other_len * sizeof(double));
    this->end += other_len;
  }
}

void darr_remove_range(Darr *this, int begin, int end) {
  int sz = darr_size(this);
  EXC_RANGE(begin, 0, sz)
  EXC_RANGE(end, begin, sz)

  int df = end - begin;
  if (df == 0) {
    return;
  }

  double *pb = this->es + begin;
  double *pe = this->es + end;
  double *pend = this->end;
  while (pe < pend) {
    *pb++ = *pe++;
  }
  this->end -= df;
}

void darr_clear (Darr *this) {
  darr_clear_bf(this, 15);
}

void darr_clear_bf (Darr *this, int sz) {
  this->es = realloc(this->es, sz * sizeof(double));
  this->end = this->es;
  this->endbf = this->es + sz;
}

void darr_reverse(Darr *this) {
  double *p = this->es;
  double *end = this->end - 1;
  double tmp;
  REPEAT ((this->end - this->es) / 2) {
    tmp = *p;
    *p++ = *end;
    *end-- = tmp;
  }_REPEAT
}

void darr_sort(Darr *this) {
  void sort(double *a, int size) {
    if (size < 2) {
      return;
    }

    Gc *gc = gc_new();
    int mid1 = size / 2;
    int mid2 = size - mid1;
    double *a1 = gc_add(gc, malloc(mid1 * sizeof(double)));
    double *a2 = gc_add(gc, malloc(mid2 * sizeof(double)));
    double *pa = a;
    double *pa1 = a1;
    double *pa2 = a2;
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
      if (*pa1 > *pa2) {
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

Js *darr_to_js(Gc *gc, Darr *this) {
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *a = arr_new(gcl);
  double *p = this->es;
  double *end = this->end;
  while (p < end) {
    arr_push(a, js_wd(gcl, *p++));
  }
  Js *r = js_wa(gc, a);
  gc_free(gcl);
  return r;
}

Darr *darr_from_js(Gc *gc, Js *js) {
  Darr *this = darr_new(gc);
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *a = js_ra(gcl, js);
  EACH(a, Js, js)
    darr_push(this, js_rd(js));
  _EACH
  gc_free(gcl);
  return this;
}

