// Copyright 02-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing files

#ifndef DM_FILE_H
# define DM_FILE_H

#include <stdbool.h>

///
typedef struct file_LckFile LckFile;
typedef struct bytes_Bytes Bytes;
typedef struct achar_Achar Achar;
typedef struct ichar_Ichar Ichar;
typedef struct ibytes_Ibytes Ibytes;

/// file_tmp returns a new file path whose template is:
/// /tmp/'prefix'/xxxxxxxxxx. Where xxxxxxxxxx is an aleatory sequence of
/// digits.
char *file_tmp (char *prefix);

/// file_cwd returns the working directory<br>
/// Throws exception
char *file_cwd ();

/// file_cd sets the working directory<br>
/// Throws exception
void file_cd (char *path);

/// file_mkdir makes a directory with 0755 permissions.<br>
/// If parent directory does not exist it creates it.<br>
/// If 'path' already exists it does nothing.<br>
/// Throws exception
void file_mkdir (char *path);

/// file_dir returns an Arr[char *] with paths of files and directories
/// existing in 'path'<br>
/// Values '.' and '..' are not in the return.<br>
/// Throws exception
Achar *file_dir (char *path);

/// file_de deletes file or directory named 'path' although it is a directory
/// not empty.
///   If 'path' does not exists it does nothing.<br>
/// Throws exception
void file_del (char *path);

/// file_raname renames 'oldname' to 'newname'<br>Throw exception.
void file_rename (char *oldname, char *newname);

/// file_link makes a symbol link from 'newpath' to 'oldpath'.<br>
/// Throw exception.
void file_link (char *oldpath, char *newpath);

/// file_exits returns true if 'path' exists in the file system.<br>
/// Throws exception
bool file_exists (char *path);

/// file_is_directory returns true if file is a directory<br>
/// Throws exception
bool file_is_directory (char *path);

/// file_infor returns information of 'path'. If path does no exist or other
/// error happens, this function produces a 'error_generic'.<p>
/// Some fields to check in 'struct stat' are:
///   mode_t <b>st_mode</b>: It can be tested with S_ISDIR(), S_ISREG or S_ISLNK
///     among others. This macros are defined in sys/stat.h
///   uid_t <b>st_uid</b>: User id
///   gid_t <b>st_gid</b>: Group id
///   off_t <b>st_size</b>: File size
///   time_t <b>st_atime</b>: Last access to file in seconds.
///   time_t <b>st_mtime</b>: Last file modification in seconds.
/// Throws exception
struct stat *file_info (char *path);

/// file_read reads data from 'path', including ends of line.<br>
/// This function opens, reads and closes file.<br>
/// Throws exception
char *file_read (char *path);

/// file_write writes 'data' on 'path'.<br>
/// This function opens, writes and closes file.<br>
/// Throws exception
void file_write (char *path, char *text);

/// file_appends appends 'data' on 'path'.<br>
/// This function opens, writes and closes file.<br>
/// Throws exception
void file_append (char *path, char *text);

/// file_copy binary copy source to target.<br>
/// Throws exception
void file_copy (char *source_path, char *target_path);

/// file_ropen opens a file to read with file_read_line or file_read_bin.<br>
/// It returns a LckFile object which will be freed when close is called.<br>
/// Throws exception.
LckFile *file_ropen (char *path);

/// file_wopen opens a file to write with file_write_line or file_write_bin.<br>
/// It returns a LckFile object which will be freed when close is called.<br>
/// Throws exception.
LckFile *file_wopen (char *path);

/// file_aopen opens a file to append with file_write_line or file_write_bin.<br>
/// It returns a LckFile object which will be freed when close is called.<br>
/// Throws exception.
LckFile *file_aopen (char *path);

/// file_read_line reads a text file opened with file_ropen.<br>
/// It does not delete ends of line.<br>
/// When reading is finished, returns a blank string.<br>
/// Throws exception.
char *file_read_line (LckFile *lck);

/// file_write_text writes a text file opened with file_wopen or file_aopen.<br>
/// Throws exception.
void file_write_text (LckFile *lck, char *text);

/// file_read_bin_buf reads a binary file opened with file_ropen.<br>
/// When reading is finished, returns an empty Bytes.<br>
/// Throws exception.
Bytes *file_read_bin_buf (LckFile *lck, int buffer);

/// file_read_bin us the same as 'file_read_bin_bf' using a buffer of 8192.<br>
/// Throws exception.
Bytes *file_read_bin (LckFile *lck);

/// file_write_bin writes a binary file opened with file_wopen.<br>
/// Returns 0 if there is no error.<br>
/// Throws exception.
void file_write_bin (LckFile *lck, Bytes *bs);

/// file_clse closes a file open with file_ropen, file_wopen or file_aopen
void file_close (LckFile *lck);

/// file_open_it opens a file to use with file_to_it or file_to_it_bin..<br>
/// Throws exception.
LckFile *file_open_it (char *path);

/// file_close_it closes a file open with file_open_it
void file_close_it (LckFile *file);

/// file_to_ir reads a text file.<br>
/// Iterator opens, reads and closes file.<br>
/// It does not delete ends of line.
Ichar *file_to_it (LckFile *file);

/// file_from_it writes a text file from a It[char *].<br>
/// It does not add ends of line.<br>
/// Throws exception.
void file_from_it (char *path, Ichar *it);

/// file_to_it_bin reads a binary file.<br>
/// Iterator opens, reads and closes file.<br>
Ibytes *file_to_it_bin (LckFile *file);

/// file_from_it_bin writes a binary file from a It[char *].<br>
/// It does not add ends of line.<br>
/// Throws exception.
void file_from_it_bin (char *path, Ibytes *it);

#endif
