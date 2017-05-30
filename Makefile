all: static

static:
	gcc -static *.c -L/usr/local/lib -ljson-c -l pthread -o testAPI-static 
	./testAPI-static

.PHONY: all static
