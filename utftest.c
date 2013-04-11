/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utf.h"

static void utftest(FILE *, const char *);

int
main(int argc, char *argv[])
{
	int i;
	FILE *fp;

	Runeerror = -1; /* distinguish error from U+FFFD */

	if(argc <= 1)
		utftest(stdin, "stdin");
	else for(i = 1; i < argc; i++) {
		if(!(fp = fopen(argv[i], "r"))) {
			perror(argv[i]);
			exit(1);
		}
		utftest(fp, argv[i]);
		fclose(fp);
	}
	return 0;
}

void
utftest(FILE *fp, const char *s)
{
	char buf[BUFSIZ], rbuf[UTFmax];
	int len, len2;
	size_t i, j, n;
	Rune r;

	for(i = 0; (n = fread(&buf[i], 1, sizeof buf - i, fp)); i = n-i) {
		for(n += i, i = 0; (len = charntorune(&r, &buf[i], n-i)); i += len) {
			if(r == Runeerror) {
				fputs("decoding error:", stderr);
				for(j = i; j < i+len; j++)
					fprintf(stderr, " %02X", (unsigned char)buf[j]);
				fputc('\n', stderr);
				r = 0xFFFD;
			}
			if(!(len2 = runetochar(rbuf, &r))) {
				fprintf(stderr, "encoding error: U+%02X\n", r);
				continue;
			}
			else if(!fwrite(rbuf, len2, 1, stdout)) {
				perror("stdout");
				exit(1);
			}
		}
		if(i < n)
			memcpy(buf, &buf[i], n-i);
	}
	if(ferror(fp)) {
		perror(s);
		exit(1);
	}
}
