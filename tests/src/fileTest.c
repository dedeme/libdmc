// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dm/dm.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

void file_test() {
  printf("file tests\n");

  char *path, *entry;
  Arr *dir;

  dir = file_dir(sys_home());
  assert(dir);
  assert(!arr_size(dir));

  path = path_cat(sys_home(), "f1.txt", NULL);
  assert(!file_exists(path));
  file_write(path, "");
  assert(file_exists(path));
  dir = file_dir(sys_home());
  assert(dir);
  struct stat *pst = file_info(sys_home());
  assert(S_ISDIR(pst->st_mode));
  entry = str_join(it_from(dir), "");
  assert(!strcmp(entry, path));

  path = path_cat(sys_home(), "f2.txt", NULL);
  file_write(path, "");
  dir = file_dir(sys_home());
  assert(arr_size(dir) == 2);
  pst = file_info(path);
  assert(S_ISREG(pst->st_mode));

  struct stat st;
  char *path0, *path10, *path11, *path12, *path2, *text, *path2b;

  path0 = path_cat(sys_home(), "f0.txt", NULL);
  file_write(path0, "");
  stat(path0, &st);
  assert(!st.st_size);
  text = file_read(path0);
  assert(!strcmp(text, ""));

  path0 = path_cat(sys_home(), "f0.txt", NULL);
  file_append(path0, "ab");
  file_append(path0, "123\n");
  file_append(path0, "xx");
  stat(path0, &st);
  assert(st.st_size);
  text = file_read(path0);
  assert(!strcmp(text, "ab123\nxx"));
  path10 = path_cat(sys_home(), "f10.txt", NULL);
  file_write(path10, "a");
  stat(path10, &st);
  assert(st.st_size == 1);
  text = file_read(path10);
  assert(!strcmp(text, "a"));
  path11 = path_cat(sys_home(), "f11.txt", NULL);
  file_write(path11, "a\nb");
  stat(path11, &st);
  assert(st.st_size == 3);
  text = file_read(path11);
  assert(!strcmp(text, "a\nb"));
  path12 = path_cat(sys_home(), "f12.txt", NULL);
  file_write(path12, "\n01234567890\n");
  stat(path12, &st);
  assert(st.st_size == 13);
  text = file_read(path12);
  assert(!strcmp(text, "\n01234567890\n"));
  path2 = path_cat(sys_home(), "f2.txt", NULL);
  file_write(path2, "aa\nb\n01234567890\n");
  stat(path2, &st);
  assert(st.st_size == 17);
  text = file_read(path2);
  assert(!strcmp(text, "aa\nb\n01234567890\n"));
  path2b = path_cat(sys_home(), "f2.bin", NULL);
  LckFile *binlk = file_open_it (path2);
  It *bss = file_to_it_bin(binlk);
  file_from_it_bin(path2b, bss);
  file_close_it(binlk);
  text = file_read(path2b);
  assert(!strcmp(text, "aa\nb\n01234567890\n"));

  file_copy(path11, path2);
  text = file_read(path2);
  assert(!strcmp(text, "a\nb"));
  stat(path2, &st);
  int size = st.st_size;
  stat(path11, &st);
  assert(st.st_size == size);

  char *dpath = sys_home();
  path2b = str_printf("%s/f2.bin", dpath);
  binlk = file_ropen (path2b);
  Bytes *bbss = bytes_new();
  Bytes *tmpBss = file_read_bin_buf(binlk, 4);
  while (bytes_length(tmpBss) != 0) {
    bytes_add(bbss, tmpBss);
    tmpBss = file_read_bin_buf(binlk, 4);
  }
  file_close(binlk);

  binlk = file_wopen(path2b);
  file_write_bin(binlk, bbss);
  file_close(binlk);

  text = file_read(path2b);
  assert(!strcmp(text, "aa\nb\n01234567890\n"));

  file_copy(path11, path2);
  text = file_read(path2);
  assert(!strcmp(text, "a\nb"));
  struct stat *st2 = file_info(path2);
  size = st2->st_size;
  st2 = file_info(path11);
  assert(st2->st_size == size);

  if (str_ends(dpath, "/file")) {
    file_del(dpath);
  }

  LckFile *lck;
  file_mkdir(dpath);

  char *test (char *path) {
    Buf *bf = buf_new();
    LckFile *lck = file_ropen(path);
    char *rs;
    for(;;) {
      rs = file_read_line(lck);
      if (!*rs) {
        break;
      }
      buf_add(bf, rs);
    }
    file_close(lck);
    return buf_to_str(bf);
  }

  path = str_printf("%s/f.txt", dpath);
  file_write(path, "aa\nb\n01234567890\n");
  text = test(path);
  assert(!strcmp("aa\nb\n01234567890\n", text));

  file_write(path, "");
  text = test(path);
  assert(!strcmp("", text));

  lck = file_wopen(path);
  RANGE0(i, 10) {
    file_write_text(lck, str_printf("%d", i));
  }_RANGE
  file_close(lck);
  text = test(path);
  assert(!strcmp("0123456789", text));

  lck = file_aopen(path);
  file_write_text(lck, "AB");
  file_close(lck);

  text = test(path);
  assert(!strcmp("0123456789AB", text));

  if (str_ends(dpath, "/file")) {
    file_del(dpath);
  }

  if (str_ends(sys_home(), "/file")) {
    file_del(sys_home());
  }

  file_mkdir(sys_home());

  char *itest (It *it) {
    Buf *bf = buf_new();
    FNX (each, char, s) { buf_add(bf, s); }_FN
    it_each(it, each);
    return buf_str(bf);
  }

  if (str_ends(sys_home(), "/dmTest")) {
    file_del(sys_home());
    file_mkdir(sys_home());
  }

  path = path_cat(sys_home(), "f.txt", NULL);
  file_write(path, "aa\nb\n01234567890\n");
  assert(!strcmp(str_cat(sys_home(), "/f.txt", NULL),
    itest(it_from(file_dir(sys_home())))));
  LckFile *f = file_open_it(path);
  assert(!strcmp("aa\nb\n01234567890\n", itest(file_to_it(f))));
  file_close_it(f);
  file_write(path, "");
  f = file_open_it(path);
  assert(!strcmp("", itest(file_to_it(f))));
  file_close_it(f);

  Arr *a = arr_new();
  RANGE0(i, 10) {
    arr_add(a, str_printf("%d", i));
  }_RANGE

  file_from_it(path, it_from(a));
  f = file_open_it(path);
  assert(!strcmp("0123456789", itest(file_to_it(f))));
  file_close_it(f);

  if (str_ends(sys_home(), "/dmTest")) {
    file_del(sys_home());
    file_mkdir(sys_home());
  }

  printf("    Finshed\n");
}
