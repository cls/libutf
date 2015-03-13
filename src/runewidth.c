/* See LICENSE file for copyright and license details. */
#include "utf.h"

int
runewidth(Rune r)
{
	/* Adapted from Markus Kuhn's wcwidth() */

	if(r == 0)
		return 0;
	else if(iscntrlrune(r))
		return -1;
	else if(iscombiningrune(r))
		return 0;
	else if((r >= 0x1100 && r <= 0x115F)
	     || (r >= 0x2329 && r <= 0x232A)
	     || (r >= 0x2E80 && r <= 0xA4CF && r != 0x303F)
	     || (r >= 0xAC00 && r <= 0xD7A3)
	     || (r >= 0xF900 && r <= 0xFAFF)
	     || (r >= 0xFE10 && r <= 0xFE19)
	     || (r >= 0xFE30 && r <= 0xFE6F)
	     || (r >= 0xFF00 && r <= 0xFF60)
	     || (r >= 0xFFE0 && r <= 0xFFE6)
	     || (r >= RUNE_C(0x20000) && r <= RUNE_C(0x2FFFD))
	     || (r >= RUNE_C(0x30000) && r <= RUNE_C(0x3FFFD)))
		return 2;
	else
		return 1;
}
