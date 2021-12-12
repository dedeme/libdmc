// Copyright 01-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing json strings.

#ifndef DMC_JS_H
  #define DMC_JS_H

#include "dmc/char/Achar.h"
#include "dmc/char/Mchar.h"

/// Returns '1' if json is "null" or '0' in another case.
int js_is_null (char *json);

/// Read boolean.
int js_rb (char *json);

/// Read int.
int js_ri (char *json);

/// Read long.
long js_rl (char *json);

/// Read double.
double js_rd (char *json);

/// Read string.
char *js_rs (char *json);

/// Read array.
Achar *js_ra (char *json);

/// Read object.
Mchar *js_ro (char *json);

/// Write a null value.
char *js_wn(void);

/// Write a boolean value.
char *js_wb(int value);

/// Write an int value.
char *js_wi(int n);

/// Write an long value.
char *js_wl(long n);

/// Write a double value with a maximum of 9 decimal positions.
char *js_wd(double n);

/// Write a string.
char *js_ws(char *s);

/// Write an Achar.
char *js_wa(Achar *a);

/// Write a Mchar.
char *js_wo(Mchar *m);

#endif
