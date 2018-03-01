// Copyright 07-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dm/dm.h"
#include <stddef.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>

char *file_tmp (char *prefix) {
  char *k0 = cryp_genk(10);
  char *k = str_creplace(k0, '/', '_');
  char *r = str_printf("/tmp/%s%s", prefix, k);
  if (file_exists(r)) {
    return file_tmp(prefix);
  }
  return r;
}

char *file_cwd () {
  char *d = getcwd(NULL, 0);
  if (!d)
    THROW
      exc_io("Working directory can no be find: %s"), strerror(errno)
    _THROW

  return d;
}

void file_cd (char *path) {
  if (chdir(path)) {
    THROW
      exc_io("Fail changing the working directory to %s: %s"),
      path, strerror(errno)
    _THROW
  }
}

void file_mkdir (char *path) {
  if (!*path) {
    return;
  } else if (*path != '/') {
    path = path_cat(file_cwd(), path, NULL);
  }
  file_mkdir(path_parent(path));
  if (mkdir(path, 0755) && errno != EEXIST)
    THROW
      exc_io("Fail making directory %s: %s"), path, strerror(errno)
    _THROW
}

Arr *file_dir (char *path) {
  DIR *d = opendir(path);
  if (d) {
    Arr *paths = arr_new();
    struct dirent *res;
    while (1) {
      if ((res = readdir(d)) == NULL) {
        closedir(d);
        return paths;
      }
      const char *name = res->d_name;
      if (strcmp(name, ".") && strcmp(name, "..")) {
        char *new_path = path_cat(path, name, NULL);
        arr_add(paths, new_path);
      }
    }
  }
  THROW
    exc_io("Fail reading directory %s: %s"), path, strerror(errno)
  _THROW
  return NULL;
}

void file_del (char *path) {
  struct stat buf;
  if (stat(path, &buf)) {
    if (errno != ENOENT) {
      THROW exc_io("Fail deleting %s: %s"), path, strerror(errno) _THROW
    }
  }
  if (S_ISDIR(buf.st_mode)) {
    Arr *a = file_dir(path);
    EACH(a, char, o) {
      file_del(o);
    }_EACH
    if (rmdir(path) && errno != ENOENT) {
      THROW exc_io("Fail deleting %s: %s"), path, strerror(errno) _THROW
    }
  }
  if (unlink(path) && errno != ENOENT) {
      THROW exc_io("Fail deleting %s: %s"), path, strerror(errno) _THROW
  }
}

void file_rename (char *oldname, char *newname) {
  if (rename(oldname, newname) == -1) {
    THROW
      exc_io("Fail renaming '%s' to '%s: %s"), oldname, newname, strerror(errno)
    _THROW
  }
}

bool file_exists (char *path) {
  struct stat buf;
  if (stat(path, &buf)) {
    if (errno == ENOENT) {
      return false;
    }
    THROW exc_io("Fail reading %s: %s"), path, strerror(errno) _THROW
  }
  return true;
}

bool file_is_directory (char *path) {
  struct stat buf;
  if (stat(path, &buf)) {
    if (errno == ENOENT) {
      return false;
    }
    THROW exc_io("Fail reading %s: %s"), path, strerror(errno) _THROW
  }
  if (S_ISDIR(buf.st_mode))
    return true;
  return false;
}

struct stat *file_info (char *path) {
  struct stat *r = MALLOC(struct stat);
  if (stat(path, r)) {
    THROW exc_io("Fail reading %s: %s"), path, strerror(errno) _THROW
  }
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
  if (!fl) {
    THROW exc_io("Fail openning %s: %s"), path, strerror(errno) _THROW
  }

  lck.l_type = F_RDLCK;
  fcntl (fileno(fl), F_SETLKW, &lck);

  Buf *bf = buf_new();
  char *line = NULL;
  while (getline(&line, &len, fl) != -1) {
    buf_add(bf, line);
  }
  free(line);

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
  if (!fl) {
    THROW exc_io("Fail openning %s: %s"), path, strerror(errno) _THROW
  }

  lck.l_type = F_WRLCK;
  fcntl (fileno(fl), F_SETLKW, &lck);

  error = fputs(text, fl);

  lck.l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lck);

  if (error == EOF || error < 0) {
    fclose(fl);
    THROW exc_io("Fail writing '%s': %s"), path, strerror(error) _THROW
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
    THROW exc_io("Fail openning %s: %s"), path, strerror(errno) _THROW
  }

  lck.l_type = F_WRLCK;
  fcntl (fileno(fl), F_SETLKW, &lck);

  error = fputs(text, fl);

  lck.l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lck);

  if (error == EOF || error < 0) {
    fclose(fl);
    THROW exc_io("Fail writing '%s': %s"), path, strerror(error) _THROW
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
    THROW exc_io("Fail openning '%s': %s"), source_path, strerror(errno) _THROW
  }
  if ((f2 = fopen(target_path, "wb")) == 0) {
    THROW exc_io("Fail openning '%s': %s"), target_path, strerror(errno) _THROW
  }

  while ((n = fread(buffer, sizeof(char), sizeof(buffer), f1)) > 0) {
    if (fwrite(buffer, sizeof(char), n, f2) != n)
      THROW exc_io("Fail writing '%s': %s"), target_path, strerror(errno) _THROW
  }

  fclose(f1);
  fclose(f2);
}

static LckFile *lck_new(FILE *file) {
  struct flock *lck = MALLOC(struct flock);
  lck->l_whence = SEEK_SET;
  lck->l_start = 0;
  lck->l_len = 0;
  return tp_new(file, lck);
}

LckFile *file_ropen (char *path) {
  FILE *file = fopen(path, "r");
  if (!file) {
    THROW exc_io("Fail opening '%s': %s"), path, strerror(errno) _THROW
  }

  LckFile *r = lck_new(file);

  ((struct flock *)r->e2)->l_type = F_RDLCK;
  fcntl (fileno(file), F_SETLKW, r->e2);
  return r;
}

LckFile *file_wopen (char *path) {
  FILE *file = fopen(path, "w");
  if (!file) {
    THROW exc_io("Fail opening '%s': %s"), path, strerror(errno) _THROW
  }

  LckFile *r = lck_new(file);

  ((struct flock *)r->e2)->l_type = F_WRLCK;
  fcntl (fileno(file), F_SETLKW, r->e2);
  return r;
}

LckFile *file_aopen (char *path) {
  FILE *file = fopen(path, "a");
  if (!file) {
    THROW exc_io("Fail opening '%s': %s"), path, strerror(errno) _THROW
  }

  LckFile *r = lck_new(file);

  ((struct flock *)r->e2)->l_type = F_WRLCK;
  fcntl (fileno(file), F_SETLKW, r->e2);
  return r;
}

char *file_read_line (LckFile *lck) {
  size_t len = 0;
  errno = 0;
  char *line= NULL;
  if (getline(&line, &len, lck->e1) != -1) {
    char *r = str_copy(line);
    free(line);
    return r;
  }
  free(line);
  if (errno) {
    file_close(lck);
    THROW exc_io("Fail file_read_line: %s"), strerror(errno) _THROW
  }
  return "";
}

void file_write_text (LckFile *lck, char *text) {
  int error = fputs(text, lck->e1);
  if (error == EOF || error < 0) {
    file_close(lck);
    THROW exc_io("Fail file_write_line: %s"), strerror(errno) _THROW
  }
}

Bytes *file_read_bin_buf (LckFile *lck, int buffer) {
  unsigned char bs[buffer];
  int len = (int)fread(bs, 1, buffer, lck->e1);
  if (len == -1) {
    file_close(lck);
    THROW exc_io("Fail file_read_bin_buf: %s"), strerror(errno) _THROW
  }
  if (len == 0) {
    return bytes_new();
  }
  return bytes_from_bytes(bs, len);
}

inline
Bytes *file_read_bin (LckFile *lck) {
  return file_read_bin_buf(lck, 8192);
}

void file_write_bin (LckFile *lck, Bytes *bs) {
  if (fwrite(bytes_bs(bs), bytes_length(bs), 1, lck->e1) == -1) {
    THROW exc_io("Fail file_write_bin: %s"), strerror(errno) _THROW
  }
}

void file_close (LckFile *file) {
  FILE *fl = (FILE *)file->e1;
  struct flock *lck = file->e2;

  lck->l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lck);
  fclose(fl);
}

LckFile *file_open_it (char *path) {
  struct flock *lck = MALLOC(struct flock);
  lck->l_whence = SEEK_SET;
  lck->l_start = 0;
  lck->l_len = 0;

  FILE *file = fopen(path, "r");
  if (!file)
    THROW exc_io("Fail opening '%s': %s"), path, strerror(errno) _THROW

  lck->l_type = F_RDLCK;
  fcntl (fileno(file), F_SETLKW, lck);

  return tp_new(file, lck);
}

inline
void file_close_it (LckFile *file) {
  file_close(file);
}

/**/typedef struct {
/**/  FILE *file;
/**/  char *next;
/**/} to_it_O;
/**/static FNP (to_it_has_next, to_it_O, o) { return (bool)o->next; }_FN
/**/static FNM (to_it_next, to_it_O, o) {
/**/  void *r = o->next;
/**/  size_t len = 0;
/**/  char *line = NULL;
/**/  if (getline(&line, &len, o->file) != -1)
/**/    o->next = str_copy(line);
/**/  else {
/**/    o->next = NULL;
/**/  }
/**/  free(line);
/**/  return r;
/**/}_FN
It/*char*/ *file_to_it (LckFile *f) {
  to_it_O *o = MALLOC(to_it_O);
  o->file = (FILE *)f->e1;
  o->next = NULL;
  to_it_next(o);

  return it_new(o, to_it_has_next, to_it_next);
}

void file_from_it (char *path, It/*char*/ *it) {
  FILE *fl;
  int error;
  struct flock lck = {
    .l_whence = SEEK_SET,
    .l_start = 0,
    .l_len = 0,
  };

  fl = fopen(path, "w");
  if (!fl) {
    THROW exc_io("Fail opening '%s'"), path _THROW
  }

  lck.l_type = F_WRLCK;
  fcntl (fileno(fl), F_SETLKW, &lck);

  while (it_has_next(it)) {
    error = fputs(it_next(it), fl);
    if (error == EOF || error < 0) {
      lck.l_type = F_UNLCK;
      fcntl (fileno(fl), F_SETLK, &lck);
      fclose(fl);
      THROW exc_io("Fail writing '%s': %s"), path, strerror(error) _THROW
    }
  }

  lck.l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lck);

  fclose(fl);
}

/**/typedef struct {
/**/  FILE *file;
/**/  Bytes *next;
/**/} to_it_bin_O;
/**/static FNP (to_it_bin_has_next, to_it_bin_O, o) { return (bool)o->next; }_FN
/**/static FNM (to_it_bin_next, to_it_bin_O, o) {
/**/  void *r = o->next;
/**/  int BUF_LEN = 8192;
/**/  unsigned char bs[BUF_LEN];
/**/  int len = (int)fread(bs, 1, BUF_LEN, o->file);
/**/  if (len) {
/**/    o->next = bytes_from_bytes(bs, len);
/**/  } else {
/**/    o->next = NULL;
/**/  }
/**/  return r;
/**/}_FN
It/*Bytes*/ *file_to_it_bin (LckFile *f) {
  to_it_bin_O *o = MALLOC(to_it_bin_O);
  o->file = (FILE *)f->e1;
  o->next = NULL;
  to_it_bin_next(o);

  return it_new(o, to_it_bin_has_next, to_it_bin_next);
}

void file_from_it_bin (char *path, It/*Bytes*/ *it) {
  FILE *fl;
  struct flock lck = {
    .l_whence = SEEK_SET,
    .l_start = 0,
    .l_len = 0,
  };

  fl = fopen(path, "w");
  if (!fl) {
    THROW exc_io("Fail opening '%s'"), path _THROW
  }

  lck.l_type = F_WRLCK;
  fcntl (fileno(fl), F_SETLKW, &lck);

  while (it_has_next(it)) {
    Bytes *bs = it_next(it);

    if (fwrite(bytes_bs(bs), bytes_length(bs), 1, fl) == -1) {
      lck.l_type = F_UNLCK;
      fcntl (fileno(fl), F_SETLK, &lck);
      fclose(fl);
      THROW exc_io("Fail writing '%s': %s"), path, strerror(errno) _THROW
    }
  }

  lck.l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lck);

  fclose(fl);
}
