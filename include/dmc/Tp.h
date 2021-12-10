// Copyright 21-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Tuple of 2 elements.

#ifndef DMC_TP_H
  #define DMC_TP_H

/// Tuple of 2 elements.
struct tp_Tp{
  void *e1;
  void *e2;
};

/// Tuple of 2 elements.
typedef struct tp_Tp Tp;

/// Constructor of a tuple of 2 elements.
Tp *tp_new (void *e1, void *e2);

#endif
