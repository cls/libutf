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

	for(i = 2; r > 03777; i++) /* too big to fit in this many (i) utf bytes */
		r >>= 5; /* gain 6 bits from continuation byte but lose 1 in leader */

	return i;
}

size_t
runenlen(const Rune *p, size_t len)
{
	size_t i, k = 0;

	for(i = 0; i < len; i++)
		k += runelen(*p++);

	return k;
}
