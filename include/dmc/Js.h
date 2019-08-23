// Copyright 17-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing json strings.

#ifndef DMC_JS_H
  #define DMC_JS_H

#include "dmc/Arr.h"
#include "dmc/Map.h"

/// 'Js' is an alias of 'char'. It is freed with 'free'.
typedef struct js_Js Js;

/// Returns true if json is "null" or false in another case.
int js_is_null (Js *json);

/// Read boolean.
int js_rb (Js *json);

/// Read int.
int js_ri (Js *json);

/// Read long.
long js_rl (Js *json);

/// Read double.
double js_rd (Js *json);

/// Read string.
char *js_rs (Js *json);

/// Read array in an Arr<Js>.
Arr *js_ra (Js *json);

/// Read object in a Map<js>.
Map *js_ro (Js *json);

/// Write a null value.
Js *js_wn(void);

/// Write a boolean value.
Js *js_wb(int value);

/// Write an int value.
Js *js_wi(int n);

/// Write an long value.
Js *js_wl(long n);

/// Write a double value with a maximum of 9 decimal positions.
Js *js_wd(double n);

/// Write a string.
Js *js_ws(char *s);

/// Write an Arr<Js>.
Js *js_wa(Arr *a);

/// Write a Map<Js>.
Js *js_wo(Map *m);

#endif
