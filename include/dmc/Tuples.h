// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Several kind of tuples

#ifndef DM_TUPLES_H
  #define DM_TUPLES_H

///
typedef struct tp_Tp Tp;

///
Tp *tp_new(void *e1, void *e2);

///
void *tp_e1(Tp *this);

///
void *tp_e2(Tp *this);

///
typedef struct tp3_Tp3 Tp3;
///
Tp3 *tp3_new(void *e1, void *e2, void *e3);

///
void *tp3_e1(Tp3 *this);

///
void *tp3_e2(Tp3 *this);

///
void *tp3_e3(Tp3 *this);

///
typedef struct kv_Kv Kv;

///
Kv *kv_new(char *key, void *value);

///
char *kv_key(Kv *this);

///
void *kv_value(Kv *this);

#endif
