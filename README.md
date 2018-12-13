# libdmc
C utilities.

Some utilities from C language.

## Compilation

### 1. Clone repository

```sh
https://github.com/dedeme/libdmc.git
```

### 2. Create directories _lib_ and _obj_ in _libdmc_ and _bin_ and _obj_ in _libdmc/tests_

```sh
cd libdmc
mkdir lib
mkdir obj
cd tests
mkdir bin
mkdir obj
cd ../..
```

### 3a. For compiling only the library run _make_ in _libdmc_

```sh
cd libdmc
make
```

### 3b. For compiling tests and library run __make__ in __libdmc/tests__

```sh
cd libdmc/tests
make
```
## Mode script

### 1. Copy _include/dmc_ as _dmc_ and libdmc.a to script directory.

### 2. Write a _.c_ file. For example:

```c
// Contents of file cv.c

#include "dmc/std.h"

Map *to_map(char *jss) {
  Arr *ajs = js_ra_new((Js *)jss);
  Arr *ks = js_ra_new(arr_get(ajs, 0));
  Arr *vs = js_ra_new(arr_get(ajs, 1));
  Map *r = map_new(free);
  EACH_IX(ks, Js, kjs, i)
    char *k = js_rs_new(kjs);
    map_put(r, k, js_rs_new(arr_get(vs, i)));
    free(k);
  _EACH
  return r;
}

void main () {
  Map *y = to_map(file_read_new("y.db"));
  Map *f = to_map(file_read_new("f.db"));
  Map *i = to_map(file_read_new("i.db"));
  Varr *ks = map_keys_new(i);
  EACH(ks, char, k)
    printf("%s -> %s | %s | %s\n",
      k, map_get_null(y, k), map_get_null(f, k), map_get_null(i, k));
  _EACH

  Arr *code = arr_new(free);
  EACH(ks, char, k)
    char *k2 = str_new(map_get_null(y, k));
    str_left(&k2, str_cindex(k2, '.'));
    arr_push(code, str_f_new("(\"%s\", \"%s\"),", k2, map_get_null(i, k)));
    free(k2);
  _EACH

  arr_sort(code, (FGREATER)str_greater);
  EACH(code, char, c)
    puts(c);
  _EACH
}
```

### 3. Compile _.c_ file

```sh
gcc cv.c -o cv -I./ -L./ -ldmc
```

### 4. Execute script

In work directory there are following files and directories:

- dmc (directory)
- libdmc.a
- cv.c
- cv

_cv_ can be executed with:

```sh
./cv
```

