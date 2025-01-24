UNAME_S := $(shell uname -s)

CC = gcc
CFLAGS = -Wall -lncursesw -I$(IDIR) -g -fsanitize=undefined

ifeq ($(UNAME_S), Darwin)
    LDFLAGS = -L/opt/homebrew/opt/ncurses/lib
    NCFLAGS = -I/opt/homebrew/opt/ncurses/include
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