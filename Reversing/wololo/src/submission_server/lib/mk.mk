RELEASE = ?

CC = gcc
CFLAGS = -g -Wall -Wshadow -Wextra -std=c99 $(EXTRAFLAGS)
INCLUDEDIR = lib/

ifeq ($(RELEASE),1)
DEFINES = -DRELEASE=1
endif

# This makes it so that each c file is compiled in the directory
CFILES = $(wildcard *.c)
OBJECTS = $(patsubst %.c,%.o,$(CFILES))

all: common.o $(OBJECTS)
	$(CC) $(CFLAGS) $(DEFINES) common.o $(OBJECTS) -o $(SVCNAME)
	strip -s $(SVCNAME)

common.o:
	$(CC) $(CFLAGS) $(DEFINES) -c -I$(INCLUDEDIR) $(INCLUDEDIR)/common.c

%.o:  %.c
	$(CC) $(CFLAGS) $(DEFINES) -I$(INCLUDEDIR) -c $< -o $@

clean:
	rm -rf $(SVCNAME) *.o

.PHONY: clean
