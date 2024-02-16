install:
	sudo apt install libsdl2-2.0-0 libsdl2-gfx-1.0-0 libsdl2-image-2.0-0 libsdl2-mixer-2.0-0 libsdl2-net-2.0-0 libsdl2-ttf-2.0-0
run:
	clang main.c -o main pkg-config --libs --cflags sdl2
clean:
	rm -f main *.o
