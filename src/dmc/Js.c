// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Js.h"
#include <ctype.h>
#include <locale.h>
#include <errno.h>
#include "dmc/std.h"

#define FAIL(js, msg, gc) THROW("Json", gc)  "%s in\n'%s'\n", msg, js _THROW

static void json_unicode(Buf *bf, char *hexdigits) {
  char hexvalue (char ch) {
    return (ch <= '9') ? ch - '0' : toupper(ch) - 'A' + 10;
  }
  unsigned int codepoint =
    hexvalue(*hexdigits) * 4096 +
    hexvalue(*(hexdigits + 1)) * 256 +
    hexvalue(*(hexdigits + 2)) * 16 +
    hexvalue(*(hexdigits + 3));
  if (codepoint < 0x80) {
    buf_cadd(bf, (codepoint>>0  & 0x7F) | 0x00);
    return;
  }
  if (codepoint < 0x0800) {
    buf_cadd(bf, (codepoint>>6  & 0x1F) | 0xC0);
    buf_cadd(bf, (codepoint>>0  & 0x3F) | 0x80);
    return;
  }
  buf_cadd(bf, (codepoint>>12 & 0x0F) | 0xE0);
  buf_cadd(bf, (codepoint>>6  & 0x3F) | 0x80);
  buf_cadd(bf, (codepoint>>0  & 0x3F) | 0x80);
}

static char *json_blanks(char *json) {
  while (*json && *json <= ' ') {
    ++json;
  }
  return json;
}

inline
static char *json_snull(char *json) {
  return json_blanks(json + 4);
}

static char *json_sbool(char *json) {
  if (*json == 't') {
    return json_blanks(json + 4);
  }
  return json_blanks(json + 5);
}

static char *json_snumber(char *j) {
  while (*j && *j != '}' && *j != ']' && *j != ',' && *j > ' ') {
    ++j;
  }
  return json_blanks(j);
}

static char *json_sstring(char *j) {
  ++j;
  while (*j && *j != '"') {
    if (*j++ == '\\') {
      if (*j) {
        ++j;
      }
    }
  }
  if (*j) {
    ++j;
  }
  return json_blanks(j);
}

static char *json_selement(char *j);

static char *json_sarray(char *j) {
  ++j;
  j = json_blanks(j);
  while (*j && *j != ']') {
    j = json_selement(j);
    if (*j == ',') {
      ++j;
      j = json_blanks(j);
    }
  }
  if (*j) {
    ++j;
  }
  return json_blanks(j);
}

static char *json_sobject(char *j) {
  ++j;
  j = json_blanks(j);
  while (*j && *j != '}') {
    j = json_sstring(j);
    ++j;
    j = json_selement(j);
    if (*j == ',') {
      ++j;
      j = json_blanks(j);
    }
  }
  if (*j) {
    ++j;
  }
  return json_blanks(j);
}

static char *json_selement(char *j) {
  j = json_blanks(j);
  switch (*j) {
  case 'n': return json_snull(j);
  case 't':
  case 'f': return json_sbool(j);
  case '"': return json_sstring(j);
  case '[': return json_sarray(j);
  case '{': return json_sobject(j);
  case '-': return json_snumber(j);
  default :
    if (*j >= '0' && *j <= '9') {
      return json_snumber(j);
    } else {
      if (*j) {
        ++j;
      }
      return j;
    }
  }
}

static int json_rend(char *json) {
  while (*json && *json <= ' ') {
    ++json;
  }
  return *json ? 0 : 1;
}

int js_is_null (Js *json) {
  char *j = json_blanks((char *)json);
  if (memcmp(j, "null", 4)) {
    return 0;
  }
  return json_rend(j + 4);
}

int js_rb (Js *json) {
  char *jsons = json_blanks((char *)json);
  int r;

  char *j = jsons;
  if (memcmp(j, "true", 4)) {
    if (memcmp(j, "false", 5))
      FAIL(jsons, "Bad value reading a boolean value", gc_new())

    r = 0;
    j += 5;
  } else {
    r = 1;
    j += 4;
  }
  if (!json_rend(j))
    FAIL(jsons, "Spare characters reading a boolean value", gc_new())

  return r;
}

int js_ri (Js *json) {
  return js_rl(json);
}

long js_rl (Js *json) {
  char *jsons = json_blanks((char *)json);
  char *j = jsons;
  if (*j != '-' && (*j < '0' || *j > '9'))
    FAIL(j, "Bad start number", gc_new())

  Gc *gc = gc_new();
  Buf *bf = buf_new(gc);
  while (*j && *j != '}' && *j != ']' && *j != ',' && *j > ' ') {
    ++j;
  }
  buf_add_buf(bf, jsons, j - jsons);
  char *n = buf_str(bf);
  if (!json_rend(j))
    FAIL(j, "Spare characters reading a number value", gc)

  char *tail;
  errno = 0;
  long r = strtol(n, &tail, 10);
  if (errno)
    FAIL(n, "Overflow", gc)

  if (*tail)
    FAIL(n, "Bad number", gc)

  gc_free(gc);
  return r;
}

double js_rd (Js *json) {
  struct lconv *lc = localeconv();
  char *jsons = json_blanks((char *)json);
  char *j = jsons;
  if (*j != '-' && (*j < '0' || *j > '9'))
    FAIL(jsons, "Bad start number", gc_new())

  Gc *gc = gc_new();
  Buf *bf = buf_new(gc);
  while (*j && *j != '}' && *j != ']' && *j != ',' && *j > ' ') {
    ++j;
  }
  buf_add_buf(bf, jsons, j - jsons);
  char *n = buf_str(bf);
  int ix = str_cindex(n, '.');
  if (ix != -1) {
    n[ix] = *lc->decimal_point;
  }
  if (!json_rend(j))
    FAIL(jsons, "Spare characters reading a number value", gc)

  errno = 0;
  char *tail;
  double r = strtod(n, &tail);
  if (errno)
    FAIL(n, "Overflow", gc)

  if (*tail)
    FAIL(jsons, "Bad number", gc)

  gc_free(gc);
  return r;
}

char *js_rs (Gc *gc, Js *j) {
  int is_hex (char ch) {
    return (ch >= '0' && ch <= '9') ||
      (ch >= 'a' && ch <= 'f') ||
      (ch >= 'A' && ch <= 'F');
  }

  char *json = json_blanks((char *)j);
  if (*json != '"')
    FAIL(json, "String does not start with \"", gc_new())

  ++json;
  Gc *gcl = gc_new();
  Buf *bf = buf_new(gcl);
  while (*json && *json != '"') {
    if (*json == '\\') {
      ++json;
      switch (*json) {
        case '"' :
        case '\\' :
        case '/' :
          buf_cadd(bf, *json);
          break;
        case 'b' :
          buf_cadd(bf, '\b');
          break;
        case 'f' :
          buf_cadd(bf, '\f');
          break;
        case 'n' :
          buf_cadd(bf, '\n');
          break;
        case 'r' :
          buf_cadd(bf, '\r');
          break;
        case 't' :
          buf_cadd(bf, '\t');
          break;
        case 'u' : {
          ++json;
          int c = 5;
          while (--c) {
            if (!is_hex(*json++))
              FAIL((char *)j, "Bad hexadecimal unicode in string", gcl)
          }
          json_unicode(bf, json - 4);
          continue;
        }
        default :
          FAIL((char *)j, "Bad escape sequence in string", gcl)
      }
      ++json;
    } else {
      buf_cadd(bf, *json++);
    }
  }
  if (!*json)
    FAIL((char *)j, "String does not end with \"", gcl)

  if (!json_rend(json + 1))
    FAIL((char *)j, "Spare characters reading a string value", gcl)

  char *r = buf_to_str(gc, bf);
  gc_free(gcl);
  return r;
}

// Arr[Js]
Arr *js_ra (Gc *gc, Js *j) {
  char *json = json_blanks((char *)j);
  if (*json != '[')
    FAIL(json, "Array does not start with [", gc_new())

  ++json;
  // Arr[Js]
  Arr *a = arr_new(gc);
  while (*json && *json != ']') {
    char *tmp = json;
    json = json_selement(json);
    Gc *gcl = gc_new();
    Buf *bf = buf_new(gcl);
    buf_add_buf(bf, tmp, json - tmp);
    arr_push(a, buf_to_str(gc, bf));
    if (*json == ',') {
      ++json;
    } else if (*json && *json != ']')
      FAIL((char *)j, "Comma missing reading an array value", gcl)

    gc_free(gcl);
  }
  if (!*json)
    FAIL((char *)j, "Array does not end with ]", gc_new())

  if (!json_rend(json + 1))
    FAIL((char *)j, "Spare characters reading an array value", gc_new())

  return a;
}

// Map[Js]
Map *js_ro (Gc *gc, Js *j) {
  char *json = json_blanks((char *)j);
  if (*json != '{')
    FAIL((char *)j, "Object does not start with {", gc_new())

  json = json_blanks(json + 1);
  // Map[Js]
  Map *m = map_new(gc);
  while (*json && *json != '}') {
    if (*json != '"')
      FAIL((char *)j, "Object key is not a string", gc_new())

    char *tmp = json;
    json = json_sstring(json);
    Gc *gcl = gc_new();
    Buf *kbf = buf_new(gcl);
    buf_add_buf(kbf, tmp, json - tmp);

    if (*json != ':')
      FAIL((char *)j, "key-value separator (:) is missing", gcl)

    ++json;
    tmp = json;
    json = json_selement(json);
    Buf *vbf = buf_new(gcl);
    buf_add_buf(vbf, tmp, json - tmp);

    map_put(m,
      js_rs(gc, (Js *)buf_str(kbf)),
      (Js *)buf_to_str(gc, vbf)
    );

    if (*json == ',') {
      json = json_blanks(json + 1);
    } else if (*json && *json != '}')
      FAIL((char *)j, "Comma missing reading an object value", gcl)

    gc_free(gcl);
  }
  if (!*json)
    FAIL((char *)j, "Object does not end with }", gc_new())

  if (!json_rend(json + 1))
    FAIL((char *)j, "Spare characters reading an object value", gc_new())

  return m;
}

Js *js_wn(Gc *gc) {
  return (Js *)str_new(gc, "null");
}

Js *js_wb(Gc *gc, int value) {
  return (Js *)str_new(gc, value ? "true" : "false");
}

Js *js_wi(Gc *gc, int n) {
  return (Js *)str_f(gc, "%d", n);
}

Js *js_wl(Gc *gc, long n) {
  return (Js *)str_f(gc, "%ld", n);
}

Js *js_wd(Gc *gc, double n) {
  struct lconv *lc = localeconv();
  Gc *gcl = gc_new();
  char *ns = str_f(gcl, "%.9f", n);
  int ix = str_cindex(ns, *lc->decimal_point);
  if (ix != -1) {
    ns[ix] = '.';
    int i;
    for (i = strlen(ns) - 1; i >= ix; --i) {
      if (ns[i] != '0') {
        break;
      }
    }
    if (ix != i) {
      ++i;
    }
    if (i) {
      ns = str_left(gc, ns, i);
    } else {
      ns = str_new(gc, "0");
    }
  } else {
    ns = str_new(gc, ns);
  }
  gc_free(gcl);
  return (Js *)ns;
}

Js *js_ws(Gc *gc, char *s) {
  Gc *gcl = gc_new();
  Buf *bf = buf_new(gcl);

  char tmp;
  buf_cadd(bf, '"');
  while (*s) {
    tmp = *s++;
    switch (tmp) {
    case '"':
      buf_add(bf, "\\\"");
      break;
    case '\\':
      buf_add(bf, "\\\\");
      break;
    case '\b':
      buf_add(bf, "\\b");
      break;
    case '\f':
      buf_add(bf, "\\f");
      break;
    case '\n':
      buf_add(bf, "\\n");
      break;
    case '\r':
      buf_add(bf, "\\r");
      break;
    case '\t':
      buf_add(bf, "\\t");
      break;
    default:
      buf_cadd(bf, tmp);
    }
  }
  buf_cadd(bf, '"');
  Js *r = (Js *)buf_to_str(gc, bf);
  gc_free(gcl);
  return r;
}

// a = Arr[Js]
Js *js_wa(Gc *gc, Arr *a) {
  Gc *gcl = gc_new();
  int size = arr_size(a);
  Buf *bf = buf_new(gcl);
  buf_cadd(bf, '[');
  if (size > 0) {
    buf_add(bf, (char *)arr_get(a, 0));
    int i;
    for (i = 1; i < size; ++i) {
      buf_cadd(bf, ',');
      buf_add(bf, (char *)arr_get(a, i));
    }
  }
  buf_cadd(bf, ']');
  Js *r = (Js *)buf_to_str(gc, bf);
  gc_free(gcl);
  return r;
}

// m = Map[Js]
Js *js_wo(Gc *gc, Map *m) {
  Gc *gcl = gc_new();
  Buf *bf = buf_new(gcl);
  buf_cadd(bf, '{');
  EACH_IX(m, Kv, kv, i) {
    if (i) {
      buf_cadd(bf, ',');
    }
    buf_add(bf, (char *)js_ws(gcl, kv_key(kv)));
    buf_cadd(bf, ':');
    buf_add(bf, kv_value(kv));
  }_EACH
  buf_cadd(bf, '}');
  Js *r = (Js *)buf_to_str(gc, bf);
  gc_free(gcl);
  return r;
}

