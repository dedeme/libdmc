// Copyright 2-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Bytes structure

#ifndef DM_BYTES_H
  #define DM_BYTES_H

#include <stddef.h>

/// Structure for working with bytes. Example:
///   Bytes *bs = bytes_new();
///   const unsigned char bss[] = {0, 23, 116, 225};
///   bytes_add_bytes(bs, bss, 4);
///   char b41 = b41_encodeBytes(bs);
///   assert(!strcmp("RRoixx", b41));
typedef struct bytes_Bytes Bytes;
typedef struct json_Json Json;

///
Bytes *bytes_new(void);

///
Bytes *bytes_new_len(size_t length);

/// Returns a new allocated 'Bytes' whitch is copy of 'bs'
Bytes *bytes_from_bytes (unsigned char *bs, size_t length);

/// Returns a 'Bytes' whitch is copy of 's' without the ending zero.
Bytes *bytes_from_str (char *s);

///
unsigned char *bytes_bs(Bytes *this);

///
size_t bytes_length(Bytes *this);

/// Adds to 'this' a new copy of 'bs'
void bytes_add_bytes (Bytes *this, unsigned char *bs, size_t length);

/// Adds to 'this' a new copy of 'another'
void bytes_add (Bytes *this, Bytes *another);

/// Adds to 'this' a copy of 's' without the ending zero
void bytes_add_str (Bytes *this, char *s);

/// bytes_to_json returns a serialization of 'this' using 'to' to
/// convert elements.
Json *bytes_to_json(Bytes *this);

/// bytes_from_json restores a serialized Bytes using 'from' to convert elements.
Bytes *bytes_from_json(Json *js);

#endif
