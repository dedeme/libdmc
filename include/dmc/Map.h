// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Map structure

#ifndef DMC_MAP_H
  #define DMC_MAP_H

#include "dmc/Arr.h"
#include "dmc/Opt.h"

///
typedef struct map_Map Map;

/// map_new initializates a map. Map can be cast to Arr[Kv].<br>
Map *map_new(void);

///
int map_size(Map *this);

/// map_put puts 'value' with key 'key'. If key already exists its value
/// is changed.
///   this  : The map
///   key   : Entry key.
///   value : New value
void map_put(Map *this, char *key, void *value);

/// map_has_key returns 1 if 'this' has key 'key'
int map_has_key(Map *this, char *key);

/// map_get returns the value pointed by key or 'opt_empty' if 'key' does
/// not exist
Opt *map_get(Map *this, char *key);

/// map_remove removes value with key 'key' or does nothing if 'key' does
/// not exist
void map_remove(Map *this, char *key);

/// map_keys returns keys of this in a Arr[char]
Arr *map_keys(Map *this);

/// Equals to (Arr *)this.<br>
/// Returns an Arr[Kv]
Arr *map_kvs(Map *this);

/// map_sort sorts 'this' from keys
void map_sort(Map *this);

/// map_sort_locale sorts 'this' in locale from keys
void map_sort_locale(Map *this);

/// Creates an iterator over 'this'
///   return: It[Kv]
It *map_to_it(Map *this);

/// Creates a Map from 'it'
///   it: It[Kv]
Map *map_from_it(It *it);

/// Returns a Js from a value of 'this'
///   to: Value converter
Js *map_to_js(Map *this, Js *(*to)(void *e));

/// ParseS a Js to a value of 'this'.<br>
///   from: Value converter
Map *map_from_js(Js *js, void *(*from)(Js *jse));

#endif
