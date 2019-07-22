// Copyright 21-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Iarr.h"
#include "string.h"
#include "dmc/std.h"

struct iarr_Iarr {
  int *es;
  int *end;
  int *endbf;
};

Iarr *iarr_new(Gc *gc) {
  return iarr_new_bf(gc, 15);
}

Iarr *iarr_new_bf(Gc *gc, int buffer) {
  Iarr *this = gc_add_bf(gc, malloc(sizeof(Iarr)));
  int *es = malloc(buffer * sizeof(int));
  this->es = es;
  this->end = es;
  this->endbf = es + buffer;
  return this;
}


Iarr *iarr_left(Gc *gc, Iarr *this, int ix) {
  int size = iarr_size(this);
  if (ix < 0) ix = size + ix;
  EXC_RANGE(ix, 0, size)

  int *source = this->es;
  Iarr *r = iarr_new_bf(gc, ix);
  int *target = r->es;
  int *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}


Iarr *iarr_right(Gc *gc, Iarr *this, int ix) {
  int size = iarr_size(this);
  if (ix < 0) ix = size + ix;
  EXC_RANGE(ix, 0, size)

  int *source = this->es + ix;
  Iarr *r = iarr_new_bf(gc, this->end - source);
  int *target = r->es;
  int *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}

Iarr *iarr_sub(Gc *gc, Iarr *this, int begin, int end) {
  int size = iarr_size(this);
  if (begin < 0) begin = size + begin;
  if (end < 0) end = size + end;

  EXC_RANGE(begin, 0, size);
  EXC_RANGE(end, begin, size);

  int *source = this->es + begin;
  Iarr *r = iarr_new_bf(gc, (this->es + end) - source);
  int *target = r->es;
  int *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}

Iarr *iarr_copy(Gc *gc, Iarr *this) {
  int *source = this->es;
  Iarr *r = iarr_new_bf(gc, this->end - source);
  int *target = r->es;
  int *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}

int iarr_eq(Iarr *this, Iarr *other) {
  int *p1 = this->es;
  int len = this->end - p1;
  int *p2 = other->es;
  if (len == other->end - p2) {
    REPEAT(len)
      if (*p1++ != *p2++) {
        return 0;
      }
    _REPEAT
    return 1;
  }
  return 0;
}

int iarr_size(Iarr *this) {
  return this->end - this->es;
}

int iarr_get(Iarr *this, int ix) {
  EXC_RANGE(ix, 0, iarr_size(this) - 1)

  return *(this->es + ix);
}

int *iarr_start(Iarr *this) {
  return this->es;
}

int *iarr_end(Iarr *this) {
  return this->end;
}

void iarr_push(Iarr *this, int e) {
  if (this->end == this->endbf) {
    int size = this->endbf - this->es;
    int new_size = size + size;
    this->es = realloc(this->es, new_size * sizeof(int));
    this->end = this->es + size;
    this->endbf = this->es + new_size;
  }
  *this->end++ = e;
}

int iarr_pop(Iarr *this) {
  if (this->es >= this->end)
    EXC_ILLEGAL_STATE("Array is empty")

  --this->end;
  return *this->end;
}

int iarr_peek(Iarr *this) {
  return *(this->end - 1);
}

void iarr_set(Iarr *this, int ix, int e) {
  EXC_RANGE(ix, 0, iarr_size(this) - 1)

  *(this->es + ix) = e;
}

void iarr_insert(Iarr *this, int ix, int e) {
  EXC_RANGE(ix, 0, iarr_size(this))

  iarr_push(this, e);
  int *p = this->end - 1;
  int *pix = this->es + ix;
  while (p > pix) {
    *p = *(p - 1);
    --p;
  }
  *p = e;
}

void iarr_remove(Iarr *this, int ix) {
  EXC_RANGE(ix, 0, iarr_size(this) - 1)

  int *p = this->es + ix;
  int *p1 = p + 1;
  int *pend = this->end;
  while (p1 < pend) {
    *p++ = *p1++;
  }
  --this->end;
}

void iarr_cat(Iarr *this, Iarr *other) {
  int other_len = other->end - other->es;
  if (other_len) {
    int this_len = this->end - this->es;
    int this_size = this->endbf - this->es;
    if (this_len + other_len >= this_size){
      int new_size = this_size + other_len;
      this->es = realloc(this->es, new_size * sizeof(int));
      this->end = this->es + this_len;
      this->endbf = this->es + new_size;
    }
    memcpy(this->end, other->es, other_len * sizeof(int));
    this->end += other_len;
  }
}

void iarr_insert_arr(Iarr *this, int ix, Iarr *other) {
  int this_len = this->end - this->es;
  int other_len = other->end - other->es;
  EXC_RANGE(ix, 0, this_len)
  if (other_len) {
    int this_size = this->endbf - this->es;
    if (this_len + other_len > this_size) {
      int new_size = this_size + other_len;
      this->es = realloc(this->es, new_size * sizeof(int));
      this->end = this->es + this_len;
      this->endbf = this->es + new_size;
    }
    memcpy(
      this->es + ix + other_len, this->es + ix, (this_len - ix) * sizeof(int)
    );
    memcpy(this->es + ix, other->es, other_len * sizeof(int));
    this->end += other_len;
  }
}

void iarr_remove_range(Iarr *this, int begin, int end) {
  int sz = iarr_size(this);
  EXC_RANGE(begin, 0, sz)
  EXC_RANGE(end, begin, sz)

  int df = end - begin;
  if (df == 0) {
    return;
  }

  int *pb = this->es + begin;
  int *pe = this->es + end;
  int *pend = this->end;
  while (pe < pend) {
    *pb++ = *pe++;
  }
  this->end -= df;
}

void iarr_clear (Iarr *this) {
  iarr_clear_bf(this, 15);
}

void iarr_clear_bf (Iarr *this, int sz) {
  this->es = realloc(this->es, sz * sizeof(int));
  this->end = this->es;
  this->endbf = this->es + sz;
}

void iarr_reverse(Iarr *this) {
  int *p = this->es;
  int *end = this->end - 1;
  int tmp;
  REPEAT ((this->end - this->es) / 2) {
    tmp = *p;
    *p++ = *end;
    *end-- = tmp;
  }_REPEAT
}

void iarr_sort(Iarr *this) {
  void sort(int *a, int size) {
    if (size < 2) {
      return;
    }

    Gc *gc = gc_new();
    int mid1 = size / 2;
    int mid2 = size - mid1;
    int *a1 = gc_add(gc, malloc(mid1 * sizeof(int)));
    int *a2 = gc_add(gc, malloc(mid2 * sizeof(int)));
    int *pa = a;
    int *pa1 = a1;
    int *pa2 = a2;
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

Js *iarr_to_js(Gc *gc, Iarr *this) {
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *a = arr_new(gcl);
  int *p = this->es;
  int *end = this->end;
  while (p < end) {
    arr_push(a, js_wd(gcl, *p++));
  }
  Js *r = js_wa(gc, a);
  gc_free(gcl);
  return r;
}

Iarr *iarr_from_js(Gc *gc, Js *js) {
  Iarr *this = iarr_new(gc);
  Gc *gcl = gc_new();
  // Arr[Js]
  Arr *a = js_ra(gcl, js);
  EACH(a, Js, js)
    iarr_push(this, js_rd(js));
  _EACH
  gc_free(gcl);
  return this;
}


