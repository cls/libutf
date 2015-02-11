/* See LICENSE file for copyright and license details. */
#include <string.h>
#include "utf.h"

char *
utfrune(const char *s, Rune r)
{
	if(r < Runeself) {
		return strchr(s, r);
	}
	else if(r == Runeerror) {
		Rune r0;
		int n;

		for(; *s != '\0'; s += n) {
			n = chartorune(&r0, s);
			if(r == r0)
				return (char *)s;
		}
	}
	else {
		char buf[UTFmax+1];
		int n;

		if(!(n = runetochar(buf, &r)))
			return NULL;
		buf[n] = '\0';
		return strstr(s, buf);
	}
	return NULL;
}
