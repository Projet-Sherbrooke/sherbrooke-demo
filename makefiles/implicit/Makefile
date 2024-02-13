CC=gcc
CFLAGS=-g

all: helloworld

.PHONY: clean

clean:
	-rm helloworld.o main.o helloworld

helloworld: main.o helloworld.o
	gcc -ohelloworld main.o helloworld.o
