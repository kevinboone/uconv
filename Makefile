all: uconv
VERSION=0.0.1
NAME=uconv
APPNAME=$(NAME)
PREFIX=/usr
BINDIR=$(PREFIX)/bin
MANDIR=$(PREFIX)/share/man/man1

MYCFLAGS=-Wall -DVERSION=\"$(VERSION)\" -DNAME=\"$(NAME)\" $(CFLAGS)
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

install: all
	mkdir -p $(DESTDIR)/$(MANDIR)
	mkdir -p $(DESTDIR)/$(BINDIR)
	cp -p man1/uconv.1 $(DESTDIR)/$(MANDIR)
	cp -p uconv $(DESTDIR)/$(BINDIR)

srcdist: clean
	(cd ..; tar cvfz /tmp/$(APPNAME)-$(VERSION).tar.gz uconv-0.0.1)

web: srcdist
	./makeman.pl > uconv.man.html
	cp /tmp/$(APPNAME)-$(VERSION).tar.gz /home/kevin/docs/kzone5/target/
	cp uconv.man.html /home/kevin/docs/kzone5/target/
	cp README_uconv.html /home/kevin/docs/kzone5/source
	(cd /home/kevin/docs/kzone5; ./make.pl README_uconv)



