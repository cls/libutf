/* See LICENSE file for copyright and license details. */
#include "utf.h"

int
runelen(Rune r)
{
	if(r < 0)
		return 0; /* negative rune */
	else if(r <= 0x7F)
		return 1;
	else if(r <= 0x07FF)
		return 2;
	else if((r >= 0xD800 && r <= 0xDFFF)
	     || (r >= 0xFDD0 && r <= 0xFDEF)
	     || (r & 0xFFFE) == 0xFFFE)
		return 0; /* surrogate or noncharacter */
	else if(r <= 0xFFFF)
		return 3;
	else if(r <= Runemax)
		return 4;
	else
		return 0; /* rune too large */
}

size_t
runenlen(const Rune *p, size_t len)
{
	size_t i, n = 0;

	for(i = 0; i < len; i++)
		n += runelen(p[i]);
	return n;
}
