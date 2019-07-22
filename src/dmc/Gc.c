// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Gc.h"

// Interface for containers (Arr, Buf, etc.)
struct gc_Bf {
  void *es;
};

struct gc_Gc {
  void **os;
  void **end;
  void **endbf;
  void **os2;
  void **end2;
  void **endbf2;
};


Gc *gc_new (void) {
  return gc_new_bf(15, 5);
}

Gc *gc_new_bf (size_t sz, size_t sz_bf) {
  Gc *this = malloc(sizeof(Gc));
  void **os = malloc(sz * sizeof(void *));
  this->os = os;
  this->end = os;
  this->endbf = os + sz;
  void **os2 = malloc(sz_bf * sizeof(struct gc_Bf));
  this->os2 = os2;
  this->end2 = os2;
  this->endbf2 = os2 + sz_bf;
  return this;
}

void *gc_add (Gc *this, void *object) {
  if (this->end == this->endbf) {
    int size = this->endbf - this->os;
    int new_size = size + size;
    this->os = realloc(this->os, new_size * sizeof(void *));
    this->end = this->os + size;
    this->endbf = this->os + new_size;
  }
  *this->end++ = object;
  return object;
}

void *gc_add_bf (Gc *this, void *object) {
  if (this->end2 == this->endbf2) {
    int size = this->endbf2 - this->os2;
    int new_size = size + size;
    this->os2 = realloc(this->os2, new_size * sizeof(void *));
    this->end2 = this->os2 + size;
    this->endbf2 = this->os2 + new_size;
  }
  *this->end2++ = object;
  return object;
}

void gc_free (Gc *this) {
  void **p = this->os;
  void **end = this->end;
  while (p < end) free(*p++);
  free(this->os);

  p = this->os2;
  end = this->end2;
  while (p < end) {
    free(((struct gc_Bf *)*p)->es);
    free(*p++);
  }
  free(this->os2);

  free(this);
}

struct gc_GcVal {
  Gc *gc;
  void *value;
};

GcVal *gcVal_new (Gc *gc, void *value) {
  GcVal *this = gc_add(gc, malloc(sizeof(GcVal)));
  this->gc = gc;
  this->value = value;
  return this;
}

Gc *gcVal_gc (GcVal *this) {
  return this->gc;
}

void *gcVal_value (GcVal *this) {
  return this->value;
}
