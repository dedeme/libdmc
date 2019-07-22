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

/// exc_init set exit function. This function only can be called once.<p>
/// Its default implementation prints in stdout them message and stack trace
/// of 'ex'
void exc_init (void (*exitf) (Exc *ex));

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
void exc_throw (
  char *type, char *message, char *file, char *func, int line
);

///
#define exc_generic_t "generic"

///
#define exc_range_t "range"

///
#define exc_illegal_argument_t "argument"

///
#define exc_illegal_state_t "state"

///
#define exc_io_t "io"

#endif
