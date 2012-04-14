# See LICENSE file for copyright and license details.

SRC = utf.c utftest.c
OBJ = $(SRC:.c=.o)

CFLAGS  = -ansi -pedantic -Wall -Wextra -D_ANSI_SOURCE
LDFLAGS = -Os -s

all: utftest

utftest: $(OBJ)
	cc $(LDFLAGS) -o $@ $(OBJ)

.c.o:
	cc $(CFLAGS) -c $<

clean:
	rm -f utftest $(OBJ)
