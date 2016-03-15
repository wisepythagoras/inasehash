
TARGET := inasehash
LIB    := libinasehash.a
MAINC  := main.c
CC     := clang 
AR     := ar 
CFLAGS := -fpic -Wall -Werror -Winline -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -O2
OUT    := $(patsubst %.c, lib%.a, $(wildcard *.c))

.PRECIOUS: %.o
.PHONY: clean

all: $(TARGET) 

$(TARGET): $(LIB) $(MAINC)
	$(CC) $(CFLAGS) -o $(TARGET) $(MAINC) -L. -linasehash


%.o: %.c %.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(LIB): inasehash.o
	$(AR) rcs $@ $< 

clean:
	rm -f *.o *.a $(TARGET) collisiontest *.so

collisiontest: $(LIB) collisiontest.c
	$(CC) $(CFLAGS) -o collisiontest collisiontest.c -L. -linasehash

phpextension: inasehash.c inasehash.h
	cd php-extension; ./make.sh

rubyextension: inasehash.c inasehash.h
	cd ruby-extension; ./make.sh
