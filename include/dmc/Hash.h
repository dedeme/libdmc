// Copyright 02-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Hash (immutable) structure.

#ifndef DM_HASH_H
  #define DM_HASH_H

#include "dmc/char/Lchar.h"
#include "dmc/Kv/LKv.h"
#include "dmc/Opt.h"

/// Hash (immutable) structure.
typedef struct hash_Hash Hash;

/// Initializates a Hash. Hash can be cast to Map<Kv>.
Hash *hash_new(void);

/// Returns the number of elements, including duplicated keys. (O(n) operation).
int hash_size(Hash *this);

/// Puts 'value' with key 'key'. If key already exists, it will not
/// be deleted; but will not be used.
///   this : The Hash.
///   key  : Entry key.
///   value: New value
Hash *hash_put(Hash *this, char *key, void *value);

/// Returns 1 if 'this' has key 'key'.
int hash_has_key(Hash *this, char *key);

/// Returns the value pointed by key or 'opt_empty' if 'key' does.
/// not exist.
Opt *hash_get(Hash *this, char *key);

/// Returns a new hash with duplicates removed.
Hash *hash_compact(Hash *this);

/// Returns a new hash with the entry with key 'key' removed.
Hash *hash_remove(Hash *this, char *key);

/// Returns keys of this in a List with duplicated keys removed.
Lchar *hash_keys(Hash *this);

/// Returns 'this' conpacted and sorted by keys.
Hash *hash_sort(Hash *this);

/// Returns 'this' conpacted and sorted in locale by keys.
Hash *hash_sort_locale(Hash *this);

/// Returns a List with dulicated keys removed.
LKv *hash_to_list(Hash *this);

/// Equals to (Hash *)this
Hash *hash_from_list(LKv *this);

/// Returns a JSON from a value of 'this'. Duplicates are removed.
///   to: Value converter.
char *hash_to_js(Hash *this, char *(*to)(void *e));

/// Parses a JSON to a value of 'this'.
///   from: Value converter.
Hash *hash_from_js(char *js, void *(*from)(char *jse));

#endif
