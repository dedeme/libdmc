// Copyright 21-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// B64 encoder - decoder.

#ifndef DMC_B64_H
  #define DMC_B64_H

#include "dmc/Bytes.h"

///
char *b64_decode(Gc *gc, char *b64);

///
Bytes *b64_decode_bytes(Gc *gc, char *b64);

///
char *b64_encode(Gc *gc, char *s);

///
char *b64_encode_bytes(Gc *gc, Bytes *bs);

#endif
