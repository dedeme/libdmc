// Copyright 01-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// B64 encoder - decoder

#ifndef DM_B64_H
  #define DM_B64_H

typedef struct bytes_Bytes Bytes;

///
char *b64_decode(char *b64);

///
Bytes *b64_decode_bytes(char *b64);

///
char *b64_encode(char *s);

///
char *b64_encode_bytes(Bytes *bs);

#endif
