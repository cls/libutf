/* See LICENSE file for copyright and license details. */
#include "utf.h"

Rune Runeerror = 0xFFFD;

/* lookup table for the number of bytes expected in a sequence */
static const char lookup[0x100] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0000xxxx */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0001xxxx */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0010xxxx */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0011xxxx */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0100xxxx */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0101xxxx */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0110xxxx */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0111xxxx */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 1000xxxx */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 1001xxxx */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 1010xxxx */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 1011xxxx */
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, /* 1100xxxx */
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, /* 1101xxxx */
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, /* 1110xxxx */
	4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 0, 0, /* 1111xxxx */
};

int
charntorune(Rune *p, const char *s, size_t len)
{
	unsigned int n, i = 1;
	Rune r;

	if(len == 0) /* can't even look at s[0] */
		return 0;

	r = (unsigned char)s[0];
	n = lookup[r];

	if(n == 1)
		goto done;

	if(n == 0) {
		r = Runeerror;
		goto done;
	}

	r &= 0xFF >> n;

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

	/* reject invalid or overlong sequences */
	if(runelen(r) < (int)n) {
		r = Runeerror;
		goto done;
	}

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
	unsigned int n, i = 1;

	if(len == 0) /* can't even look at s[0] */
		return 0;

	n = lookup[(unsigned char)s[0]];

	if(len >= n) /* must be long enough */
		return 1;

	/* check if an error means this rune is full */
	for(; i < len; i++)
		if((s[i] & 0xC0) != 0x80) /* not a continuation byte */
			return 1;

	return 0;
}
