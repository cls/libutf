/* See LICENSE file for copyright and license details. */
#ifndef RUNETYPE_H
#define RUNETYPE_H

#include <stdlib.h>
#include "runetype.h"

int rune1cmp(const void *, const void *);
int rune2cmp(const void *, const void *);

#ifdef ISRUNE
#define LEN(X) (sizeof (X) / sizeof *(X))

int
ISRUNE(Rune r)
{
	Rune *match;

#ifdef RANGES
	if((match = bsearch(&r, RANGES, LEN(RANGES), sizeof *RANGES, rune2cmp)))
		return 1;
#endif
#ifdef LACES1
	if((match = bsearch(&r, LACES1, LEN(LACES1), sizeof *LACES1, rune2cmp)))
		return !((r - match[0]) % 2);
#endif
#ifdef LACES2
	if((match = bsearch(&r, LACES2, LEN(LACES2), sizeof *LACES2, rune2cmp)))
		return !((r - match[0]) % 2);
#endif
#ifdef SINGLES
	if((match = bsearch(&r, SINGLES, LEN(SINGLES), sizeof *SINGLES, rune1cmp)))
		return 1;
#endif

	return 0;
}

#ifdef TORUNE
Rune
TORUNE(Rune r)
{
	Rune *match;

#ifdef RANGES
	if((match = bsearch(&r, RANGES, LEN(RANGES), sizeof *RANGES, rune2cmp)))
		return match[2] + (r - match[0]);
#endif
#ifdef LACES1
	if((match = bsearch(&r, LACES1, LEN(LACES1), sizeof *LACES1, rune2cmp)))
		return ((r - match[0]) % 2) ? r : r+1;
#endif
#ifdef LACES2
	if((match = bsearch(&r, LACES2, LEN(LACES2), sizeof *LACES2, rune2cmp)))
		return ((r - match[0]) % 2) ? r : r-1;
#endif
#ifdef SINGLES
	if((match = bsearch(&r, SINGLES, LEN(SINGLES), sizeof *SINGLES, rune1cmp)))
		return match[1];
#endif

	return r;
}
#endif /* TORUNE */

#endif /* ISRUNE */

#endif /* RUNETYPE_H */
