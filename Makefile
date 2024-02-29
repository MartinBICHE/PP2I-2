CC = gcc

main: main.o
	gcc -o main main.o -lSDL2 -lSDL2_image

main.o: main.c
	gcc -c main.c

run: main
	./main

clean:
	rm -f main *.o
