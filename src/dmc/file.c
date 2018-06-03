// Copyright 02-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <gc.h>
#include "dmc/file.h"
#include "dmc/exc.h"
#include "dmc/str.h"
#include "dmc/cryp.h"
#include "dmc/path.h"
#include "dmc/Bytes.h"
#include "dmc/ct/Achar.h"
#include "dmc/ct/Ichar.h"
#include "dmc/ct/Ibytes.h"
#include "dmc/Buf.h"
#include "dmc/Tuples.h"
#include "dmc/It.h"
#include "dmc/DEFS.h"

char *file_tmp (char *prefix) {
  XNULL(prefix)

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
    THROW(exc_io_t)
      exc_io("Working directory can no be find: %s"), strerror(errno)
    _THROW

  return d;
}

void file_cd (char *path) {
  XNULL(path)

  if (chdir(path)) {
    THROW(exc_io_t)
      exc_io("Fail changing the working directory to %s: %s"),
      path, strerror(errno)
    _THROW
  }
}

void file_mkdir (char *path) {
  XNULL(path)

  if (!*path) {
    return;
  } else if (*path != '/') {
    path = path_cat(file_cwd(), path, NULL);
  }
  file_mkdir(path_parent(path));
  if (mkdir(path, 0755) && errno != EEXIST)
    THROW(exc_io_t)
      exc_io("Fail making directory %s: %s"), path, strerror(errno)
    _THROW
}

Achar *file_dir (char *path) {
  XNULL(path)

  DIR *d = opendir(path);
  if (d) {
    Achar *paths = achar_new();
    struct dirent *res;
    while (1) {
      if ((res = readdir(d)) == NULL) {
        closedir(d);
        return paths;
      }
      const char *name = res->d_name;
      if (strcmp(name, ".") && strcmp(name, "..")) {
        char *new_path = path_cat(path, name, NULL);
        achar_add(paths, new_path);
      }
    }
  }
  THROW(exc_io_t)
    exc_io("Fail reading directory %s: %s"), path, strerror(errno)
  _THROW
  return NULL;
}

void file_del (char *path) {
  XNULL(path)

  struct stat buf;
  if (stat(path, &buf)) {
    if (errno != ENOENT) {
      THROW(exc_io_t)
        exc_io("Fail deleting %s: %s"), path, strerror(errno)
      _THROW
    }
  }
  if (S_ISDIR(buf.st_mode)) {
    Achar *a = file_dir(path);
    EACH(a, char, o) {
      file_del(o);
    }_EACH
    if (rmdir(path) && errno != ENOENT) {
      THROW(exc_io_t)
        exc_io("Fail deleting %s: %s"), path, strerror(errno)
      _THROW
    }
  } else {
    if (unlink(path) && errno != ENOENT) {
        THROW(exc_io_t)
          exc_io("Fail deleting %s: %s"), path, strerror(errno)
        _THROW
    }
  }
}

void file_rename (char *oldname, char *newname) {
  XNULL(oldname)
  XNULL(newname)

  if (rename(oldname, newname) == -1) {
    THROW(exc_io_t)
      exc_io("Fail renaming '%s' to '%s: %s"), oldname, newname, strerror(errno)
    _THROW
  }
}

void file_link (char *oldpath, char *newpath) {
  XNULL(oldpath)
  XNULL(newpath)

  if (symlink(oldpath, newpath) == -1) {
    THROW(exc_io_t)
      exc_io("Fail linking '%s' to '%s: %s"), newpath, oldpath, strerror(errno)
    _THROW
  }
}

bool file_exists (char *path) {
  XNULL(path)

  struct stat buf;
  if (stat(path, &buf)) {
    if (errno == ENOENT) {
      return false;
    }
    THROW(exc_io_t) exc_io("Fail reading %s: %s"), path, strerror(errno) _THROW
  }
  return true;
}

bool file_is_directory (char *path) {
  XNULL(path)

  struct stat buf;
  if (stat(path, &buf)) {
    if (errno == ENOENT) {
      return false;
    }
    THROW(exc_io_t) exc_io("Fail reading %s: %s"), path, strerror(errno) _THROW
  }
  if (S_ISDIR(buf.st_mode))
    return true;
  return false;
}

struct stat *file_info (char *path) {
  XNULL(path)

  struct stat *r = MALLOC(struct stat);
  if (stat(path, r)) {
    THROW(exc_io_t) exc_io("Fail reading %s: %s"), path, strerror(errno) _THROW
  }
  return r;
}

char *file_read (char *path) {
  XNULL(path)

  FILE *fl;
  size_t len = 0;
  struct flock lck = {
    .l_whence = SEEK_SET,
    .l_start = 0,
    .l_len = 0,
  };

  fl = fopen(path, "r");
  if (!fl) {
    THROW(exc_io_t) exc_io("Fail openning %s: %s"), path, strerror(errno) _THROW
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
  XNULL(path)
  XNULL(text)

  FILE *fl;
  int error;
  struct flock lck = {
    .l_whence = SEEK_SET,
    .l_start = 0,
    .l_len = 0,
  };

  fl = fopen(path, "w");
  if (!fl) {
    THROW(exc_io_t) exc_io("Fail openning %s: %s"), path, strerror(errno) _THROW
  }

  lck.l_type = F_WRLCK;
  fcntl (fileno(fl), F_SETLKW, &lck);

  error = fputs(text, fl);

  lck.l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lck);

  if (error == EOF || error < 0) {
    fclose(fl);
    THROW(exc_io_t) exc_io("Fail writing '%s': %s"), path, strerror(error) _THROW
  }
  fclose(fl);
}

void file_append (char *path, char *text) {
  XNULL(path)
  XNULL(text)

  FILE *fl;
  int error;
  struct flock lck = {
    .l_whence = SEEK_SET,
    .l_start = 0,
    .l_len = 0,
  };

  fl = fopen(path, "a");
  if (!fl) {
    THROW(exc_io_t) exc_io("Fail openning %s: %s"), path, strerror(errno) _THROW
  }

  lck.l_type = F_WRLCK;
  fcntl (fileno(fl), F_SETLKW, &lck);

  error = fputs(text, fl);

  lck.l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lck);

  if (error == EOF || error < 0) {
    fclose(fl);
    THROW(exc_io_t) exc_io("Fail writing '%s': %s"), path, strerror(error) _THROW
  }
  fclose(fl);
}

void file_copy (char *source_path, char *target_path) {
  XNULL(source_path)
  XNULL(target_path)

  if (!strcmp(source_path, target_path)) {
    return;
  }

  FILE *f1;
  FILE *f2;

  char  buffer[32678];
  size_t  n;

  if ((f1 = fopen(source_path, "rb")) == 0)
    THROW(exc_io_t)
      exc_io("Fail openning '%s': %s"), source_path, strerror(errno)
    _THROW
  if ((f2 = fopen(target_path, "wb")) == 0)
    THROW(exc_io_t)
      exc_io("Fail openning '%s': %s"), target_path, strerror(errno)
    _THROW


  while ((n = fread(buffer, sizeof(char), sizeof(buffer), f1)) > 0) {
    if (fwrite(buffer, sizeof(char), n, f2) != n)
      THROW(exc_io_t)
        exc_io("Fail writing '%s': %s"), target_path, strerror(errno)
      _THROW
  }

  fclose(f1);
  fclose(f2);
}

static LckFile *lck_new(FILE *file) {
  struct flock *lck = MALLOC(struct flock);
  lck->l_whence = SEEK_SET;
  lck->l_start = 0;
  lck->l_len = 0;
  return (LckFile *)tp_new(file, lck);
}

LckFile *file_ropen (char *path) {
  XNULL(path)

  FILE *file = fopen(path, "r");
  if (!file)
    THROW(exc_io_t)
      exc_io("Fail opening '%s': %s"), path, strerror(errno)
    _THROW

  LckFile *r = lck_new(file);

  ((struct flock *)tp_e2((Tp *)r))->l_type = F_RDLCK;
  fcntl (fileno(file), F_SETLKW, tp_e2((Tp *)r));
  return r;
}

LckFile *file_wopen (char *path) {
  XNULL(path)

  FILE *file = fopen(path, "w");
  if (!file)
    THROW(exc_io_t)
      exc_io("Fail opening '%s': %s"), path, strerror(errno)
    _THROW

  LckFile *r = lck_new(file);

  ((struct flock *)tp_e2((Tp *)r))->l_type = F_WRLCK;
  fcntl (fileno(file), F_SETLKW, tp_e2((Tp *)r));
  return r;
}

LckFile *file_aopen (char *path) {
  XNULL(path)

  FILE *file = fopen(path, "a");
  if (!file)
    THROW(exc_io_t)
      exc_io("Fail opening '%s': %s"), path, strerror(errno)
    _THROW

  LckFile *r = lck_new(file);

  ((struct flock *)tp_e2((Tp *)r))->l_type = F_WRLCK;
  fcntl (fileno(file), F_SETLKW, tp_e2((Tp *)r));
  return r;
}

char *file_read_line (LckFile *lck) {
  XNULL(lck)

  size_t len = 0;
  errno = 0;
  char *line= NULL;
  if (getline(&line, &len, tp_e1((Tp *)lck)) != -1) {
    char *r = str_copy(line);
    free(line);
    return r;
  }
  free(line);
  if (errno) {
    file_close(lck);
    THROW(exc_io_t) exc_io("Fail file_read_line: %s"), strerror(errno) _THROW
  }
  return "";
}

void file_write_text (LckFile *lck, char *text) {
  XNULL(lck)
  XNULL(text)

  int error = fputs(text, tp_e1((Tp *)lck));
  if (error == EOF || error < 0) {
    file_close(lck);
    THROW(exc_io_t) exc_io("Fail file_write_line: %s"), strerror(errno) _THROW
  }
}

Bytes *file_read_bin_buf (LckFile *lck, int buffer) {
  XNULL(lck)
  XNULL(buffer)

  unsigned char bs[buffer];
  int len = (int)fread(bs, 1, buffer, tp_e1((Tp *)lck));
  if (len == -1) {
    file_close(lck);
    THROW(exc_io_t) exc_io("Fail file_read_bin_buf: %s"), strerror(errno) _THROW
  }
  if (len == 0) {
    return bytes_new();
  }
  return bytes_from_bytes(bs, len);
}

Bytes *file_read_bin (LckFile *lck) {
  XNULL(lck)

  return file_read_bin_buf(lck, 8192);
}

void file_write_bin (LckFile *lck, Bytes *bs) {
  XNULL(lck)
  XNULL(bs)

  if (fwrite(bytes_bs(bs), bytes_length(bs), 1, tp_e1((Tp *)lck)) == -1) {
    THROW(exc_io_t) exc_io("Fail file_write_bin: %s"), strerror(errno) _THROW
  }
}

void file_close (LckFile *file) {
  XNULL(file)

  FILE *fl = (FILE *)tp_e1((Tp*)file);
  struct flock *lck = tp_e2((Tp *)file);

  lck->l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lck);
  fclose(fl);
}

LckFile *file_open_it (char *path) {
  XNULL(path)

  struct flock *lck = MALLOC(struct flock);
  lck->l_whence = SEEK_SET;
  lck->l_start = 0;
  lck->l_len = 0;

  FILE *file = fopen(path, "r");
  if (!file)
    THROW(exc_io_t)
      exc_io("Fail opening '%s': %s"), path, strerror(errno)
    _THROW

  lck->l_type = F_RDLCK;
  fcntl (fileno(file), F_SETLKW, lck);

  return (LckFile *)tp_new(file, lck);
}

void file_close_it (LckFile *file) {
  XNULL(file)

  file_close(file);
}

/**/typedef struct {
/**/  FILE *file;
/**/} to_it_O;
/**/static FNM (to_it_next, to_it_O, o) {
/**/  size_t len = 0;
/**/  char *line = NULL;
/**/  void *r;
/**/  if (getline(&line, &len, o->file) != -1)
/**/    r = str_copy(line);
/**/  else {
/**/    r = NULL;
/**/  }
/**/  free(line);
/**/  return r;
/**/}_FN
Ichar *file_to_it (LckFile *f) {
  XNULL(f)

  to_it_O *o = MALLOC(to_it_O);
  o->file = (FILE *)tp_e1((Tp *)f);

  return (Ichar *)it_new(o, to_it_next);
}

void file_from_it (char *path, Ichar *it) {
  XNULL(path)
  XNULL(it)

  FILE *fl;
  int error;
  struct flock lck = {
    .l_whence = SEEK_SET,
    .l_start = 0,
    .l_len = 0,
  };

  fl = fopen(path, "w");
  if (!fl) {
    THROW(exc_io_t) exc_io("Fail opening '%s'"), path _THROW
  }

  lck.l_type = F_WRLCK;
  fcntl (fileno(fl), F_SETLKW, &lck);

  while (ichar_has_next(it)) {
    error = fputs(ichar_next(it), fl);
    if (error == EOF || error < 0) {
      lck.l_type = F_UNLCK;
      fcntl (fileno(fl), F_SETLK, &lck);
      fclose(fl);
      THROW(exc_io_t)
        exc_io("Fail writing '%s': %s"), path, strerror(error)
      _THROW
    }
  }

  lck.l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lck);

  fclose(fl);
}

/**/static int BUF_LEN = 8192;
/**/typedef struct {
/**/  FILE *file;
/**/} to_it_bin_O;
/**/static FNM (to_it_bin_next, to_it_bin_O, o) {
/**/  unsigned char bs[BUF_LEN];
/**/  int len = (int)fread(bs, 1, BUF_LEN, o->file);
/**/  if (len) {
/**/    return bytes_from_bytes(bs, len);
/**/  } else {
/**/    return NULL;
/**/  }
/**/}_FN
Ibytes *file_to_it_bin (LckFile *f) {
  XNULL(f)

  to_it_bin_O *o = MALLOC(to_it_bin_O);
  o->file = (FILE *)tp_e1((Tp*)f);

  return (Ibytes *)it_new(o, to_it_bin_next);
}

void file_from_it_bin (char *path, Ibytes *it) {
  XNULL(path)
  XNULL(it)

  FILE *fl;
  struct flock lck = {
    .l_whence = SEEK_SET,
    .l_start = 0,
    .l_len = 0,
  };

  fl = fopen(path, "w");
  if (!fl) {
    THROW(exc_io_t) exc_io("Fail opening '%s'"), path _THROW
  }

  lck.l_type = F_WRLCK;
  fcntl (fileno(fl), F_SETLKW, &lck);

  while (ibytes_has_next(it)) {
    Bytes *bs = ibytes_next(it);

    if (fwrite(bytes_bs(bs), bytes_length(bs), 1, fl) == -1) {
      lck.l_type = F_UNLCK;
      fcntl (fileno(fl), F_SETLK, &lck);
      fclose(fl);
      THROW(exc_io_t)
        exc_io("Fail writing '%s': %s"), path, strerror(errno)
      _THROW
    }
  }

  lck.l_type = F_UNLCK;
  fcntl (fileno(fl), F_SETLK, &lck);

  fclose(fl);
}
