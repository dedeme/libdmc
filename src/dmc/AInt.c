// Copyright 04-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/AInt.h"
#include <string.h>
#include "dmc/js.h"
#include "dmc/err.h"
#include "dmc/str.h"

AInt *aInt_new(void) {
  return aInt_bf_new(15);
}

AInt *aInt_bf_new(int buffer) {
  AInt *this = MALLOC(AInt);
  int *es = ATOMIC(buffer * sizeof(int));
  this->es = es;
  this->end = es;
  this->endbf = es + buffer;
  return this;
}

AInt *aInt_new_c (int size, int *es) {
  int buffer = size + size;
  int bf_size = buffer * sizeof(int);
  AInt *this = MALLOC(AInt);
  this->es = ATOMIC(bf_size);
  this->end = this->es + size;
  this->endbf = this->es + buffer;
  memcpy(this->es, es, bf_size);
  return this;
}

AInt *aInt_left(AInt *this, int ix) {
  if (ix < 0 || ix > aInt_size(this)) {
    FAIL (str_f(
      "Index (%d) out of range (%d-%d)", ix, 0, aInt_size(this)
    ));
  }

  int *source = this->es;
  AInt *r = aInt_bf_new(ix);
  int *target = r->es;
  int *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}


AInt *aInt_right(AInt *this, int ix) {
  if (ix < 0 || ix > aInt_size(this)) {
    FAIL (str_f(
      "Index (%d) out of range (%d-%d)", ix, 0, aInt_size(this)
    ));
  }

  int *source = this->es + ix;
  AInt *r = aInt_bf_new(this->end - source);
  int *target = r->es;
  int *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}

AInt *aInt_sub(AInt *this, int begin, int end) {
  int size = aInt_size(this);
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

  int *source = this->es + begin;
  AInt *r = aInt_bf_new(end - begin);
  int *target = r->es;
  int *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}

AInt *aInt_copy(AInt *this) {
  int *source = this->es;
  AInt *r = aInt_bf_new(this->end - source);
  int *target = r->es;
  int *end_target = r->endbf;
  r->end = end_target;
  while (target < end_target) {
    *target++ = *source++;
  }
  return r;
}

int aInt_size(AInt *this) {
  return this->end - this->es;
}

int aInt_eq(AInt *this, AInt *other) {
  int *p1 = this->es;
  int len = this->end - p1;
  int *p2 = other->es;
  if (len == other->end - p2) {
    for (int i = 0; i < len; ++i) {
      if (*p1++ != *p2++) {
        return 0;
      }
    }
    return 1;
  }
  return 0;
}

int aInt_get(AInt *this, int ix) {
  if (ix < 0 || ix > aInt_size(this) - 1) {
    FAIL (str_f(
      "Index (%d) out of range (%d-%d)", ix, 0, aInt_size(this) - 1
    ));
  }

  return *(this->es + ix);
}

int *aInt_start(AInt *this) {
  return this->es;
}

int *aInt_end(AInt *this) {
  return this->end;
}

void aInt_push(AInt *this, int e) {
  if (this->end == this->endbf) {
    int size = this->endbf - this->es;
    int new_size = size + size;
    this->es = GC_REALLOC(this->es, new_size * sizeof(int));
    this->end = this->es + size;
    this->endbf = this->es + new_size;
  }
  *this->end++ = e;
}

void aInt_set(AInt *this, int ix, int e) {
  if (ix < 0 || ix > aInt_size(this) - 1) {
    FAIL (str_f(
      "Index (%d) out of range (%d-%d)", ix, 0, aInt_size(this) - 1
    ));
  }

  *(this->es + ix) = e;
}

void aInt_insert(AInt *this, int ix, int e) {
  if (ix < 0 || ix > aInt_size(this)) {
    FAIL (str_f(
      "Index (%d) out of range (%d-%d)", ix, 0, aInt_size(this)
    ));
  }

  AInt *new = aInt_bf_new((this->endbf - this->es) + 1);
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
void aInt_remove(AInt *this, int ix) {
  if (ix < 0 || ix > aInt_size(this) - 1) {
    FAIL (str_f(
      "Index (%d) out of range (%d-%d)", ix, 0, aInt_size(this) - 1
    ));
  }

  AInt *new = aInt_bf_new((this->endbf - this->es) - 1);
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

void aInt_cat(AInt *this, AInt *other) {
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

void aInt_insert_arr(AInt *this, int ix, AInt *other) {
  if (ix < 0 || ix > aInt_size(this)) {
    FAIL (str_f(
      "Index (%d) out of range (%d-%d)", ix, 0, aInt_size(this)
    ));
  }

  AInt *left = aInt_left(this, ix);
  AInt *right = aInt_right(this, ix);
  aInt_cat(left, other);
  aInt_cat(left, right);
  this->es = left->es;
  this->end = left->end;
  this->endbf = left->endbf;
}

void aInt_remove_range(AInt *this, int begin, int end) {
  int size = aInt_size(this);
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

  AInt *left = aInt_left(this, begin);
  AInt *right = aInt_right(this, end);
  aInt_cat(left, right);
  this->es = left->es;
  this->end = left->end;
  this->endbf = left->endbf;
}

void aInt_clear (AInt *this) {
  aInt_bf_clear(this, 15);
}

void aInt_bf_clear (AInt *this, int buffer) {
  int *es = ATOMIC(buffer * sizeof(int));
  this->es = es;
  this->end = es;
  this->endbf = es + buffer;
}

void aInt_reverse(AInt *this) {
  int *p = this->es;
  int *end = this->end - 1;
  int tmp;
  for (int i = 0; i < (this->end - this->es) / 2; ++i) {
    tmp = *p;
    *p++ = *end;
    *end-- = tmp;
  }
}

void aInt_sort(AInt *this) {
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

char *aInt_to_js(AInt *this) {
  Achar *a = achar_new();
  int *p = this->es;
  int *end = this->end;
  while (p < end) {
    achar_push(a, js_wi(*p++));
  }
  return js_wa(a);
}

AInt *aInt_from_js(char *js) {
  AInt *this = aInt_new();
  Achar *a = js_ra(js);
  char **p = a->es;
  while (p < a->end) aInt_push(this, js_ri(*p++));
  return this;
}
