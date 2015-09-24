/* See LICENSE file for copyright and license details. */
#ifndef UTF_H
#define UTF_H

#include <stddef.h>
#include <stdint.h>

#if __STDC_VERSION__ >= 201112L
#include <uchar.h>
#ifdef __STDC_UTF_32__
#define RUNE_C INT32_C
typedef char32_t Rune;
#endif
#endif

#ifndef RUNE_C
#ifdef INT32_C
#define RUNE_C INT32_C
typedef uint_least32_t Rune;
#else
#define RUNE_C(x) x##L
typedef unsigned long Rune;
#endif
#endif

#define UTFmax 6 /* maximum bytes per rune */

#define Runeself 0x80             /* rune and utf are equal (<) */
#define Runemax  RUNE_C(0x10FFFF) /* maximum rune value */

#define runestrcat(s1, s2) runestrncat(s1, s2, SIZE_MAX)
#define runestrcmp(s1, s2) runestrncmp(s1, s2, SIZE_MAX)
#define runestrcpy(s1, s2) runestrncpy(s1, s2, SIZE_MAX)

extern Rune Runeerror; /* decoding error in utf */

#ifdef __cplusplus
extern "C" {
#endif

int runetochar(char *, const Rune *);
int charntorune(Rune *, const char *, size_t);
int chartorune(Rune *, const char *);
int fullrune(const char *, size_t);
int runelen(const Rune);
size_t runenlen(const Rune *, size_t);
size_t utflen(const char *);
size_t utfnlen(const char *, size_t);
char *utfrune(const char *, Rune);
char *utfrrune(const char *, Rune);
char *utfutf(const char *, const char *);

int isalnumrune(Rune);
int isalpharune(Rune);
int isblankrune(Rune);
int iscntrlrune(Rune);
int isdigitrune(Rune);
int isgraphrune(Rune);
int islowerrune(Rune);
int isprintrune(Rune);
int ispunctrune(Rune);
int isspacerune(Rune);
int istitlerune(Rune);
int isupperrune(Rune);
int isxdigitrune(Rune);
int iscombiningrune(Rune);
Rune tolowerrune(Rune);
Rune toupperrune(Rune);

Rune *runestrncat(Rune *, const Rune *, size_t);
int runestrncmp(const Rune *, const Rune *, size_t);
Rune *runestrncpy(Rune *, const Rune *, size_t);
size_t runestrlen(const Rune *);
Rune *runestrchr(const Rune *, Rune);
Rune *runestrrchr(const Rune *, Rune);
Rune *runestrdup(const Rune *);
Rune *runestrstr(const Rune *, const Rune *);

int runewidth(Rune);
int utftorunestr(Rune *, const char *);

#ifdef __cplusplus
}
#endif

#endif
