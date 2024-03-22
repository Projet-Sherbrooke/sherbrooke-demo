CC = gcc
CFLAGS = -O2 -g -Wall -Werror
LDFLAGS = -li2c -lssd1306_i2c
DEPS = bmp280.h
OBJ = main.o bmp280.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm *.o bmp280
