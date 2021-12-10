// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/char/Mchar.h"
#include "dmc/Map.h"

Mchar *mchar_new (void) {
  return (Mchar *)map_new();
}

/// Initializates a map. Mchar can be cast to Arr<Kv>.
Mchar *mchar_new(void);

///
int mchar_size(Mchar *this) {
  return map_size((Map *)this);
}

void mchar_put(Mchar *this, char *key, char *value) {
  map_put((Map *)this, key, value);
}

int mchar_has_key(Mchar *this, char *key) {
  return map_has_key((Map *)this, key);
}

Ochar *mchar_get(Mchar *this, char *key) {
  return (Ochar *)map_get((Map *)this, key);
}

void mchar_remove(Mchar *this, char *key) {
  map_remove((Map *)this, key);
}

Achar *mchar_keys(Mchar *this) {
  return map_keys((Map *)this);
}

AKchar *mchar_to_array(Mchar *this) {
  return (AKchar *)this;
}

Mchar *mchar_from_array(AKchar *this) {
  return (Mchar *) this;
}

void mchar_sort(Mchar *this) {
  return map_sort((Map *)this);
}

void mchar_sort_locale(Mchar *this) {
  return map_sort_locale((Map *)this);
}

char *mchar_to_js(Mchar *this, char *(*to)(char *e)) {
  return map_to_js((Map *)this, (char *(*)(void *)) to);
}

Mchar *mchar_from_js(char *js, char *(*from)(char *jse)) {
  return (Mchar *)map_from_js(js, (void *(*)(char *))from);
}



//--// Not remove

