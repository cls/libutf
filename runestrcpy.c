/* See LICENSE file for copyright and license details. */
#include <stdint.h>
#include "utf.h"

Rune *
runestrcpy(Rune *s1, const Rune *s2)
{
	return runestrncpy(s1, s2, SIZE_MAX);
}

Rune *
runestrncpy(Rune *s1, const Rune *s2, size_t n)
{
	size_t i;

	for(i = 0; i < n && *s2 != 0; s1++, s2++, i++)
		*s1 = *s2;
	return s1;
}
