// Copyright 22-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Map structure.

#ifndef DMC_MAP_H
  #define DMC_MAP_H

#include "dmc/char/Achar.h"
#include "dmc/Kv/AKv.h"
#include "dmc/Opt.h"

/// Map structure
typedef struct map_Map Map;

/// Initializates a map. Map can be cast to Arr<Kv>.
Map *map_new(void);

///
int map_size(Map *this);

/// Puts 'value' with key 'key'. If key already exists its value
/// is changed.
///   this : The map.
///   key  : Entry key.
///   value: New value
void map_put(Map *this, char *key, void *value);

/// Returns 1 if 'this' has key 'key'.
int map_has_key(Map *this, char *key);

/// Returns the value pointed by key.
Opt *map_get(Map *this, char *key);

/// Removes value with key 'key' or does nothing if 'key' does not exist.
void map_remove(Map *this, char *key);

/// Returns keys of this.
Achar *map_keys(Map *this);

/// Equals to (AKv *)this
AKv *map_to_array(Map *this);

/// Equals to (Map *)this
Map *map_from_array(AKv *this);

/// Sorts 'this' by keys.
void map_sort(Map *this);

/// Sorts 'this' in locale by keys.
void map_sort_locale(Map *this);

/// Returns this JSONized.
///   this: Container.
///   to  : Converter of container value to JSON.
char *map_to_js (Map *this, char *(*to)(void *e));

/// Returns the container from its JSON representation.
///   js  : Container JSONized.
///   from: Converter from JSON to container value.
Map *map_from_js (char *js, void *(*from)(char *ejs));


#endif
