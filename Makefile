LIB = dmc

CFLAGS = -Wall -rdynamic
OBJECTS = $(subst src/dm,obj,$(patsubst %.c,%.o,$(wildcard src/dm/*.c)))

lib/lib$(LIB).a : $(OBJECTS)
	ar rcs lib/lib$(LIB).a $(OBJECTS)

obj/%.o : src/dm/%.c include/dm/%.h
	gcc $(CFLAGS) -c $< -o $@ -Iinclude \
	  -lgc

