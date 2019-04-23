// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Bytes.h"
#include <string.h>
#include "dmc/std.h"
#include "dmc/b64.h"

struct bytes_Bytes {
  unsigned char *bs;
  int length;
};

inline
Bytes *bytes_new() {
  return bytes_bf_new(0);
}

Bytes *bytes_bf_new(int length) {
  Bytes *this = MALLOC(Bytes);
  this->bs = ATOMIC(length);
  this->length = length;
  return this;
}

Bytes *bytes_from_bytes(unsigned char *bs, int length) {
  Bytes *this = MALLOC(Bytes);
  this->bs = ATOMIC(length);
  memcpy(this->bs, bs, length);
  this->length = length;
  return this;
}

Bytes *bytes_from_str (char *s) {
  return bytes_from_bytes((unsigned char *)s, strlen(s));
}

unsigned char *bytes_bs(Bytes *this) {
  return this->bs;
}

int bytes_len(Bytes *this) {
  return this->length;
}

void bytes_add_bytes (Bytes *this, unsigned char *bs, int length) {
  int sum = this->length + length;
  if (sum) {
    this->bs = GC_REALLOC(this->bs, sum);
    memcpy(this->bs + this->length, bs, length);
    this->length = sum;
  }
}

void bytes_add (Bytes *this, Bytes *another) {
  bytes_add_bytes(this, another->bs, another->length);
}

void bytes_add_str (Bytes *this, char *s) {
  bytes_add_bytes(this, (unsigned char *)s, strlen(s));
}

Js *bytes_to_js(Bytes *this) {
  return js_ws(b64_encode_bytes(this));
}

Bytes *bytes_from_js(Js *js) {
  return b64_decode_bytes(js_rs(js));
}
