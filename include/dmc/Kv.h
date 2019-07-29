// Copyright 23-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Pair key - value

#ifndef DMC_KV_H
  #define DMC_KV_H

///
typedef struct kv_Kv Kv;

///
Kv *kv_new(char *key, void *value);

///
char *kv_key(Kv *this);

///
void *kv_value(Kv *this);

#endif
