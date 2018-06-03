// Copyright 2-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include <string.h>
#include "dmc/Bytes.h"
#include "dmc/exc.h"
#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"
#include "dmc/DEFS.h"

struct bytes_Bytes {
  unsigned char *bs; // R
  size_t length;     // R
};

inline
Bytes *bytes_new() {
  return bytes_new_len(0);
}

Bytes *bytes_new_len(size_t length) {
  Bytes *this = MALLOC(Bytes);
  this->bs = ATOMIC(length);
  this->length = length;
  return this;
}

Bytes *bytes_from_bytes (unsigned char *bs, size_t length) {
  XNULL(bs)

  Bytes *this = MALLOC(Bytes);
  this->bs = ATOMIC(length);
  memcpy(this->bs, bs, length);
  this->length = length;
  return this;
}

Bytes *bytes_from_str (char *s) {
  XNULL(s)
  return bytes_from_bytes((unsigned char *)s, strlen(s));
}

unsigned char *bytes_bs(Bytes *this) {
  XNULL(this)
  return this->bs;
}

size_t bytes_length(Bytes *this) {
  XNULL(this)
  return this->length;
}

void bytes_add_bytes (Bytes *this, unsigned char *bs, size_t length) {
  XNULL(this)
  XNULL(bs)

  size_t sum = this->length + length;
  this->bs = GC_REALLOC(this->bs, sum);
  memcpy(this->bs + this->length, bs, length);
  this->length = sum;
}

void bytes_add (Bytes *this, Bytes *another) {
  XNULL(this)
  XNULL(another)
  bytes_add_bytes(this, another->bs, another->length);
}

void bytes_add_str (Bytes *this, char *s) {
  XNULL(this)
  XNULL(s)
  bytes_add_bytes(this, (unsigned char *)s, strlen(s));
}

Ajson *bytes_to_json(Bytes *this) {
  XNULL(this)

  Ajson *r = ajson_new();
  unsigned char *p = this->bs;
  REPEAT(this->length) {
    ajson_add(r, json_wuint((unsigned)*p++));
  }_REPEAT
  return r;
}

Bytes *bytes_from_json(Ajson *js) {
  XNULL(js)

  Bytes *r = bytes_new_len(ajson_size(js));
  unsigned char *p = r->bs;
  EACH(js, Json, j) {
    *p++ = (unsigned char)json_ruint(j);
  }_EACH
  return r;
}
