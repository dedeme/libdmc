// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/file.h"
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include "dmc/std.h"
#include "dmc/cryp.h"

struct file_FileLck {
  struct flock *lock;
  FILE *f;
};

static FileLck *lckFile_new(struct flock *lock, FILE *f) {
  FileLck *this = MALLOC(FileLck);
  this->lock = lock;
  this->f = f;
  return this;
}

char *file_tmp (char *prefix) {
  char *r = cryp_genk(10);
  r = str_creplace(r, '/', '_');
  r = str_f("/tmp/%s%s", prefix, r);
  if (file_exists(r)) {
    return file_tmp(prefix);
  }
  return r;
}

char *file_cwd (void) {
  char *d = getcwd(NULL, 0);
  if (!d)
    EXC_IO(str_f("Working directory can no be find: %s", strerror(errno)))

  char *r = str_new(d);
  free(d);  // free is ok
  return r;
}

void file_cd (char *path) {
  if (chdir(path)) {
    EXC_IO(str_f(
      "Fail changing the working directory to %s: %s",
      path, strerror(errno)
    ))
  }
}

void file_mkdir (char *path) {
  if (!*path) {
    return;
  }

  char *p;
  if (*path != '/') {
    char *cwd = file_cwd();
    p = path_cat(cwd, path, NULL);
  } else {
    p = str_new(path);
  }
  p = path_parent(p);
  file_mkdir(p);

  if (mkdir(path, 0755) && errno != EEXIST)
    EXC_IO(str_f("Fail making directory %s: %s" , path, strerror(errno)))
}

// Arr[char]
Arr *file_dir (char *path) {
  DIR *d = opendir(path);
  if (!d)
    EXC_IO(str_f("Fail reading directory %s: %s", path, strerror(errno)))

  // Arr[char]
  Arr *a = arr_new();
  struct dirent *res;
  while ((res = readdir(d))) {
    char *name = res->d_name;
    if (str_eq(name, ".") || str_eq(name, "..")) {
      continue;
    }
    arr_push(a, str_new(name));
  }
  closedir(d);
  return a;
}

void file_del (char *path) {
  struct stat buf;
  if (stat(path, &buf)) {
    if (errno != ENOENT) {
      EXC_IO(str_f("Fail deleting %s: %s", path, strerror(errno)))
    }
    return;
  }

  if (S_ISDIR(buf.st_mode)) {
    // Arr[char]
    Arr *a = file_dir(path);
    EACH(a, char, f) {
      file_del(path_cat(path, f, NULL));
    }_EACH
    if (rmdir(path) && errno != ENOENT)
      EXC_IO(str_f("Fail deleting %s: %s", path, strerror(errno)))
  } else {
    if (unlink(path) && errno != ENOENT)
      EXC_IO(str_f("Fail deleting %s: %s", path, strerror(errno)))
  }
}

void file_rename (char *oldname, char *newname) {
  if (rename(oldname, newname) == -1)
    EXC_IO(str_f(
      "Fail renaming '%s' to '%s: %s", oldname, newname, strerror(errno)
    ))
}

void file_link (char *oldpath, char *newpath) {
  if (symlink(oldpath, newpath) == -1)
    EXC_IO(str_f(
      "Fail linking '%s' to '%s: %s", newpath, oldpath, strerror(errno)
    ))
}

int file_exists (char *path) {
  struct stat buf;
  if (stat(path, &buf)) {
    if (errno == ENOENT) {
      return 0;
    }
    EXC_IO(str_f("Fail reading %s: %s", path, strerror(errno)))
  }
  return 1;
}

int file_is_directory (char *path) {
  struct stat buf;
  if (stat(path, &buf)) {
    if (errno == ENOENT) {
      return 1;
    }
    EXC_IO(str_f("Fail reading %s: %s", path, strerror(errno)))
  }
  if (S_ISDIR(buf.st_mode))
    return 1;
  return 0;
}

struct stat *file_info (char *path) {
  struct stat *r = MALLOC(struct stat);
  if (stat(path, r))
    EXC_IO(str_f("Fail reading %s: %s", path, strerror(errno)))

  return r;
}

int file_size(char *path) {
  struct stat *i = file_info (path);
  int r = i->st_size;
  return r;
}

time_t file_modified(char *path) {
  struct stat *i = file_info (path);
  time_t r = i->st_mtime;
  return r;
}

char *file_read (char *path) {
  FILE *fl;
  size_t len = 0;
  struct flock lck = {
    .l_whence = SEEK_SET,
    .l_start = 0,
    .l_len = 0,
  };

  fl = fopen(path, "r");
  if (!fl)
    EXC_IO(str_f("Fail openning %s: %s", path, strerror(errno)))

  lck.l_type = F_RDLCK;
  fcntl (fileno(fl), F_SETLKW, &lck);

  Buf *bf = buf_new();
  char *line = NULL;
  while (getline(&line, &len, fl) != -1) {
    buf_add(bf, line);
    free(line); // free ok
    line = NULL;
  }
  free(line); // free ok

  lck.l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lck);
  fclose(fl);

  return buf_to_str(bf);
}

void file_write (char *path, char *text) {
  FILE *fl;
  int error;
  struct flock lck = {
    .l_whence = SEEK_SET,
    .l_start = 0,
    .l_len = 0,
  };

  fl = fopen(path, "w");
  if (!fl)
    EXC_IO(str_f("Fail openning %s: %s", path, strerror(errno)))

  lck.l_type = F_WRLCK;
  fcntl (fileno(fl), F_SETLKW, &lck);

  error = fputs(text, fl);

  lck.l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lck);

  if (error == EOF || error < 0) {
    fclose(fl);
    EXC_IO(str_f("Fail writing '%s': %s", path, strerror(error)))
  }
  fclose(fl);
}

void file_append (char *path, char *text) {
  FILE *fl;
  int error;
  struct flock lck = {
    .l_whence = SEEK_SET,
    .l_start = 0,
    .l_len = 0,
  };

  fl = fopen(path, "a");
  if (!fl)
    EXC_IO(str_f("Fail openning %s: %s", path, strerror(errno)))

  lck.l_type = F_WRLCK;
  fcntl (fileno(fl), F_SETLKW, &lck);

  error = fputs(text, fl);

  lck.l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lck);

  if (error == EOF || error < 0) {
    fclose(fl);
    EXC_IO(str_f("Fail writing '%s': %s", path, strerror(error)))
  }
  fclose(fl);
}

void file_copy (char *source_path, char *target_path) {
  if (!strcmp(source_path, target_path)) {
    return;
  }

  FILE *f1;
  FILE *f2;

  char  buffer[32678];
  size_t  n;

  if ((f1 = fopen(source_path, "rb")) == 0)
    EXC_IO(str_f("Fail openning '%s': %s", source_path, strerror(errno)))

  if ((f2 = fopen(target_path, "wb")) == 0)
    EXC_IO(str_f("Fail openning '%s': %s", target_path, strerror(errno)))

  while ((n = fread(buffer, sizeof(char), sizeof(buffer), f1)) > 0) {
    if (fwrite(buffer, sizeof(char), n, f2) != n)
      EXC_IO(str_f("Fail writing '%s': %s", target_path, strerror(errno)))
  }

  fclose(f1);
  fclose(f2);
}

static FileLck *lck_new(FILE *file) {
  struct flock *lck = MALLOC(struct flock);
  lck->l_whence = SEEK_SET;
  lck->l_start = 0;
  lck->l_len = 0;
  return lckFile_new(lck, file);
}

FileLck *file_ropen (char *path) {
  FILE *file = fopen(path, "r");
  if (!file)
    EXC_IO(str_f("Fail opening '%s': %s", path, strerror(errno)))

  FileLck *r = lck_new(file);
  r->lock->l_type = F_RDLCK;
  fcntl (fileno(file), F_SETLKW, r->lock);
  return r;
}

FileLck *file_wopen (char *path) {
  FILE *file = fopen(path, "w");
  if (!file)
    EXC_IO(str_f("Fail opening '%s': %s", path, strerror(errno)))

  FileLck *r = lck_new(file);
  r->lock->l_type = F_WRLCK;
  fcntl (fileno(file), F_SETLKW, r->lock);
  return r;
}

FileLck *file_aopen (char *path) {
  FILE *file = fopen(path, "a");
  if (!file)
    EXC_IO(str_f("Fail opening '%s': %s", path, strerror(errno)))

  FileLck *r = lck_new(file);
  r->lock->l_type = F_WRLCK;
  fcntl (fileno(file), F_SETLKW, r->lock);
  return r;
}

char *file_read_line (FileLck *lck) {
  size_t len = 0;
  errno = 0;
  char *line= NULL;
  if (getline(&line, &len, lck->f) != -1) {
    char *r = str_new(line);
    free(line); // free ok
    return r;
  }
  free(line); // free ok
  if (errno) {
    file_close(lck);
    EXC_IO(str_f("Fail file_read_line: %s", strerror(errno)))
  }
  return "";
}

void file_write_text (FileLck *lck, char *text) {
  int error = fputs(text, lck->f);
  if (error == EOF || error < 0) {
    file_close(lck);
    EXC_IO(str_f("Fail file_write_line: %s", strerror(errno)))
  }
}

Bytes *file_read_bin_buf (FileLck *lck, int buffer) {
  unsigned char bs[buffer];
  int len = (int)fread(bs, 1, buffer, lck->f);
  if (len == -1) {
    file_close(lck);
    EXC_IO(str_f("Fail file_read_bin_buf: %s", strerror(errno)))
  }
  if (len == 0) {
    return bytes_new();
  }
  return bytes_from_bytes(bs, len);
}

Bytes *file_read_bin (FileLck *lck) {
  return file_read_bin_buf(lck, 8192);
}

void file_write_bin (FileLck *lck, Bytes *bs) {
  if (fwrite(bytes_bs(bs), bytes_len(bs), 1, lck->f) == -1) {
    EXC_IO(str_f("Fail file_write_bin: %s", strerror(errno)))
  }
}

void file_close (FileLck *lck) {
  FILE *fl = lck->f;
  struct flock *lock = lck->lock;

  lock->l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lock);
  fclose(fl);
}
