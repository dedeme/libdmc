// Copyright 30-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Tasks scheduler

#ifndef DMC_SCHD_H
  #define DMC_SCHD_H

#include "dmc/std.h"

///
typedef struct schd_Task SchdTask;

/// Deletes a task
void schdTask_del (SchdTask *this);

///
typedef struct schd_Schd Schd;

///
Schd *schd_new (void);

/// Adds a task
SchdTask *schd_add (Schd *this, void (*fn)(void *), void *value);

/// Returns '1' if 'task' is active in 'this'
int schd_exists (Schd *this, SchdTask *task);

/// Adds a task to do every 'millis' milliseconds
///   NOTES:
///     * The task can be stopped with 'schd_del'
///     * The first execution of 'fn' is immedate.
///     * 'millis' is forced to be at least 4 milliseconds.
SchdTask *schd_loop (
  Schd *this,
  void (*fn)(void *value, SchdTask *tk),
  void *value,
  int millis
);

/// Adds a task to do after 'millis' milliseconds.
SchdTask *schd_delay (
  Schd *this, void (*fn)(void *), void *value, int millis
);

/// Starts 'this' and bloks the program until its finallization.
void schd_start (Schd *this);

/// Ends 'this' when the current task finishes
void schd_end (Schd *this);

///
void schd_cmd (Schd *this, void (*fn)(char *result), char *cmd);

#endif
