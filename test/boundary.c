/* See LICENSE file for copyright and license details. */
#include <utf.h>
#include "tap.h"

int
main(void)
{
	Rune r;

	Runeerror = -1;

	plan(50);

	#define _CHECK(S,N,R,RS) \
		if(is(utflen(S), 1, #S" is 1 rune long")) { \
			is(chartorune(&r, (S)), (N), "rune in "#S" is "#N" bytes long"); \
			is(r, (R), "rune in "#S" is "RS); \
		} \
		else \
			skip(2, #S" is an unexpected number of runes long");

	is(chartorune(&r, ""), 1, "rune in \"\" is 1 byte long");
	is(r, 0x000000L, "rune in \"\" is U+000000 NULL");

	_CHECK("\xC2\x80", 2, 0x000080L, "U+000080 <control>")
	_CHECK("\xE0\xA0\x80", 3, 0x000800L, "U+000800 SAMARITAN LETTER ALAF")
	_CHECK("\xF0\x90\x80\x80", 4, 0x010000L, "U+010000 LINEAR B SYLLABLE B008 A")
	_CHECK("\xF8\x88\x80\x80\x80", 5, Runeerror, "in error (invalid)")
	_CHECK("\xFC\x84\x80\x80\x80\x80", 6, Runeerror, "in error (invalid)")

	_CHECK("\x7F", 1, 0x00007FL, "U+00007F DELETE")
	_CHECK("\xDF\xBF", 2, 0x0007FFL, "U+0007FF (unallocated)")
	_CHECK("\xEF\xBF\xBF", 3, Runeerror, "in error (invalid)")
	_CHECK("\xF7\xBF\xBF\xBF", 4, Runeerror, "in error (invalid)")
	_CHECK("\xFB\xBF\xBF\xBF\xBF", 5, Runeerror, "in error (invalid)")
	_CHECK("\xFD\xBF\xBF\xBF\xBF\xBF", 6, Runeerror, "in error (invalid)")

	_CHECK("\xED\x9F\xBF", 3, 0x00D7FFL, "U+00D7FF (unallocated)")
	_CHECK("\xEE\x80\x80", 3, 0x00E000L, "U+00E000 <Private Use, First>")
	_CHECK("\xEF\xBF\xBD", 3, 0x00FFFDL, "U+00FFFD REPLACEMENT CHARACTER")
	_CHECK("\xF4\x8F\xBF\xBF", 4, 0x10FFFFL, "U+10FFFF (noncharacter)")
	_CHECK("\xF4\x90\x80\x80", 4, Runeerror, "in error (invalid)")

	return 0;
}
