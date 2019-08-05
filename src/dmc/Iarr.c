// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Iarr.h"
#include "dmc/std.h"
#include "string.h"
#include "dmc/Exc.h"

struct iarr_Iarr {
  int *es;
  int *end;
  int *endbf;
};

Iarr *iarr_new(void) {
  return iarr_bf_new(15);
}

Iarr *iarr_bf_new(int buffer) {
  Iarr *this = MALLOC(Iarr);
  int *es = ATOMIC(buffer * sizeof(int));
  this->es = es;
  this->end = es;
  this->endbf = es + buffer;
  return this;
}

Iarr *iarr_new_c (int size, int *es) {
  int buffer = size + size;
  int bf_size = buffer * sizeof(int);
  Iarr *this = MALLOC(Iarr);
  this->es = ATOMIC(bf_size);
  this->end = this->es + size;
  this->endbf = this->es + buffer;
  memcpy(this->es, es, bf_size);
  return this;
}

Iarr *iarr_left(Iarr *this, int ix) {
  EXC_RANGE(ix, 0, iarr_size(this))

  int *source = this->es;
  Iarr *r = iarr_bf_new(ix);
  int *target = r->es;
  int *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}


Iarr *iarr_right(Iarr *this, int ix) {
  EXC_RANGE(ix, 0, iarr_size(this))

  int *source = this->es + ix;
  Iarr *r = iarr_bf_new(this->end - source);
  int *target = r->es;
  int *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}

Iarr *iarr_sub(Iarr *this, int begin, int end) {
  int size = iarr_size(this);
  EXC_RANGE(begin, 0, size);
  EXC_RANGE(end, begin, size);

  int *source = this->es + begin;
  Iarr *r = iarr_bf_new((this->es + end) - source);
  int *target = r->es;
  int *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}

Iarr *iarr_copy(Iarr *this) {
  int *source = this->es;
  Iarr *r = iarr_bf_new(this->end - source);
  int *target = r->es;
  int *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}

int iarr_size(Iarr *this) {
  return this->end - this->es;
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
    this->es = GC_REALLOC(this->es, new_size * sizeof(int));
    this->end = this->es + size;
    this->endbf = this->es + new_size;
  }
  *this->end++ = e;
}

void iarr_set(Iarr *this, int ix, int e) {
  EXC_RANGE(ix, 0, iarr_size(this) - 1)

  *(this->es + ix) = e;
}

void iarr_insert(Iarr *this, int ix, int e) {
  EXC_RANGE(ix, 0, iarr_size(this))

  Iarr *new = iarr_bf_new((this->endbf - this->es) + 1);
  int *p = this->es;
  int *p_end = this->end;
  int *t = new->es;
  int c = 0;
  while (p < p_end) {
    if (c == ix) {
      *t++ = e;
      break;
    }
    *t++ = *p++;
    ++c;
  }
  if (p < p_end) {
    while (p < p_end) {
      *t++ = *p++;
    }
  } else {
    *t++ = e;
  }

  this->es = new->es;
  this->end = t;
  this->endbf = new->endbf;
}

///
void iarr_remove(Iarr *this, int ix) {
  EXC_RANGE(ix, 0, iarr_size(this) - 1)

  Iarr *new = iarr_bf_new((this->endbf - this->es) - 1);
  int *p = this->es;
  int *p_end = this->end;
  int *t = new->es;
  int c = 0;
  while (p < p_end) {
    if (c == ix) {
      ++p;
      break;
    }
    *t++ = *p++;
    ++c;
  }
  while (p < p_end) {
    *t++ = *p++;
  }

  this->es = new->es;
  this->end = t;
  this->endbf = new->endbf;
}

void iarr_cat(Iarr *this, Iarr *other) {
  int other_len = other->end - other->es;
  if (other_len) {
    int this_len = this->end - this->es;
    int this_size = this->endbf - this->es;
    if (this_len + other_len >= this_size){
      int new_size = this_size + other_len;
      this->es = GC_REALLOC(this->es, new_size * sizeof(int));
      this->end = this->es + this_len;
      this->endbf = this->es + new_size;
    }
    memcpy(this->end, other->es, other_len * sizeof(int));
    this->end += other_len;
  }
}

void iarr_insert_arr(Iarr *this, int ix, Iarr *other) {
  EXC_RANGE(ix, 0, iarr_size(this))

  Iarr *left = iarr_left(this, ix);
  Iarr *right = iarr_right(this, ix);
  iarr_cat(left, other);
  iarr_cat(left, right);
  this->es = left->es;
  this->end = left->end;
  this->endbf = left->endbf;
}

void iarr_remove_range(Iarr *this, int begin, int end) {
  int size = iarr_size(this);
  EXC_RANGE(begin, 0, size)
  EXC_RANGE(end, begin, size)

  Iarr *left = iarr_left(this, begin);
  Iarr *right = iarr_right(this, end);
  iarr_cat(left, right);
  this->es = left->es;
  this->end = left->end;
  this->endbf = left->endbf;
}

void iarr_clear (Iarr *this) {
  iarr_bf_clear(this, 15);
}

void iarr_bf_clear (Iarr *this, int buffer) {
  int *es = ATOMIC(buffer * sizeof(int));
  this->es = es;
  this->end = es;
  this->endbf = es + buffer;
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
    int mid1 = size / 2;
    int mid2 = size - mid1;
    int *a1 = ATOMIC(mid1 * sizeof(int));
    int *a2 = ATOMIC(mid2 * sizeof(int));
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
  }
  sort(this->es, this->end - this->es);
}

Js *iarr_to_js(Iarr *this) {
  // Arr[Js]
  Arr *a = arr_new();
  int *p = this->es;
  int *end = this->end;
  while (p < end) {
    arr_push(a, js_wi(*p++));
  }
  Js *r = js_wa(a);
  return r;
}

Iarr *iarr_from_js(Js *js) {
  Iarr *this = iarr_new();
  // Arr[Js]
  Arr *a = js_ra(js);
  EACH(a, Js, js)
    iarr_push(this, js_ri(js));
  _EACH
  return this;
}
