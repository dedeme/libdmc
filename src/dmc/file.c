// Copyright 21-Jul-2019 ºDeme
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

static FileLck *lckFile_new(Gc *gc, struct flock *lock, FILE *f) {
  FileLck *this = gc_add(gc, malloc(sizeof(FileLck)));
  this->lock = lock;
  this->f = f;
  return this;
}

char *file_tmp (Gc *gc, char *prefix) {
  return file_tmp_in(gc, "/tmp", prefix);
}

char *file_tmp_in (Gc *gc, char *dir, char *prefix) {
  Gc *gcl = gc_new();
  char *r = cryp_genk(gcl, 10);
  r = str_creplace(gcl, r, '/', '_');
  r = str_f(gcl, "%s/%s%s", dir, prefix, r);
  if (file_exists(r)) {
    gc_free(gcl);
    return file_tmp_in(gc, dir, prefix);
  }
  return str_new(gc, r);
}

char *file_cwd (Gc *gc) {
  char *d = getcwd(NULL, 0);
  if (!d) {
    Gc *gcl = gc_new();
    EXC_IO(str_f(gcl,
      "Working directory can no be find: %s", strerror(errno)
    ), gcl)
  }

  return gc_add(gc, d);
}

void file_cd (char *path) {
  if (chdir(path)) {
    Gc *gcl = gc_new();
    EXC_IO(str_f(gcl,
      "Fail changing the working directory to %s: %s",
      path, strerror(errno)
    ), gcl)
  }
}

void file_mkdir (char *path) {
  if (!*path) {
    return;
  }

  Gc *gc = gc_new();
  char *p;
  if (*path != '/') {
    char *cwd = file_cwd(gc);
    p = path_cat(gc, cwd, path, NULL);
  } else {
    p = str_new(gc, path);
  }
  p = path_parent(gc, p);
  file_mkdir(p);

  if (mkdir(path, 0755) && errno != EEXIST)
    EXC_IO(str_f(gc,
      "Fail making directory %s: %s" , path, strerror(errno)
    ), gc)

  gc_free(gc);
}

// Arr[char]
Arr *file_dir (Gc *gc, char *path) {
  DIR *d = opendir(path);
  if (!d) {
    Gc *gcl = gc_new();
    EXC_IO(str_f(gcl,
      "Fail reading directory %s: %s", path, strerror(errno)
    ), gcl)
  }

  // Arr[char]
  Arr *a = arr_new(gc);
  struct dirent *res;
  while ((res = readdir(d))) {
    char *name = res->d_name;
    if (str_eq(name, ".") || str_eq(name, "..")) {
      continue;
    }
    arr_push(a, str_new(gc, name));
  }
  closedir(d);
  return a;
}

void file_del (char *path) {
  Gc *gc = gc_new();
  struct stat buf;
  if (stat(path, &buf)) {
    if (errno != ENOENT)
      EXC_IO(str_f(gc, "Fail deleting %s: %s", path, strerror(errno)), gc)

    return;
  }

  if (S_ISDIR(buf.st_mode)) {
    // Arr[char]
    Arr *a = file_dir(gc, path);
    EACH(a, char, f) {
      file_del(path_cat(gc, path, f, NULL));
    }_EACH
    if (rmdir(path) && errno != ENOENT)
      EXC_IO(str_f(gc, "Fail deleting %s: %s", path, strerror(errno)), gc)
  } else {
    if (unlink(path) && errno != ENOENT)
      EXC_IO(str_f(gc, "Fail deleting %s: %s", path, strerror(errno)), gc)
  }
  gc_free(gc);
}

void file_rename (char *old_path, char *new_path) {
  if (rename(old_path, new_path) == -1) {
    Gc *gc = gc_new();
    EXC_IO(str_f(gc,
      "Fail renaming '%s' to '%s: %s", old_path, new_path, strerror(errno)
    ), gc)
  }
}

void file_link (char *old_path, char *new_path) {
  if (symlink(old_path, new_path) == -1) {
    Gc *gc = gc_new();
    EXC_IO(str_f(gc,
      "Fail linking '%s' to '%s: %s", new_path, old_path, strerror(errno)
    ), gc)
  }
}

int file_exists (char *path) {
  struct stat buf;
  if (stat(path, &buf)) {
    if (errno == ENOENT) {
      return 0;
    }
    Gc *gc = gc_new();
    EXC_IO(str_f(gc, "Fail reading %s: %s", path, strerror(errno)), gc)
  }
  return 1;
}

int file_is_directory (char *path) {
  struct stat buf;
  if (stat(path, &buf)) {
    if (errno == ENOENT) {
      return 0;
    }
    Gc *gc = gc_new();
    EXC_IO(str_f(gc, "Fail reading %s: %s", path, strerror(errno)), gc)
  }
  if (S_ISDIR(buf.st_mode))
    return 1;
  return 0;
}

struct stat *file_info (Gc *gc, char *path) {
  struct stat *r = gc_add(gc, malloc(sizeof(struct stat)));
  if (stat(path, r)) {
    Gc *gcl = gc_new();
    EXC_IO(str_f(gcl, "Fail reading %s: %s", path, strerror(errno)), gcl)
  }

  return r;
}

int file_size(char *path) {
  Gc *gc = gc_new();
  struct stat *i = file_info(gc, path);
  int r = i->st_size;
  gc_free(gc);
  return r;
}

time_t file_modified(char *path) {
  Gc *gc = gc_new();
  struct stat *i = file_info(gc, path);
  time_t r = i->st_mtime;
  gc_free(gc);
  return r;
}

char *file_read (Gc *gc, char *path) {
  Gc *gcl = gc_new();
  FILE *fl;
  size_t len = 0;
  struct flock lck = {
    .l_whence = SEEK_SET,
    .l_start = 0,
    .l_len = 0,
  };

  fl = fopen(path, "r");
  if (!fl) {
    EXC_IO(str_f(gcl, "Fail openning %s: %s", path, strerror(errno)), gcl)
  }

  lck.l_type = F_RDLCK;
  fcntl (fileno(fl), F_SETLKW, &lck);

  Buf *bf = buf_new(gcl);
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

  char *r = buf_to_str(gc, bf);
  gc_free(gcl);
  return r;
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
  if (!fl) {
    Gc *gc = gc_new();
    EXC_IO(str_f(gc, "Fail openning %s: %s", path, strerror(errno)), gc)
  }

  lck.l_type = F_WRLCK;
  fcntl (fileno(fl), F_SETLKW, &lck);

  error = fputs(text, fl);

  lck.l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lck);

  if (error == EOF || error < 0) {
    fclose(fl);
    Gc *gc = gc_new();
    EXC_IO(str_f(gc, "Fail writing '%s': %s", path, strerror(error)), gc)
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
  if (!fl) {
    Gc *gc = gc_new();
    EXC_IO(str_f(gc, "Fail openning %s: %s", path, strerror(errno)), gc)
  }

  lck.l_type = F_WRLCK;
  fcntl (fileno(fl), F_SETLKW, &lck);

  error = fputs(text, fl);

  lck.l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lck);

  if (error == EOF || error < 0) {
    fclose(fl);
    Gc *gc = gc_new();
    EXC_IO(str_f(gc, "Fail writing '%s': %s", path, strerror(error)), gc)
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

  if ((f1 = fopen(source_path, "rb")) == 0) {
    Gc *gc = gc_new();
    EXC_IO(
      str_f(gc, "Fail openning '%s': %s", source_path, strerror(errno)), gc
    )
  }

  if ((f2 = fopen(target_path, "wb")) == 0) {
    Gc *gc = gc_new();
    EXC_IO(
      str_f(gc, "Fail openning '%s': %s", target_path, strerror(errno)), gc
    )
  }

  while ((n = fread(buffer, sizeof(char), sizeof(buffer), f1)) > 0) {
    if (fwrite(buffer, sizeof(char), n, f2) != n) {
      Gc *gc = gc_new();
      EXC_IO(
        str_f(gc, "Fail writing '%s': %s", target_path, strerror(errno)), gc
      )
    }
  }

  fclose(f1);
  fclose(f2);
}

static FileLck *lck_new(Gc *gc, FILE *file) {
  struct flock *lck = gc_add(gc, malloc(sizeof(struct flock)));
  lck->l_whence = SEEK_SET;
  lck->l_start = 0;
  lck->l_len = 0;
  return lckFile_new(gc, lck, file);
}

FileLck *file_ropen (Gc *gc, char *path) {
  FILE *file = fopen(path, "r");
  if (!file) {
    Gc *gcl = gc_new();
    EXC_IO(str_f(gcl, "Fail opening '%s': %s", path, strerror(errno)), gcl)
  }

  FileLck *r = lck_new(gc, file);
  r->lock->l_type = F_RDLCK;
  fcntl (fileno(file), F_SETLKW, r->lock);
  return r;
}

FileLck *file_wopen (Gc *gc, char *path) {
  FILE *file = fopen(path, "w");
  if (!file) {
    Gc *gcl = gc_new();
    EXC_IO(str_f(gcl, "Fail opening '%s': %s", path, strerror(errno)), gcl)
  }

  FileLck *r = lck_new(gc, file);
  r->lock->l_type = F_WRLCK;
  fcntl (fileno(file), F_SETLKW, r->lock);
  return r;
}

FileLck *file_aopen (Gc *gc, char *path) {
  FILE *file = fopen(path, "a");
  if (!file) {
    Gc *gcl = gc_new();
    EXC_IO(str_f(gcl, "Fail opening '%s': %s", path, strerror(errno)), gcl)
  }

  FileLck *r = lck_new(gc, file);
  r->lock->l_type = F_WRLCK;
  fcntl (fileno(file), F_SETLKW, r->lock);
  return r;
}

char *file_read_line (Gc *gc, FileLck *lck) {
  size_t len = 0;
  errno = 0;
  char *line= NULL;
  if (getline(&line, &len, lck->f) != -1) {
    return gc_add(gc, line);
  }
  free(line);
  if (errno) {
    file_close(lck);
    Gc *gcl = gc_new();
    EXC_IO(str_f(gcl, "Fail file_read_line: %s", strerror(errno)), gcl)
  }
  return "";
}

void file_write_text (FileLck *lck, char *text) {
  int error = fputs(text, lck->f);
  if (error == EOF || error < 0) {
    file_close(lck);
    Gc *gcl = gc_new();
    EXC_IO(str_f(gcl, "Fail file_write_line: %s", strerror(errno)), gcl)
  }
}

Bytes *file_read_bin_buf (Gc *gc, FileLck *lck, int buffer) {
  unsigned char bs[buffer];
  int len = (int)fread(bs, 1, buffer, lck->f);
  if (len == -1) {
    file_close(lck);
    Gc *gcl = gc_new();
    EXC_IO(str_f(gcl, "Fail file_read_bin_buf: %s", strerror(errno)), gcl)
  }
  if (len == 0) {
    return bytes_new(gc);
  }
  return bytes_from_bytes(gc, bs, len);
}

Bytes *file_read_bin (Gc *gc, FileLck *lck) {
  return file_read_bin_buf(gc, lck, 8192);
}

void file_write_bin (FileLck *lck, Bytes *bs) {
  if (fwrite(bytes_bs(bs), bytes_len(bs), 1, lck->f) == -1) {
    Gc *gc = gc_new();
    EXC_IO(str_f(gc, "Fail file_write_bin: %s", strerror(errno)), gc)
  }
}

void file_close (FileLck *lck) {
  FILE *fl = lck->f;
  struct flock *lock = lck->lock;

  lock->l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lock);
  fclose(fl);
}

