# See LICENSE file for copyright and license details.

include config.mk

SRC = chartorune.c fgetrune.c fputrune.c runelen.c runetochar.c runetype.c \
      utfecpy.c utflen.c utfnlen.c utfrrune.c utfrune.c utftorunestr.c utfutf.c
OBJ = $(SRC:.c=.o)

LIB = libutf.a
HDR = utf.h utfio.h

all: $(LIB) utftest

$(LIB): $(OBJ)
	@rm -f $@
	$(AR) -rcs $@ $(OBJ)

utftest: utftest.o $(LIB)
	$(CC) $(LDFLAGS) -o $@ utftest.o $(LIB)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(OBJ): $(HDR)

runetype.c: runetypebody.i

runetypebody.i: mkrunetype.awk UnicodeData-$(UNICODE).txt
	$(AWK) -f mkrunetype.awk UnicodeData-$(UNICODE).txt > $@

install: $(LIB) $(HDR) $(MAN)
	@echo @ install libutf to $(DESTDIR)$(PREFIX)
	@mkdir -p $(DESTDIR)$(PREFIX)/lib
	@cp $(LIB) $(DESTDIR)$(PREFIX)/lib/$(LIB)
	@mkdir -p $(DESTDIR)$(PREFIX)/include
	@cp $(HDR) $(DESTDIR)$(PREFIX)/include/
	@mkdir -p $(DESTDIR)$(PREFIX)/share/man/man3
	@cp rune.3 $(DESTDIR)$(PREFIX)/share/man/man3/rune.3
	@sed 's/$$UNICODE/$(UNICODE)/g' isalpharune.3 > $(DESTDIR)$(PREFIX)/share/man/man3/isalpharune.3

uninstall:
	@echo @ uninstall libutf from $(DESTDIR)$(PREFIX)
	@rm -f $(DESTDIR)$(PREFIX)/lib/$(LIB)
	@for f in $(HDR); do rm -f $(DESTDIR)$(PREFIX)/include/$$f; done
	@rm -f $(DESTDIR)$(PREFIX)/share/man/man3/rune.3
	@rm -f $(DESTDIR)$(PREFIX)/share/man/man3/isalpharune.3

clean:
	rm -f runetypebody.i $(OBJ) $(LIB) utftest.o utftest
