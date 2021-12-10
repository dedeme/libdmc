// Copyright 08-Dec-2021 ºDeme
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
///   to  : Converter of container element to JSON.
char *kchar_to_js (Kchar *this, char *(*to)(char *e));

/// Returns the container from its JSON representation.
///   js  : Container JSONized.
///   from: Converter from JSON to container element.
Kchar *kchar_from_js (char *js, char *(*from)(char *ejs));


//--// Not remove

#endif