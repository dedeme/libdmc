// Copyright 21-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Structure for working with bytes.
/// Example:
///   Bytes *bs = bytes_new();
///   unsigned char bss[] = {0, 23, 116, 225};
///   bytes_add_bytes(bs, bss, 4);
///   char *b64 = b64_encode_bytes(bs);
///   assert(!strcmp("ABd04Q==", b64));

#ifndef DMC_BYTES_H
  #define DMC_BYTES_H

/// Bytes structure
struct bytes_Bytes {
  unsigned char *bs;
  int len;
};
/// Bytes structure
typedef struct bytes_Bytes Bytes;

/// Returns an empty 'Bytes'.
Bytes *bytes_new(void);

/// Returns a 'Bytes' of length 'length' of indetermined values.
Bytes *bytes_bf_new(int length);

/// Returns a new allocated 'Bytes' whitch is copy of 'bs'.
Bytes *bytes_from_bytes(unsigned char *bs, int length);

/// Returns a 'Bytes' whitch is copy of 's' without the ending zero.
Bytes *bytes_from_str(char *s);

/// Adds to 'this' a new copy of 'bs'.
void bytes_add_bytes(Bytes *this, unsigned char *bs, int length);

/// Adds to 'this' a new copy of 'another'.
void bytes_add(Bytes *this, Bytes *another);

/// Adds to 'this' a copy of 's' without the ending zero.
void bytes_add_str(Bytes *this, char *s);

/// Returns a JSON representation of 'this'.
char *bytes_to_js(Bytes *this);

/// Returns a 'Bytes' from its JSON representation.
Bytes *bytes_from_js(char *js);

#endif
