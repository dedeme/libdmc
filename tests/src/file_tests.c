// Copyright 22-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "file_tests.h"
#include <assert.h>
#include <sys/stat.h>

void file_tests(void) {
  puts("File tests");

  char *path, *entry;
  Achar *dir;

 // file_del(sys_home());file_mkdir(sys_home());

  dir = file_dir(sys_home());
  assert(dir);
  assert(!achar_size(dir));

  path = path_cat(sys_home(), "f1.txt", NULL);
  assert(!file_exists(path));
  file_write(path, "");
  assert(file_exists(path));

  dir = file_dir(sys_home());
  assert(dir);
  struct stat *pst = file_info(sys_home());
  assert(S_ISDIR(pst->st_mode));
  entry = str_join(dir, "");
  path = path_name(path);
  assert(str_eq(entry, path));

  path = path_cat(sys_home(), "f2.txt", NULL);
  file_write(path, "");
  dir = file_dir(sys_home());
  assert(achar_size(dir) == 2);
  pst = file_info(path);
  assert(S_ISREG(pst->st_mode));

  struct stat st;
  char *path0, *text, *path10, *path11, *path12, *path2, *path2b;

  path0 = path_cat(sys_home(), "f0.txt", NULL);
  file_write(path0, "");
  stat(path0, &st);
  assert(!st.st_size);
  text = file_read(path0);
  assert(str_eq(text, ""));

  file_append(path0, "ab");
  file_append(path0, "123\n");
  file_append(path0, "xx");
  stat(path0, &st);
  assert(st.st_size);
  text = file_read(path0);
  assert(str_eq(text, "ab123\nxx"));

  path10 = path_cat(sys_home(), "f10.txt", NULL);
  file_write(path10, "a");
  stat(path10, &st);
  assert(st.st_size == 1);
  text = file_read(path10);
  assert(str_eq(text, "a"));

  path11 = path_cat(sys_home(), "f11.txt", NULL);
  file_write(path11, "a\nb");
  stat(path11, &st);
  assert(st.st_size == 3);
  text = file_read(path11);
  assert(str_eq(text, "a\nb"));

  path12 = path_cat(sys_home(), "f12.txt", NULL);
  file_write(path12, "\n01234567890\n");
  stat(path12, &st);
  assert(st.st_size == 13);
  text = file_read(path12);
  assert(str_eq(text, "\n01234567890\n"));

  path2 = path_cat(sys_home(), "f2.txt", NULL);
  file_write(path2, "aa\nb\n01234567890\n");
  stat(path2, &st);
  assert(st.st_size == 17);
  text = file_read(path2);
  assert(str_eq(text, "aa\nb\n01234567890\n"));

  FileLck *binlk = file_ropen(path2);
  Bytes *bss = file_read_bin_buf(binlk, 200);
  file_close(binlk);

  path2b = path_cat(sys_home(), "f2.bin", NULL);
  binlk = file_wopen(path2b);
  file_write_bin(binlk, bss);
  file_close(binlk);
  text = file_read(path2b);
  assert(str_eq(text, "aa\nb\n01234567890\n"));


  file_copy(path11, path2);
  text = file_read(path2);
  assert(str_eq(text, "a\nb"));
  stat(path2, &st);
  int size = st.st_size;
  stat(path11, &st);
  assert(st.st_size == size);

  binlk = file_ropen(path2b);
  Bytes *bbss = bytes_new();
  Bytes *tmpBss = file_read_bin_buf(binlk, 4);
  while (tmpBss->len != 0) {
    bytes_add(bbss, tmpBss);
    tmpBss = file_read_bin_buf(binlk, 4);
  }
  file_close(binlk);

  binlk = file_wopen(path2b);
  file_write_bin(binlk, bbss);
  file_close(binlk);

  text = file_read(path2b);
  assert(str_eq(text, "aa\nb\n01234567890\n"));

  file_copy(path11, path2);
  text = file_read(path2);
  assert(str_eq(text, "a\nb"));
  struct stat *st2 = file_info(path2);
  size = st2->st_size;
  st2 = file_info(path11);
  assert(st2->st_size == size);

  char *dpath = str_f("%s/file", sys_home());
  FileLck *lck;
  file_mkdir(dpath);

  char *test_new (char *path) {
    Buf *bf = buf_new();
    FileLck *lck = file_ropen(path);
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

  path = str_f("%s/f.txt", dpath);
  file_write(path, "aa\nb\n01234567890\n");
  text = test_new(path);
  assert(str_eq("aa\nb\n01234567890\n", text));

  file_write(path, "");
  text = test_new(path);
  assert(str_eq("", text));

  lck = file_wopen(path);
  for (int i = 0; i < 10; ++i) {
    char *tx = str_f("%d", i);
    file_write_text(lck, tx);
  }
  file_close(lck);
  text = test_new(path);
  assert(str_eq("0123456789", text));

  lck = file_aopen(path);
  file_write_text(lck, "AB");
  file_close(lck);

  file_append(path, "XX");

  text = test_new(path);
  assert(str_eq("0123456789ABXX", text));

  file_del(sys_home());file_mkdir(sys_home());
  puts("    Finished");
}

