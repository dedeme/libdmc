// Copyright 25-Dec-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Kv[char *] is a tuple of char*, char*

#ifndef DMC_CHAR_KCHAR_H
  #define DMC_CHAR_KCHAR_H

// Kv[char *] structure.
struct kchar_Kchar {
  char *k;
  char *v;
};

// Kv[char *] structure.
typedef struct kchar_Kchar Kchar;

/// Constructor.
Kchar *kchar_new (char *k, char *v);

/// Returns this JSONized.
///   this: Container.
char *kchar_to_js (Kchar *this);

/// Returns the container from its JSON representation.
///   js  : Container JSONized.
Kchar *kchar_from_js (char *js);


//--// Not remove

#endif