# libdmc

C utilities.

Some utilities from C language.

This utilities depend on libgc (Garbage collector)

```sh
apt-get install libgc-dev
```

## Example of use

### Create a directory and open it.

```sh
mkdir dmctest
cd dmctests
```

### Clone repository

```sh
git clone https://github.com/dedeme/libdmc.git
```

### Open 'libdmc' and run 'make'

```sh
cd libdmc
make
```

### Create a 'test.c' file

```c
// Contents of file cv.c

#include "dmc/std.h"

void main () {
  Arr *a = arr_new();
  arr_push(a, "one");
  arr_push(a, "two");
  arr_push(a, "three");

  EACH(a, char, e)
    puts(e);
  _EACH
}
```

### Compile and execute

```sh
gcc test.c -o test -I./include -L./lib -ldmc -lgc
./test
```
And console will show:

```text
one
two
thre
```
