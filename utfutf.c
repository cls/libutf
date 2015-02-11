/* See LICENSE file for copyright and license details. */
#include "utf.h"

char *
utfutf(const char *s, const char *t)
{
	const char *p, *q;
	Rune r0, r1, r2;
	int n, m;

	for(chartorune(&r0, t); (s = utfrune(s, r0)); s++) {
		for(p = s, q = t; *q && *p; p += n, q += m) {
			n = chartorune(&r1, p);
			m = chartorune(&r2, q);
			if(r1 != r2)
				break;
		}
		if(!*q)
			return (char *)s;
	}
	return NULL;
}
