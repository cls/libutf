/* See LICENSE file for copyright and license details. */
#ifndef UTF_H
#define UTF_H

#include <stddef.h>

typedef int Rune;

enum {
	UTFmax    = 6,       /* maximum bytes per rune */
	Runeself  = 0x80,    /* rune and utf are equal (<) */
	Runeerror = 0xFFFD,  /* decoding error in utf */
	Runemax   = 0x10FFFF /* maximum rune value */
};

int runetochar(char *, const Rune *);
int chartorune(Rune *, const char *);
int charntorune(Rune *, const char *, size_t);
int runelen(const Rune);
size_t runenlen(const Rune *, size_t);
int fullrune(const char *, size_t);
char *utfecpy(char *, char *, const char *);
size_t utflen(const char *);
size_t utfnlen(const char *, size_t);
char *utfrune(const char *, Rune);
char *utfrrune(const char *, Rune);
char *utfutf(const char *, const char *);

#endif
