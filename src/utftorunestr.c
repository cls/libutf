/* See LICENSE file for copyright and license details. */
#include "utf.h"

int
utftorunestr(Rune *p, const char *s)
{
	int i, n;

	for(i = 0; (n = chartorune(&p[i], s)), p[i] != 0; i++)
		s += n;
	return i;
}
