# See LICENSE file for copyright and license details.

include config.mk

SRC = rune.c runetype.c
OBJ = $(SRC:.c=.o)

UCD = UnicodeData-6.1.0.txt

all: libutf.a utftest

libutf.a: $(OBJ)
	rm -f $@
	$(AR) rc $@ $(OBJ)

utftest: utftest.o libutf.a
	$(CC) $(LDFLAGS) -o $@ utftest.o libutf.a

.c.o:
	$(CC) $(CFLAGS) -c $<

runetypebody.h: mkrunetype.awk $(UCD)
	$(AWK) -f mkrunetype.awk $(UCD) > $@

clean:
	rm -f libutf.a utftest utftest.o $(OBJ)
