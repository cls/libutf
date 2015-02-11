/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include "utf.h"
#include "utfio.h"

int
fputrune(const Rune *p, FILE *fp)
{
	char buf[UTFmax];

	return fwrite(buf, runetochar(buf, p), 1, fp);
}
