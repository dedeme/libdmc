// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Tuples of values

#ifndef DMC_TP_H
  #define DMC_TP_H

#include "Gc.h"

///
typedef struct tp_Tp Tp;

///
Tp *tp_new(Gc *gc, void *e1, void *e2);

///
void *tp_e1(Tp *this);

///
void *tp_e2(Tp *this);

///
typedef struct tp3_Tp3 Tp3;

///
Tp3 *tp3_new(Gc *gc, void *e1, void *e2, void *e3);

///
void *tp3_e1(Tp3 *this);

///
void *tp3_e2(Tp3 *this);

///
void *tp3_e3(Tp3 *this);

#endif
