/* See LICENSE file for copyright and license details. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utf.h"

#define ARGBEGIN                                                                                \
	{                                                                                           \
		Rune _argr;                                                                             \
                                                                                                \
		if(!argv0)                                                                              \
			argv0 = argv[0];                                                                    \
		for(argc--, argv++; *argv && (*argv)[0] == '-' && (*argv)[1] != '\0'; argc--, argv++) { \
			if((*argv)[1] == '-' && (*argv)[2] == '\0') { /* -- signifies end of flags */       \
				argc--; argv++;                                                                 \
				break;                                                                          \
			}                                                                                   \
			(*argv)++;                                                                          \
			while(**argv != '\0' && (*argv += chartorune(&_argr, *argv)))                       \
				switch(_argr)
#define ARGEND                                                                                  \
		}                                                                                       \
	}
#define ARGC()  _argr

static void eprintf(const char *, ...);
static void usage(void);

static const char *argv0;

int
main(int argc, char *argv[])
{
	int aflag = 0, dflag = 0, lflag = 0, sflag = 0, tflag = 0, uflag = 0, vflag = 0;
	char buf[BUFSIZ], rbuf[UTFmax];
	int len, len2;
	size_t i, j, n;
	Rune r;

	ARGBEGIN {
	case 'a': aflag = 1; break;
	case 'd': dflag = 1; break;
	case 'l': lflag = 1; break;
	case 's': sflag = 1; break;
	case 't': tflag = 1; break;
	case 'u': uflag = 1; break;
	case 'v': vflag = 1; break;
	default:
		r = ARGC();
		rbuf[runetochar(rbuf, &r)] = '\0';
		eprintf("unknown flag -%s\n", rbuf);
		usage();
	} ARGEND;

	if(argc != 0)
		usage();

	for(i = 0; (n = fread(&buf[i], 1, sizeof buf - i, stdin)); i = n-i) {
		for(n += i, i = 0; (len = charntorune(&r, &buf[i], n-i)); i += len) {
			if(r == Runeerror) {
				fprintf(stderr, "%s: error converting char to rune:", argv0);
				for(j = i; j < i+len; j++)
					fprintf(stderr, " %02X", (unsigned char)buf[j]);
				fputc('\n', stderr);
			}
			if(((aflag && isalpharune(r)) || (dflag && isdigitrune(r))
			 || (lflag && islowerrune(r)) || (sflag && isspacerune(r))
			 || (tflag && istitlerune(r)) || (uflag && isupperrune(r))
			 || (!aflag && !dflag && !lflag && !sflag && !tflag && !uflag)) == vflag)
				continue;

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

void
usage(void)
{
	fprintf(stderr, "usage: %s [-adlstu]\n", argv0);
	exit(EXIT_FAILURE);
}
