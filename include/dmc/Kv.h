// Copyright 23-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Kv[char *] is a tuple of char*, void*

#ifndef DMC_KV_H
  #define DMC_KV_H

struct kv_Kv {
  char *k;
  void *v;
};

/// Kv[char *] structure.
typedef struct kv_Kv Kv;

/// Kv[char *] structure.
Kv *kv_new (char *k, void *v);

/// Returns this JSONized.
///   this: Container.
///   to  : Converter of container element to JSON.
char *kv_to_js (Kv *this, char *(*to)(void *e));

/// Returns the container from its JSON representation.
///   js  : Container JSONized.
///   from: Converter from JSON to container element.
Kv *kv_from_js (char *js, void *(*from)(char *ejs));

#endif
