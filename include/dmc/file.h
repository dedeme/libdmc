// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing files.

#ifndef DMC_FILE_H
  #define DMC_FILE_H

#include <time.h>
#include "dmc/Arr.h"
#include "dmc/Bytes.h"

///
typedef struct file_FileLck FileLck;

/// Returns a new file path whose template is:
/// /tmp/'prefix'xxxxxxxxxx. Where xxxxxxxxxx is an aleatory sequence of
/// digits.
char *file_tmp (char *prefix);

/// Returns a new file path whose template is:
/// 'dir'/'prefix'xxxxxxxxxx. Where xxxxxxxxxx is an aleatory sequence of
/// digits.
char *file_tmp_in (char *dir, char *prefix);

/// Returns the working directory
char *file_cwd (void);

/// Sets the working directory
void file_cd (char *path);

/// Makes a directory with 0755 permissions.
/// If parent directory does not exist it creates it.
/// If 'path' already exists it does nothing.
void file_mkdir (char *path);

/// fReturns an Arr[char] with names of files and directories existing in
/// 'path'.
/// Values '.' and '..' are not in the return.
Arr *file_dir (char *path);

/// Deletes file or directory named 'path' although it is a not empty directory.
/// If 'path' does not exists it does nothing.
void file_del (char *path);

/// Renames 'old_path' to 'new_path'.
void file_rename (char *old_path, char *new_path);

/// Makes a symbol link from 'new_path' to 'old_path'.
void file_link (char *old_path, char *new_path);

/// file_exits returns true if 'path' exists in the file system.
int file_exists (char *path);

/// Returns true if file is a directory
int file_is_directory (char *path);

/// Returns information of 'path'. If path does no exist or another error
/// happens, this function produces a exception.
/// Some fields to check in 'struct stat' are:
///   mode_t st_mode  - It can be tested with S_ISDIR(), S_ISREG or S_ISLNK
///                     among others. This macros are defined in sys/stat.h
///   uid_t st_uid    - User id
///   gid_t st_gid    - Group id
///   off_t st_size   - File size
///   time_t st_atime - Last access to file in seconds.
///   time_t st_mtime - Last file modification in seconds.
struct stat *file_info (char *path);

/// Returns the size of 'path'.
int file_size(char *path);

/// Returns the last access in seconds.
time_t file_modified(char *path);

/// reads data from 'path', including ends of line.
/// This function opens, reads and closes file.
char *file_read (char *path);

/// Writes 'data' on 'path'.
/// This function opens, writes and closes file.
void file_write (char *path, char *text);

/// Appends 'data' on 'path'.
/// This function opens, writes and closes file.
void file_append (char *path, char *text);

/// Binary copy source to target.
void file_copy (char *source_path, char *target_path);

/// Opens a file to read with file_read_line or file_read_bin.
/// It returns a FileLck object which will be freed when close is called.
FileLck *file_ropen (char *path);

/// Opens a file to write with file_write_line or file_write_bin.
/// It returns a FileLck object which will be freed when close is called.
FileLck *file_wopen (char *path);

/// Opens a file to append with file_write_line or file_write_bin.
/// It returns a FileLck object which will be freed when close is called..
FileLck *file_aopen (char *path);

/// Reads a text file opened with file_ropen.
/// It does not delete ends of line.
/// When reading is finished, returns a blank string.
char *file_read_line (FileLck *lck);

/// Writes a text file opened with file_wopen or file_aopen.
void file_write_text (FileLck *lck, char *text);

/// Reads a binary file opened with file_ropen.
/// When reading is finished, returns an empty Bytes.
Bytes *file_read_bin_buf (FileLck *lck, int buffer);

/// file_read_bin is the same as 'file_read_bin_bf' using a buffer of 8192.
Bytes *file_read_bin (FileLck *lck);

/// Writes a binary file opened with file_wopen.
/// Returns 0 if there is no error.
void file_write_bin (FileLck *lck, Bytes *bs);

/// Closes a file open with file_ropen, file_wopen or file_aopen.
void file_close (FileLck *lck);

#endif
