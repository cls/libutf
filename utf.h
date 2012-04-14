/* See LICENSE file for copyright and license details. */
#ifndef UTF_H
#define UTF_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint32_t rune_t;

enum {
	UTFmax    = 6,
	Runeerror = 0xFFFD
};

int runetochar(char *, rune_t *);
int chartorune(rune_t *, const char *);
int charntorune(rune_t *, const char *, size_t);
int runelen(rune_t);
bool fullrune(const char *, size_t);
char *utfecpy(char *, char *, const char *);
size_t utflen(const char *);
size_t utfnlen(const char *, size_t);
char *utfrune(const char *, rune_t);
char *utfrrune(const char *, rune_t);
char *utfutf(const char *, const char *);

#endif
