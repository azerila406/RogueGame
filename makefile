CC = gcc
CFLAGS = -Wall -lncursesw -I$(IDIR) -g -fsanitize=undefined

IDIR = ./include/
SRCDIR = ./src/

SOURCES = $(SRCDIR)*.c\

all: rogue run clean

rogue: $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) -o $@

run:
	./rogue

clean:
	rm rogue