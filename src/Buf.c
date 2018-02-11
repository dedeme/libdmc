// Copyright 04-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dm.h"

struct buf_Buf{
  char *str;     // R
  size_t length; // R Length of string in str.
  size_t _size;  // - Buffer size
};

Buf *buf_new () {
  Buf *this = MALLOC(Buf);
  size_t memsize = 151;
  this->_size = memsize - 1;
  this->str = ATOMIC(memsize);
  memset(this->str, 0, memsize);
  this->length = 0;
  return this;
}

inline
char *buf_str (Buf *this) {
  return this->str;
}

inline
size_t buf_length (Buf *this) {
  return this->length;
}

void buf_add_buf (Buf *this, const char *data, size_t length) {
  int ixend = this->length + length;
  if (this->_size < ixend) {
      while (this->_size < ixend) {
          this->_size += this->_size;
      }
      size_t memsize = this->_size + 1;
      char *newstr = ATOMIC(memsize);
      memset(newstr, 0, memsize);
      memcpy(newstr, this->str, this->length);
      this->str = newstr;
  }
  memcpy(this->str + this->length, data, length);
  this->length = ixend;
}

inline
void buf_add (Buf *this, const char *data) {
  buf_add_buf(this, data, strlen(data));
}

inline
void buf_cadd (Buf *this, char data) {
  buf_add_buf(this, &data, 1);
}

inline
char *buf_to_str (Buf *this) {
  return str_copy(this->str);
}
