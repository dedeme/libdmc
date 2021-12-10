// Copyright 02-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/char/Hchar.h"
#include "dmc/Hash.h"

Hchar *hchar_new(void) {
  return (Hchar *)hash_new();
}

int hchar_size(Hchar *this) {
  return hash_size((Hash *)this);
}

Hchar *hchar_put(Hchar *this, char *key, char *value) {
  return (Hchar *)hash_put((Hash *)this, key, value);
}

int hchar_has_key(Hchar *this, char *key) {
  return hash_has_key((Hash *)this, key);
}

Ochar *hchar_get(Hchar *this, char *key) {
  return (Ochar *)hash_get((Hash *)this, key);
}

Hchar *hchar_compact(Hchar *this) {
  return (Hchar *)hash_compact((Hash *)this);
}

Hchar *hchar_remove(Hchar *this, char *key) {
  return (Hchar *)hash_remove((Hash *)this, key);
}

Lchar *hchar_keys(Hchar *this) {
  return hash_keys((Hash *)this);
}

Hchar *hchar_sort(Hchar *this) {
  return (Hchar *)hash_sort((Hash *)this);
}

Hchar *hchar_sort_locale(Hchar *this) {
  return (Hchar *)hash_sort_locale((Hash *)this);
}

LKchar *hchar_to_list(Hchar *this) {
  return (LKchar *)hash_to_list((Hash *)this);
}

Hchar *hchar_from_list(LKchar *this) {
  return (Hchar *)this;
}

char *hchar_to_js(Hchar *this, char *(*to)(char *e)) {
  return hash_to_js((Hash *)this, (char *(*)(void *)) to);
}

Hchar *hchar_from_js(char *js, char *(*from)(char *jse)) {
  return (Hchar *)hash_from_js(js, (void *(*)(char *))from);
}


//--// Not remove

