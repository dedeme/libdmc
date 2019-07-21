// Copyright 21-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Structure for working with bytes. Example:
///   Bytes *bs = bytes_new(gc);
///   const unsigned char bss[] = {0, 23, 116, 225};
///   bytes_add_bytes(bs, bss, 4);
///   char b41 = b41_encodeBytes(gc, bs);
///   assert(!strcmp("RRoixx", b41));

#ifndef DMC_BYTES_H
  #define DMC_BYTES_H

#include "Js.h"

///
typedef struct bytes_Bytes Bytes;

///
Bytes *bytes_new(Gc *gc);

///
Bytes *bytes_new_bf(Gc *gc, int length);

/// Returns a new allocated 'Bytes' whitch is copy of 'bs'
Bytes *bytes_from_bytes(Gc *gc, unsigned char *bs, int length);

/// Returns a 'Bytes' whitch is copy of 's' without the ending zero.
Bytes *bytes_from_str(Gc *gc, char *s);

///
unsigned char *bytes_bs(Bytes *this);

///
int bytes_len(Bytes *this);

/// Adds to 'this' a new copy of 'bs'
void bytes_add_bytes(Bytes *this, unsigned char *bs, int length);

/// Adds to 'this' a new copy of 'another'
void bytes_add(Bytes *this, Bytes *another);

/// Adds to 'this' a copy of 's' without the ending zero
void bytes_add_str(Bytes *this, char *s);

///
Js *bytes_to_js(Gc *gc, Bytes *this);

///
Bytes *bytes_from_js(Gc *gc, Js *js);

#endif
