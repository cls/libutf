/* See LICENSE file for copyright and license details. */
#include "utf.h"

int
isxdigitrune(Rune r)
{
	return (r >= '0' && (r - '0') < 10) || (r >= 'a' && (r - 'a') < 6);
}
