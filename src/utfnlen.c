/* See LICENSE file for copyright and license details. */
#include "utf.h"

size_t
utfnlen(const char *s, size_t len)
{
	const char *p = s;
	size_t i;
	Rune r;
	int n;

	for(i = 0; (n = charntorune(&r, p, len-(p-s))) && r != 0; i++)
		p += n;
	return i;
}
