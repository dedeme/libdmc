// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities to manage exceptions.<p>
/// Only functions 'exc_init', 'exc_stack'  and 'exc_throw' must be used
/// directly. The rest must be used through the macros TRY-CATCH-_TRY. (see
/// <a href="?libdm@DEFS#hp:TRY">DEFS</a>)
#ifndef DM_EXC_H
  #define DM_EXC_H

#include <setjmp.h>

/// Intialize the jumps buffer. This function has to be called before use
/// macros TRY-CATCH-FINALLY-_TRY or throw.
void exc_init();

/// Adds a exception to buffer
void exc_add(jmp_buf *jump);

/// Remove the top of jumps buffer.
void exc_remove();

/// Send an exception.<p>
/// If no TRY block has been defined cause a generic error.</p>
/// This function never returns. Its 'void *' retun value is intended to be
/// used in constructions like "<tt>return exc_throw("An exception");</tt>"
void *exc_throw(char *e, const char *file, const char *func, int line);

/// Returns the last exception message
char *exc_msg();

/// Returns the last exception stack
Arr/*char*/ *exc_stack();

#endif


