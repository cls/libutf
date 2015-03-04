/* See LICENSE file for copyright and license details. */
#include <utf.h>
#include "tap.h"

#define CHECK(S,N,RS) do { \
	Rune r; \
	if(is(utflen(S), 1, #S" is 1 rune long")) { \
		is(chartorune(&r, (S)), (N), "rune in "#S" is "#N" bytes long"); \
		is(r, Runeerror, RS" is in error"); \
	} \
	else \
		skip(2, #S" is an unexpected number of runes long"); \
} while(0)

int
main(void)
{
	Runeerror = -1;

	plan(45);

	CHECK("\xC0\xAF", 2, "2-byte overlong U+002F SLASH");
	CHECK("\xE0\x80\xAF", 3, "3-byte overlong U+002F SLASH");
	CHECK("\xF0\x80\x80\xAF", 4, "4-byte overlong U+002F SLASH");
	CHECK("\xF8\x80\x80\x80\xAF", 5, "5-byte overlong U+002F SLASH");
	CHECK("\xFC\x80\x80\x80\x80\xAF", 6, "6-byte overlong U+002F SLASH");

	CHECK("\xC1\xBF", 2, "2-byte overlong U+0000007F DELETE");
	CHECK("\xE0\x9F\xBF", 3, "3-byte overlong U+000007FF (unallocated)");
	CHECK("\xF0\x8F\xBF\xBF", 4, "4-byte overlong U+0000FFFF (invalid)");
	CHECK("\xF8\x87\xBF\xBF\xBF", 5, "5-byte overlong U+001FFFFF (invalid)");
	CHECK("\xFC\x83\xBF\xBF\xBF\xBF", 6, "6-byte overlong U+03FFFFFF (invalid)");

	CHECK("\xC0\x80", 2, "2-byte overlong U+0000 NULL");
	CHECK("\xE0\x80\x80", 3, "3-byte overlong U+0000 NULL");
	CHECK("\xF0\x80\x80\x80", 4, "4-byte overlong U+0000 NULL");
	CHECK("\xF8\x80\x80\x80\x80", 5, "5-byte overlong U+0000 NULL");
	CHECK("\xFC\x80\x80\x80\x80\x80", 6, "6-byte overlong U+0000 NULL");

	return 0;
}
