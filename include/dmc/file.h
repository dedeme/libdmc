// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing files

#ifndef DMC_FILE_H
  #define DMC_FILE_H

#include <time.h>
#include "dmc/Arr.h"
#include "dmc/Bytes.h"

///
typedef struct file_FileLck FileLck;

/// file_tmp returns a new file path whose template is:
/// /tmp/'prefix'xxxxxxxxxx. Where xxxxxxxxxx is an aleatory sequence of
/// digits.
char *file_tmp (char *prefix);

/// file_tmp returns a new file path whose template is:
/// 'dir'/'prefix'xxxxxxxxxx. Where xxxxxxxxxx is an aleatory sequence of
/// digits.
char *file_tmp_in (char *dir, char *prefix);

/// file_cwd returns the working directory
char *file_cwd (void);

/// file_cd sets the working directory
void file_cd (char *path);

/// file_mkdir makes a directory with 0755 permissions.<br>
/// If parent directory does not exist it creates it.<br>
/// If 'path' already exists it does nothing.
void file_mkdir (char *path);

/// file_dir returns an Arr[char] with names of files and directories
/// existing in 'path'<br>
/// Values '.' and '..' are not in the return.
Arr *file_dir (char *path);

/// file_de deletes file or directory named 'path' although it is a directory
/// not empty.
///   If 'path' does not exists it does nothing.
void file_del (char *path);

/// file_raname renames 'old_path' to 'new_path'.
void file_rename (char *old_path, char *new_path);

/// file_link makes a symbol link from 'new_path' to 'old_path'.
void file_link (char *old_path, char *new_path);

/// file_exits returns true if 'path' exists in the file system.
int file_exists (char *path);

/// file_is_directory returns true if file is a directory
int file_is_directory (char *path);

/// file_infor returns information of 'path'. If path does no exist or another
/// error happens, this function produces a exception.<p>
/// Some fields to check in 'struct stat' are:
///   mode_t <b>st_mode</b>: It can be tested with S_ISDIR(), S_ISREG or S_ISLNK
///     among others. This macros are defined in sys/stat.h
///   uid_t <b>st_uid</b>: User id
///   gid_t <b>st_gid</b>: Group id
///   off_t <b>st_size</b>: File size
///   time_t <b>st_atime</b>: Last access to file in seconds.
///   time_t <b>st_mtime</b>: Last file modification in seconds.
struct stat *file_info (char *path);

/// file_size returns the size of 'path'.
int file_size(char *path);

/// file_modified returns the last access in seconds
time_t file_modified(char *path);

/// file_read reads data from 'path', including ends of line.<br>
/// This function opens, reads and closes file.
char *file_read (char *path);

/// file_write writes 'data' on 'path'.<br>
/// This function opens, writes and closes file.
void file_write (char *path, char *text);

/// file_appends appends 'data' on 'path'.<br>
/// This function opens, writes and closes file.
void file_append (char *path, char *text);

/// file_copy binary copy source to target.
void file_copy (char *source_path, char *target_path);

/// file_ropen opens a file to read with file_read_line or file_read_bin.<br>
/// It returns a FileLck object which will be freed when close is called.
FileLck *file_ropen (char *path);

/// file_wopen opens a file to write with file_write_line or file_write_bin.<br>
/// It returns a FileLck object which will be freed when close is called.
FileLck *file_wopen (char *path);

/// file_aopen opens a file to append with file_write_line or file_write_bin.<br>
/// It returns a FileLck object which will be freed when close is called..
FileLck *file_aopen (char *path);

/// file_read_line reads a text file opened with file_ropen.<br>
/// It does not delete ends of line.<br>
/// When reading is finished, returns a blank string.
char *file_read_line (FileLck *lck);

/// file_write_text writes a text file opened with file_wopen or file_aopen.
void file_write_text (FileLck *lck, char *text);

/// file_read_bin_buf reads a binary file opened with file_ropen.<br>
/// When reading is finished, returns an empty Bytes.
Bytes *file_read_bin_buf (FileLck *lck, int buffer);

/// file_read_bin is the same as 'file_read_bin_bf' using a buffer of 8192.
Bytes *file_read_bin (FileLck *lck);

/// file_write_bin writes a binary file opened with file_wopen.<br>
/// Returns 0 if there is no error.
void file_write_bin (FileLck *lck, Bytes *bs);

/// file_clse closes a file open with file_ropen, file_wopen or file_aopen
void file_close (FileLck *lck);

#endif
