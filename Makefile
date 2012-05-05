# Edited for Debian GNU/Linux
DESTDIR = 
BIN = $(DESTDIR)/usr/games
MAN = $(DESTDIR)/usr/share/man/man6
CC = gcc
OBJFILES = main.o rolldice.o
LIBS = -lm -lreadline
INCLUDES = rolldice.h

all: rolldice
	
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

clean:
	rm -f ./rolldice $(OBJFILES) rolldice.6.gz
