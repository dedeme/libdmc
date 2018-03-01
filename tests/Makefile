LIB = dmc

CFLAGS = -Wall -rdynamic
C_TESTS = $(wildcard src/*.c)
H_TESTS = $(wildcard src/*.h)
TESTS = $(C_TESTS) $(H_TESTS)
OBJECTS = $(subst src/dm,obj,$(patsubst %.c,%.o,$(wildcard ../src/dm/*.c)))

bin/tests : $(TESTS) ../lib/lib$(LIB).a
	gcc $(CFLAGS) $(TESTS) -o bin/tests -I../include -L../lib -l$(LIB) \
		-lgc

../lib/lib$(LIB).a : $(OBJECTS)
	ar rcs ../lib/lib$(LIB).a $(OBJECTS)

../obj/%.o : ../src/dm/%.c ../include/dm/%.h
	gcc $(CFLAGS) -c $< -o $@ -I../include \
	  -lgc

