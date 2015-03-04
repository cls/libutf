/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include "utf.h"
#include "utfio.h"

int
fgetrune(Rune *p, FILE *fp)
{
	char buf[UTFmax];
	int c, i = 0;

	do {
		if((c = fgetc(fp)) == EOF)
			break;
		buf[i++] = c;
	} while(!fullrune(buf, i));

	if(i > 0 && charntorune(p, buf, i) == 0)
		*p = Runeerror;
	return i;
}

int
fgetrunestr(Rune *s, int len, FILE *fp)
{
	int i, n, m = 0;

	for(i = 0; i < len-1; i++)
		if((n = fgetrune(&s[i], fp)) == 0)
			break;
		else if(s[i] == '\n') {
			i++;
			break;
		}
		else
			m += n;

	s[i] = 0;
	return m;
}
