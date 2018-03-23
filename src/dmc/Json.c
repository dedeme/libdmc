// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/all.h"
#include <ctype.h>
#include <locale.h>

static void json_unicode (Buf *bf, char *hexdigits) {
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

static char *json_blanks (char *json) {
  while (*json && *json <= ' ') {
    ++json;
  }
  return json;
}

inline
static char *json_snull (char *json) {
  return json_blanks(json + 4);
}

static char *json_sbool (char *json) {
  if (*json == 't') {
    return json_blanks(json + 4);
  }
  return json_blanks(json + 5);
}

static char *json_snumber (char *j) {
  while (*j && *j != '}' && *j != ']' && *j != ',' && *j > ' ') {
    ++j;
  }
  return json_blanks(j);
}

static char *json_sstring (char *j) {
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

static char *json_selement (char *j);

static char *json_sarray (char *j) {
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

static char *json_sobject (char *j) {
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

static char *json_selement (char *j) {
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

static bool json_rend (char *json) {
  while (*json && *json <= ' ') {
    ++json;
  }
  return *json ? false : true;
}

static char *json_error(char *json, char *msg) {
  return str_printf("%s in\n'%s'\n", msg, json);
}

bool json_rnull (Json *json) {
  char *j = json_blanks(json);
  if (memcmp(j, "null", 4)) {
    return false;
  }
  return json_rend(j + 4);
}

bool json_rbool (Json *json) {
  json = json_blanks(json);
  bool r;

  char *j = json;
  if (memcmp(j, "true", 4)) {
    if (memcmp(j, "false", 5)) {
      THROW json_error(json, "Bad value reading a boolean value") _THROW
    }
    r = false;
    j += 5;
  } else {
    r = true;
    j += 4;
  }
  if (json_rend(j)) {
    return r;
  }
  THROW json_error(json, "Spare characters reading a boolean value") _THROW
  return false;
}


int json_rint (Json *json) {
  json = json_blanks(json);
  char *j = json;
  if (*j == '-' || (*j >= '0' && *j <= '9')) {
    Buf *bf = buf_new();
    while (*j && *j != '}' && *j != ']' && *j != ',' && *j > ' ') {
      ++j;
    }
    buf_add_buf(bf, json, j - json);
    char *n =buf_str(bf);
    if (json_rend(j)) {
      char *tail;
      int r = strtol(n, &tail, 10);
      if (*tail) {
        THROW json_error(n, "Bad number") _THROW
      }
      return r;
    }
    THROW json_error(j, "Spare characters reading a number value") _THROW
  }
  THROW json_error(j, "Bad start number") _THROW
  return 0;
}

unsigned json_ruint (Json *json) {
  json = json_blanks(json);
  char *j = json;
  if (*j == '-' || (*j >= '0' && *j <= '9')) {
    Buf *bf = buf_new();
    while (*j && *j != '}' && *j != ']' && *j != ',' && *j > ' ') {
      ++j;
    }
    buf_add_buf(bf, json, j - json);
    char *n =buf_str(bf);
    if (json_rend(j)) {
      char *tail;
      unsigned r = strtoul(n, &tail, 10);
      if (*tail) {
        THROW json_error(json, "Bad number") _THROW
      }
      return r;
    }
    THROW json_error(json, "Spare characters reading a number value") _THROW
  }
  THROW json_error(json, "Bad start number") _THROW
  return 0;
}

double json_rdouble (Json *json) {
  struct lconv *lc = localeconv();
  json = json_blanks(json);
  char *j = json;
  if (*j == '-' || (*j >= '0' && *j <= '9')) {
    Buf *bf = buf_new();
    while (*j && *j != '}' && *j != ']' && *j != ',' && *j > ' ') {
      ++j;
    }
    buf_add_buf(bf, json, j - json);
    char *n =buf_str(bf);
    int ix = str_cindex(n, '.');
    n[ix] = *lc->decimal_point;
    if (json_rend(j)) {
      char *tail;
      double r = strtod(n, &tail);
      if (*tail) {
        THROW json_error(json, "Bad number") _THROW
      }
      return r;
    }
    THROW json_error(json, "Spare characters reading a number value") _THROW
  }
  THROW json_error(json, "Bad start number") _THROW
  return 0;
}

char *json_rstring (Json *j) {
  int is_hex (char ch) {
    return (ch >= '0' && ch <= '9') ||
      (ch >= 'a' && ch <= 'f') ||
      (ch >= 'A' && ch <= 'F');
  }

  char *json = json_blanks(j);
  if (*json != '"') {
    THROW json_error(json, "String does not start with \"") _THROW
  }
  ++json;
  Buf *bf = buf_new();
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
          if (!is_hex(*json++)) {
            THROW json_error(j, "Bad hexadecimal unicode in string") _THROW
          }
        }
        json_unicode(bf, json - 4);
        continue;
      }
      default :
        THROW json_error(j, "Bad escape sequence in string") _THROW
      }
      ++json;
    } else {
      buf_cadd(bf, *json++);
    }
  }
  if (*json) {
    if (json_rend(json + 1)) {
      return buf_to_str(bf);
    }
    THROW json_error(j, "Spare characters reading a string value") _THROW
  }
  THROW json_error(j, "String does not end with \"") _THROW
  return NULL;
}

Arr/*Json*/ *json_rarray (Json *j) {
  char *json = json_blanks(j);
  if (*json != '[') {
    THROW json_error(json, "Array does not start with [") _THROW
  }
  ++json;
  Arr/*Json*/ *a = arr_new();
  while (*json && *json != ']') {
    char *tmp = json;
    json = json_selement(json);
    Buf *bf = buf_new();
    buf_add_buf(bf, tmp, json - tmp);
    arr_add(a, buf_str(bf));
    if (*json == ',') {
      ++json;
    } else if (*json && *json != ']') {
      THROW json_error(j, "Comma missing reading an array value") _THROW
    }
  }
  if (*json) {
    if (json_rend(json + 1)) {
      return a;
    }
    THROW json_error(j, "Spare characters reading an array value") _THROW
  }
  THROW json_error(j, "Array does not end with ]") _THROW
  return NULL;
}

Map/*Json*/ *json_robject (Json *j) {
  char *json = json_blanks(j);
  if (*json != '{') {
    THROW json_error(j, "Object does not start with {") _THROW
  }
  json = json_blanks(json + 1);
  Map/*Json*/ *m = map_new();
  while (*json && *json != '}') {
    if (*json != '"') {
      THROW json_error(j, "Object key is not a string") _THROW
    }
    char *tmp = json;
    json = json_sstring(json);
    Buf *kbf = buf_new();
    buf_add_buf(kbf, tmp, json - tmp);

    if (*json != ':') {
      THROW json_error(j, "key-value separator (:) is missing") _THROW
    }
    ++json;
    tmp = json;
    json = json_selement(json);
    Buf *vbf = buf_new();
    buf_add_buf(vbf, tmp, json - tmp);

    char *kf = json_rstring(buf_str(kbf));
    map_put(m, kf, buf_to_str(vbf));

    if (*json == ',') {
      json = json_blanks(json + 1);
    } else if (*json && *json != '}') {
      THROW json_error(j, "Comma missing reading an object value") _THROW
    }
  }
  if (*json) {
    if (json_rend(json + 1)) {
      return m;
    }
    THROW json_error(j, "Spare characters reading an object value") _THROW
  }
  THROW json_error(j, "Object does not end with }") _THROW
  return NULL;
}

inline
Json *json_wnull() {
  return "null";
}

inline
Json *json_wbool(bool value) {
  return value ? "true" : "false";
}

inline
Json *json_wint(int n) {
  return str_printf("%d", n);
}

inline
Json *json_wuint(unsigned n) {
  return str_printf("%u", n);
}

Json *json_wdouble(double n, int scale) {
  struct lconv *lc = localeconv();
  scale = (scale < 0) ? 0 : (scale > 9) ? 9 : scale;
  char format[] = "%.xf";
  format[2] = '0' + scale;
  char *ns = str_printf(format, n);
  int ix = str_cindex(ns, *lc->decimal_point);
  ns[ix] = '.';
  return ns;
}

Json *json_wstring(char *s) {
  if (!s) THROW exc_null_pointer("s") _THROW;

  Buf *bf = buf_new();

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
  return str_copy(buf_str(bf));
}

Json *json_warray(Arr/*Json*/ *a) {
  if (!a) THROW exc_null_pointer("a") _THROW;

  int size = arr_size(a);
  Buf *bf = buf_new();
  buf_cadd(bf, '[');
  if (size > 0) {
    buf_add(bf, arr_get(a, 0));
    int i;
    for (i = 1; i < size; ++i) {
      buf_cadd(bf, ',');
      buf_add(bf, arr_get(a, i));
    }
  }
  buf_cadd(bf, ']');
  return str_copy(buf_str(bf));;
}

Json *json_wobject(Map/*Json*/ *m) {
  if (!m) THROW exc_null_pointer("m") _THROW;

  Buf *bf = buf_new();
  buf_cadd(bf, '{');
  EACH(m, Tp, tp) {
    if (_i) {
      buf_cadd(bf, ',');
    }
    buf_add(bf, json_wstring(tp->e1));
    buf_cadd(bf, ':');
    buf_add(bf, tp->e2);
  }_EACH
  buf_cadd(bf, '}');
  return str_copy(buf_str(bf));
}

inline
void jmap_pnull(Map/*Json*/ *this, char *key) {
  map_put(this, key, json_wnull());
}

inline
void jmap_pbool(Map/*Json*/ *this, char *key, bool value) {
  map_put(this, key, json_wbool(value));
}

inline
void jmap_pint(Map/*Json*/ *this, char *key, int n) {
  map_put(this, key, json_wint(n));
}

inline
void jmap_puint(Map/*Json*/ *this, char *key, unsigned n) {
  map_put(this, key, json_wuint(n));
}

inline
void jmap_pdouble(Map/*Json*/ *this, char *key, double n, int s) {
  map_put(this, key, json_wdouble(n, s));
}

inline
void jmap_pstring(Map/*Json*/ *this, char *key, char *s) {
  map_put(this, key, json_wstring(s));
}

void jmap_parray(Map/*Json*/ *this, char *key, Arr *a, Json *(*to)(void *)) {
  Arr/*Json*/ *ajs = arr_new();
  EACH(a, void, e) {
    arr_add(ajs, to(e));
  }_EACH
  map_put(this, key, json_warray(ajs));
}

void jmap_pobject(Map/*Json*/ *this, char *key, Map *m, Json *(*to)(void *)) {
  Map /*Json*/ *mjs = map_new();
  EACH(m, Kv, kv) {
    arr_add(mjs, kv_new(kv->key, to(kv->value)));
  }_EACH
  map_put(this, key, json_wobject(mjs));
}

static char *key_error(char *key) {
  return str_printf("Key '%s' does not exist", key);
}

bool jmap_gnull(Map/*Json*/ *this, char *key) {
  Json *value = map_get(this, key);
  if (!value) {
    THROW key_error(key) _THROW
  }
  return json_rnull(value);
}

bool jmap_gbool(Map/*Json*/ *this, char *key) {
  Json *value = map_get(this, key);
  if (!value) {
    THROW key_error(key) _THROW
  }
  return json_rbool(value);
}

int jmap_gint(Map/*Json*/ *this, char *key) {
  Json *value = map_get(this, key);
  if (!value) {
    THROW key_error(key) _THROW
  }
  return json_rint(value);
}

unsigned jmap_guint(Map/*Json*/ *this, char *key) {
  Json *value = map_get(this, key);
  if (!value) {
    THROW key_error(key) _THROW
  }
  return json_ruint(value);
}

double jmap_gdouble(Map/*Json*/ *this, char *key) {
  Json *value = map_get(this, key);
  if (!value) {
    THROW key_error(key) _THROW
  }
  return json_rdouble(value);
}

char *jmap_gstring(Map/*Json*/ *this, char *key) {
  Json *value = map_get(this, key);
  if (!value) {
    THROW key_error(key) _THROW
  }
  return json_rnull(value) ? NULL : json_rstring(value);
}

Arr *jmap_garray(Map/*Json*/ *this, char *key, void *(*from)(Json *)) {
  Json *value = map_get(this, key);
  if (!value) {
    THROW key_error(key) _THROW
  }
  if (json_rnull(value)) {
    return NULL;
  }
  Arr *r = arr_new();
  EACH(json_rarray(value), Json, js) {
    arr_add(r, from(js));
  }_EACH

  return r;
}

Map *jmap_gobject(Map/*Json*/ *this, char *key, void *(*from)(Json *)) {
  Json *value = map_get(this, key);
  if (!value) {
    THROW key_error(key) _THROW
  }
  if (json_rnull(value)) {
    return NULL;
  }
  Map *m = map_new();
  EACH(json_robject(value), Kv, kv) {
    arr_add(m, kv_new(kv->key, from(kv->value)));
  }_EACH
  return m;
}

inline
void jarr_anull(Arr/*Json*/ *this) {
  arr_add(this, json_wnull());
}

inline
void jarr_abool(Arr/*Json*/ *this, bool value) {
  arr_add(this, json_wbool(value));
}

inline
void jarr_aint(Arr/*Json*/ *this, int n) {
  arr_add(this, json_wint(n));
}

inline
void jarr_auint(Arr/*Json*/ *this, unsigned n) {
  arr_add(this, json_wuint(n));
}

inline
void jarr_adouble(Arr/*Json*/ *this, double n, int scale) {
  arr_add(this, json_wdouble(n, scale));
}

inline
void jarr_astring(Arr/*Json*/ *this, char *s) {
  arr_add(this, json_wstring(s));
}

void jarr_aarray(Arr/*Json*/ *this, Arr *a, Json *(*to)(void *)) {
  Arr/*Json*/ *ajs = arr_new();
  EACH(a, void, e) {
    arr_add(ajs, to(e));
  }_EACH
  arr_add(this, json_warray(ajs));
}

void jarr_aobject(Arr/*Json*/ *this, Map *m, Json *(*to)(void *)) {
  Map /*Json*/ *mjs = map_new();
  EACH(m, Kv, kv) {
    arr_add(mjs, kv_new(kv->key, to(kv->value)));
  }_EACH
  arr_add(this, json_wobject(mjs));
}

inline
bool jarr_gnull(Arr/*Json*/ *this, size_t ix) {
  return json_rnull(arr_get(this, ix));
}

inline
bool jarr_gbool(Arr/*Json*/ *this, size_t ix) {
  return json_rbool(arr_get(this, ix));
}

inline
int jarr_gint(Arr/*Json*/ *this, size_t ix) {
  return json_rint(arr_get(this, ix));
}

inline
unsigned jarr_guint(Arr/*Json*/ *this, size_t ix) {
  return json_ruint(arr_get(this, ix));
}

inline
double jarr_gdouble(Arr/*Json*/ *this, size_t ix) {
  return json_rdouble(arr_get(this, ix));
}

inline
char *jarr_gstring(Arr/*Json*/ *this, size_t ix) {
  Json *value = arr_get(this, ix);
  if (json_rnull(value)) {
    return NULL;
  }
  return json_rstring(value);
}

Arr *jarr_garray(Arr/*Json*/ *this, size_t ix, void *(*from)(Json *)) {
  Json *value = arr_get(this, ix);
  if (json_rnull(value)) {
    return NULL;
  }
  Arr *r = arr_new();
  EACH(json_rarray(value), Json, js) {
    arr_add(r, from(js));
  }_EACH
  return r;
}

inline
Map *jarr_gobject(Arr/*Json*/ *this, size_t ix, void *(*from)(Json *)) {
  Json *value = arr_get(this, ix);
  if (json_rnull(value)) {
    return NULL;
  }
  Map *m = map_new();
  EACH(json_robject(value), Kv, kv) {
    arr_add(m, kv_new(kv->key, from(kv->value)));
  }_EACH
  return m;
}

inline
void jarr_snull(Arr/*Json*/ *this, size_t ix) {
  arr_set(this, ix, json_wnull());
}

inline
void jarr_sbool(Arr/*Json*/ *this, size_t ix, bool value) {
  arr_set(this, ix, json_wbool(value));
}

inline
void jarr_sint(Arr/*Json*/ *this, size_t ix, int n) {
  arr_set(this, ix, json_wint(n));
}

inline
void jarr_suint(Arr/*Json*/ *this, size_t ix, unsigned n) {
  arr_set(this, ix, json_wuint(n));
}

inline
void jarr_sdouble(Arr/*Json*/ *this, size_t ix, double n, int scale) {
  arr_set(this, ix, json_wdouble(n, scale));
}

inline
void jarr_sstring(Arr/*Json*/ *this, size_t ix, char *s) {
  arr_set(this, ix, json_wstring(s));
}

void jarr_sarray(Arr/*Json*/ *this, size_t ix, Arr *a, Json *(*to)(void *)) {
  Arr/*Json*/ *ajs = arr_new();
  EACH(a, void, e) {
    arr_add(ajs, to(e));
  }_EACH
  arr_set(this, ix, json_warray(ajs));
}

inline
void jarr_sobject(Arr/*Json*/ *this, size_t ix, Map *m, Json *(*to)(void *)) {
  Map /*Json*/ *mjs = map_new();
  EACH(m, Kv, kv) {
    arr_add(mjs, kv_new(kv->key, to(kv->value)));
  }_EACH
  arr_set(this, ix, json_wobject(mjs));
}
