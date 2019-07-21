// Copyright 21-Jul-2019 ºDeme
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
Bytes *bytes_new(Gc *gc) {
  return bytes_new_bf(gc, 0);
}

Bytes *bytes_new_bf(Gc *gc, int length) {
  Bytes *this = gc_add_bf(gc, malloc(sizeof(Bytes)));
  this->bs = malloc(length);
  this->length = length;
  return this;
}

Bytes *bytes_from_bytes(Gc *gc, unsigned char *bs, int length) {
  Bytes *this = gc_add_bf(gc, malloc(sizeof(Bytes)));
  this->bs = malloc(length);
  memcpy(this->bs, bs, length);
  this->length = length;
  return this;
}

Bytes *bytes_from_str (Gc *gc, char *s) {
  return bytes_from_bytes(gc, (unsigned char *)s, strlen(s));
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
    this->bs = realloc(this->bs, sum);
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

Js *bytes_to_js(Gc *gc, Bytes *this) {
  Gc *gcl = gc_new();
  char *s = b64_encode_bytes(gcl, this);
  Js *r = js_ws(gc, s);
  gc_free(gcl);
  return r;
}

Bytes *bytes_from_js(Gc *gc, Js *js) {
  Gc *gcl = gc_new();
  char *s = js_rs(gcl, js);
  Bytes *r = b64_decode_bytes(gc, s);
  gc_free(gcl);
  return r;
}

