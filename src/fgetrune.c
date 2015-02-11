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

	if(i == 0)
		return EOF;

	return charntorune(p, buf, i);
}
