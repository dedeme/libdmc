// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing json strings.

#ifndef DMC_JS_H
  #define DMC_JS_H

#include "Gc.h"
#include "Arr.h"
#include "Map.h"

/// 'Js' is an alias of 'char'. It is freed with 'free'.
typedef struct js_Js Js;

/// Returns true if json is "null" or false in another case.
int js_is_null (Js *json);

/// Read boolean
int js_rb (Js *json);

/// Read int
int js_ri (Js *json);

/// Read long
long js_rl (Js *json);

/// Read double
double js_rd (Js *json);

/// Read string
char *js_rs (Gc *gc, Js *json);

/// Read array in an Arr[Js]
Arr *js_ra (Gc *gc, Js *json);

/// Read object in a Map[js]
Map *js_ro (Gc *gc, Js *json);
/// Write a null value
Js *js_wn(Gc *gc);

/// Write a boolean value
Js *js_wb(Gc *gc, int value);

/// Write an int value
Js *js_wi(Gc *gc, int n);

/// Write an long value
Js *js_wl(Gc *gc, long n);

/// Write a double value with a maximum of 9 decimal positions.
Js *js_wd(Gc *gc, double n);

/// Write a string
Js *js_ws(Gc *gc, char *s);

/// Write an Arr[Js]
Js *js_wa(Gc *gc, Arr *a);

/// Write a Map[Js]
Js *js_wo(Gc *gc, Map *m);

#endif
