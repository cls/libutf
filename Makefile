# See LICENSE file for copyright and license details.

include config.mk

SRC = rune.c runetype.c
OBJ = $(SRC:.c=.o)

UCD = UnicodeData-6.1.0.txt

all: libutf.a utftest

libutf.a: $(OBJ)
	$(AR) -rcs $@ $(OBJ)

utftest: utftest.o libutf.a
	$(CC) $(LDFLAGS) -o $@ utftest.o libutf.a

.c.o:
	$(CC) $(CFLAGS) -c $<

runetypebody.h: mkrunetype.awk $(UCD)
	$(AWK) -f mkrunetype.awk $(UCD) > $@

install: libutf.a
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	cp libutf.a $(DESTDIR)$(PREFIX)/lib

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/lib/libutf.a

clean:
	rm -f libutf.a utftest utftest.o $(OBJ)
