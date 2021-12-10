LIB=dmc

CFLAGS = -Wall -rdynamic

OBJECTS = $(subst src/$(LIB),obj,$(patsubst %.c,%.o,$(wildcard src/$(LIB)/*.c)))
DIRS = char Kv TpIntInt # x/char
OBJECTS2 = $(foreach DIR, $(DIRS),\
  $(subst src/$(LIB)/$(DIR)/,obj/$(subst /,_,$(DIR)_),\
	$(patsubst %.c,%.o,$(wildcard src/$(LIB)/$(DIR)/*.c))))

lib/lib$(LIB).a : $(OBJECTS) $(OBJECTS2)
	if [ ! -e lib ];then mkdir lib;fi
	ar rcs lib/lib$(LIB).a $(OBJECTS)

obj/%.o : src/dmc/%.c include/dmc/%.h
	if [ ! -e obj ];then mkdir obj;fi
	gcc $(CFLAGS) -c $< -o $@ -Iinclude

obj/char_%.o : src/$(LIB)/char/%.c include/$(LIB)/char/%.h
	gcc $(CFLAGS) -c $< -o $@ -Iinclude

obj/Kv_%.o : src/$(LIB)/Kv/%.c include/$(LIB)/Kv/%.h
	gcc $(CFLAGS) -c $< -o $@ -Iinclude

obj/TpIntInt_%.o : src/$(LIB)/TpIntInt/%.c include/$(LIB)/TpIntInt/%.h
	gcc $(CFLAGS) -c $< -o $@ -Iinclude

#obj/x_char_%.o : src/$(LIB)/x/char/%.c include/$(LIB)/x/char/%.h
#	gcc $(CFLAGS) -c $< -o $@ -Iinclude
