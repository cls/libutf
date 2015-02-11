/* See LICENSE file for copyright and license details. */
#include "utf.h"

size_t
utflen(const char *s)
{
	const char *p = s;
	size_t i;
	Rune r;

	for(i = 0; *p != '\0'; i++)
		p += chartorune(&r, p);
	return i;
}
