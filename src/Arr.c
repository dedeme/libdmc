// Copyright 04-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dm.h"


struct arr_Arr{
  size_t size;           // R
  void **es;             // R & W (elements. See arr_set && arr_get)
  size_t _max_size;      // -
};

inline
Arr *arr_new () {
  return arr_new_buf(15);
}

Arr *arr_new_buf (size_t size_buf) {
  Arr *this = MALLOC(Arr);
  this->size = 0;
  this->_max_size = size_buf;
  this->es = (void **)GC_MALLOC(sizeof(void *) * size_buf);
  return this;
}

inline
size_t arr_size (Arr *this) {
  return this->size;
}

inline
void **arr_es (Arr *this) {
  return this->es;
}

void arr_add (Arr *this, void *element) {
  *(this->es + this->size++) = element;
  size_t max = this->_max_size;
  if (this->size >= max) {
    max = max * 2;
    this->es = (void **)GC_REALLOC(this->es, max * sizeof(void *));
    this->_max_size = max;
  }
}

void arr_add_arr (Arr *this, Arr *another) {
  void **array = another->es;
  size_t asize = another->size;
  size_t sum = this->size + asize;
  size_t max = this->_max_size;
  if (sum > max) {
    max = sum + max;
    void **os = (void **)GC_REALLOC(this->es, max * sizeof(void *));
    this->es = os;
    this->_max_size = max;
  }
  void **source = array;
  void **target = this->es + this->size;
  REPEAT(asize) {
    *target++ = *source++;
  }_REPEAT
  this->size = sum;
}

void *arr_get (Arr *this, size_t index) {
  if (index < 0 || index >= this->size) {
    THROW exc_range(0, this->size, index) _THROW
  }
  return this->es[index];
}

void arr_insert (Arr *this, size_t index, void *element) {
  const size_t size = this->size;
  if (index > size || index < 0) {
    THROW exc_range(0, size + 1, index) _THROW
  }
  if (index == size) {
    arr_add(this, element);
    return;
  }

  const size_t sum = size + 1;
  size_t max = this->_max_size;
  if (sum > max) {
    max = sum + max;
    void **os = (void **)GC_REALLOC(this->es, max * sizeof(void *));
    this->es = os;
    this->_max_size = max;
  }
  void **target = this->es + sum - 1;
  void **source = target - 1;
  REPEAT(size - index) {
    *target-- = *source--;
  }_REPEAT
  *target = element;
  this->size = sum;
}

void arr_insert_arr (Arr *this, size_t index, Arr *another) {
  const size_t size = this->size;
  if (index > size || index < 0) {
    THROW exc_range(0, size + 1, index) _THROW
  }

  if (index == size) {
    arr_add_arr(this, another);
    return;
  }

  void **array = another->es;
  size_t asize = another->size;
  const size_t sum = size + asize;
  size_t max = this->_max_size;
  if (sum > max) {
    max = sum + max;
    void **os = (void **)GC_REALLOC(this->es, max * sizeof(void *));
    this->es = os;
    this->_max_size = max;
  }
  void **target = this->es + sum - 1;
  void **source = target - asize;
  REPEAT(size - index) {
    *target-- = *source--;
  }_REPEAT
  source = array + asize - 1;
  REPEAT(asize) {
    *target-- = *source--;
  }_REPEAT
  this->size = sum;
}

void arr_remove (Arr *this, size_t index) {
  if (index < 0 || index >= this->size) {
    THROW exc_range(0, this->size, index) _THROW
  }

  void **target = this->es + index;
  void **source = target + 1;
  REPEAT(this->size - index - 1) {
    *target++ = *source++;
  }_REPEAT
  this->size--;
  if (this->size > 15 && this->size * 4 < this->_max_size) {
    size_t max = this->_max_size / 2;
    void **os = (void **)GC_REALLOC(this->es, max * sizeof(void *));
    this->es = os;
    this->_max_size = max;
  }
}

void arr_remove_range (Arr *this, size_t begin, size_t end) {
  if (begin < 0 || begin >= this->size) {
    THROW exc_range(0, this->size, begin) _THROW
  }
  if (end < 0 || end > this->size) {
    THROW exc_range(0, this->size, end) _THROW
  }

  void **source = this->es + end;
  void **target = this->es + begin;
  REPEAT(this->size - end) {
    *target++ = *source++;
  }_REPEAT
  this->size = this->size + begin - end;
  if (this->size > 15 && this->size * 4 < this->_max_size) {
    size_t max = this->_max_size / 2;
    void **os = (void **)GC_REALLOC(this->es, max * sizeof(void *));
    this->es = os;
    this->_max_size = max;
  }
}

void arr_reverse (Arr *this) {
  void **begin = this->es;
  void **end = begin + (this->size - 1);
  void *tmp;
  REPEAT (this->size / 2) {
    tmp = *begin;
    *begin++ = *end;
    *end-- = tmp;
  }_REPEAT
}

void arr_set (Arr *this, size_t index, void *element) {
  if (index < 0 || index >= this->size) {
    THROW exc_range(0, this->size, index) _THROW
  }

  this->es[index] = element;
}

int arr_sindex (Arr *this, void *e, int (*f)(void *, void*)) {
  size_t start = 0;
  size_t end = this->size;
  size_t ix;
  for (;;) {
    if (end == start) return -1;

    ix = (start + end) >> 1;
    int r = f(e, this->es[ix]);

    if (r < 0) end = ix;
    else if (r > 0) start = ix + 1;
    else return ix;
  }
}

void arr_sort (Arr *this, bool (*f)(void *, void *)) {
  size_t cx = this->size;
  void **x = this->es;
  void **y;
  void *tmp;
  while (cx--) {
    y = x + 1;
    REPEAT(cx) {
      if (f(*x, *y)) {
        tmp = *x;
        *x = *y;
        *y = tmp;
      }
      ++y;
    }_REPEAT
    ++x;
  }
}

void arr_sort_str (Arr *this) {
  FNE (cmp, char, e1, e2) { return strcmp(e1, e2) > 0; }_FN
  arr_sort(this, cmp);
}

void arr_sort_locale (Arr *this) {
  FNE (cmp, char, e1, e2) { return strcoll(e1, e2) > 0; }_FN
  arr_sort(this, cmp);
}

void arr_shuffle (Arr *this) {
  int size = this->size;
  void **os = this->es;
  int j, k;
  void *tmp;
  RANGE0(i, size) {
    j = rnd_i(size);
    tmp = os[i]; os[i] = os[j]; os[j] = tmp;
    j = rnd_i(size);
    tmp = os[i]; os[i] = os[j]; os[j] = tmp;
    k = size - 1 - i;
    j = rnd_i(size);
    tmp = os[k]; os[k] = os[j]; os[j] = tmp;
    j = rnd_i(size);
    tmp = os[k]; os[k] = os[j]; os[j] = tmp;
  }_RANGE
}

/**/typedef struct {
/**/  void **es;
/**/  size_t n;
/**/  size_t i;
/**/} to_it_O;
/**/
/**/inline
/**/static FNP (to_it_has_next, to_it_O, o) { return o->i < o->n; }_FN
/**/
/**/static FNM (to_it_next, to_it_O, o) {
/**/  void *r = o->es[o->i];
/**/  o->i += 1;
/**/  return r;
/**/}_FN
It *arr_to_it (Arr *this) {
  to_it_O *o = MALLOC(to_it_O);
  o->es = this->es;
  o->n = this->size;
  o->i = 0;
  return it_new(o, to_it_has_next, to_it_next);
}

Arr *arr_from_it (It *it) {
  Arr *r = arr_new();
  while (it_has_next(it)) {
    arr_add(r, it_next(it));
  }
  return r;
}

