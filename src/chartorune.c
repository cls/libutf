/* See LICENSE file for copyright and license details. */
#include "utf.h"

Rune Runeerror = 0xFFFD;

int
charntorune(Rune *p, const char *s, size_t len)
{
	unsigned int n, i = 1;
	Rune r;

	if(len == 0) /* can't even look at s[0] */
		return 0;

	if((s[0] & 0x80) == 0x00) {      /* 0xxxxxxx */
		r = s[0];
		goto done;
	}
	else if((s[0] & 0xE0) == 0xC0) { /* 110xxxxx */
		r = s[0] & 0x1F;
		n = 2;
	}
	else if((s[0] & 0xF0) == 0xE0) { /* 1110xxxx */
		r = s[0] & 0x0F;
		n = 3;
	}
	else if((s[0] & 0xF8) == 0xF0) { /* 11110xxx */
		r = s[0] & 0x07;
		n = 4;
	}
	else if((s[0] & 0xFC) == 0xF8) { /* 111110xx */
		r = s[0] & 0x03;
		n = 5;
	}
	else if((s[0] & 0xFE) == 0xFC) { /* 1111110x */
		r = s[0] & 0x01;
		n = 6;
	}
	else { /* invalid leading byte */
		r = Runeerror;
		goto done;
	}

	if(len > n)
		len = n;

	/* add values from continuation bytes */
	for(i = 1; i < len; i++)
		if((s[i] & 0xC0) == 0x80) {
			/* add bits from continuation byte to rune value
			 * cannot overflow: 6 byte sequences contain 31 bits */
			r = (r << 6) | (s[i] & 0x3F); /* 10xxxxxx */
		}
		else { /* expected continuation */
			r = Runeerror;
			goto done;
		}

	if(i < n) /* must have reached len limit */
		return 0;

	/* reject invalid or overlong sequences */
	if(runelen(r) < (int)n) {
		r = Runeerror;
		goto done;
	}

done:
	if(p)
		*p = r;
	return i;
}
