// Copyright 1-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Map structure

#ifndef DM_MAP_H
  #define DM_MAP_H

#include <stdbool.h>
#include <stddef.h>

///
typedef struct map_Map Map;
typedef struct opt_Opt Opt;
typedef struct it_It It;
typedef struct ichar_Ichar Ichar;
typedef struct ikv_Ikv Ikv;
typedef struct ajson_Ajson Ajson;

/// map_new initializates a map. Map can be cast to Arr.
Map *map_new(void);

///
size_t map_size(Map *this);

/// map_put puts 'value' with key 'key'. If key already exists its value
/// is changed.
///   this  : The map
///   key   : Entry key. It must not be NULL.
///   value : New value. It must not be NULL.
void map_put(Map *this, char *key, void *value);

/// map_get returns the value pointed by key or opt_null() if 'key' does
/// not exist
Opt *map_get(Map *this, char *key);

/// map_opget returns the value pointer by key or 'option' if 'key' does not
/// exist
void *map_oget(Map *this, char *key, void *option);

/// map_remove removes value with key 'key' or does nothing if 'key' does
/// not exist
void map_remove(Map *this, char *key);

/// map_has_key returns 'true' if 'this' contains key 'key'.
bool map_has_key(Map *this, char *key);

/// map_keys returns keys of this
Ichar *map_keys(Map *this);

/// map_values returns values of this
It *map_values(Map *this);

///
Ikv *map_to_it(Map *this);

/// map_to_it_sort returns an iterator of Kv's sorted by key
Ikv *map_to_it_sort(Map *this);

/// map_to_it_sort_locale returns an iterator of Kv's sorted by key in locale
Ikv *map_to_it_sort_locale(Map *this);

///
Map *map_from_it(Ikv *it);

/// map_to_json returns a serialization of 'this' using 'to' to
/// convert elements.
Ajson *map_to_json(Map *this, Ajson *(*to)(void *));

/// map_from_json restores a serialized Map using 'from' to convert elements.
Map *map_from_json(Ajson *js, void *(*from)(Ajson *));

#endif
