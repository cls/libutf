/* See LICENSE file for copyright and license details. */
#include "utf.h"

char *
utfutf(const char *s1, const char *s2)
{
	const char *p1, *p2;
	Rune r0, r1, r2;
	int n1, n2;

	chartorune(&r0, s2);
	for(s1 = utfrune(s1, r0); *s1 != 0; s1 = utfrune(s1+1, r0))
		for(p1 = s1, p2 = s2;; p1 += n1, p2 += n2) {
			n2 = chartorune(&r2, p2);
			if(r2 == 0)
				return (char *)s1;
			n1 = chartorune(&r1, p1);
			if(r1 != r2)
				break;
		}

	return NULL;
}
