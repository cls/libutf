# See LICENSE file for copyright and license details.

include config.mk

GEN = \
	src/isalpharune.c \
	src/iscntrlrune.c \
	src/iscombiningrune.c \
	src/isdigitrune.c \
	src/islowerrune.c \
	src/isspacerune.c \
	src/istitlerune.c \
	src/isupperrune.c \

GENOBJ = $(GEN:.c=.o)

SRC = $(GEN) \
	src/chartorune.c \
	src/isalnumrune.c \
	src/isblankrune.c \
	src/isgraphrune.c \
	src/isprintrune.c \
	src/ispunctrune.c \
	src/isxdigitrune.c \
	src/runelen.c \
	src/runestrcat.c \
	src/runestrchr.c \
	src/runestrcmp.c \
	src/runestrcpy.c \
	src/runestrdup.c \
	src/runestrecpy.c \
	src/runestrlen.c \
	src/runestrrchr.c \
	src/runestrstr.c \
	src/runetochar.c \
	src/runetype.c \
	src/runewidth.c \
	src/utfecpy.c \
	src/utflen.c \
	src/utfnlen.c \
	src/utfrrune.c \
	src/utfrune.c \
	src/utftorunestr.c \
	src/utfutf.c \

OBJ = $(SRC:.c=.o)

TESTSRC = \
	test/boundary.c \
	test/kosme.c \
	test/malformed.c \
	test/overlong.c

TEST = $(TESTSRC:.c=)

LIB = lib/libutf.a
HDR = include/utf.h
MAN = share/man/rune.3 share/man/isalpharune.3

all: $(LIB)

$(LIB): $(OBJ)
	@echo AR -r $@
	@mkdir -p lib
	@rm -f $@
	@$(AR) -rcs $@ $(OBJ)

.c.o:
	@echo CC -c $<
	@$(CC) $(CFLAGS) -c -o $@ $<

.c:
	@echo CC -o $@
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIB)

$(OBJ): $(HDR)

$(GEN): bin/mkrunetype.awk share/UnicodeData-$(UNICODE).txt src/runetype.h src/runetypebody.h
	@echo AWK -f bin/mkrunetype.awk
	@$(AWK) -f bin/mkrunetype.awk share/UnicodeData-$(UNICODE).txt

$(GENOBJ) src/runetype.o: src/runetype.h

$(TEST): $(LIB) test/tap.h

tests: $(TEST)
	@echo testing
	@prove $(TEST)

install: $(LIB) $(HDR) $(MAN)
	@echo installing libutf to $(DESTDIR)$(PREFIX)
	@mkdir -p $(DESTDIR)$(PREFIX)/lib
	@cp $(LIB) $(DESTDIR)$(PREFIX)/lib/
	@mkdir -p $(DESTDIR)$(PREFIX)/include
	@cp $(HDR) $(DESTDIR)$(PREFIX)/include/
	@mkdir -p $(DESTDIR)$(PREFIX)/share/man/man3
	@cp share/man/rune.3 $(DESTDIR)$(PREFIX)/share/man/man3/
	@sed 's/$$UNICODE/$(UNICODE)/g' share/man/isalpharune.3 > $(DESTDIR)$(PREFIX)/share/man/man3/isalpharune.3

uninstall:
	@echo uninstalling libutf from $(DESTDIR)$(PREFIX)
	@rm -f $(DESTDIR)$(PREFIX)/lib/$(LIB)
	@rm -f $(DESTDIR)$(PREFIX)/include/utf.h
	@rm -f $(DESTDIR)$(PREFIX)/share/man/man3/rune.3
	@rm -f $(DESTDIR)$(PREFIX)/share/man/man3/isalpharune.3

clean:
	@echo cleaning
	@rm -f $(GEN) $(OBJ) $(LIB) $(TEST)
