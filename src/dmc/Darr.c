// Copyright 15-Oct-2018 ºDeme
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

Darr *darr_new(void) {
  return darr_bf_new(15);
}

///
Darr *darr_bf_new(int buffer) {
  Darr *this = MALLOC(Darr);
  double *es = ATOMIC(buffer * sizeof(double));
  this->es = es;
  this->end = es;
  this->endbf = es + buffer;
  return this;
}


Darr *darr_left(Darr *this, int ix) {
  EXC_RANGE(ix, 0, darr_size(this))

  double *source = this->es;
  Darr *r = darr_bf_new(ix);
  double *target = r->es;
  double *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}


Darr *darr_right(Darr *this, int ix) {
  EXC_RANGE(ix, 0, darr_size(this))

  double *source = this->es + ix;
  Darr *r = darr_bf_new(this->end - source);
  double *target = r->es;
  double *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}

Darr *darr_sub(Darr *this, int begin, int end) {
  int size = darr_size(this);
  EXC_RANGE(begin, 0, size);
  EXC_RANGE(end, begin, size);

  double *source = this->es + begin;
  Darr *r = darr_bf_new((this->es + end) - source);
  double *target = r->es;
  double *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}

Darr *darr_copy(Darr *this) {
  double *source = this->es;
  Darr *r = darr_bf_new(this->end - source);
  double *target = r->es;
  double *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}

int darr_size(Darr *this) {
  return this->end - this->es;
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
    this->es = GC_REALLOC(this->es, new_size * sizeof(double));
    this->end = this->es + size;
    this->endbf = this->es + new_size;
  }
  *this->end++ = e;
}

void darr_set(Darr *this, int ix, double e) {
  EXC_RANGE(ix, 0, darr_size(this) - 1)

  *(this->es + ix) = e;
}

void darr_insert(Darr *this, int ix, double e) {
  EXC_RANGE(ix, 0, darr_size(this))

  Darr *new = darr_bf_new((this->endbf - this->es) + 1);
  double *p = this->es;
  double *p_end = this->end;
  double *t = new->es;
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

void darr_remove(Darr *this, int ix) {
  EXC_RANGE(ix, 0, darr_size(this) - 1)

  Darr *new = darr_bf_new((this->endbf - this->es) - 1);
  double *p = this->es;
  double *p_end = this->end;
  double *t = new->es;
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

void darr_cat(Darr *this, Darr *other) {
  int other_len = other->end - other->es;
  if (other_len) {
    int this_len = this->end - this->es;
    int this_size = this->endbf - this->es;
    if (this_len + other_len >= this_size){
      int new_size = this_size + other_len;
      this->es = GC_REALLOC(this->es, new_size * sizeof(double));
      this->end = this->es + this_len;
      this->endbf = this->es + new_size;
    }
    memcpy(this->end, other->es, other_len * sizeof(double));
    this->end += other_len;
  }
}

void darr_insert_arr(Darr *this, int ix, Darr *other) {
  EXC_RANGE(ix, 0, darr_size(this))

  Darr *left = darr_left(this, ix);
  Darr *right = darr_right(this, ix);
  darr_cat(left, other);
  darr_cat(left, right);
  this->es = left->es;
  this->end = left->end;
  this->endbf = left->endbf;
}

void darr_remove_range(Darr *this, int begin, int end) {
  int size = darr_size(this);
  EXC_RANGE(begin, 0, size)
  EXC_RANGE(end, begin, size)

  Darr *left = darr_left(this, begin);
  Darr *right = darr_right(this, end);
  darr_cat(left, right);
  this->es = left->es;
  this->end = left->end;
  this->endbf = left->endbf;
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
    int mid1 = size / 2;
    int mid2 = size - mid1;
    double *a1 = ATOMIC(mid1 * sizeof(double));
    double *a2 = ATOMIC(mid2 * sizeof(double));
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
  }
  sort(this->es, this->end - this->es);
}

Js *darr_to_js(Darr *this) {
  // Arr[Js]
  Arr *a = arr_new();
  double *p = this->es;
  double *end = this->end;
  while (p < end) {
    char *str = (char *)js_wd(*p++, 9);
    if (str_cindex(str, '.') != -1) {
      char *s = str;
      char *p = s + (strlen(s) - 1);
      while (p >= s && *p == '0') {
        --p;
      }
      char *tmp = str;
      str = str_left(tmp, p - s + (*p == '.' ? 0 : 1));
    }
    arr_push(a, str);
  }
  Js *r = js_wa(a);
  return r;
}

Darr *darr_from_js(Js *js) {
  Darr *this = darr_new();
  // Arr[Js]
  Arr *a = js_ra(js);
  EACH(a, Js, js)
    darr_push(this, js_rd(js));
  _EACH
  return this;
}
