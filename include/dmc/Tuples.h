// Copyright 07-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Several kind of tuples

#ifndef DM_TUPLES_H
  #define DM_TUPLES_H

///
typedef struct {
  void *e1; // R&W
  void *e2; // R&W
} Tp;

///
Tp *tp_new(void *e1, void *e2);

///
typedef struct {
  void *e1; // R&W
  void *e2; // R&W
  void *e3; // R&W
} Tp3;

///
Tp3 *tp3_new(void *o1, void *o2, void *o3);

///
typedef struct {
  char *key;   // R&W
  void *value; // R&W
} Kv;

///
Kv *kv_new(char *key, void *value);

#endif
