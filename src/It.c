// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dm.h"

struct it_It {
  void *o;
  bool (*has_next)(void *o);
  void *(*next)(void *o);
};

It *it_new(
  void *o,
  bool (*has_next)(void *o),
  void *(*next)(void *o)
) {
  It *this = MALLOC(It);
  this->o = o;
  this->has_next = has_next;
  this->next = next;
  return this;
}

/**/static bool empty_has_next (void *o) { return false; }
/**/static void *empty_next (void *o) { return NULL; }
It *it_empty() {
  return it_new(NULL, empty_has_next, empty_next);
}

/**/typedef struct {
/**/  void *e;
/**/  int is_first;
/**/} unary_O;
/**/static FNP (unary_has_next, unary_O, o) { return o->is_first; }_FN
/**/static FNM (unary_next, unary_O, o) {
/**/  if (o->is_first) {
/**/    o->is_first = 0;
/**/    return o->e;
/**/  }
/**/  return NULL;
/**/}_FN
It *it_unary(void *e) {
  unary_O *o = MALLOC(unary_O);
  o->e = e;
  o->is_first = 1;
  return it_new(o, unary_has_next, unary_next);
}

/**/typedef struct {
/**/  int i;
/**/  int end;
/**/} range_O;
/**/static FNP (range_has_next, range_O, o) {
/**/  return o->i < o->end;
/**/}_FN
/**/static FNM (range_next, range_O, o) {
/**/  int *r = ATOMIC(sizeof(int));
/**/  *r = o->i++;
/**/  return r;
/**/}_FN
It *it_range(int begin, int end) {
  range_O *o = MALLOC(range_O);
  o->i = begin;
  o->end = end;
  return it_new(o, range_has_next, range_next);
}

/// Equals to it_range(0, end);
It *it_range0(int end) {
  return it_range(0, end);
}

/**/typedef struct {
/**/  It *it;
/**/  int has_next;
/**/  void *e;
/**/} to_peek_O;
/**/static FNP (to_peek_has_next, to_peek_O, aux) { return aux->has_next; }_FN
/**/static FNM (to_peek_next, to_peek_O, aux) {
/**/  void *r = aux->e;
/**/  if (it_has_next(aux->it)) {
/**/    aux->has_next = 1;
/**/    aux->e = it_next(aux->it);
/**/  } else {
/**/    aux->has_next = 0;
/**/    aux->e = NULL;
/**/  }
/**/  return r;
/**/}_FN
ItPeek *it_to_peek(It *this) {
  to_peek_O *aux = MALLOC(to_peek_O);
  aux->it = this;
  aux->has_next = 1;
  aux->e = NULL;
  to_peek_next(aux);

  return it_new(aux, to_peek_has_next, to_peek_next);
}


inline
bool it_has_next (It *this) {
  return this->has_next(this->o);
}

inline
void *it_next (It *this) {
  return this->next(this->o);
}

inline
void *it_peek (ItPeek *this) {
  return ((to_peek_O *)this->o)->e;
}

/**/typedef struct {
/**/  It *it1;
/**/  It *it2;
/**/} cat_O;
/**/static FNP (cat_has_next, cat_O, o) {
/**/  return it_has_next(o->it1) || it_has_next(o->it2);
/**/}_FN
/**/static FNM (cat_next, cat_O, o) {
/**/  if (it_has_next(o->it1))
/**/    return it_next(o->it1);
/**/  return it_next(o->it2);
/**/}_FN
It *it_cat (It *this, It *another) {
  cat_O *o = MALLOC(cat_O);
  o->it1 = this;
  o->it2 = another;
  return it_new(o, cat_has_next, cat_next);
}

/**/typedef struct {
/**/  It *it;
/**/  size_t i;
/**/  size_t n;
/**/} take_O;
/**/static FNP (take_has_next, take_O, o) {
/**/  return it_has_next(o->it) && o->i < o->n;
/**/}_FN
/**/static FNM (take_next, take_O, o) { o->i += 1; return it_next(o->it); } _FN
It *it_take (It *this, size_t n) {
  take_O *o = MALLOC(take_O);
  o->it = this;
  o->n = n;
  o->i = 0;
  return it_new(o, take_has_next, take_next);
}

/**/typedef struct {
/**/  It *it;
/**/  bool (*f)(void *e);
/**/} takef_O;
/**/static FNP (takef_has_next, takef_O, o) {
/**/  return it_has_next(o->it) && o->f(it_peek(o->it));
/**/}_FN
/**/static FNM (takef_next, takef_O, o) { return it_next(o->it); } _FN
It *it_takef (It *this, bool (*predicate)(void *e)) {
  takef_O *o = MALLOC(takef_O);
  o->it = it_to_peek(this);
  o->f = predicate;
  return it_new(o, takef_has_next, takef_next);
}

It *it_drop (It *this, size_t n) {
  size_t i = 0;
  while (it_has_next(this) && i++ < n)
    it_next(this);
  return this;
}

ItPeek *it_dropf (It *this, bool (*predicate)(void *e)) {
  ItPeek *it = it_to_peek(this);
  while (it_has_next(it) && predicate(it_peek(it)))
    it_next(it);
  return it;
}

/**/typedef struct {
/**/  It *it;
/**/  void *e;
/**/  bool has_next;
/**/  bool (*f)(void *e);
/**/} filter_O;
/**/static FNP (filter_has_next, filter_O, o) { return o->has_next; }_FN
/**/static FNM (filter_next, filter_O, o) {
/**/  void *r = o->e;
/**/  void *e = NULL;
/**/  int has_next = false;
/**/  while (it_has_next(o->it)) {
/**/    e = it_next(o->it);
/**/    if (o->f(e)) {
/**/      has_next = 1;
/**/      break;
/**/    }
/**/  }
/**/  o->has_next = has_next;
/**/  o->e = e;
/**/  return r;
/**/} _FN
It *it_filter (It *this, bool (*predicate)(void *e)) {
  filter_O *o = MALLOC(filter_O);
  o->it = this;
  o->e = NULL;
  o->has_next = true;
  o->f = predicate;
  filter_next(o);
  return it_new(o, filter_has_next, filter_next);
}

/**/typedef struct {
/**/  It *it;
/**/  void *(*f)(void *);
/**/} map_O;
/**/static FNP (map_has_next, map_O, o) { return it_has_next(o->it); }_FN
/**/static FNM (map_next, map_O, o) { return o->f(it_next(o->it)); } _FN
It *it_map (It *this, void *(*converter)(void *e)) {
  map_O *o = MALLOC(map_O);
  o->it = this;
  o->f = converter;
  return it_new(o, map_has_next, map_next);
}

/**/typedef struct {
/**/  It *it;
/**/  int is_first;
/**/  void *(*f1)(void *);
/**/  void *(*f)(void *);
/**/} map2_O;
/**/static FNP (map2_has_next, map2_O, o) { return it_has_next(o->it); }_FN
/**/static FNM (map2_next, map2_O, o) {
/**/  if (o->is_first) {
/**/    o->is_first = 0;
/**/    return o->f1(it_next(o->it));
/**/  }
/**/  return o->f(it_next(o->it));
/**/} _FN
It *it_map2 (It *this, void *(*conv1)(void *e), void *(*conv2)(void *e)) {
  map2_O *o = MALLOC(map2_O);
  o->it = this;
  o->is_first = 1;
  o->f1 = conv1;
  o->f = conv2;
  return it_new(o, map2_has_next, map2_next);
}

It *it_reverse (It *this) {
  Arr *a = it_to(this);
  arr_reverse(a);
  return it_from(a);
}

It *it_sort (It *this, bool (*comparator)(void *, void *)) {
  Arr *a = it_to(this);
  arr_sort(a, comparator);
  return it_from(a);
}

It *it_sort_str (It *this) {
  Arr *a = it_to(this);
  arr_sort_str(a);
  return it_from(a);
}

It *it_rsort_str (It *this) {
  Arr *a = it_to(this);
  arr_sort_locale(a);
  return it_from(a);
}

It *it_sort_locale (It *this) {
  Arr *a = it_to(this);
  arr_sort_str(a);
  arr_reverse(a);
  return it_from(a);
}

It *it_rsort_locale (It *this){
  Arr *a = it_to(this);
  arr_sort_locale(a);
  arr_reverse(a);
  return it_from(a);
}

/**/typedef struct {
/**/  It *it1;
/**/  It *it2;
/**/} zip_O;
/**/static FNP (zip_has_next, zip_O, o) {
/**/  return it_has_next(o->it1) && it_has_next(o->it2);
/**/}_FN
/**/static FNM (zip_next, zip_O, o) {
/**/  return tp_new(it_next(o->it1), it_next(o->it2));
/**/} _FN
It *it_zip (It *it1, It *it2) {
  zip_O *o = MALLOC(zip_O);
  o->it1 = it1;
  o->it2 = it2;
  return it_new(o, zip_has_next, zip_next);
}

/**/typedef struct {
/**/  It *it1;
/**/  It *it2;
/**/  It *it3;
/**/} zip3_O;
/**/static FNP (zip3_has_next, zip3_O, o) {
/**/  return it_has_next(o->it1) && it_has_next(o->it2) && it_has_next(o->it3);
/**/}_FN
/**/static FNM (zip3_next, zip3_O, o) {
/**/  return tp3_new(it_next(o->it1), it_next(o->it2), it_next(o->it3));
/**/} _FN
It *it_zip3 (It *it1, It *it2, It *it3) {

  zip3_O *o = MALLOC(zip3_O);
  o->it1 = it1;
  o->it2 = it2;
  o->it3 = it3;

  return it_new(o, zip3_has_next, zip3_next);
}

inline
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

bool it_eq (It *it1, It *it2, bool (*feq)(void *e1, void *e2)) {
  if (it1 && it2) {
    while (it_has_next(it1) && it_has_next(it2))
      if (!feq(it_next(it1), it_next(it2)))
        return false;
    if (it_has_next(it1) || it_has_next(it2))
      return false;
    return true;
  }
  return it1 == it2;
}

inline
bool it_eq_str (It *it1, It *it2) {
  return it_eq(it1, it2, str_eq);
}

int it_index (It *this, bool (*predicate)(void *e)) {
  int ix = 0;
  while(it_has_next(this)) {
    if (predicate(it_next(this)))
      return ix;
    ++ix;
  }
  return -1;
}

bool it_contains (It *this, bool (*predicate)(void *e)) {
  while(it_has_next(this))
    if (predicate(it_next(this)))
      return 1;
  return 0;
}

int it_last_index (It *this, bool (*predicate)(void *e)) {
  int r = -1;
  int ix = 0;
  while(it_has_next(this)) {
    if (predicate(it_next(this)))
      r = ix;
    ++ix;
  }
  return r;
}

int it_index_str (It *this, char *s) {
  FNP (eq, char, e) { return str_eq(e, s); }_FN
  return it_index(this, eq);
}

bool it_contains_str (It *this, char *s) {
  FNP (eq, char, e) { return str_eq(e, s); }_FN
  return it_contains(this, eq);
}

int it_last_index_str (It *this, char *s) {
  FNP (eq, char, e) { return str_eq(e, s); }_FN
  return it_last_index(this, eq);
}

void *it_find (It *this, bool (*predicate)(void *e)) {
  while(it_has_next(this)) {
    void *next = it_next(this);
    if (predicate(next))
      return next;
  }
  return NULL;
}

char *it_serialize (It *this, char *(*serialize)(void *)) {
  FNM(map, void, e) {
    return str_to_escape(serialize(e));
  }_FN

  Buf *bf = buf_new();
  FNX(add, char, s) {
    buf_add(bf, s);
  }_FN

  it_each(it_map(this, map), add);
  return buf_to_str(bf);
}

/**/typedef struct {
/**/  char *s;
/**/  void *(*restore)(char *);
/**/} restore_O;
/**/static FNP (restore_has_next, restore_O, o) { return *o->s; }_FN
/**/static FNM (restore_next, restore_O, o) {
/**/  char *st = o->s;
/**/  char *s = st + 1;
/**/  while (*s) {
/**/    char ch = *s++;
/**/    if (ch == '"') {
/**/      break;
/**/    }
/**/    if (ch == '\\') {
/**/      if (*s) {
/**/        ++s;
/**/      } else {
/**/        break;
/**/      }
/**/    }
/**/  }
/**/  int len = s - st;
/**/  o->s = str_sub_end(st, len);
/**/  return o->restore(str_from_escape(str_sub(st, 0, len)));
/**/} _FN
It *it_restore (char *serial, void *(*restore)(char *)) {
  restore_O *o = MALLOC(restore_O);
  o->s = serial;
  o->restore = restore;
  return it_new(o, restore_has_next, restore_next);
}
