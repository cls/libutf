/* See LICENSE file for copyright and license details. */
#include "utf.h"

int
runetochar(char *s, const Rune *p)
{
	int i, n;
	unsigned char x;
	Rune r = *p;

	n = runelen(r);

	if(n == 1) {
		s[0] = r;
		return 1;
	}

	if(n == 0)
		return 0;

	for(i = n; --i > 0; r >>= 6)
		s[i] = 0x80 | (r & 0x3F);

	x = 0xFF >> n;
	s[0] = ~x | r;

	return n;
}
