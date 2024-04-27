#Fichiers
TARGET = demo
SRCS = main.c map.c menu.c display.c time.c enemy1.c enemy2.c  enemy3.c  perso.c dialog_box.c fight.c #à completer par vos fichiers
OBJS = $(SRCS:.c=.o)

#Compilation
CC = clang
CFLAGS = -std=gnu11 -Wall -pedantic -O3
LDFLAGS = -lm

#Debug 
CFLAGS += -g
LDFLAGS += -lm
CFLAGS += -fsanitize=address -fno-omit-frame-pointer
LDFLAGS += -fsanitize=address

#Libs
CFLAGS += $(shell pkg-config --cflags sdl2)
LDFLAGS += $(shell pkg-config --libs sdl2)
CFLAGS += $(shell pkg-config --cflags sdl2_image)
LDFLAGS += $(shell pkg-config --libs sdl2_image)
CFLAGS += $(shell pkg-config --cflags sdl2_gfx)
LDFLAGS += $(shell pkg-config --libs  sdl2_gfx)
CFLAGS += $(shell pkg-config --cflags sdl2_mixer)
LDFLAGS += $(shell pkg-config --libs  sdl2_mixer)
# CFLAGS += $(shell pkg-config --cflags SDL2_ttf)   à decommenter si ça fonctionne pour vous et dont à enlèver le -lSDL2_ttf 
# LDFLAGS += $(shell pkg-config --libs SDL2_tff)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@ -lSDL2_ttf 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run:
	make
	./demo

