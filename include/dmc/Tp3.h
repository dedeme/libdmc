// Copyright 23-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Tuple of three values.

#ifndef DMC_TP3_H
  #define DMC_TP3_H

///
typedef struct tp3_Tp3 Tp3;

///
Tp3 *tp3_new(void *e1, void *e2, void *e3);

///
void *tp3_e1(Tp3 *this);

///
void *tp3_e2(Tp3 *this);

///
void *tp3_e3(Tp3 *this);

#endif
