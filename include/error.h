// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing errors.<br>
/// You can use ERROR_DATA to facilitate to throw errors. For example:
///   error_null_pointer("this", ERROR_DATA);

#ifndef DM_ERROR_H
 #define DM_ERROR_H

/// Finishes program showing a formatted message on the error output and
/// exiting with code 'value'.
void error_crash (int value, char *format, ...);

/// Generic error.
///   message: Message to show
///   file   : Error file
///   func   : Error function
///   line   : Error line number
void error_generic(char *message, char *file, const char *func, int line);

/// Index out of range.
///   begin: Lower limit inclusive
///   end  : Upper limit exclusive
///   index: The index out of range
///   file : Error file
///   func : Error function
///   line : Error line number
void error_range(
  int begin,
  int end,
  int index,
  char *file,
  const char *func,
  int line
);

/// Argument with a NULL value.
///   argument_name: Name of wrong argument
///   file         : Error file
///   func         : Error function
///   line         : Error line number
void error_null_pointer(
  char *argument_name,
  char *file,
  const char *func,
  int line
);

/// Argument without a NULL value.
///   argument_name: Name of wrong argument
///   file         : Error file
///   func         : Error function
///   line         : Error line number
void error_not_null_pointer(
  char *argument_name,
  char *file,
  const char *func,
  int line
);

/// Argument with a wrong value.
///   argument_name: Name of wrong argument
///   file         : Error file
///   func         : Error function
///   line         : Error line number
void error_illegal_argument(
  char *argument_name,
  char *file,
  const char *func,
  int line
);

/// Attempt to use an object in bad condition.
///   cause: Description of problem
///   file : Error file
///   func : Error function
///   line : Error line number
void error_illegal_state(char *cause, char *file, const char *func, int line);

#endif

