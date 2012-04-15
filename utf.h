/* See LICENSE file for copyright and license details. */
#ifndef UTF_H
#define UTF_H

#include <stddef.h>

typedef unsigned int Rune;

enum {
	UTFmax    = 6,
	Runeerror = 0xFFFD
};

int runetochar(char *, Rune *);
int chartorune(Rune *, const char *);
int charntorune(Rune *, const char *, size_t);
int runelen(Rune);
int fullrune(const char *, size_t);
char *utfecpy(char *, char *, const char *);
size_t utflen(const char *);
size_t utfnlen(const char *, size_t);
char *utfrune(const char *, Rune);
char *utfrrune(const char *, Rune);
char *utfutf(const char *, const char *);

#endif
