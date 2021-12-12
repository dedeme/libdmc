// Copyright 22-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/rnd.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "dmc/sys.h"
#include "dmc/str.h"
#include "dmc/err.h"

void rnd_init (void) {
  srand(time(0));
}

double rnd_d (void) {
  return (double)rand() / (double)RAND_MAX;
}

int rnd_i (int top) {
  return (int)(rnd_d() * top);
}

struct rnd_Box {
  int i;
  int size;
  Arr *a;
};

Box *rnd_box_new(Arr *a) {
  if (!arr_size(a)) FAIL("Arr 'a' is empty");

  Arr *va = arr_copy(a);
  arr_shuffle(va);

  Box *this = malloc(sizeof(Box));
  this->i = 0;
  this->size = arr_size(va);
  this->a = va;
  return this;
}

/// rnd_box_next returns the next element of 'this'.
void *rnd_box_next(Box *this) {
  if (this->i == this->size) {
    arr_shuffle(this->a);
    this->i = 0;
  }
  return arr_get(this->a, this->i++);
}
