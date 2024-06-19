all: uconv
VERSION = 0.0.4
NAME    ?= uconv
APPNAME = $(NAME)
PREFIX  := /usr
BINDIR  = $(PREFIX)/bin
MANDIR  = $(PREFIX)/share/man/man1
CFLAGS  ?= 
LDFLAGS ?=
DESTDIR ?= /

MYCFLAGS=-O2 -Wall -Wextra -Wno-unused-result -DVERSION=\"$(VERSION)\" -DNAME=\"$(NAME)\" $(CFLAGS)
MYLDFLAGS=$(LDFLAGS)

uconv: uconv.o units.o
#	$(CC) -s -o uconv uconv.o units.o -lm
	$(CC) $(MYLDFLAGS) -s -o uconv uconv.o units.o -lm

uconv.o: uconv.c units.h
	$(CC) $(MYCFLAGS) -g -o uconv.o -c uconv.c

units.o: units.c units.h
	$(CC) $(MYCFLAGS) -g -o units.o -c units.c

clean:
	rm -f *.o *.stackdump uconv uconv.man.html

install: 
	install -D -m 755 uconv $(DESTDIR)/$(BINDIR)/$(NAME)
	install -D -m 644 man1/uconv.1 $(DESTDIR)/$(MANDIR)/$(NAME).1
	sed -i s/uconv/unconv/g $(DESTDIR)/$(MANDIR)/$(NAME).1

doc:
	perl makeman.pl > uconv.man.html

