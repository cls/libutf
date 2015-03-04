/* See LICENSE file for copyright and license details. */
#include <utf.h>
#include "tap.h"

#define CHECK(S,N,R,RS) do { \
	Rune r; \
	if(is(utflen(S), 1, #S" is 1 rune long")) { \
		is(chartorune(&r, (S)), (N), "rune in "#S" is "#N" bytes long"); \
		is(r, (R), "rune in "#S" is "RS); \
	} \
	else \
		skip(2, #S" is an unexpected number of runes long"); \
} while(0)

int
main(void)
{
	Runeerror = -1;

	plan(50);

	{
		Rune r;
		is(chartorune(&r, ""), 1, "rune in \"\" is 1 byte long");
		is(r, 0x000000L, "rune in \"\" is U+000000 NULL");
	}

	CHECK("\xC2\x80", 2, 0x000080L, "U+000080 <control>");
	CHECK("\xE0\xA0\x80", 3, 0x000800L, "U+000800 SAMARITAN LETTER ALAF");
	CHECK("\xF0\x90\x80\x80", 4, 0x010000L, "U+010000 LINEAR B SYLLABLE B008 A");
	CHECK("\xF8\x88\x80\x80\x80", 5, Runeerror, "in error (invalid)");
	CHECK("\xFC\x84\x80\x80\x80\x80", 6, Runeerror, "in error (invalid)");

	CHECK("\x7F", 1, 0x00007FL, "U+00007F DELETE");
	CHECK("\xDF\xBF", 2, 0x0007FFL, "U+0007FF (unallocated)");
	CHECK("\xEF\xBF\xBF", 3, Runeerror, "in error (invalid)");
	CHECK("\xF7\xBF\xBF\xBF", 4, Runeerror, "in error (invalid)");
	CHECK("\xFB\xBF\xBF\xBF\xBF", 5, Runeerror, "in error (invalid)");
	CHECK("\xFD\xBF\xBF\xBF\xBF\xBF", 6, Runeerror, "in error (invalid)");

	CHECK("\xED\x9F\xBF", 3, 0x00D7FFL, "U+00D7FF (unallocated)");
	CHECK("\xEE\x80\x80", 3, 0x00E000L, "U+00E000 <Private Use, First>");
	CHECK("\xEF\xBF\xBD", 3, 0x00FFFDL, "U+00FFFD REPLACEMENT CHARACTER");
	CHECK("\xF4\x8F\xBF\xBF", 4, 0x10FFFFL, "U+10FFFF (noncharacter)");
	CHECK("\xF4\x90\x80\x80", 4, Runeerror, "in error (invalid)");

	return 0;
}
