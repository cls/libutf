/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utf.h"

static void utferror(const char *, int);

int
main(int argc, char *argv[])
{
	char buf[BUFSIZ], buf2[UTFmax], *p;
	int len, len2;
	size_t i, n;
	rune_t r;

	switch(argc) {
	case 1:
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
		break;
	case 2:
		fprintf(stderr, "\"%s\" (%lu,%lu)\n",
		        argv[1], utflen(argv[1]), strlen(argv[1]));
		break;
	case 3:
		if((n = strtol(argv[2], NULL, 0)) >= sizeof buf) {
			fprintf(stderr, "%lu is too large (max %lu)\n", n, sizeof buf - 1);
			exit(EXIT_FAILURE);
		}
		if((p = utfecpy(buf, &buf[n], argv[1])) == buf)
			buf[0] = '\0';
		printf("\"%s\" (%lu,%lu; %lu,%lu) -> \"%s\" (%lu,%lu)\n",
		       argv[1], utflen(argv[1]), strlen(argv[1]), utfnlen(argv[1], n),
		       n, buf, utflen(buf), p-buf);
		break;
	case 4:
	case 5:
		if(utflen(argv[2]) <= 1 && utflen(argv[3]) <= 1) {
			n = chartorune(&r, argv[2]);
			if(r == Runeerror)
				utferror(argv[2], n);
			if((p = utfrune(argv[1], r)))
				printf("utfrune  '%s': \"%s\"\n", argv[2], p);
			else
				printf("utfrune  '%s': null\n", argv[2]);

			n = chartorune(&r, argv[3]);
			if(r == Runeerror)
				utferror(argv[2], n);
			if((p = utfrrune(argv[1], r)))
				printf("utfrrune '%s': \"%s\"\n", argv[3], p);
			else
				printf("utfrrune '%s': null\n", argv[3]);

			if(argc > 4) {
				if((p = utfutf(argv[1], argv[4])))
					printf("utfutf   \"%s\": \"%s\"\n", argv[4], p);
				else
					printf("utfutf   \"%s\": null\n", argv[4]);
			}
			break;
		}
	default:
		fprintf(stderr, "%s [<string> [<len> | <rune> <rune> [<string>]]]\n", argv[0]);
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
		fprintf(stderr, " %02X", (uint8_t)s[i]);
	fputc('\n', stderr);
}
