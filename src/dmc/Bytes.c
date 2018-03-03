// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/all.h"

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
  Bytes *this = MALLOC(Bytes);
  this->bs = ATOMIC(length);
  memcpy(this->bs, bs, length);
  this->length = length;
  return this;
}

inline
Bytes *bytes_from_str (char *s) {
  return bytes_from_bytes((unsigned char *)s, strlen(s));
}

inline
unsigned char *bytes_bs(Bytes *this) {
  return this->bs;
}

inline
size_t bytes_length(Bytes *this) {
  return this->length;
}

void bytes_add_bytes (Bytes *this, unsigned char *bs, size_t length) {
  size_t sum = this->length + length;
  this->bs = GC_REALLOC(this->bs, sum);
  memcpy(this->bs + this->length, bs, length);
  this->length = sum;
}

inline
void bytes_add (Bytes *this, Bytes *another) {
  bytes_add_bytes(this, another->bs, another->length);
}

inline
void bytes_add_str (Bytes *this, char *s) {
  bytes_add_bytes(this, (unsigned char *)s, strlen(s));
}

