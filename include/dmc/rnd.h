// Copyright 17-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Generator of random numbers.

#ifndef DMC_RND_H
  #define DMC_RND_H

#include "dmc/Arr.h"

///
typedef struct rnd_Box Box;

/// Intializates the random number generator.
void rnd_init(void);

/// Generates a new double between 0.0 (inclusive) and 1.0 (exclusive).
double rnd_d(void);

/// Generates a new int between 0 (inclusive) and 'top' (exclusive).
int rnd_i(int top);

/// Returns an 'Box' that iterates over 'a' elements randomly. When it finishes
/// with all the elements of 'a', restarts again.
Box *rnd_box_new(Arr *a);

/// Returns the next element of 'this'.
void *rnd_box_next(Box *this);

#endif
