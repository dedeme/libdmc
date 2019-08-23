// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Strings constructor.

#ifndef DMC_BUF_H
  #define DMC_BUF_H

///
typedef struct buf_Buf Buf;

/// Initializes a 'buf' with size 150.
Buf *buf_new(void);

/// Initializes a 'buf'.
Buf *buf_bf_new(int buffer_size);

/// Returns the length of the enveloped string.
int buf_len(Buf *this);

/// Returns a reference to the string wrapped. Return is intended to
/// be not modified.
char *buf_str(Buf *this);

/// Adds 'length bytes of 'data' to 'buf'.
/// 'length' must be less or equals to 'strlen(data)'.
/// It is not necessary that 'data' be a null-terminated string, but it must
/// no have characters \0
void buf_add_buf(Buf *this, char *data, int length);

/// Adds 'data' to 'buf'.
void buf_add(Buf *this, char *data);

/// Adds a character.
void buf_cadd(Buf *this, char data);

/// Returns a copy of the enveloped string.
char *buf_to_str(Buf *this);

/// Resets buffer (but does not reduce its size).
void buf_reset(Buf *this);

#endif
