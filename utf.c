/* See LICENSE file for copyright and license details. */
#include <string.h>
#include "utf.h"

char *
utfecpy(char *to, char *end, const char *from)
{
	Rune r = Runeerror;
	size_t i, n;

	/* seek through to find final full rune */
	for(i = 0; r != '\0' && (n = charntorune(&r, &from[i], end - &to[i])); i += n)
		;
	memcpy(to, from, i); /* copy over bytes up to this rune */

	if(i > 0 && r != '\0')
		to[i] = '\0'; /* terminate if unterminated */
	return &to[i];
}

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

size_t
utfnlen(const char *s, size_t len)
{
	const char *p = s;
	size_t i;
	Rune r;
	int n;

	for(i = 0; (n = charntorune(&r, p, len-(p-s))) && r != '\0'; i++)
		p += n;
	return i;
}

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

char *
utfrrune(const char *s, Rune r)
{
	const char *p = NULL;
	Rune r0;
	int n;

	if(r < Runeself)
		return strrchr(s, r);

	for(; *s != '\0'; s += n) {
		n = chartorune(&r0, s);
		if(r == r0)
			p = s;
	}
	return (char *)p;
}

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
