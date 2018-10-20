// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "file_tests.h"
#include <assert.h>
#include <sys/stat.h>

void file_tests(void) {
  puts("File tests");

  char *path, *entry;
  // Arr[char]
  Arr *dir;

 // file_del(sys_home());file_mkdir(sys_home());

  dir = file_dir_new(sys_home());
  assert(dir);
  assert(!arr_size(dir));

  path = path_cat_new(sys_home(), "f1.txt", NULL);
  assert(!file_exists(path));
  file_write(path, "");
  assert(file_exists(path));

  arr_free(dir);

  dir = file_dir_new(sys_home());
  assert(dir);
  struct stat *pst = file_info_new(sys_home());
  assert(S_ISDIR(pst->st_mode));
  entry = str_join_new(dir, "");
  path_name(&path);
  assert(str_eq(entry, path));

  free(path);
  free(pst);
  free(entry);
  arr_free(dir);

  path = path_cat_new(sys_home(), "f2.txt", NULL);
  file_write(path, "");
  dir = file_dir_new(sys_home());
  assert(arr_size(dir) == 2);
  pst = file_info_new(path);
  assert(S_ISREG(pst->st_mode));

  free(path);
  free(pst);
  arr_free(dir);

  struct stat st;
  char *path0, *text, *path10, *path11, *path12, *path2, *path2b;

  path0 = path_cat_new(sys_home(), "f0.txt", NULL);
  file_write(path0, "");
  stat(path0, &st);
  assert(!st.st_size);
  text = file_read_new(path0);
  assert(str_eq(text, ""));

  free(text);

  file_append(path0, "ab");
  file_append(path0, "123\n");
  file_append(path0, "xx");
  stat(path0, &st);
  assert(st.st_size);
  text = file_read_new(path0);
  assert(str_eq(text, "ab123\nxx"));

  free(text);
  free(path0);

  path10 = path_cat_new(sys_home(), "f10.txt", NULL);
  file_write(path10, "a");
  stat(path10, &st);
  assert(st.st_size == 1);
  text = file_read_new(path10);
  assert(str_eq(text, "a"));

  free(text);
  free(path10);


  path11 = path_cat_new(sys_home(), "f11.txt", NULL);
  file_write(path11, "a\nb");
  stat(path11, &st);
  assert(st.st_size == 3);
  text = file_read_new(path11);
  assert(str_eq(text, "a\nb"));

  free(text);

  path12 = path_cat_new(sys_home(), "f12.txt", NULL);
  file_write(path12, "\n01234567890\n");
  stat(path12, &st);
  assert(st.st_size == 13);
  text = file_read_new(path12);
  assert(str_eq(text, "\n01234567890\n"));

  free(text);
  free(path12);

  path2 = path_cat_new(sys_home(), "f2.txt", NULL);
  file_write(path2, "aa\nb\n01234567890\n");
  stat(path2, &st);
  assert(st.st_size == 17);
  text = file_read_new(path2);
  assert(str_eq(text, "aa\nb\n01234567890\n"));

  free(text);

  LckFile *binlk = file_ropen(path2);
  Bytes *bss = file_read_bin_buf_new(binlk, 200);
  file_close(binlk);

  path2b = path_cat_new(sys_home(), "f2.bin", NULL);
  binlk = file_wopen(path2b);
  file_write_bin(binlk, bss);
  file_close(binlk);
  text = file_read_new(path2b);
  assert(str_eq(text, "aa\nb\n01234567890\n"));

  bytes_free(bss);
  free(text);

  file_copy(path11, path2);
  text = file_read_new(path2);
  assert(str_eq(text, "a\nb"));
  stat(path2, &st);
  int size = st.st_size;
  stat(path11, &st);
  assert(st.st_size == size);

  free(text);

  binlk = file_ropen(path2b);
  Bytes *bbss = bytes_new();
  Bytes *tmpBss = file_read_bin_buf_new(binlk, 4);
  while (bytes_len(tmpBss) != 0) {
    bytes_add(bbss, tmpBss);
    bytes_free(tmpBss);
    tmpBss = file_read_bin_buf_new(binlk, 4);
  }
  bytes_free(tmpBss);
  file_close(binlk);

  binlk = file_wopen(path2b);
  file_write_bin(binlk, bbss);
  file_close(binlk);

  text = file_read_new(path2b);
  assert(str_eq(text, "aa\nb\n01234567890\n"));

  bytes_free(bbss);
  free(path2b);
  free(text);


  file_copy(path11, path2);
  text = file_read_new(path2);
  assert(str_eq(text, "a\nb"));
  struct stat *st2 = file_info_new(path2);
  size = st2->st_size;
  free(st2);
  st2 = file_info_new(path11);
  assert(st2->st_size == size);

  free(path11);
  free(path2);
  free(text);
  free(st2);

  char *dpath = str_f_new("%s/file", sys_home());
  LckFile *lck;
  file_mkdir(dpath);

  char *test_new (char *path) {
    Buf *bf = buf_new();
    LckFile *lck = file_ropen(path);
    char *rs;
    for(;;) {
      rs = file_read_line_new(lck);
      if (!*rs) {
        free(rs);
        break;
      }
      buf_add(bf, rs);
      free(rs);
    }
    file_close(lck);
    char *r = buf_to_str_new(bf);
    buf_free(bf);
    return r;
  }

  path = str_f_new("%s/f.txt", dpath);
  file_write(path, "aa\nb\n01234567890\n");
  text = test_new(path);
  assert(str_eq("aa\nb\n01234567890\n", text));

  free(text);
  free(dpath);

  file_write(path, "");
  text = test_new(path);
  assert(str_eq("", text));

  free(text);

  lck = file_wopen(path);
  RANGE0(i, 10) {
    char *tx = str_f_new("%d", i);
    file_write_text(lck, tx);
    free(tx);
  }_RANGE
  file_close(lck);
  text = test_new(path);
  assert(str_eq("0123456789", text));

  free(text);

  lck = file_aopen(path);
  file_write_text(lck, "AB");
  file_close(lck);

  file_append(path, "XX");

  text = test_new(path);
  assert(str_eq("0123456789ABXX", text));

  free(path);
  free(text);


  file_del(sys_home());file_mkdir(sys_home());
  puts("    Finished");
}

