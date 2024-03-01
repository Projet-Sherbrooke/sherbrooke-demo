CFLAGS=-O0 -g
CC=gcc

all: i2c-linux i2c-bcm2835

.PHONY: clean

clean:
	-rm *.o i2c-linux i2c-bcm2835

i2c-linux.o: i2c-linux.c
i2c-bcm2835.o: i2c-bcm2835.c

i2c-linux: i2c-linux.o

i2c-bcm2835: i2c-bcm2835.o
	gcc $(CFLAGS) $< -o$@ -lbcm2835
