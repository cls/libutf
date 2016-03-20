/* See LICENSE file for copyright and license details. */
#include "utf.h"

Rune Runeerror = 0xFFFD;

/* lookup table for the number of bytes expected in a sequence */
static const char lookup[128] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 000xxxxx */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 001xxxxx */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 010xxxxx */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 011xxxxx */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 100xxxxx */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 101xxxxx */
	0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, /* 110xxxxx */
	3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 6, 0, /* 111xxxxx */
};

int
charntorune(Rune *p, const char *s, size_t len)
{
	unsigned int i, n, x;
	Rune r;

	if(len == 0) /* can't even look at s[0] */
		return 0;

	r = (unsigned char)s[0];
	n = lookup[r/2];
	i = 1;

	if(n == 1)
		goto done;

	if(n == 0) {
		r = Runeerror;
		goto done;
	}

	if(len == 1) /* reached len limit */
		return 0;

	if((s[1] & 0xC0) != 0x80) { /* not a continuation byte */
		r = Runeerror;
		goto done;
	}

	x = 0xFF >> n;
	r = ((r & x) << 6) | (s[1] & 0x3F); /* 10xxxxxx */
	i = 2;

	if(r <= x) { /* overlong sequence */
		r = Runeerror;
		goto done;
	}

	if(len > n)
		len = n;

	/* add values from continuation bytes */
	for(; i < len; i++) {
		if((s[i] & 0xC0) != 0x80) { /* not a continuation byte */
			r = Runeerror;
			goto done;
		}
		/* add bits from continuation byte to rune value
		 * cannot overflow: 6 byte sequences contain 31 bits */
		r = (r << 6) | (s[i] & 0x3F); /* 10xxxxxx */
	}

	if(i < n) /* must have reached len limit */
		return 0;

done:
	*p = r;
	return i;
}

int
chartorune(Rune *p, const char *s)
{
	return charntorune(p, s, UTFmax);
}

int
fullrune(const char *s, size_t len)
{
	unsigned int i, n, x;
	Rune r;

	if(len == 0) /* can't even look at s[0] */
		return 0;

	r = (unsigned char)s[0];
	n = lookup[r/2];

	if(len >= n) /* must be long enough */
		return 1;

	if(len == 1) /* reached len limit */
		return 0;

	/* check if an error means this rune is full */

	if((s[1] & 0xC0) != 0x80) /* not a continuation byte */
		return 1;

	x = 0xFF >> n;
	r = ((r & x) << 6) | (s[1] & 0x3F); /* 10xxxxxx */

	if(r <= x) /* overlong sequence */
		return 1;

	for(i = 2; i < len; i++)
		if((s[i] & 0xC0) != 0x80) /* not a continuation byte */
			return 1;

	return 0;
}
