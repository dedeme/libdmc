LIB = dmc

CFLAGS = -Wall -rdynamic
C_TESTS = $(wildcard src/*.c)
H_TESTS = $(wildcard src/*.h)
TESTS = $(C_TESTS) $(H_TESTS)
OBJECTS = $(subst src/dmc,obj,$(patsubst %.c,%.o,$(wildcard ../src/dmc/*.c)))
OBJECTS2 = $(subst src/dmc/ct,obj/ct,$(patsubst %.c,%.o,$(wildcard ../src/dmc/ct/*.c)))

bin/tests : $(TESTS) ../lib/lib$(LIB).a
	gcc $(CFLAGS) $(TESTS) -o bin/tests -I../include -L../lib -l$(LIB) \
		-lgc

../lib/lib$(LIB).a : $(OBJECTS) $(OBJECTS2)
	if [ ! -e ../lib ];then mkdir ../lib;fi
	ar rcs ../lib/lib$(LIB).a $(OBJECTS)
	ar rcs ../lib/lib$(LIB).a $(OBJECTS2)

../obj/%.o : ../src/dmc/%.c ../include/dmc/%.h
	if [ ! -e ../obj ];then mkdir ../obj;fi
	if [ ! -e ../obj/ct ];then mkdir ../obj/ct;fi
	gcc $(CFLAGS) -c $< -o $@ -I../include \
	  -lgc

