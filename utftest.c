/* See LICENSE file for copyright and license details. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utf.h"

static void eprintf(const char *, ...);

static const char *argv0;

int
main(void)
{
	char buf[BUFSIZ], rbuf[UTFmax];
	int len, len2;
	size_t i, j, n;
	Rune r;

	for(i = 0; (n = fread(&buf[i], 1, sizeof buf - i, stdin)); i = n-i) {
		for(n += i, i = 0; (len = charntorune(&r, &buf[i], n-i)); i += len) {
			if(r == Runeerror) {
				fprintf(stderr, "%s: error converting char to rune:", argv0);
				for(j = i; j < i+len; j++)
					fprintf(stderr, " %02X", (unsigned char)buf[j]);
				fputc('\n', stderr);
			}
			if((len2 = runetochar(rbuf, &r))) {
				if(!fwrite(rbuf, len2, 1, stdout))
					eprintf("write error:");
			}
			else
				fprintf(stderr, "%s: error converting rune to char: U+%02X\n", argv0, r);
		}
		if(i < n)
			memcpy(buf, &buf[i], n-i);
	}
	if(ferror(stdin))
		eprintf("read error:");

	return EXIT_SUCCESS;
}

void
eprintf(const char *fmt, ...)
{
	va_list ap;

	fprintf(stderr, "%s: ", argv0);

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	if(fmt[0] && fmt[strlen(fmt)-1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	}
	exit(EXIT_FAILURE);
}
