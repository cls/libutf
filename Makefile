# See LICENSE file for copyright and license details.

SRC = utf.c utftest.c
OBJ = $(SRC:.c=.o)

CC = cc

CFLAGS  = -ansi -pedantic -std=c89 -Wall -Wextra
LDFLAGS = -Os -s

all: utftest

utftest: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f utftest $(OBJ)
