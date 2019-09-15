CC := clang

CFLAGS := -Wall -Wshadow -Wextra -std=c11 -O3
LDFLAGS := 

all: 
	$(CC) -o container container.c lzcsaw.c $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf container *.dSYM

.PHONY: clean
