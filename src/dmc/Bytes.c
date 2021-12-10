// Copyright 21-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Bytes.h"
#include <string.h>
#include "dmc/std.h"

inline
Bytes *bytes_new() {
  return bytes_bf_new(0);
}

Bytes *bytes_bf_new(int length) {
  Bytes *this = MALLOC(Bytes);
  this->bs = ATOMIC(length);
  this->len = length;
  return this;
}

Bytes *bytes_from_bytes(unsigned char *bs, int length) {
  Bytes *this = MALLOC(Bytes);
  this->bs = ATOMIC(length);
  memcpy(this->bs, bs, length);
  this->len = length;
  return this;
}

Bytes *bytes_from_str (char *s) {
  return bytes_from_bytes((unsigned char *)s, strlen(s));
}

void bytes_add_bytes (Bytes *this, unsigned char *bs, int length) {
  int sum = this->len + length;
  if (sum) {
    this->bs = GC_REALLOC(this->bs, sum);
    memcpy(this->bs + this->len, bs, length);
    this->len = sum;
  }
}

void bytes_add (Bytes *this, Bytes *another) {
  bytes_add_bytes(this, another->bs, another->len);
}

void bytes_add_str (Bytes *this, char *s) {
  bytes_add_bytes(this, (unsigned char *)s, strlen(s));
}


char *bytes_to_js(Bytes *this) {
  char *b64 = b64_encode_bytes(this);
  size_t len = strlen(b64);
  char *r = ATOMIC(len + 3);
  r[0] = '"';
  memcpy(r + 1, b64, len);
  r[len + 1] = '"';
  r[len + 2] = 0;
  return r;
}

Bytes *bytes_from_js(char *js) {
  size_t len = strlen(js) - 2;
  char *b64 = ATOMIC(len + 1);
  memcpy(b64, js + 1, len);
  b64[len] = 0;

  return b64_decode_bytes(b64);
}

