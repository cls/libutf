/* See LICENSE file for copyright and license details. */
#include "utf.h"

int
runelen(Rune r)
{
	int i;

	if(r < 0)
		return 0;

	if(r < Runeself)
		return 1;

	for(i = 2; r > 0x07FF; i++)
		r >>= 5;

	return i;
}

size_t
runenlen(const Rune *p, size_t len)
{
	size_t i, k, n = 0;

	for(i = 0; i < len; i++)
		if((k = runelen(p[i])) < UTFmax)
			n += k;

	return n;
}
