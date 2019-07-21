// Copyright 20-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing file paths.

#ifndef DMC_PATH_H
  #define DMC_PATH_H

#include "Gc.h"

/// path_name returns name and extension of path.<br>
/// If path is "" or ends at ("/") it returns "".
char *path_name (Gc *gc, char *path);

/// path_parent returns the parent path of 'path'.<br>
/// If 'path' is "/" or a string without '/', it returns an empty string.
char *path_parent (Gc *gc, char *path);

/// path_extendsion returns only extension of path. Extension is
/// set with point (e.g., ".", ".txt") <br>
/// If path does not have extension it returns "".
char *path_extension (Gc *gc, char *path);

/// path_only_name returns only name of path.<br>
/// If path is "", ends at ("/"), or if file starts with point, it returns "".
char *path_only_name (Gc *gc, char *path);

/// path_cat concatenates paths. Variable argumens must finish with NULL.
char *path_cat (Gc *gc, char *s, char *more, ...);

#endif
