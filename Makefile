main: main.o 
	clang main.c -o main `pkg-config --libs --cflags sdl2 SDL2_image` -lSDL2_ttf

main.o: main.c
	clang main.c -c -Wall `pkg-config --libs --cflags sdl2 SDL2_image`

clean:
	rm -f *.o

run:
	make main
	./main
