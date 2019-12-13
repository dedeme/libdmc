// Copyright 13-Dec-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef DMC_ITP_H
  #define DMC_ITP_H

/// Tuple of two integers.

#include "dmc/std.h"

///
typedef struct itp_Itp Itp;

///
Itp *itp_new(int i1, int i2);

///
int itp_e1(Itp *this);

///
int itp_e2(Itp *this);

#endif
