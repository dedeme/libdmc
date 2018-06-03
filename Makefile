LIB = dmc

CFLAGS = -Wall -rdynamic
OBJECTS = $(subst src/dmc,obj,$(patsubst %.c,%.o,$(wildcard src/dmc/*.c)))
OBJECTS2 = $(subst src/dmc/ct,obj/ct,$(patsubst %.c,%.o,$(wildcard src/dmc/ct/*.c)))

lib/lib$(LIB).a : $(OBJECTS) $(OBJECTS2)
	if [ ! -e lib ];then mkdir lib;fi
	ar rcs lib/lib$(LIB).a $(OBJECTS)
	ar rcs lib/lib$(LIB).a $(OBJECTS2)

obj/%.o : src/dmc/%.c include/dmc/%.h
	if [ ! -e obj ];then mkdir obj;fi
	if [ ! -e obj/ct ];then mkdir obj/ct;fi
	gcc $(CFLAGS) -c $< -o $@ -Iinclude \
	  -lgc

