.PHONY: all clean test
CFLAGS = -O0 -g
all: bms

bms: bms.c bms.h
	gcc $(CFLAGS) -std=c99 -o bms bms.c

test: bms
	./bms 0 1 2

clean:
	rm -f bms

