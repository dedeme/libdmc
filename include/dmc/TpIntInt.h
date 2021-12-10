// Copyright 05-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Tuple of 2 integers.

#ifndef DMC_TPINTINT_H
  #define DMC_TPINTINT_H

/// Tuple of 2 integers.
struct tpIntInt_TpIntInt{
  int e1;
  int e2;
};

/// Tuple of 2 integers.
typedef struct tpIntInt_TpIntInt TpIntInt;

/// Constructor of a tuple of 2 elements.
TpIntInt *tpIntInt_new (int e1, int e2);

#endif
