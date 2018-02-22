// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Generator of random numbers

#ifndef DM_RND_H
  #define DM_RND_H

#include "Arr.h"
#include "It.h"

/// Intializates the random number generator.
void rnd_init (void);

/// Generates a new double between 0.0 (inclusive) and 1.0 (exclusive)
double rnd_d (void);

/// Generates a new int between 0 (inclusive) and 'top' (exclusive)
size_t rnd_i (size_t top);

/// Returns an 'It' that iterates over 'a' elements randomly. When it finishes
/// with all the elements of 'a', restarts again.
It *rnd_box (Arr *a);

#endif
