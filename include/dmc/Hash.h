// Copyright 04-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Hash structure

#ifndef DM_HASH_H
  #define DM_HASH_H

#include <stdbool.h>
#include <stddef.h>

///
typedef struct hash_Hash Hash;
typedef struct opt_Opt Opt;
typedef struct it_It It;
typedef struct ichar_Ichar Ichar;
typedef struct ikv_Ikv Ikv;
typedef struct json_Json Json;

/// hash_new nitializates a Hash. Hash is an immutable struct which can be
/// cast to List.
Hash *hash_new(void);

/// hash_count returns elements number of 'this' <u>including duplicates</u>.
size_t hash_count(Hash *this);

/// hash_put returns a new Hash prepending a new Kv to the former one.
///   this  : The Hash
///   key   : Entry key. It must not be NULL.
///   value : New value. It must not be NULL.
Hash *hash_put(Hash *this, char *key, void *value);

/// hash_get returns the value pointed by key or opt_null() if 'key' does
/// not exist.
Opt *hash_get(Hash *this, char *key);

/// hash_opget returns the value pointer by key or 'option' if 'key' does not
/// exist.
void *hash_oget(Hash *this, char *key, void *option);

/// hash_remove returns a new Hash without 'key'.
Hash *hash_remove(Hash *this, char *key);

/// hash_compact returns a new Hash without duplicated keys.
Hash *hash_compact(Hash *this);

/// hash_has_key returns 'true' if 'this' contains key 'key'.
bool hash_has_key(Hash *this, char *key);

/// hash_keys returns keys of this.
Ichar *hash_keys(Hash *this);

/// map_values returns values of this
It *hash_values(Hash *this);

///
Ikv *hash_to_it(Hash *this);

/// hash_to_it_sort returns an iterator of Kv's sorted by key
Ikv *hash_to_it_sort(Hash *this);

/// hash_to_it_sort_locale returns an iterator of Kv's sorted by key in locale
Ikv *hash_to_it_sort_locale(Hash *this);

///
Hash *hash_from_it(Ikv *it);

/// hash_to_json returns a serialization of 'this' using 'to' to
/// convert elements.
Json *hash_to_json(Hash *this, Json *(*to)(void *));

/// hash_from_json restores a serialized Hash using 'from' to convert elements.
Hash *hash_from_json(Json *js, void *(*from)(Json *));

#endif
