// Copyright 21-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Errors management.

#ifndef DMC_ERR_H
  #define DMC_ERR_H

#include "dmc/DEFS.h"

/// Sets 'fn' as function to manage errors.
/// If no function is set 'err_fail' will print 'err' and 'stack' and will finish
/// the program with code 1.
///   fn: Function which is sent next parameters:
///     err  : Error message.
///     stack: Stack trace.
///     len  : 'stack' length.
void err_fn (void(*fn)(char *err, char **stack, int len));

/// Function to call the error function (see 'err_fn'). It returns 'void *'
/// for convenience since it never returns.
/// This function can be more easily called with the macro 'FAIL' (see DEFS.h).
///   message: Error message.
///   file   : Absolute path of file with the error.
///   func   : Name of function with the error.
///   line   : Number of line with the error.
void *err_fail (char *message, char *file, char *func, int line);

#endif
