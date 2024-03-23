# Fichiers 
TARGET = demo 
SRCS = main.c
OBJS = $(SRCS:.c=.o)

# Compilation
CC = clang
CFLAGS = -std=gnu11 -Wall -pedantic -O3

# Debug
CFLAGS += -g
CFLAGS += -fsanitize=address -fno-omit-frame-pointer
LDFLAGS += -fsanitize=address

# Libs 
CFLAGS += $(shell pkg-config --cflags sdl2)
LDFLAGS += $(shell pkg-config --libs  sdl2)
CFLAGS += $(shell pkg-config --cflags sdl2_image)
LDFLAGS += $(shell pkg-config --libs  sdl2_image)
CFLAGS += $(shell pkg-config --cflags sdl2_gfx)
LDFLAGS += $(shell pkg-config --libs  sdl2_gfx)
CFLAGS += $(shell pkg-config --cflags sdl2_mixer)
LDFLAGS += $(shell pkg-config --libs  sdl2_mixer)
CFLAGS += $(shell pkg-config --cflags sdl2_ttf)
LDFLAGS += $(shell pkg-config --libs  sdl2_ttf)

# par défaut 
all: $(TARGET)

# linkage 
$(TARGET): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

# compilation
%.o: %.cflags
	$(CC) $(CFLAGS) -c $< -o $@

# nettoyage 
clean:
	rm -f $(OBJS) $(TARGET)