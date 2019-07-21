// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Buf.h"
#include <string.h>
#include "dmc/str.h"

struct buf_Buf{
  char *str;
  int length;
  int size;
};

Buf *buf_new(Gc *gc) {
  return buf_bf_new(gc, 250);
}

Buf *buf_bf_new(Gc *gc, int sz) {
  Buf *this = gc_add_bf(gc, malloc(sizeof(Buf)));
  this->size = sz - 1;
  this->str = malloc(sz);
  memset(this->str, 0, sz);
  this->length = 0;
  return this;
}

int buf_len(Buf *this) {
  return this->length;
}

char *buf_str(Buf *this) {
  return this->str;
}

void buf_add_buf (Buf *this, char *data, int length) {
  int ixend = this->length + length;
  if (this->size < ixend) {
      while (this->size < ixend) {
          this->size += this->size;
      }
      int memsize = this->size + 1;
      this->str = realloc(this->str, memsize);
  }
  memcpy(this->str + this->length, data, length);
  this->length = ixend;
}

void buf_add (Buf *this, char *data) {
  buf_add_buf(this, data, strlen(data));
}

void buf_cadd (Buf *this, char data) {
  buf_add_buf(this, &data, 1);
}

char *buf_to_str(Gc *gc, Buf *this) {
  return str_new(gc, this->str);
}

void buf_reset(Buf *this) {
  memset(this->str, 0, this->size);
  this->length = 0;
}
