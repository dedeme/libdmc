LIB=dmc

CFLAGS = -Wall -rdynamic
OBJECTS = $(subst src/$(LIB),obj,$(patsubst %.c,%.o,$(wildcard src/$(LIB)/*.c)))

lib/lib$(LIB).a : $(OBJECTS)
	if [ ! -e lib ];then mkdir lib;fi
	ar rcs lib/lib$(LIB).a $(OBJECTS)

obj/%.o : src/dmc/%.c include/dmc/%.h
	if [ ! -e obj ];then mkdir obj;fi
	gcc $(CFLAGS) -c $< -o $@ -Iinclude \
	-lpthread -lm
