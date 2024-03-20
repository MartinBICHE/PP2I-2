CC = clang
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lSDL2

TARGET = main
SOURCE = main.c

install:
	sudo apt-get install libsdl2-dev 

run: 
	$(CC) $(CFLAGS) -o main main.c $(LDFLAGS)
	./main

test:
	$(CC) $(CFLAGS) -o test test.c $(LDFLAGS)
	./test


clean:
	rm -f $(TARGET)
