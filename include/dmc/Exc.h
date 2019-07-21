// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities to manage exceptions.<p>
/// Only functions 'exc_init', 'exc_stack'  and 'exc_throw' must be used
/// directly. The rest must be used through the macros TRY-CATCH-_TRY and
/// THROW-_THROW. (see <a href="?libdmc@dmc/DEFS#hp:TRY">DEFS</a>)

#ifndef DMC_EXC_H
  #define DMC_EXC_H

#include <setjmp.h>
#include "Gc.h"

typedef struct arr_Arr Arr;

///
typedef struct exc_Exc Exc;

/// Returns the exception garbage collector. Example:
///   TRY
///     ...
///   CATCH (e)
///     puts(exc_msg(e));
///     gc_free(exc_gc(e);
///   _TRY
Gc *exc_gc(Exc *this);

/// Return exception type. Predefined types are:
///   exc_generic_t
///   exc_range_t
///   exc_illegal_argument_t
///   exc_illegal_state_t
///   exc_io_t
char *exc_type (Exc *this);

/// Return exception message.
char *exc_msg (Exc *this);

/// Returns Arr[char]. The exception stack trace.
Arr *exc_stack (Exc *this);

/// exc_init intializes jumps buffer. This function has to be called before
/// using macros TRY-CATCH-FINALLY-_TRY or THROW-_THROW.
Gc *exc_init ();

/// exc_thread_init () Initializes thread data. It is intended to beeng use only
/// by 'async_thread'.
void exc_thread_init (void);

/// exc_thread_end () Removes a thread data. It is intended to beeng use only
/// by 'async_thread'.
void exc_thread_end (void);

/// Adds a exception to buffer of current Exc in current thread.
void exc_add (jmp_buf *jump);

/// Returns current Exc in current thread.
Exc *exc_get(void);

/// Remove the top of jumps buffer of current Exc in current thread.
void exc_remove ();

/// exc_throw sends an exception.<p>
/// If no TRY block has been defined it stops the program.
///   type   : Excepion type
///   message: Message to show
///   file   : Error file
///   func   : Error function
///   line   : Error line number
///   gc     : Garbage collector. exc_throw call gc_free with 'gc'
void exc_throw (
  char *type, char *message, char *file, char *func, int line, Gc *gc
);

///
#define exc_generic_t "generic"

///
#define exc_range_t "range"

/// exc_range. Exception for index out of range.
///   gc   : Garbage collector
///   begin: Lower limit inclusive
///   end  : Upper limit inclusive
///   index: The index out of range (< begin and > end)
char *exc_range (Gc *gc, int begin, int end, int index);

///
#define exc_illegal_argument_t "argument"

/// exception_illegal_argument. Exception for argument with a wrong value.
///   gc   : Garbage collector
///   argument_name: Name of wrong argument
///   expected: Value expected
///   actual: Actual value
char *exc_illegal_argument (
  Gc *gc, char *argument_name, char *expected, char *actual
);

///
#define exc_illegal_state_t "state"

/// Attempt to use an object in bad condition.
///   gc   : Garbage collector
///   cause: Description of problem
char *exc_illegal_state (Gc *gc, char *cause);

///
#define exc_io_t "io"

/// exception_io. Exception for Input - Output error.
///   gc   : Garbage collector
///   cause: Description of problem
char *exc_io (Gc *gc, char *cause);

#endif
