// Copyright 04-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/ADouble.h"
#include <string.h>
#include "dmc/Dec.h"
#include "dmc/err.h"
#include "dmc/str.h"
#include "dmc/js.h"

ADouble *aDouble_new(void) {
  return aDouble_bf_new(15);
}

///
ADouble *aDouble_bf_new(int buffer) {
  ADouble *this = MALLOC(ADouble);
  double *es = ATOMIC(buffer * sizeof(double));
  this->es = es;
  this->end = es;
  this->endbf = es + buffer;
  return this;
}

ADouble *aDouble_new_c (int size, double *es) {
  int buffer = size + size;
  int bf_size = buffer * sizeof(double);
  ADouble *this = MALLOC(ADouble);
  this->es = ATOMIC(bf_size);
  this->end = this->es + size;
  this->endbf = this->es + buffer;
  memcpy(this->es, es, bf_size);
  return this;
}

ADouble *aDouble_left(ADouble *this, int ix) {
  if (ix < 0 || ix > aDouble_size(this)) {
    FAIL (str_f(
      "Index (%d) out of range (%d-%d)", ix, 0, aDouble_size(this)
    ));
  }

  double *source = this->es;
  ADouble *r = aDouble_bf_new(ix);
  double *target = r->es;
  double *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}


ADouble *aDouble_right(ADouble *this, int ix) {
  if (ix < 0 || ix > aDouble_size(this)) {
    FAIL (str_f(
      "Index (%d) out of range (%d-%d)", ix, 0, aDouble_size(this)
    ));
  }

  double *source = this->es + ix;
  ADouble *r = aDouble_bf_new(this->end - source);
  double *target = r->es;
  double *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}

ADouble *aDouble_sub(ADouble *this, int begin, int end) {
  int size = aDouble_size(this);
  if (begin < 0 || begin > size) {
    FAIL (str_f(
      "Index (%d) out of range (%d-%d)", begin, 0, size
    ));
  }
  if (end < begin || end > size) {
    FAIL (str_f(
      "Index (%d) out of range (%d-%d)", end, begin, size
    ));
  }

  double *source = this->es + begin;
  ADouble *r = aDouble_bf_new(end - begin);
  double *target = r->es;
  double *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}

ADouble *aDouble_copy(ADouble *this) {
  double *source = this->es;
  ADouble *r = aDouble_bf_new(this->end - source);
  double *target = r->es;
  double *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}

int aDouble_size(ADouble *this) {
  return this->end - this->es;
}

int aDouble_eq(ADouble *this, ADouble *other, double gap) {
  double *p1 = this->es;
  double len = this->end - p1;
  double *p2 = other->es;
  if (len == other->end - p2) {
    for (int i = 0; i < len; ++i) {
      if (!dec_eq_gap(*p1++, *p2++, gap)) {
        return 0;
      }
    }
    return 1;
  }
  return 0;
}

double aDouble_get(ADouble *this, int ix) {
  if (ix < 0 || ix > aDouble_size(this) - 1) {
    FAIL (str_f(
      "Index (%d) out of range (%d-%d)", ix, 0, aDouble_size(this) - 1
    ));
  }

  return *(this->es + ix);
}

double *aDouble_start(ADouble *this) {
  return this->es;
}

double *aDouble_end(ADouble *this) {
  return this->end;
}

void aDouble_push(ADouble *this, double e) {
  if (this->end == this->endbf) {
    int size = this->endbf - this->es;
    int new_size = size + size;
    this->es = GC_REALLOC(this->es, new_size * sizeof(double));
    this->end = this->es + size;
    this->endbf = this->es + new_size;
  }
  *this->end++ = e;
}

void aDouble_set(ADouble *this, int ix, double e) {
  if (ix < 0 || ix > aDouble_size(this) - 1) {
    FAIL (str_f(
      "Index (%d) out of range (%d-%d)", ix, 0, aDouble_size(this) - 1
    ));
  }

  *(this->es + ix) = e;
}

void aDouble_insert(ADouble *this, int ix, double e) {
  if (ix < 0 || ix > aDouble_size(this)) {
    FAIL (str_f(
      "Index (%d) out of range (%d-%d)", ix, 0, aDouble_size(this)
    ));
  }

  aDouble_push(this, e);
  double *p = this->end - 1;
  double *pix = this->es + ix;
  while (p > pix) {
    *p = *(p - 1);
    --p;
  }
  *p = e;
}

void aDouble_remove(ADouble *this, int ix) {
  if (ix < 0 || ix > aDouble_size(this) - 1) {
    FAIL (str_f(
      "Index (%d) out of range (%d-%d)", ix, 0, aDouble_size(this) - 1
    ));
  }

  ADouble *new = aDouble_bf_new((this->endbf - this->es) - 1);
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

void aDouble_cat(ADouble *this, ADouble *other) {
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

void aDouble_insert_arr(ADouble *this, int ix, ADouble *other) {
  if (ix < 0 || ix > aDouble_size(this)) {
    FAIL (str_f(
      "Index (%d) out of range (%d-%d)", ix, 0, aDouble_size(this)
    ));
  }

  ADouble *left = aDouble_left(this, ix);
  ADouble *right = aDouble_right(this, ix);
  aDouble_cat(left, other);
  aDouble_cat(left, right);
  this->es = left->es;
  this->end = left->end;
  this->endbf = left->endbf;
}

void aDouble_remove_range(ADouble *this, int begin, int end) {
  int size = aDouble_size(this);
  if (begin < 0 || begin > size) {
    FAIL (str_f(
      "Index (%d) out of range (%d-%d)", begin, 0, size
    ));
  }
  if (end < begin || end > size) {
    FAIL (str_f(
      "Index (%d) out of range (%d-%d)", end, begin, size
    ));
  }

  ADouble *left = aDouble_left(this, begin);
  ADouble *right = aDouble_right(this, end);
  aDouble_cat(left, right);
  this->es = left->es;
  this->end = left->end;
  this->endbf = left->endbf;
}

void aDouble_clear (ADouble *this) {
  aDouble_bf_clear(this, 15);
}

void aDouble_bf_clear (ADouble *this, int buffer) {
  double *es = ATOMIC(buffer * sizeof(double));
  this->es = es;
  this->end = es;
  this->endbf = es + buffer;
}

void aDouble_reverse(ADouble *this) {
  double *p = this->es;
  double *end = this->end - 1;
  double tmp;
  for (int i = 0; i < (this->end - this->es) / 2; ++i) {
    tmp = *p;
    *p++ = *end;
    *end-- = tmp;
  }
}

void aDouble_sort(ADouble *this) {
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
    for (int i = 0; i < mid1; ++i) *pa1++ = *pa++;
    for (int i = 0; i < mid2; ++i) *pa2++ = *pa++;
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

char *aDouble_to_js(ADouble *this) {
  Achar *a = achar_new();
  double *p = this->es;
  double *end = this->end;
  while (p < end) {
    achar_push(a, js_wd(*p++));
  }
  return js_wa(a);
}

ADouble *aDouble_from_js(char *js) {
  ADouble *this = aDouble_new();
  Achar *a = js_ra(js);
  char **p = a->es;
  while (p < a->end) aDouble_push(this, js_rd(*p++));
  return this;
}
