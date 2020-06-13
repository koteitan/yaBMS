.PHONY: all clean test

all: bms

bms: bms.c bms.h
	gcc -O0 -o bms bms.c

test: bms
	./bms 0 1 2

clean:
	rm -f bms

