/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <string.h>
#include "utf.h"

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
				fputs("error converting char to rune:", stderr);
				for(j = i; j < i+len; j++)
					fprintf(stderr, " %02X", (unsigned char)buf[j]);
				fputc('\n', stderr);
			}
			if(!(len2 = runetochar(rbuf, &r))) {
				fprintf(stderr, "error converting rune to char: U+%02X\n", r);
				continue;
			}
			else if(!fwrite(rbuf, len2, 1, stdout)) {
				perror("write error");
				return 1;
			}
		}
		if(i < n)
			memcpy(buf, &buf[i], n-i);
	}
	if(ferror(stdin)) {
		perror("read error");
		return 1;
	}
	return 0;
}
