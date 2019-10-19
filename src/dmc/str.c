// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/str.h"
#include <wctype.h>
#include "dmc/std.h"

char *str_new (char *s) {
  char *r = ATOMIC(strlen(s) + 1);
  strcpy(r, s);
  return r;
}

char *str_c (char ch) {
  char *r = ATOMIC(2);
  *r = ch;
  r[1] = 0;
  return r;
}

int str_cmp_locale(char *s1, char *s2) {
  return strcoll(s1, s2);
}

int str_greater(char *s1, char *s2) {
  return strcmp(s1, s2) > 0;
}

int str_greater_locale(char *s1, char *s2) {
  return strcoll(s1, s2) > 0;
}

int str_eq (char *str1, char *str2) {
  return !strcmp(str1, str2);
}

int str_starts (char *str, char  *substr) {
  int sublen = strlen(substr);
  return strlen(str) >= sublen && !memcmp(str, substr, sublen);
}

int str_ends (char *str, char  *substr) {
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

int str_cindex_from (char *str, char ch, int from) {
  if (from >= strlen(str)) {
    return -1;
  }
  int c = from;
  char *p  = str + from;
  while (*p) {
    if (*p++ == ch) {
      return c;
    };
    ++c;
  }
  return -1;
}

int str_index(char *str, char *substr) {
  if (!*substr) {
    return 0;
  }
  int c = 0;
  int limit = strlen(str) - strlen(substr);
  char *p  = str;
  for (;;) {
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

int str_index_from(char *str, char *substr, int from) {
  if (from >= strlen(str)) {
    return -1;
  }

  if (!*substr) {
    return from;
  }
  int c = from;
  int limit = strlen(str) - strlen(substr);
  char *p  = str + from;
  for (;;) {
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

int str_last_cindex(char *str, char ch) {
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

int str_last_index(char *str, char *substr) {
  int r = -1;
  if (!*substr) {
    return strlen(str);
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

char *str_cat(char *s, ...) {
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

char *str_sub(char *str, int begin, int end) {
  int l = strlen(str);
  int df = -1;
  if (begin < 0) {
    begin = l + begin;
  }
  EXC_RANGE(begin, 0, l)

  if (end < 0) {
    end = l + end;
  }
  EXC_RANGE(end, 0, l)

  df = end - begin;

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

char *str_left(char *str, int end) {
  return str_sub(str, 0, end);
}

char *str_right(char *str, int begin) {
  return str_sub(str, begin, strlen(str));
}

char *str_ltrim(char *str) {
  unsigned char ch = *str;
  while (ch && ch <= ' ') ch = *++str;
  return str_new(str);
}

char *str_rtrim(char *str) {
  char *p = str + (strlen(str) - 1);
  while (p >= str && ((unsigned char)*p) <= ' ') {
    --p;
  }
  return str_left(str, (p - str) + 1);
}

char *str_trim(char *str) {
  unsigned char ch = *str;
  while (ch && ch <= ' ') ch = *++str;
  return str_rtrim(str);
}

// Returns Arr[char]
Arr *str_csplit(char *s, char sep) {
  // Arr[char]
  Arr *r = arr_new();
  int i = str_cindex(s, sep);
  while (i != -1) {
    char *sub = ATOMIC(i + 1);
    sub[i] = 0;
    memcpy(sub, s, i);
    arr_push(r, sub);
    s = s + i + 1;
    i = str_cindex(s, sep);
  }
  arr_push(r, str_new(s));
  return r;
}

// Returns Arr[char]
Arr *str_csplit_trim(char *str, char sep) {
  // Arr[char]
  Arr *r = str_csplit(str, sep);
  char **p = (char **)arr_start(r);
  char **p_end = (char **)arr_end(r);
  while (p < p_end) {
    *p = str_trim(*p);
    ++p;
  }
  return r;
}

// Returns Arr[char]
Arr *str_split(char *s, char *sep) {
  // Arr[char]
  Arr *r = arr_new();
  int len = strlen(sep);
  if (!len) {
    char *rune;
    s = str_next_rune(&rune, s);
    while (*rune) {
      arr_push(r, rune);
      s = str_next_rune(&rune, s);
    }
    return r;
  }

  int i = str_index(s, sep);
  while (i != -1) {
    char *sub = ATOMIC(i + 1);
    sub[i] = 0;
    memcpy(sub, s, i);
    arr_push(r, sub);
    s = s + i + len;
    i = str_index(s, sep);
  }
  arr_push(r, str_new(s));
  return r;
}

Arr *str_split_trim(char *str, char *sep) {
  Arr *r = str_split(str, sep);
  char **p = (char **)arr_start(r);
  char **p_end = (char **)arr_end(r);
  while (p < p_end) {
    *p = str_trim(*p);
    ++p;
  }
  return r;
}

char *str_cjoin(Arr *a, char sep) {
  Buf *bf = buf_new();
  int first = 1;
  EACH(a, char, e)
    if (first) {
      first = 0;
    } else {
      buf_cadd(bf, sep);
    }
    buf_add(bf, e);
  _EACH
  return buf_to_str(bf);
}

char *str_join(Arr *a, char *sep) {
  Buf *bf = buf_new();
  int first = 1;
  EACH(a, char, e)
    if (first) {
      first = 0;
    } else {
      buf_add(bf, sep);
    }
    buf_add(bf, e);
  _EACH
  return buf_to_str(bf);
}

char *str_creplace(char *s, char old, char new) {
  char *r = str_new(s);
  char *p = r;
  while (*p) {
    if (*p == old)
      *p = new;
    ++p;
  }
  return r;
}

char *str_replace(char *s, char *old, char *new) {
  if (!*old) return str_new(s);

  Buf *bf = buf_new();
  int len = strlen(old);
  int i = str_index(s, old);
  while (i != -1) {
    buf_add_buf(bf, s, i);
    buf_add(bf, new);
    s = s + i + len;
    i = str_index(s, old);
  }
  buf_add_buf(bf, s, strlen(s));
  return buf_to_str(bf);
}

char *str_vf(char *format, va_list args) {
  int length = 0;
  char *rs = ATOMIC(length);
  va_list args2;
  va_copy(args2, args);
  int nchars = vsnprintf(rs, length, format, args);
  if (nchars >= length) {
    length = nchars + 1;
    rs = ATOMIC(length);
    vsnprintf(rs, length, format, args2);
  }
  va_end(args2);
  return rs;
}

char *str_f(char *format, ...) {
  va_list args;
  va_start(args, format);
  char *r = str_vf(format, args);
  va_end(args);
  return r;
}

int str_runes(char *s) {
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

static char *_str_next_rune(char *s) {
  unsigned char b1, b2, b3, b4;
  b1 = *s++;
  if (b1) {
    if (b1 < 0xC2) {
      return str_f("%c", b1);
    } else if (b1 < 0xE0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) {
        return str_f("%c", 0xFF);
      }
      return str_f("%c%c", b1, b2);
    } else if (b1 < 0xF0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80 ||
          (b1 == 0xE0 && b2 < 0xA0)
      ) {
        return str_f("%c", 0xFF);
      }
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) {
        return str_f("%c", 0xFF);
      }
      return str_f("%c%c%c", b1, b2, b3);
    } else if (b1 < 0xF5) {
      b2 = *s++;
      if (((b2 & 0xC0) != 0x80) ||
          (b1 == 0xF0 && b2 < 0x90) ||
          (b1 == 0xF4 && b2 >= 0x90)
      ) {
        return str_f("%c", 0xFF);
      }
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) {
        return str_f("%c", 0xFF);
      }
      b4 = *s++;
      if ((b4 & 0xC0) != 0x80) {
        return str_f("%c", 0xFF);
      }
      return str_f("%c%c%c%c", b1, b2, b3, b4);
    } else {
      return str_f("%c", 0xFF);
    }
  }
  return str_new("");
}

char *str_next_rune(char **rune, char *s) {
  char *r = _str_next_rune(s);
  *rune = r;
  return s + strlen(r);
}

// Opt[unsigned]
Opt *str_to_unicode(char *s0) {
  unsigned char *s = (unsigned char *)s0;
  unsigned b1, b2, b3, b4;
  int lg = str_runes(s0) + 1;
  unsigned *r = ATOMIC(lg * sizeof(unsigned));
  unsigned *pr = r;
  while (*s) {
    b1 = *s++;
    if (b1 < 0x80) *pr++ = b1;
    else if (b1 < 0xC2) return opt_empty();
    else if (b1 < 0xE0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return opt_empty();
      *pr++ = (b1 << 6) + b2 - 0x3080;
    } else if (b1 < 0xF0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return opt_empty();
      if (b1 == 0xE0 && b2 < 0xA0) return opt_empty();
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) return opt_empty();
      *pr++ = (b1 << 12) + (b2 << 6) + b3 - 0xE2080;
    } else if (b1 < 0xF5) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return opt_empty();
      if (b1 == 0xF0 && b2 < 0x90) return opt_empty();
      if (b1 == 0xF4 && b2 >= 0x90) return opt_empty();
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) return opt_empty();
      b4 = *s++;
      if ((b4 & 0xC0) != 0x80) return opt_empty();
      *pr++ = (b1 << 18) + (b2 << 12) + (b3 << 6) + b4 - 0x3C82080;
    } else {
      return opt_empty();
    }
  }
  *pr = 0;
  return opt_new(r);
}

// Opt[char]
Opt *str_from_unicode(unsigned *u) {
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
      return opt_empty();
    }
  }

  return opt_new(buf_to_str(bf));
}

char *str_from_iso(char *s) {
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
  unsigned *ws = opt_eget(
    str_to_unicode(s),
    "str_to_upper: 's' is not a valid utf8 string"
  );

  unsigned *p = ws;
  while (*p) {
    *p = (unsigned)towupper(*p);
    ++p;
  }

  return opt_eget(
    str_from_unicode(ws),
    "str_to_upper: 'ws' is not a valid unicode string"
  );
}

char *str_to_lower (char *s) {
  unsigned *ws = opt_eget(
    str_to_unicode(s),
    "str_to_lower: 's' is not a valid utf8 string"
  );

  unsigned *p = ws;
  while (*p) {
    *p = (unsigned)towlower(*p);
    ++p;
  }

  return opt_eget(
    str_from_unicode(ws),
    "str_to_lower: 'ws' is not a valid unicode string"
  );
}

char *str_to_escape (char *s) {
  Buf *bf = buf_new();
  buf_cadd(bf, '"');
  while (*s) {
    char ch = *s++;
    if (ch == '"' || ch == '\\') {
      buf_cadd(bf, '\\');
    }
    buf_cadd(bf, ch);
  }
  buf_cadd(bf, '"');

  return buf_to_str(bf);
}

char *str_from_escape (char *s) {
  int len = strlen(s);
  if (len < 2) {
    return str_new(s);
  }

  ++s;
  Buf *bf = buf_new();
  while (*s) {
    char ch = *s++;
    if (ch != '\\') {
      buf_cadd(bf, ch);
    } else {
      buf_cadd(bf, *s++);
    }
  }

  return str_left(buf_str(bf), buf_len(bf) - 1);
}
