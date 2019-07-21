// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef DMC_SYS_H
  #define DMC_SYS_H

#include "dmc/Opt.h"

/// Initializates a normal program and call 'rnd_init()'.<br>
/// After call this function 'sys_home' and 'sys_user' are available.<p>
/// It creates the user directory in "~/.dmCApp/" + 'path'
Gc *sys_init (char *path);

///
char *sys_home (void);

///
char *sys_uname (void);

/// Sets LC_ALL, for example: <tt>sys_locale("es_ES.utf8")</tt>
void sys_locale (char *language);

/// Executes 'command', redirecting stderr to stdout, and returns its standard
/// out and standard error. If command fails, function returns an empty Opt.
///   return: Opt[char]
Opt *sys_cmd(Gc *gc, char *command);

/// Stops the current thread
void sys_sleep (int millis);

#endif
