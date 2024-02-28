main: main.o 
	clang main.c -o main `pkg-config --libs --cflags sdl2`

main.o: main.c
	clang main.c -c -Wall `pkg-config --libs --cflags sdl2`

clean:
	rm -f *.o

run:
	make main
	./main
