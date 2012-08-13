# See LICENSE file for copyright and license details.

include config.mk

SRC = rune.c utf.c
OBJ = $(SRC:.c=.o)

LIB = libutf.a
INC = utf.h

all: $(LIB) utftest

$(LIB): $(OBJ)
	$(AR) -rcs $@ $(OBJ)

utftest: utftest.o $(LIB)
	$(CC) $(LDFLAGS) -o $@ utftest.o $(LIB)

.c.o:
	$(CC) $(CFLAGS) -c $<

install: $(LIB) $(INC) $(MAN)
	@echo @ install libutf to $(DESTDIR)$(PREFIX)
	@mkdir -p $(DESTDIR)$(PREFIX)/lib
	@cp $(LIB) $(DESTDIR)$(PREFIX)/lib/$(LIB)
	@mkdir -p $(DESTDIR)$(PREFIX)/include
	@cp $(INC) $(DESTDIR)$(PREFIX)/include/$(INC)
	@mkdir -p $(DESTDIR)$(PREFIX)/share/man/man3
	@cp rune.3 $(DESTDIR)$(PREFIX)/share/man/man3/rune.3

uninstall:
	@echo @ uninstall libutf from $(DESTDIR)$(PREFIX)
	@rm -f $(DESTDIR)$(PREFIX)/lib/$(LIB)
	@rm -f $(DESTDIR)$(PREFIX)/include/$(INC)
	@rm -f $(DESTDIR)$(PREFIX)/share/man/man3/rune.3

clean:
	rm -f $(LIB) utftest utftest.o $(OBJ)
