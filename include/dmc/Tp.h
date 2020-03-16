// Copyright 23-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Pair of two values.

#ifndef DMC_TP_H
  #define DMC_TP_H

///
typedef struct tp_Tp Tp;

///
Tp *tp_new(void *e1, void *e2);

///
void *tp_e1(Tp *this);

///
void *tp_e2(Tp *this);

#endif
