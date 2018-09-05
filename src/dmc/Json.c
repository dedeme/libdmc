// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dmc/Json.h"
#include "dmc/Arr.h"
#include "dmc/ct/Ojson.h"
#include "dmc/ct/Ajson.h"
#include "dmc/Map.h"
#include "dmc/ct/Mjson.h"
#include "dmc/Buf.h"
#include "dmc/Tuples.h"
#include "dmc/exc.h"
#include "dmc/str.h"
#include "dmc/DEFS.h"

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
  XNULL(json)

  char *j = json_blanks((char *)json);
  if (memcmp(j, "null", 4)) {
    return false;
  }
  return json_rend(j + 4);
}

bool json_rbool (Json *json) {
  XNULL(json)

  char *jsons = json_blanks((char *)json);
  bool r;

  char *j = jsons;
  if (memcmp(j, "true", 4)) {
    if (memcmp(j, "false", 5))
      THROW(json_err_t)
        json_error(jsons, "Bad value reading a boolean value")
      _THROW
    r = false;
    j += 5;
  } else {
    r = true;
    j += 4;
  }
  if (json_rend(j)) {
    return r;
  }
  THROW(json_err_t)
    json_error(jsons, "Spare characters reading a boolean value")
  _THROW
  return false;
}


int json_rint (Json *json) {
  XNULL(json)

  char *jsons = json_blanks((char *)json);
  char *j = jsons;
  if (*j == '-' || (*j >= '0' && *j <= '9')) {
    Buf *bf = buf_new();
    while (*j && *j != '}' && *j != ']' && *j != ',' && *j > ' ') {
      ++j;
    }
    buf_add_buf(bf, jsons, j - jsons);
    char *n =buf_str(bf);
    if (json_rend(j)) {
      char *tail;
      int r = strtol(n, &tail, 10);
      if (*tail) {
        THROW(json_err_t) json_error(n, "Bad number") _THROW
      }
      return r;
    }
    THROW(json_err_t)
      json_error(j, "Spare characters reading a number value")
    _THROW
  }
  THROW(json_err_t) json_error(j, "Bad start number") _THROW
  return 0;
}

unsigned json_ruint (Json *json) {
  XNULL(json)

  char *jsons = json_blanks((char *)json);
  char *j = jsons;
  if (*j == '-' || (*j >= '0' && *j <= '9')) {
    Buf *bf = buf_new();
    while (*j && *j != '}' && *j != ']' && *j != ',' && *j > ' ') {
      ++j;
    }
    buf_add_buf(bf, jsons, j - jsons);
    char *n =buf_str(bf);
    if (json_rend(j)) {
      char *tail;
      unsigned r = strtoul(n, &tail, 10);
      if (*tail) {
        THROW(json_err_t) json_error(jsons, "Bad number") _THROW
      }
      return r;
    }
    THROW(json_err_t)
      json_error(jsons, "Spare characters reading a number value")
    _THROW
  }
  THROW(json_err_t) json_error(jsons, "Bad start number") _THROW
  return 0;
}

double json_rdouble (Json *json) {
  XNULL(json)

  struct lconv *lc = localeconv();
  char *jsons = json_blanks((char *)json);
  char *j = jsons;
  if (*j == '-' || (*j >= '0' && *j <= '9')) {
    Buf *bf = buf_new();
    while (*j && *j != '}' && *j != ']' && *j != ',' && *j > ' ') {
      ++j;
    }
    buf_add_buf(bf, jsons, j - jsons);
    char *n =buf_str(bf);
    int ix = str_cindex(n, '.');
    n[ix] = *lc->decimal_point;
    if (json_rend(j)) {
      char *tail;
      double r = strtod(n, &tail);
      if (*tail) {
        THROW(json_err_t) json_error(jsons, "Bad number") _THROW
      }
      return r;
    }
    THROW(json_err_t)
      json_error(jsons, "Spare characters reading a number value")
    _THROW
  }
  THROW(json_err_t) json_error(jsons, "Bad start number") _THROW
  return 0;
}

char *json_rstring (Json *j) {
  XNULL(j)

  int is_hex (char ch) {
    return (ch >= '0' && ch <= '9') ||
      (ch >= 'a' && ch <= 'f') ||
      (ch >= 'A' && ch <= 'F');
  }

  char *json = json_blanks((char *)j);
  if (*json != '"') {
    THROW(json_err_t)
      json_error(json, "String does not start with \"")
    _THROW
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
          if (!is_hex(*json++))
            THROW(json_err_t)
              json_error((char *)j, "Bad hexadecimal unicode in string")
            _THROW
        }
        json_unicode(bf, json - 4);
        continue;
      }
      default :
        THROW(json_err_t)
          json_error((char *)j, "Bad escape sequence in string")
        _THROW
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
    THROW(json_err_t)
      json_error((char *)j, "Spare characters reading a string value")
    _THROW
  }
  THROW(json_err_t) json_error((char *)j, "String does not end with \"") _THROW
  return NULL;
}

Ajson *json_rarray (Json *j) {
  XNULL(j)

  char *json = json_blanks((char *)j);
  if (*json != '[') {
    THROW(json_err_t) json_error(json, "Array does not start with [") _THROW
  }
  ++json;
  Ajson *a = ajson_new();
  while (*json && *json != ']') {
    char *tmp = json;
    json = json_selement(json);
    Buf *bf = buf_new();
    buf_add_buf(bf, tmp, json - tmp);
    ajson_add(a, (Json *)buf_str(bf));
    if (*json == ',') {
      ++json;
    } else if (*json && *json != ']') {
      THROW(json_err_t)
        json_error((char *)j, "Comma missing reading an array value")
      _THROW
    }
  }
  if (*json) {
    if (json_rend(json + 1)) {
      return a;
    }
    THROW(json_err_t)
      json_error((char *)j, "Spare characters reading an array value")
    _THROW
  }
  THROW(json_err_t) json_error((char *)j, "Array does not end with ]") _THROW
  return NULL;
}

Mjson *json_robject (Json *j) {
  XNULL(j)

  char *json = json_blanks((char *)j);
  if (*json != '{')
    THROW(json_err_t)
      json_error((char *)j, "Object does not start with {")
    _THROW
  json = json_blanks(json + 1);
  Mjson *m = mjson_new();
  while (*json && *json != '}') {
    if (*json != '"')
      THROW(json_err_t)
        json_error((char *)j, "Object key is not a string")
      _THROW
    char *tmp = json;
    json = json_sstring(json);
    Buf *kbf = buf_new();
    buf_add_buf(kbf, tmp, json - tmp);

    if (*json != ':')
      THROW(json_err_t)
        json_error((char *)j, "key-value separator (:) is missing")
      _THROW
    ++json;
    tmp = json;
    json = json_selement(json);
    Buf *vbf = buf_new();
    buf_add_buf(vbf, tmp, json - tmp);

    char *kf = json_rstring((Json *)buf_str(kbf));
    mjson_put(m, kf, (Json *)buf_to_str(vbf));

    if (*json == ',') {
      json = json_blanks(json + 1);
    } else if (*json && *json != '}') {
      THROW(json_err_t)
        json_error((char *)j, "Comma missing reading an object value")
      _THROW
    }
  }
  if (*json) {
    if (json_rend(json + 1)) {
      return m;
    }
    THROW(json_err_t)
      json_error((char *)j, "Spare characters reading an object value")
    _THROW
  }
  THROW(json_err_t) json_error((char *)j, "Object does not end with }") _THROW
  return NULL;
}

inline
Json *json_wnull() {
  return (Json *)"null";
}

inline
Json *json_wbool(bool value) {
  return value ? (Json *)"true" : (Json *)"false";
}

inline
Json *json_wint(int n) {
  return (Json *)str_printf("%d", n);
}

inline
Json *json_wuint(unsigned n) {
  return (Json *)str_printf("%u", n);
}

Json *json_wdouble(double n, int scale) {
  struct lconv *lc = localeconv();
  scale = (scale < 0) ? 0 : (scale > 9) ? 9 : scale;
  char format[] = "%.xf";
  format[2] = '0' + scale;
  char *ns = str_printf(format, n);
  int ix = str_cindex(ns, *lc->decimal_point);
  ns[ix] = '.';
  return (Json *)ns;
}

Json *json_wstring(char *s) {
  XNULL(s)

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
  return (Json *)str_copy(buf_str(bf));
}

Json *json_warray(Ajson *a) {
  XNULL(a)

  int size = ajson_size(a);
  Buf *bf = buf_new();
  buf_cadd(bf, '[');
  if (size > 0) {
    buf_add(bf, (char *)ajson_get(a, 0));
    int i;
    for (i = 1; i < size; ++i) {
      buf_cadd(bf, ',');
      buf_add(bf, (char *)ajson_get(a, i));
    }
  }
  buf_cadd(bf, ']');
  return (Json *)str_copy(buf_str(bf));;
}

Json *json_wobject(Mjson *m) {
  XNULL(m)

  Buf *bf = buf_new();
  buf_cadd(bf, '{');
  EACH(m, Tp, tp) {
    if (_i) {
      buf_cadd(bf, ',');
    }
    buf_add(bf, (char *)json_wstring(tp_e1(tp)));
    buf_cadd(bf, ':');
    buf_add(bf, tp_e2(tp));
  }_EACH
  buf_cadd(bf, '}');
  return (Json *)str_copy(buf_str(bf));
}

void jmap_pnull(Mjson *this, char *key) {
  XNULL(this)
  XNULL(key)

  mjson_put(this, key, json_wnull());
}

void jmap_pbool(Mjson *this, char *key, bool value) {
  XNULL(this)
  XNULL(key)

  mjson_put(this, key, json_wbool(value));
}

void jmap_pint(Mjson *this, char *key, int n) {
  XNULL(this)
  XNULL(key)

  mjson_put(this, key, json_wint(n));
}

void jmap_puint(Mjson *this, char *key, unsigned n) {
  XNULL(this)
  XNULL(key)

  mjson_put(this, key, json_wuint(n));
}

void jmap_pdouble(Mjson *this, char *key, double n, int s) {
  XNULL(this)
  XNULL(key)

  mjson_put(this, key, json_wdouble(n, s));
}

void jmap_pstring(Mjson *this, char *key, char *s) {
  XNULL(this)
  XNULL(key)

  mjson_put(this, key, json_wstring(s));
}

void jmap_parray(Mjson *this, char *key, Arr *a, Json *(*to)(void *)) {
  XNULL(this)
  XNULL(key)
  XNULL(a)

  Ajson *ajs = ajson_new();
  EACH(a, void, e) {
    ajson_add(ajs, to(e));
  }_EACH
  mjson_put(this, key, json_warray(ajs));
}

void jmap_pobject(Mjson *this, char *key, Map *m, Json *(*to)(void *)) {
  XNULL(this)
  XNULL(key)
  XNULL(m)

  Mjson *mjs = mjson_new();
  EACH(m, Kv, kv) {
    arr_add((Arr *)mjs, kv_new(kv_key(kv), to(kv_value(kv))));
  }_EACH
  mjson_put(this, key, json_wobject(mjs));
}

bool jmap_gnull(Mjson *this, char *key) {
  XNULL(this)
  XNULL(key)
  if (!mjson_has_key(this, key))
    THROW(exc_generic_t) "'this' has not the key '%s'", key _THROW

  Json *value = ojson_value(mjson_get(this, key));
  return json_rnull(value);
}

bool jmap_gbool(Mjson *this, char *key) {
  XNULL(this)
  XNULL(key)
  if (!mjson_has_key(this, key))
    THROW(exc_generic_t) "'this' has not the key '%s'", key _THROW

  Json *value = ojson_value(mjson_get(this, key));
  return json_rbool(value);
}

int jmap_gint(Mjson *this, char *key) {
  XNULL(this)
  XNULL(key)
  if (!mjson_has_key(this, key))
    THROW(exc_generic_t) "'this' has not the key '%s'", key _THROW

  Json *value = ojson_value(mjson_get(this, key));
  return json_rint(value);
}

unsigned jmap_guint(Mjson *this, char *key) {
  XNULL(this)
  XNULL(key)
  if (!mjson_has_key(this, key))
    THROW(exc_generic_t) "'this' has not the key '%s'", key _THROW

  Json *value = ojson_value(mjson_get(this, key));
  return json_ruint(value);
}

double jmap_gdouble(Mjson *this, char *key) {
  XNULL(this)
  XNULL(key)
  if (!mjson_has_key(this, key))
    THROW(exc_generic_t) "'this' has not the key '%s'", key _THROW

  Json *value = ojson_value(mjson_get(this, key));
  return json_rdouble(value);
}

char *jmap_gstring(Mjson *this, char *key) {
  XNULL(this)
  XNULL(key)
  if (!mjson_has_key(this, key))
    THROW(exc_generic_t) "'this' has not the key '%s'", key _THROW

  Json *value = ojson_value(mjson_get(this, key));
  return json_rstring(value);
}

Arr *jmap_garray(Mjson *this, char *key, void *(*from)(Json *)) {
  XNULL(this)
  XNULL(key)
  if (!mjson_has_key(this, key))
    THROW(exc_generic_t) "'this' has not the key '%s'", key _THROW

  Json *value =ojson_value(mjson_get(this, key));
  Arr *r = arr_new();
  EACH(json_rarray(value), Json, js) {
    arr_add(r, from(js));
  }_EACH

  return r;
}

Map *jmap_gobject(Mjson *this, char *key, void *(*from)(Json *)) {
  XNULL(this)
  XNULL(key)
  if (!mjson_has_key(this, key))
    THROW(exc_generic_t) "'this' has not the key '%s'", key _THROW

  Json *value = ojson_value(mjson_get(this, key));
  Map *m = map_new();
  EACH(json_robject(value), Kv, kv) {
    arr_add((Arr *)m, kv_new(kv_key(kv), from(kv_value(kv))));
  }_EACH
  return m;
}

void jarr_anull(Ajson *this) {
  XNULL(this)

  ajson_add(this, json_wnull());
}

void jarr_abool(Ajson *this, bool value) {
  XNULL(this)

  ajson_add(this, json_wbool(value));
}

void jarr_aint(Ajson *this, int n) {
  XNULL(this)

  ajson_add(this, json_wint(n));
}

void jarr_auint(Ajson *this, unsigned n) {
  XNULL(this)

  ajson_add(this, json_wuint(n));
}

void jarr_adouble(Ajson *this, double n, int scale) {
  XNULL(this)

  ajson_add(this, json_wdouble(n, scale));
}

void jarr_astring(Ajson *this, char *s) {
  XNULL(this)

  ajson_add(this, json_wstring(s));
}

void jarr_aarray(Ajson *this, Arr *a, Json *(*to)(void *)) {
  XNULL(this)
  XNULL(a)

  Ajson *ajs = ajson_new();
  EACH(a, void, e) {
    ajson_add(ajs, to(e));
  }_EACH
  ajson_add(this, json_warray(ajs));
}

void jarr_aobject(Ajson *this, Map *m, Json *(*to)(void *)) {
  XNULL(this)
  XNULL(m)

  Mjson *mjs = mjson_new();
  EACH(m, Kv, kv) {
    arr_add((Arr *)mjs, kv_new(kv_key(kv), to(kv_value(kv))));
  }_EACH
  ajson_add(this, json_wobject(mjs));
}

bool jarr_gnull(Ajson *this, size_t ix) {
  XNULL(this)

  return json_rnull(ajson_get(this, ix));
}

bool jarr_gbool(Ajson *this, size_t ix) {
  XNULL(this)

  return json_rbool(ajson_get(this, ix));
}

int jarr_gint(Ajson *this, size_t ix) {
  XNULL(this)

  return json_rint(ajson_get(this, ix));
}

unsigned jarr_guint(Ajson *this, size_t ix) {
  XNULL(this)

  return json_ruint(ajson_get(this, ix));
}

double jarr_gdouble(Ajson *this, size_t ix) {
  XNULL(this)

  return json_rdouble(ajson_get(this, ix));
}

char *jarr_gstring(Ajson *this, size_t ix) {
  XNULL(this)

  Json *value = ajson_get(this, ix);
  return json_rstring(value);
}

Arr *jarr_garray(Ajson *this, size_t ix, void *(*from)(Json *)) {
  XNULL(this)

  Json *value = ajson_get(this, ix);
  Arr *r = arr_new();
  EACH(json_rarray(value), Json, js) {
    arr_add(r, from(js));
  }_EACH
  return r;
}

Map *jarr_gobject(Ajson *this, size_t ix, void *(*from)(Json *)) {
  XNULL(this)

  Json *value = ajson_get(this, ix);
  Map *m = map_new();
  EACH(json_robject(value), Kv, kv) {
    arr_add((Arr *)m, kv_new(kv_key(kv), from(kv_value(kv))));
  }_EACH
  return m;
}

void jarr_snull(Ajson *this, size_t ix) {
  XNULL(this)

  ajson_set(this, ix, json_wnull());
}

void jarr_sbool(Ajson *this, size_t ix, bool value) {
  XNULL(this)

  ajson_set(this, ix, json_wbool(value));
}

void jarr_sint(Ajson *this, size_t ix, int n) {
  XNULL(this)

  ajson_set(this, ix, json_wint(n));
}

void jarr_suint(Ajson *this, size_t ix, unsigned n) {
  XNULL(this)

  ajson_set(this, ix, json_wuint(n));
}

void jarr_sdouble(Ajson *this, size_t ix, double n, int scale) {
  XNULL(this)

  ajson_set(this, ix, json_wdouble(n, scale));
}

void jarr_sstring(Ajson *this, size_t ix, char *s) {
  XNULL(this)

  ajson_set(this, ix, json_wstring(s));
}

void jarr_sarray(Ajson *this, size_t ix, Arr *a, Json *(*to)(void *)) {
  XNULL(this)
  XNULL(a)

  Ajson *ajs = ajson_new();
  EACH(a, void, e) {
    ajson_add(ajs, to(e));
  }_EACH
  ajson_set(this, ix, json_warray(ajs));
}

void jarr_sobject(Ajson *this, size_t ix, Map *m, Json *(*to)(void *)) {
  XNULL(this)
  XNULL(m)

  Mjson *mjs = mjson_new();
  EACH(m, Kv, kv) {
    arr_add((Arr *)mjs, kv_new(kv_key(kv), to(kv_value(kv))));
  }_EACH
  ajson_set(this, ix, json_wobject(mjs));
}

