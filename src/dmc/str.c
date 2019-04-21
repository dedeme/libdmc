// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/str.h"
#include <ctype.h>
#include <wctype.h>
#include "dmc/std.h"

char *str_new (const char *s) {
  char *r = malloc(strlen(s) + 1);
  strcpy(r, s);
  return r;
}

char *str_c_new (char ch) {
  char *r = malloc(2);
  *r = ch;
  r[1] = 0;
  return r;
}

int str_cmp_locale(const char *s1, const char *s2) {
  return strcoll(s1, s2);
}

int str_greater(const char *s1, const char *s2) {
  return strcmp(s1, s2) > 0;
}

int str_greater_locale(const char *s1, const char *s2) {
  return strcoll(s1, s2) > 0;
}

int str_eq (const char *str1, const char *str2) {
  return !strcmp(str1, str2);
}

int str_starts (const char *str, const char  *substr) {
  int sublen = strlen(substr);
  return strlen(str) >= sublen && !memcmp(str, substr, sublen);
}

int str_ends (const char *str, const char  *substr) {
  int slen = strlen(str);
  int sublen = strlen(substr);
  return slen >= sublen && !memcmp(str + slen - sublen, substr, sublen);
}

int str_cindex (const char *str, char ch) {
  int c = 0;
  const char *p  = str;
  while (*p) {
    if (*p++ == ch) {
      return c;
    };
    ++c;
  }
  return -1;
}

int str_cindex_from (const char *str, char ch, int from) {
  if (from >= strlen(str)) {
    return -1;
  }
  int c = from;
  const char *p  = str + from;
  while (*p) {
    if (*p++ == ch) {
      return c;
    };
    ++c;
  }
  return -1;
}

int str_index(const char *str, const char *substr) {
  if (!*substr) {
    return 0;
  }
  int c = 0;
  int limit = strlen(str) - strlen(substr);
  const char *p  = str;
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

int str_index_from(const char *str, const char *substr, int from) {
  if (from >= strlen(str)) {
    return -1;
  }

  if (!*substr) {
    return from;
  }
  int c = from;
  int limit = strlen(str) - strlen(substr);
  const char *p  = str + from;
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

int str_last_cindex(const char *str, char ch) {
  int r = -1;
  int c = 0;
  const char *p  = str;
  while (*p) {
    if (*p++ == ch) {
      r = c;
    };
    ++c;
  }
  return r;
}

int str_last_index(const char *str, const char *substr) {
  int r = -1;
  if (!*substr) {
    return 0;
  }
  int c = 0;
  int limit = strlen(str) - strlen(substr);
  const char *p  = str;
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

char *str_cat_new(const char *s, ...) {
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

  char *r = buf_to_str_new(bf);
  buf_free(bf);
  return r;
}

char *str_sub_new(const char *str, int begin, int end) {
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
    char *r = malloc(1);
    *r = 0;
    return r;
  }
  r = malloc(df + 1);
  r[df] = 0;
  memcpy(r, str + begin, df);
  return r;
}

char *str_left_new(const char *str, int end) {
  return str_sub_new(str, 0, end);
}

char *str_right_new(const char *str, int begin) {
  return str_sub_new(str, begin, strlen(str));
}

char *str_ltrim_new(const char *str) {
  while (isspace(*str)) ++str;
  return str_new(str);
}

char *str_rtrim_new(const char *str) {
  const char *p = str + (strlen(str) - 1);
  while (p >= str && isspace(*p)) {
    --p;
  }
  return str_left_new(str, (p - str) + 1);
}

char *str_trim_new(const char *str) {
  while (isspace(*str)) ++str;
  return str_rtrim_new(str);
}

Arr *str_csplit_new(const char *s, char sep) {
  Arr *r = arr_new(free);
  int i = str_cindex(s, sep);
  while (i != -1) {
    char *sub = malloc(i + 1);
    sub[i] = 0;
    memcpy(sub, s, i);
    arr_push(r, sub);
    s = s + i + 1;
    i = str_cindex(s, sep);
  }
  if (*s) arr_push(r, str_new(s));
  return r;
}

Arr *str_csplit_trim_new(const char *str, char sep) {
  Arr *r = str_csplit_new(str, sep);
  char **p = (char **)arr_start(r);
  char **p_end = (char **)arr_end(r);
  char *tmp;
  while (p < p_end) {
    tmp = *p;
    *p++ = str_trim_new(tmp);
    free(tmp);
  }
  return r;
}

Arr *str_split_new(const char *s, const char *sep) {
  Arr *r = arr_new(free);
  int len = strlen(sep);
  if (!len) {
    arr_push(r, str_new(s));
    return r;
  }
  int i = str_index(s, sep);
  while (i != -1) {
    char *sub = malloc(i + 1);
    sub[i] = 0;
    memcpy(sub, s, i);
    arr_push(r, sub);
    s = s + i + len;
    i = str_index(s, sep);
  }
  if (*s) arr_push(r, str_new(s));
  return r;
}

Arr *str_split_trim_new(const char *str, const char *sep) {
  Arr *r = str_split_new(str, sep);
  char **p = (char **)arr_start(r);
  char **p_end = (char **)arr_end(r);
  char *tmp;
  while (p < p_end) {
    tmp = *p;
    *p++ = str_trim_new(tmp);
    free(tmp);
  }
  return r;
}

char *str_cjoin_new(Arr *a, const char sep) {
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
  char *r = buf_to_str_new(bf);
  buf_free(bf);
  return r;
}

char *str_join_new(Arr *a, const char *sep) {
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
  char *r = buf_to_str_new(bf);
  buf_free(bf);
  return r;
}

void str_creplace(char **s, char old, char new) {
  char *p = *s;
  while (*p) {
    if (*p == old)
      *p = new;
    ++p;
  }
}

char *str_replace_new(const char *s, const char *old, const char *new) {
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
  char *r = buf_to_str_new(bf);
  buf_free(bf);
  return r;
}

char *str_vf_new(const char *format, va_list args) {
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
  va_end(args2);
  return rs;
}

char *str_f_new(const char *format, ...) {
  va_list args;
  va_start(args, format);
  char *r = str_vf_new(format, args);
  va_end(args);
  return r;
}

int str_runes(const char *s) {
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

static char *str_next_rune_new(const char *s) {
  unsigned char b1, b2, b3, b4;
  b1 = *s++;
  if (b1) {
    if (b1 < 0xC2) {
      return str_f_new("%c", b1);
    } else if (b1 < 0xE0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) {
        return str_f_new("%c", 0xFF);
      }
      return str_f_new("%c%c", b1, b2);
    } else if (b1 < 0xF0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80 ||
          (b1 == 0xE0 && b2 < 0xA0)
      ) {
        return str_f_new("%c", 0xFF);
      }
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) {
        return str_f_new("%c", 0xFF);
      }
      return str_f_new("%c%c%c", b1, b2, b3);
    } else if (b1 < 0xF5) {
      b2 = *s++;
      if (((b2 & 0xC0) != 0x80) ||
          (b1 == 0xF0 && b2 < 0x90) ||
          (b1 == 0xF4 && b2 >= 0x90)
      ) {
        return str_f_new("%c", 0xFF);
      }
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) {
        return str_f_new("%c", 0xFF);
      }
      b4 = *s++;
      if ((b4 & 0xC0) != 0x80) {
        return str_f_new("%c", 0xFF);
      }
      return str_f_new("%c%c%c%c", b1, b2, b3, b4);
    } else {
      return str_f_new("%c", 0xFF);
    }
  }
  return str_new("");
}

const char *str_next_rune(char **rune, const char *s) {
  char *r = str_next_rune_new(s);
  free(*rune);
  *rune = r;
  return s + strlen(r);
}

unsigned *str_to_unicode_new_null(const char *s0) {
  unsigned char *s = (unsigned char *)s0;
  unsigned b1, b2, b3, b4;
  int lg = str_runes(s0) + 1;
  unsigned *r = malloc(lg * sizeof(unsigned));
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

char *str_from_unicode_new_null(unsigned *u) {
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
      buf_free(bf);
      return NULL;
    }
  }

  char *r = buf_to_str_new(bf);
  buf_free(bf);
  return r;
}

char *str_from_iso_new(const char *s) {
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
  char *r = buf_to_str_new(bf);
  buf_free(bf);
  return r;
}

char *str_to_upper_new (const char *s) {
  unsigned *ws = str_to_unicode_new_null(s);
  if (!ws) {
    FAIL("str_to_upper: 's' is not a valid utf8 string")
  }

  unsigned *p = ws;
  while (*p) {
    *p = (unsigned)towupper(*p);
    ++p;
  }

  char *r = str_from_unicode_new_null(ws);
  free(ws);
  if (!r) {
    FAIL("str_to_upper: 'ws' is not a valid unicode string")
  }
  return r;
}

char *str_to_lower_new (const char *s) {
  unsigned *ws = str_to_unicode_new_null(s);
  if (!ws) {
    FAIL("str_to_lower: 's' is not a valid utf8 string")
  }

  unsigned *p = ws;
  while (*p) {
    *p = (unsigned)towlower(*p);
    ++p;
  }

  char *r = str_from_unicode_new_null(ws);
  free(ws);
  if (!r) {
    FAIL("str_to_lower: 'ws' is not a valid unicode string")
  }
  return r;
}

char *str_to_escape_new (const char *s) {
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

  char *r = buf_to_str_new(bf);
  buf_free(bf);
  return r;
}

char *str_from_escape_new (const char *s) {
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

  char *r = str_left_new(buf_str(bf), buf_len(bf) - 1);
  buf_free(bf);
  return r;
}
