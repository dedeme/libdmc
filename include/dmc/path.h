// Copyright 17-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing file paths.

#ifndef DMC_PATH_H
  #define DMC_PATH_H

#include "Opt.h"

/// Returns name and extension of path.
/// If path is "" or ends at ("/") it returns "".
char *path_name (char *path);

/// Returns the parent path of 'path'.
/// If 'path' is "/" or a string without '/', it returns an empty string.
char *path_parent (char *path);

/// Returns only extension of path. Extension is returned with point,
/// (e.g., ".", ".txt").
/// If path does not have extension it returns "".
char *path_extension (char *path);

/// Returns only name of path.
/// If path is "", ends at ("/"), or if file starts with point, it returns "".
char *path_only_name (char *path);

/// Concatenates paths. Variable argumens must finish with NULL.
char *path_cat (char *s, char *more, ...);

/// Returns Opt<char> Cannonical representation of 'path'.
///   - If some component of 'path' is not in file system, returns 'opt_empty'.
///   - Directories do not finish in '/'.
Opt *path_canonical (char *s);

#endif
