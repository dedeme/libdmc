# libdmc

C utilities.

Some utilities from C language.

Version - 202111

Dependencies:

  - libpthread
  - libm
  - libgc (Garbage collector)

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
git clone --branch 202111 https://github.com/dedeme/libdmc.git
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
  Achar *a = achar_new();
  achar_push(a, "one");
  achar_push(a, "two");
  achar_push(a, "three");

  char **p = a->es;
  while (p < a->end) puts(*p++);
}
```

### Compile and execute

```sh
gcc test.c -o test -I./include -L./lib -ldmc -lgc -lm
./test
```
And console will show:

```text
one
two
thre
```
