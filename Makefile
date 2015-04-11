all: uconv
VERSION=0.0.1
NAME=uconv
APPNAME=$(NAME)
BINDIR=$(PREFIX)/usr/bin
MANDIR=$(PREFIX)/usr/share/man/man1

CFLAGS=-Wall -DVERSION=\"$(VERSION)\" -DNAME=\"$(NAME)\" $(EXTRA_CFLAGS)
LDFLAGS=$(EXTRA_LDFLAGS)

uconv: uconv.o units.o
#	$(CC) -s -o uconv uconv.o units.o -lm
	$(CC) $(LDFLAGS) -s -o uconv uconv.o units.o -lm

uconv.o: uconv.c units.h
	$(CC) $(CFLAGS) -g -o uconv.o -c uconv.c

units.o: units.c units.h
	$(CC) $(CFLAGS) -g -o units.o -c units.c

clean:
	rm -f *.o *.stackdump uconv uconv.man.html

install:
	cp -p man1/uconv.1 $(MANDIR)
	cp -p uconv $(BINDIR)

srcdist: clean
	(cd ..; tar cvfz /tmp/$(APPNAME)-$(VERSION).tar.gz uconv-0.0.1)

web: srcdist
	./makeman.pl > uconv.man.html
	cp /tmp/$(APPNAME)-$(VERSION).tar.gz /home/kevin/docs/kzone5/target/
	cp uconv.man.html /home/kevin/docs/kzone5/target/
	cp README_uconv.html /home/kevin/docs/kzone5/source
	(cd /home/kevin/docs/kzone5; ./make.pl README_uconv)



