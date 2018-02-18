// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities to manage exceptions.<p>
/// Only functions 'exc_init', 'exc_stack'  and 'exc_throw' must be used
/// directly. The rest must be used through the macros TRY-CATCH-_TRY. (see
/// <a href="?libdm@DEFS#hp:TRY">DEFS</a>)
#ifndef DM_EXC_H
  #define DM_EXC_H

#include <setjmp.h>
#include "Arr.h"

/// Intialize the jumps buffer. This function has to be called before use
/// macros TRY-CATCH-FINALLY-_TRY or throw.
void exc_init();

/// Adds a exception to buffer
void exc_add(jmp_buf *jump);

/// Remove the top of jumps buffer.
void exc_remove();

/// Returns the last exception message
char *exc_msg();

/// Returns the last exception stack
Arr/*char*/ *exc_stack();

/// Sends an exception.<p>
/// If no TRY block has been defined it stops the program.///
//    message: Message to show
///   file   : Error file
///   func   : Error function
///   line   : Error line number
void exc_throw(char *message, char *file, char *func, int line);

/// Index out of range.
///   begin: Lower limit inclusive
///   end  : Upper limit exclusive
///   index: The index out of range
char *exc_range(int begin, int end, int index);

/// Argument with a NULL value.
///   argument_name: Name of wrong argument
char *exc_null_pointer(char *argument_name);

/// Argument without a NULL value.
///   argument_name: Name of wrong argument
char *exc_not_null_pointer(char *argument_name);

/// Argument with a wrong value.
///   argument_name: Name of wrong argument
///   expected: Value expected
///   actual: Actual value
char *exc_illegal_argument(char *argument_name, char *expected, char *actual);

/// Attempt to use an object in bad condition.
///   cause: Description of problem
char *exc_illegal_state(char *cause);

/// Input - Output error.
///   cause: Description of problem
char *exc_io(char *cause);


#endif


