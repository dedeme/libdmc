// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Vmap.h"
#include "dmc/std.h"

struct vmap_Kv {
  char *key;
  void *value;
};

static Vkv *vkv_new(char *key, void *value) {
  Vkv *this = malloc(sizeof(Vkv));
  this->key = str_new(key);
  this->value = value;
  return this;
}

static void vkv_free(Vkv *this) {
  free(this->key);
  free(this);
}

Vmap *vmap_new(void) {
  return (Vmap *)arr_new((FPROC)vkv_free);
}

void vmap_free(Vmap *this) {
  arr_free((Arr *)this);
}

int vmap_size(Vmap *this) {
  return arr_size((Arr *) this);
}

void vmap_put(Vmap *this, char *key, void *value) {
  int no_added = 1;
  EACH(this, Vkv, e)
    if (str_eq(e->key, key)) {
      e->value = value;
      no_added = 0;
      break;
    }
  _EACH
  if (no_added) {
    arr_push((Arr *)this, vkv_new(key, value));
  }
}

void *vmap_get_null(Vmap *this, char *key) {
  EACH(this, Vkv, e)
    if (str_eq(e->key, key)) {
      return e->value;
    }
  _EACH
  return NULL;
}

void vmap_remove(Vmap *this, char *key) {
  int ix = -1;
  EACH_IX(this, Vkv, e, i)
    if (str_eq(e->key, key)) {
      ix = i;
      break;
    }
  _EACH
  if (ix != -1) {
    arr_remove((Arr *)this, ix);
  }
}

// Varr[char]
Varr *vmap_keys_new(Vmap *this) {
  // Varr[char]
  Varr *r = varr_new();
  EACH(this, Vkv, e)
    varr_push(r, e->key);
  _EACH
  return r;
}

char *vmap_key(Vkv *entry) {
  return entry->key;
}

void *vmap_value(Vkv *entry) {
  return entry->value;
}


