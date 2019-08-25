# libdmc

C utilities.

Some utilities from C language.

Version - 201909

Dependencies:
  -libpthread
  -libm
  -libgc (Garbage collector)

```sh
apt-get install libgc-dev
```

## Example of use

### Create a directory and open it.

```sh
mkdir dmctest
cd dmctest
```

### Clone repository

```sh
git clone --branch 201909 https://github.com/dedeme/libdmc.git
```

### Open 'libdmc' and run 'make'

```sh
cd libdmc
make
```

### Create a 'test.c' file

```c
// Contents of file test.c

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
gcc test.c -o test -I./include -L./lib -ldmc -lgc -lpthread -lm
./test
```
And console will show:

```text
one
two
thre
```
