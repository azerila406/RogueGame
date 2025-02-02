UNAME_S := $(shell uname -s)

CC = gcc
CFLAGS = -Wall -lncursesw -lsqlite3 -I$(IDIR) -g -fsanitize=undefined -DNCURSES_WIDECHAR=1 -lSDL2 -lSDL2_mixer

ifeq ($(UNAME_S), Darwin)
    LDFLAGS = -L/opt/homebrew/opt/ncurses/lib -L/opt/homebrew/lib -L/opt/homebrew/Cellar/sdl2/2.30.12/lib
    NCFLAGS = -I/opt/homebrew/opt/ncurses/include -I/opt/homebrew/include/SDL2 -I/opt/homebrew/Cellar/sdl2/2.30.12/include/SDL2
else
    LDFLAGS =
    NCFLAGS =
endif

IDIR = ./include/
SRCDIR = ./src/
SOURCES = $(SRCDIR)*.c

all: rogue run clean

rogue: $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) $(LDFLAGS) $(NCFLAGS) -o $@

run:
	./rogue

clean:
	rm -f rogue