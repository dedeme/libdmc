// Copyright 04-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dm/dm.h"
#include <ctype.h>
#include <wctype.h>

inline
bool str_eq (void *str1, void  *str2) {
  return str1 && str2 ? !strcmp(str1, str2) : str1 == str2;
}

bool str_starts (char *str, char  *substr) {
  int sublen = strlen(substr);
  return strlen(str) >= sublen && !memcmp(str, substr, sublen);
}

bool str_ends (char *str, char  *substr) {
  int slen = strlen(str);
  int sublen = strlen(substr);
  return slen >= sublen && !memcmp(str + slen - sublen, substr, sublen);
}

int str_cindex (char *str, char ch) {
  int c = 0;
  char *p  = str;
  while (*p) {
    if (*p++ == ch) {
      return c;
    };
    ++c;
  }
  return -1;
}

int str_index (char *str, char *substr) {
  if (!*substr) {
    return 0;
  }
  int c = 0;
  int limit = strlen(str) - strlen(substr);
  char *p  = str;
  while (*p) {
    if (c > limit) {
      break;
    }
    if (str_starts(p++, substr)) {
      return c;
    };
    ++c;
  }
  return -1;
}

int str_last_cindex (char *str, char ch) {
  int r = -1;
  int c = 0;
  char *p  = str;
  while (*p) {
    if (*p++ == ch) {
      r = c;
    };
    ++c;
  }
  return r;
}

int str_last_index (char *str, char *substr) {
  int r = -1;
  if (!*substr) {
    return 0;
  }
  int c = 0;
  int limit = strlen(str) - strlen(substr);
  char *p  = str;
  while (*p) {
    if (c > limit) {
      break;
    }
    if (str_starts(p++, substr)) {
      r = c;
    };
    ++c;
  }
  return r;
}

char *str_copy (char *s) {
  if (!s) return NULL;

  char *r = ATOMIC(strlen(s) + 1);
  strcpy(r, s);
  return r;
}

char *str_cat (char *s, ...) {
  va_list args;
  char *tmp;

  Buf *bf = buf_new();
  buf_add(bf, s);

  va_start(args, s);
  tmp = va_arg(args, char *);
  while (tmp) {
    buf_add(bf, tmp);
    tmp = va_arg(args, char *);
  }
  va_end(args);
  return buf_to_str(bf);
}

char *str_sub (char *str, int begin, int end) {
  int l = strlen(str);
  int df = -1;
  if (begin < 0) {
    begin = l + begin;
  }
  if (end < 0) {
    end = l + end;
  }
  df = end - begin;
  if (begin < 0 || end > l) {
    df = -1;
  }

  char *r;
  if (df < 0) {
    char *r = ATOMIC(1);
    *r = 0;
    return r;
  }
  r = ATOMIC(df + 1);
  r[df] = 0;
  memcpy(r, str + begin, df);
  return r;
}

inline
char *str_sub_end (char *str, int begin) {
  return str_sub(str, begin, strlen(str));
}

char *str_ltrim (char *str) {
  while (isspace(*str)) ++str;

  return str_copy(str);
}

char *str_rtrim (char *str) {
  int ix = strlen(str) - 1;
  while (ix >= 0 && isspace(*(str + ix))) {
    --ix;
  }
  return str_sub(str, 0, ix + 1);
}

inline
char *str_trim (char *str) {
  return str_ltrim(str_rtrim(str));
}

Arr/*char*/ *str_csplit (char *s, char sep) {
  Arr/*char*/ *r = arr_new();
  int i;
  while (*s) {
    i = str_cindex(s, sep);
    if (i == -1) break;
    arr_add(r, str_sub(s, 0, i));
    s = s + i + 1;
  }
  if (*s) arr_add(r, s);
  return r;
}

Arr/*char*/ *str_csplit_trim (char *str, char sep) {
  Arr/*char*/ *r = str_csplit(str, sep);
  RANGE0(i, arr_size(r)) {
    arr_es(r)[i] = str_trim(arr_es(r)[i]);
  }_RANGE
  return r;
}

Arr/*char*/ *str_split (char *s, char *sep) {
  Arr/*char*/ *r = arr_new();
  int len = strlen(sep);
  if (!len) {
    arr_add(r, s);
    return r;
  }
  int i;
  while (*s) {
    i = str_index(s, sep);
    if (i == -1) break;
    arr_add(r, str_sub(s, 0, i));
    s = s + i + len;
  }
  if (*s) arr_add(r, s);
  return r;
}

Arr/*char*/ *str_split_trim (char *str, char *sep) {
  Arr/*char*/ *r = str_split(str, sep);
  RANGE0(i, arr_size(r)) {
    arr_es(r)[i] = str_trim(arr_es(r)[i]);
  }_RANGE
  return r;
}

char *str_cjoin (It/*char*/ *it, char sep) {
  Buf *bf = buf_new();
  if (it_has_next(it)) {
    char *e1 = it_next(it);
    if (it_has_next(it)) {
      buf_add(bf, e1);
      do {
        buf_cadd(bf, sep);
        buf_add(bf, it_next(it));
      } while (it_has_next(it));
      return buf_to_str(bf);
    }
    return e1;
  }
  return "";
}

char *str_join (It/*char*/ *it, char *sep) {
  Buf *bf = buf_new();
  if (it_has_next(it)) {
    char *e1 = it_next(it);
    if (it_has_next(it)) {
      buf_add(bf, e1);
      do {
        buf_add(bf, sep);
        buf_add(bf, it_next(it));
      } while (it_has_next(it));
      return buf_to_str(bf);
    }
    return e1;
  }
  return "";
}

char *str_creplace(char *s, char old, char new) {
  char *r = str_copy(s);
  char *p = r;
  while (*p) {
    if (*p == old)
      *p = new;
    ++p;
  }
  return r;
}

char *str_replace(char *s, char *old, char *new) {
  if (!*old) return str_copy(s);

  Buf *bf = buf_new();
  int len = strlen(old);
  int ix;

  ix = str_index(s, old);
  while (ix != -1) {
    buf_add(bf, str_sub(s, 0, ix));
    buf_add(bf, new);
    s += ix + len;
    ix = str_index(s, old);
  }
  buf_add(bf, s);
  return buf_to_str(bf);
}

char *str_printf(char *format, ...) {
  va_list args;
  va_start(args, format);
  char *r = str_vprintf(format, args);
  va_end(args);
  return r;
}

char *str_vprintf(char *format, va_list args) {
  int length = 0;
  char *rs = (char *)malloc(length);
  va_list args2;
  va_copy(args2, args);
  int nchars = vsnprintf(rs, length, format, args);
  if (nchars >= length) {
    length = nchars + 1;
    free(rs);
    rs = (char *)malloc(length);
    vsnprintf(rs, length, format, args2);
  }
  char *r = str_copy(rs);
  va_end(args2);
  free(rs);
  return r;
}

int str_runes (char *s) {
  unsigned char b1, b2, b3, b4;
  int r = 0;
  while (*s) {
    ++r;
    b1 = *s++;
    if (b1 < 0x80) continue;
    else if (b1 < 0xC2) return -1;
    else if (b1 < 0xE0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return -1;
    } else if (b1 < 0xF0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return -1;
      if (b1 == 0xE0 && b2 < 0xA0) return -1;
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) return -1;
    } else if (b1 < 0xF5) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return -1;
      if (b1 == 0xF0 && b2 < 0x90) return -1;
      if (b1 == 0xF4 && b2 >= 0x90) return -1;
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) return -1;
      b4 = *s++;
      if ((b4 & 0xC0) != 0x80) return -1;
    } else return -1;
  }
  return r;
}

char *str_next_rune (char *s) {
  unsigned char b1, b2, b3, b4;
  b1 = *s++;
  if (b1) {
    if (b1 < 0xC2) {
      return str_printf("%c", b1);
    } else if (b1 < 0xE0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) {
        return str_printf("%c", 0xFF);
      }
      return str_printf("%c%c", b1, b2);
    } else if (b1 < 0xF0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80 ||
          (b1 == 0xE0 && b2 < 0xA0)
      ) {
        return str_printf("%c", 0xFF);
      }
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) {
        return str_printf("%c", 0xFF);
      }
      return str_printf("%c%c%c", b1, b2, b3);
    } else if (b1 < 0xF5) {
      b2 = *s++;
      if (((b2 & 0xC0) != 0x80) ||
          (b1 == 0xF0 && b2 < 0x90) ||
          (b1 == 0xF4 && b2 >= 0x90)
      ) {
        return str_printf("%c", 0xFF);
      }
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) {
        return str_printf("%c", 0xFF);
      }
      b4 = *s++;
      if ((b4 & 0xC0) != 0x80) {
        return str_printf("%c", 0xFF);
      }
      return str_printf("%c%c%c%c", b1, b2, b3, b4);
    } else {
      return str_printf("%c", 0xFF);
    }
  }
  return "";
}

unsigned *str_to_unicode (char *s0) {
  unsigned char *s = (unsigned char *)s0;
  unsigned b1, b2, b3, b4;
  int lg = str_runes(s0) + 1;
  unsigned *r = ATOMIC(lg * sizeof(unsigned));
  unsigned *pr = r;
  while (*s) {
    b1 = *s++;
    if (b1 < 0x80) *pr++ = b1;
    else if (b1 < 0xC2) return NULL;
    else if (b1 < 0xE0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return NULL;
      *pr++ = (b1 << 6) + b2 - 0x3080;
    } else if (b1 < 0xF0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return NULL;
      if (b1 == 0xE0 && b2 < 0xA0) return NULL;
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) return NULL;
      *pr++ = (b1 << 12) + (b2 << 6) + b3 - 0xE2080;
    } else if (b1 < 0xF5) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return NULL;
      if (b1 == 0xF0 && b2 < 0x90) return NULL;
      if (b1 == 0xF4 && b2 >= 0x90) return NULL;
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) return NULL;
      b4 = *s++;
      if ((b4 & 0xC0) != 0x80) return NULL;
      *pr++ = (b1 << 18) + (b2 << 12) + (b3 << 6) + b4 - 0x3C82080;
    } else {
      return NULL;
    }
  }
  *pr = 0;
  return r;
}

char *str_from_unicode (unsigned *u) {
  Buf *bf = buf_new();

  while (*u) {
    unsigned code_point = *u++;
    if (code_point < 0x80) {
      buf_cadd(bf, (unsigned char)code_point);
    } else if (code_point <= 0x7FF) {
      buf_cadd(bf, (unsigned char)(code_point >> 6) + 0xC0);
      buf_cadd(bf, (unsigned char)(code_point & 0x3F) + 0x80);
    } else if (code_point <= 0xFFFF) {
      buf_cadd(bf, (unsigned char)(code_point >> 12) + 0xE0);
      buf_cadd(bf, (unsigned char)((code_point >> 6) & 0x3F) + 0x80);
      buf_cadd(bf, (unsigned char)(code_point & 0x3F) + 0x80);
    } else if (code_point <= 0x10FFFF) {
      buf_cadd(bf, (unsigned char)(code_point >> 18) + 0xF0);
      buf_cadd(bf, (unsigned char)((code_point >> 12) & 0x3F) + 0x80);
      buf_cadd(bf, (unsigned char)((code_point >> 6) & 0x3F) + 0x80);
      buf_cadd(bf, (unsigned char)(code_point & 0x3F) + 0x80);
    } else {
      return NULL;
    }
  }

  return buf_to_str(bf);
}

char *str_from_iso (char *s) {
  Buf *bf = buf_new();
  unsigned char ch = *s++;
  while (ch) {
    if(ch < 0x80) {
      buf_cadd(bf, ch);
    } else {
      buf_cadd(bf, 0xc0 | (ch & 0xc0) >> 6);
      buf_cadd(bf, 0x80 | (ch & 0x3f));
    }
    ch = *s++;
  }
  return buf_to_str(bf);
}

char *str_to_upper (char *s) {
  unsigned *ws = str_to_unicode(s);
  unsigned *p = ws;
  while (*p) {
    *p = (unsigned)towupper(*p);
    ++p;
  }
  return str_from_unicode(ws);
}

char *str_to_lower (char *s) {
  unsigned *ws = str_to_unicode(s);
  unsigned *p = ws;
  while (*p) {
    *p = (unsigned)towlower(*p);
    ++p;
  }
  return str_from_unicode(ws);
}

char *str_to_escape (char *s) {
  Buf *bf = buf_new();
  while (*s) {
    char ch = *s++;
    if (ch == '"' || ch == '\\') {
      buf_cadd(bf, '\\');
    }
    buf_cadd(bf, ch);
  }
  return str_printf("\"%s\"", buf_str(bf));
}

char *str_from_escape (char *s) {
  int len = strlen(s);
  if (len < 2) {
    return s;
  }

  char *p = s + 1;
  Buf *bf = buf_new();
  while (*p) {
    char ch = *p++;
    if (ch != '\\') {
      buf_cadd(bf, ch);
    } else {
      buf_cadd(bf, *p++);
    }
  }

  buf_str(bf)[buf_length(bf) - 1] = 0;
  return buf_to_str(bf);
}
