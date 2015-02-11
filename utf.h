/* See LICENSE file for copyright and license details. */
#ifndef UTF_H
#define UTF_H

#include <stddef.h>

typedef int Rune;

enum {
	UTFmax   = 6,       /* maximum bytes per rune */
	Runeself = 0x80,    /* rune and utf are equal (<) */
	Runemax  = 0x10FFFF /* maximum rune value */
};

extern Rune Runeerror; /* decoding error in utf */

int runetochar(char *, const Rune *);
int chartorune(Rune *, const char *);
int charntorune(Rune *, const char *, size_t);
int runelen(const Rune);
size_t runenlen(const Rune *, size_t);
int fullrune(const char *, size_t);
size_t utflen(const char *);
size_t utfnlen(const char *, size_t);
char *utfrune(const char *, Rune);
char *utfrrune(const char *, Rune);
char *utfutf(const char *, const char *);

int isalpharune(Rune);
int islowerrune(Rune);
int isspacerune(Rune);
int istitlerune(Rune);
int isupperrune(Rune);
int isdigitrune(Rune);

Rune *runestrcat(Rune *, const Rune *);
Rune *runestrncat(Rune *, const Rune *, size_t);
int runestrcmp(const Rune *, const Rune *);
int runestrncmp(const Rune *, const Rune *, size_t);
Rune *runestrcpy(Rune *, const Rune *);
Rune *runestrncpy(Rune *, const Rune *, size_t);
size_t runestrlen(const Rune *);
Rune *runestrchr(const Rune *, Rune);
Rune *runestrrchr(const Rune *, Rune);
Rune *runestrdup(const Rune *);
Rune *runestrstr(const Rune *, const Rune *);

int utftorunestr(Rune *, const char *);

#endif
