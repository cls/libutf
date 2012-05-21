# See LICENSE file for copyright and license details.

include config.mk

SRC = rune.c runetype.c
OBJ = $(SRC:.c=.o)

LIB = libutf.a
INC = utf.h

UCD = UnicodeData-6.1.0.txt

all: $(LIB) utftest

$(LIB): $(OBJ)
	$(AR) -rcs $@ $(OBJ)

utftest: utftest.o libutf.a
	$(CC) $(LDFLAGS) -o $@ utftest.o $(LIB)

.c.o:
	$(CC) $(CFLAGS) -c $<

runetypebody.h: mkrunetype.awk $(UCD)
	$(AWK) -f mkrunetype.awk $(UCD) > $@

install: libutf.a
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	cp $(LIB) $(DESTDIR)$(PREFIX)/lib/$(LIB)
	mkdir -p $(DESTDIR)$(PREFIX)/include
	cp $(INC) $(DESTDIR)$(PREFIX)/include/$(INC)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/lib/$(LIB)
	rm -f $(DESTDIR)$(PREFIX)/include/$(INC)

clean:
	rm -f libutf.a utftest utftest.o $(OBJ)
