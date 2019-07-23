// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Pair key - value.

#ifndef DMC_KV_H
  #define DMC_KV_H

#include "Gc.h"

///
typedef struct kv_Kv Kv;

///
Kv *kv_new(Gc *gc, char *key, void *value);

///
char *kv_key(Kv *this);

///
void *kv_value(Kv *this);

#endif
