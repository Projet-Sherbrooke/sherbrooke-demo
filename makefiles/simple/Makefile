all: helloworld

.PHONY: clean

clean:
	-rm helloworld.o main.o

helloworld.o: helloworld.c
	gcc -c -ohelloworld.o helloworld.c

main.o: main.c
	gcc -c -omain.o main.c

helloworld: main.o helloworld.o
	gcc -ohelloworld main.o helloworld.o
