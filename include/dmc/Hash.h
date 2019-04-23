// Copyright 23-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Hash (immutable) structure

#ifndef DM_HASH_H
  #define DM_HASH_H

#include "dmc/Arr.h"
#include "dmc/Opt.h"

///
typedef struct hash_Hash Hash;

/// hash_new initializates a Hash. Hash can be cast to Map[Kv].
Hash *hash_new(void);

/// hash_count returns the number of elements. (O(n) operation)
int hash_count(Hash *this);

/// hash_put puts 'value' with key 'key'. If key already exists, it will not
/// be deleted; but will not be used.
///   this  : The Hash
///   key   : Entry key.
///   value : New value
Hash *hash_put(Hash *this, char *key, void *value);

/// hash_has_key returns 1 if 'this' has key 'key'
int hash_has_key(Hash *this, char *key);

/// hash_get returns the value pointed by key or 'opt_empty' if 'key' does
/// not exist.
Opt *hash_get(Hash *this, char *key);

/// hash_compact returns a new hash with duplicates removed.
Hash *hash_compact(Hash *this);

/// hash_remove returns a new hash with the entry with key 'key' removed
Hash *hash_remove(Hash *this, char *key);

/// hash_keys returns keys of this in a Arr[char] with duplicated keys removed.
Arr *hash_keys(Hash *this);

/// Returns an Arr[Kv] with dulicated keys removed.
Arr *hash_kvs(Hash *this);

/// Creates an iterator over 'this'. Duplicates are removed.
///   return: It[Kv]
It *hash_to_it(Hash *this);

/// Creates a Hash from a 'it'
///   it: It[Kv]
Hash *hash_from_it(It *it);

/// Returns a Js from a value of 'this'. Duplicates are removed.
///   to: Value converter
Js *hash_to_js(Hash *this, Js *(*to)(void *e));

/// ParseS a Js to a value of 'this'.<br>
///   from: Value converter
Hash *hash_from_js(Js *js, void *(*from)(Js *jse));

#endif
