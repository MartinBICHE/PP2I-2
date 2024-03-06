CC = clang
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lSDL2 -lSDL2_image

TARGET = main
SOURCE = main.c

install:
	sudo apt-get install libsdl2-dev

run: 
	$(CC) $(CFLAGS) -o main main.c $(LDFLAGS)
	./main


clean:
	rm -f $(TARGET)
