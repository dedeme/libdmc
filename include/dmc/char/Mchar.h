// Copyright 25-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Map[char *].

#ifndef DMC_CHAR_MCHAR_H
  #define DMC_CHAR_MCHAR_H

#include "dmc/char/Achar.h"
#include "dmc/char/Ochar.h"
#include "dmc/char/AKchar.h"

/// Map[char *].
typedef struct mchar_Mchar Mchar;

/// Initializates a map. Mchar can be cast to Arr<Kv>.
Mchar *mchar_new(void);

///
int mchar_size(Mchar *this);

/// Puts 'value' with key 'key'. If key already exists its value
/// is changed.
///   this : The map.
///   key  : Entry key.
///   value: New value
void mchar_put(Mchar *this, char *key, char *value);

/// Returns 1 if 'this' has key 'key'.
int mchar_has_key(Mchar *this, char *key);

/// Returns the value pointed by key.
Ochar *mchar_get(Mchar *this, char *key);

/// Removes value with key 'key' or does nothing if 'key' does not exist.
void mchar_remove(Mchar *this, char *key);

/// Returns keys of this.
Achar *mchar_keys(Mchar *this);

/// Equals to (AKvchar *)this
AKchar *mchar_to_array(Mchar *this);

/// Equals to (Mchar *)this
Mchar *mchar_from_array(AKchar *this);

/// Sorts 'this' from keys.
void mchar_sort(Mchar *this);

/// Sorts 'this' in locale from keys.
void mchar_sort_locale(Mchar *this);

/// Returns a JSON from a value of 'this'. Duplicates are removed.
char *mchar_to_js(Mchar *this);

/// Parses a JSON to a value of 'this'.
Mchar *mchar_from_js(char *js);


//--// Not remove

#endif