// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdlib.h>
#include <time.h>
#include <gc.h>
#include "dmc/rnd.h"
#include "dmc/Arr.h"
#include "dmc/It.h"
#include "dmc/DEFS.h"

void rnd_init () {
  srand(time(0));
}

inline
double rnd_d () {
  return (double)rand() / (double)RAND_MAX;
}

inline
size_t rnd_i (size_t top) {
  return (size_t)(rnd_d() * top);
}

/**/typedef struct {
/**/  Arr *a;
/**/  int i;
/**/} box_Box;
/**/static FNM (box_next, box_Box, box) {
/**/  if (box->i >= arr_size(box->a)) {
/**/    box->i = 0;
/**/    arr_shuffle(box->a);
/**/  }
/**/  void *r = arr_get(box->a, box->i);
/**/  box->i = box->i + 1;
/**/  return r;
/**/}_FN
It *rnd_box (Arr *a) {
  if (arr_size(a)) {
    arr_shuffle(a);
    box_Box *box = MALLOC(box_Box);
    box->a = a;
    box->i = 0;

    return it_new(box, box_next);
  }
  return it_empty();
}
