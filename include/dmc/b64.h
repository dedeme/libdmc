// Copyright 17-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// B64 encoder - decoder

#ifndef DMC_B64_H
  #define DMC_B64_H

#include "dmc/Bytes.h"

///
char *b64_decode(char *b64);

///
Bytes *b64_decode_bytes(char *b64);

///
char *b64_encode(char *s);

///
char *b64_encode_bytes(Bytes *bs);

#endif
