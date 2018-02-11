# libdmc
C utilities.

libdmc use the Boehm garbage collector and requires libgc.

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
