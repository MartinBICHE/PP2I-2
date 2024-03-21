CC = clang
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lSDL2

TARGET = main test test.o map.o
SOURCE = main.c

install:
	sudo apt-get install libsdl2-dev 

run: 
	$(CC) $(CFLAGS) -o main main.c $(LDFLAGS)
	./main

test: test.o map.o
	$(CC) -o test $(CFLAGS) test.o map.o $(LDFLAGS) $(SDL_LIBS)

test.o: test.c test.h
	$(CC) -c $(CFLAGS) test.c $(LDFLAGS) $(SDL_LIBS)

map.o: map.c map.h
	$(CC) -c $(CFLAGS) map.c $(LDFLAGS) $(SDL_LIBS)

clean:
	rm -f $(TARGET)
