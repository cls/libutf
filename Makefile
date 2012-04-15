# See LICENSE file for copyright and license details.

include config.mk

SRC = utf.c utftest.c
OBJ = $(SRC:.c=.o)

all: utftest

utftest: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f utftest $(OBJ)
