LIB=dmc

CFLAGS = -Wall -rdynamic
C_TESTS = $(wildcard src/*.c)
H_TESTS = $(wildcard src/*.h)
TESTS = $(C_TESTS) $(H_TESTS)

OBJECTS = $(subst src/$(LIB),obj,$(patsubst %.c,%.o,$(wildcard ../src/$(LIB)/*.c)))
DIRS = char Kv TpIntInt # x/char
OBJECTS2 = $(foreach DIR, $(DIRS),\
  $(subst src/$(LIB)/$(DIR)/,obj/$(subst /,_,$(DIR))_,\
	$(patsubst %.c,%.o,$(wildcard ../src/$(LIB)/$(DIR)/*.c))))

bin/tests : $(TESTS) ../lib/lib$(LIB).a
	gcc $(CFLAGS) $(TESTS) -o bin/tests -Iinclude \
		-I../include -L../lib -l$(LIB) \
		-lgc -lpthread -lm

../lib/lib$(LIB).a : $(OBJECTS) $(OBJECTS2)
	ar rcs ../lib/lib$(LIB).a $(OBJECTS) $(OBJECTS2)

../obj/%.o : ../src/$(LIB)/%.c ../include/$(LIB)/%.h
	gcc $(CFLAGS) -c $< -o $@ -I../include

../obj/char_%.o : ../src/$(LIB)/char/%.c ../include/$(LIB)/char/%.h
	gcc $(CFLAGS) -c $< -o $@ -I../include

../obj/Kv_%.o : ../src/$(LIB)/Kv/%.c ../include/$(LIB)/Kv/%.h
	gcc $(CFLAGS) -c $< -o $@ -I../include

../obj/TpIntInt_%.o : ../src/$(LIB)/TpIntInt/%.c ../include/$(LIB)/TpIntInt/%.h
	gcc $(CFLAGS) -c $< -o $@ -I../include

#../obj/x_char_%.o : ../src/$(LIB)/x/char/%.c ../include/$(LIB)/x/char/%.h
#	gcc $(CFLAGS) -c $< -o $@ -I../include
