// Copyright 21-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Buf.h"
#include <string.h>
#include "dmc/DEFS.h"

Buf *buf_new(void) {
  return buf_bf_new(151);
}

Buf *buf_bf_new(int buffer_size) {
  Buf *this = MALLOC(Buf);
  this->bf_size = buffer_size - 1;
  this->str = ATOMIC(buffer_size);
  memset(this->str, 0, buffer_size);
  this->len = 0;
  return this;
}

void buf_add_buf (Buf *this, char *data, int length) {
  int ixend = this->len + length;
  if (this->bf_size < ixend) {
      while (this->bf_size < ixend) {
          this->bf_size += this->bf_size;
      }
      int memsize = this->bf_size + 1;
      char *newstr = ATOMIC(memsize);
      memset(newstr, 0, memsize);
      memcpy(newstr, this->str, this->len);
      this->str = newstr;
  }
  memcpy(this->str + this->len, data, length);
  this->len = ixend;
}

void buf_add (Buf *this, char *data) {
  buf_add_buf(this, data, strlen(data));
}

void buf_cadd (Buf *this, char data) {
  buf_add_buf(this, &data, 1);
}

char *buf_to_str (Buf *this) {
  char *s = this->str;
  char *r = ATOMIC(strlen(s) + 1);
  strcpy(r, s);
  return r;
}

void buf_reset(Buf *this) {
  memset(this->str, 0, this->bf_size);
  this->len = 0;
}
