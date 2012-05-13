# Edited for Debian GNU/Linux
DESTDIR = 
BIN = $(DESTDIR)/usr/games
MAN = $(DESTDIR)/usr/share/man/man6
CC = gcc
OBJFILES = main.o rolldice.o
LIBS = -lm -lreadline
INCLUDES = rolldice.h


MAJOR_VERSION = $(shell grep MAJOR_VERSION version.h | cut --delim " " --field 6 | cut --delim ";" --field 1)
MINOR_VERSION = $(shell grep MINOR_VERSION version.h | cut --delim " " --field 6 | cut --delim ";" --field 1)
VERSION = $(MAJOR_VERSION).$(MINOR_VERSION)

all: rolldice man

rolldice: $(OBJFILES)
	$(CC) $(OBJFILES) -g -o rolldice $(LIBS)

main.o: main.c $(INCLUDES)
	$(CC) -g -c main.c

rolldice.o: rolldice.c $(INCLUDES)
	$(CC) -g -c rolldice.c

install: $(EXECFILES)
	install -d $(BIN) $(MAN)
	install ./rolldice $(BIN)
	strip $(BIN)/rolldice
	gzip -9 -c rolldice.6 > rolldice.6.gz
	install -m644 rolldice.6.gz $(MAN)

man:
	sed s/__VERSION__/$(VERSION)/ rolldice.6.src > rolldice.6

clean:
	rm -f ./rolldice $(OBJFILES) rolldice.6 rolldice.6.gz
