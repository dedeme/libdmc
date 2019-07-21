// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// String constructor.

#ifndef DMC_BUF_H
  #define DMC_BUF_H

#include "Gc.h"

///
typedef struct buf_Buf Buf;

/// buf_new initializes a 'buf' with size 150
Buf *buf_new(Gc *gc);

/// buf_bf_new initializes a 'buf'
Buf *buf_bf_new(Gc *gc, int sz);

/// buf_len returns the length of the enveloped string
int buf_len(Buf *this);

/// buf_str returns a reference to the string wrapped. Return is intended to
/// be not modified.
char *buf_str(Buf *this);

/// buf_add_buf adds 'length bytes of 'data' to 'buf'.<br>
/// 'length' must be less or equals to 'strlen(data)'.<br>
/// It is not necessary that 'data' be a null-terminated string, but it must
/// no have characters \0
void buf_add_buf(Buf *this, char *data, int length);

/// buf_add adds 'data' to 'buf'.
void buf_add(Buf *this, char *data);

/// buf_cadd adds a character
void buf_cadd(Buf *this, char data);

/// buf_to_str returns a copy of the enveloped string
char *buf_to_str(Gc *gc, Buf *this);

/// buf_reset resets buffer (but does not reduce its size)
void buf_reset(Buf *this);

#endif
