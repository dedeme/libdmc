// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Map structure

#ifndef DM_MAP_H
# define DM_MAP_H

#include "Arr.h"

/// A Map is an Arr[Kv] and you can apply <tt>arr_xxx</tt> functions to it.
#define Map Arr

/// Initializates a map
Map *map_new(void);


/// Puts 'value' with key 'key'. If key already exists its value is changed.
///   this  : The map
///   key   : Entry key. It must not be NULL.
///   value : New value
void map_put(Map *this, char *key, void *value);

/// Returns a reference to value whose key is 'key' or NULL if 'this'
/// does not contain 'key'.<br>
/// It can return NULL if the key value es NULL too. In this case is necessary to
/// call the 'map_has_key' to distinguish each case.
void *map_get(Map *this, char *key);

/// Removes value with key 'key' or does nothing if 'key' does not exists
void map_remove(Map *this, char *key);

/// Returns 'true' if 'this' contains key 'key'.
bool map_has_key(Map *this, char *key);

/// Returns an iterator of Kv's
It/*Kv*/ *map_to_it (Map *this);

/// Returns an iterator of Kv's sorted by key
It/*Kv*/ *map_to_it_sort (Map *this);

/// Returns an iterator of Kv's sorted by key in locale
It/*Kv*/ *map_to_it_sort_locale (Map *this);

#endif
