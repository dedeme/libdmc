// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dm.h"
#include <limits.h>

void rnd_init () {
  srand48 (time (0));
  int ix = 7;
  unsigned short int param[ix];
  while (ix--) {
    param[ix] = drand48() * USHRT_MAX;
  }
  lcong48(param);
}

inline
double rnd_d () {
  return drand48();
}

inline
int rnd_i (int top) {
  return drand48() * top;
}

/**/typedef struct {
/**/  Arr *a;
/**/  int i;
/**/} box_Box;
/**/static bool box_has_next(void *o) { return true; }
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
    box_Box *box = MALLOC(box_Box);
    box->a = a;
    box->i = 0;

    return it_new(box, box_has_next, box_next);
  }
  return it_empty();
}
