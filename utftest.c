/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utf.h"

static void utferror(const char *, int);

int
main(void)
{
	char buf[BUFSIZ], buf2[UTFmax];
	int len, len2;
	size_t i, n;
	Rune r;

	for(i = 0; (n = fread(&buf[i], 1, sizeof buf - i, stdin)); i = n-i) {
		for(n += i, i = 0; (len = charntorune(&r, &buf[i], n-i)); i += len) {
			if(r == Runeerror)
				utferror(&buf[i], len);
			if((len2 = runetochar(buf2, &r))) {
				if(!fwrite(buf2, len2, 1, stdout)) {
					perror("write error");
					exit(EXIT_FAILURE);
				}
			}
			else
				fprintf(stderr, "error converting rune to char: U+%02X\n", r);
		}
		if(i < n)
			memcpy(buf, &buf[i], n-i);
	}
	if(ferror(stdin)) {
		perror("read error");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}

void
utferror(const char *s, int n)
{
	int i = 0;

	fprintf(stderr, "error converting char to rune:");
	for(i = 0; i < n; i++)
		fprintf(stderr, " %02X", (unsigned char)s[i]);
	fputc('\n', stderr);
}
