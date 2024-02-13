CFLAGS=-g -O0

all: signal1 signal2 signal3

.PHONY: clean
clean: 
	-rm signal1 signal2 signal3

signal1: signal1.c
	gcc $(CFLAGS) -osignal1 signal1.c

signal2: signal2.c
	gcc $(CFLAGS) -osignal2 signal2.c

signal3: signal3.c
	gcc $(CFLAGS) -osignal3 signal3.c