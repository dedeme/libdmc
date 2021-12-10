// Copyright 08-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Hash[char *].

#ifndef DMC_CHAR_HCHAR_H
  #define DMC_CHAR_HCHAR_H

#include "dmc/char/Lchar.h"
#include "dmc/char/Ochar.h"
#include "dmc/char/LKchar.h"

/// Hchar[char *].
typedef struct hchar_Hchar Hchar;

/// Initializates a Hchar. Hchar can be cast to Map<Kv>.
Hchar *hchar_new(void);

/// Returns the number of elements, including duplicated keys. (O(n) operation).
int hchar_size(Hchar *this);

/// Puts 'value' with key 'key'. If key already exists, it will not
/// be deleted; but will not be used.
///   this : The Hchar.
///   key  : Entry key.
///   value: New value
Hchar *hchar_put(Hchar *this, char *key, char *value);

/// Returns 1 if 'this' has key 'key'.
int hchar_has_key(Hchar *this, char *key);

/// Returns the value pointed by key or 'opt_empty' if 'key' does.
/// not exist.
Ochar *hchar_get(Hchar *this, char *key);

/// Returns a new hash with duplicates removed.
Hchar *hchar_compact(Hchar *this);

/// Returns a new hash with the entry with key 'key' removed.
Hchar *hchar_remove(Hchar *this, char *key);

/// Returns keys of this in a List with duplicated keys removed.
Lchar *hchar_keys(Hchar *this);

/// Returns 'this' conpacted and sorted by keys.
Hchar *hchar_sort(Hchar *this);

/// Returns 'this' conpacted and sorted in locale by keys.
Hchar *hchar_sort_locale(Hchar *this);

/// Returns a List with dulicated keys removed.
LKchar *hchar_to_list(Hchar *this);

/// Equals to (Hchar *)this
Hchar *hchar_from_list(LKchar *this);

/// Returns a JSON from a value of 'this'. Duplicates are removed.
///   to: Value converter.
char *hchar_to_js(Hchar *this, char *(*to)(char *e));

/// Parses a JSON to a value of 'this'.
///   from: Value converter.
Hchar *hchar_from_js(char *js, char *(*from)(char *jse));


//--// Not remove

#endif