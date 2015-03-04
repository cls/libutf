/* See LICENSE file for copyright and license details. */
#include <utf.h>
#include "tap.h"

#define CHECK(S,N,RS) do { \
	if(is(utflen(S), (N), RS" is "#N" runes long")) { \
		Rune r; \
		int i; \
		const char *p = (S); \
		for(i = 0; *p != '\0'; i++) { \
			p += chartorune(&r, p); \
			if(r != Runeerror) \
				break; \
		} \
		is(i, (N), RS" read as in error"); \
	} \
	else \
		skip(1, #S" is an unexpected number of runes long"); \
} while(0)

int
main(void)
{
	Runeerror = -1;

	plan(34);

	CHECK("\xED\xA0\x80", 1, "U+D800 <Non Private Use High Surrogate, First>");
	CHECK("\xED\xAD\xBF", 1, "U+DB7F <Non Private Use High Surrogate, Last>");
	CHECK("\xED\xAE\x80", 1, "U+DB80 <Private Use High Surrogate, First>");
	CHECK("\xED\xAF\xBF", 1, "U+DBFF <Private Use High Surrogate, Last>");
	CHECK("\xED\xB0\x80", 1, "U+DC00 <Low Surrogate, First>");
	CHECK("\xED\xBE\x80", 1, "U+DF80");
	CHECK("\xED\xBF\xBF", 1, "U+DFFF <Low Surrogate, Last>");

	CHECK("\xED\xA0\x80\xED\xB0\x80", 2, "<Non Private Use High Surrogate, First> <Low Surrogate, First>");
	CHECK("\xED\xA0\x80\xED\xBF\xBF", 2, "<Non Private Use High Surrogate, First> <Low Surrogate, Last>");
	CHECK("\xED\xAD\xBF\xED\xB0\x80", 2, "<Non Private Use High Surrogate, Last> <Low Surrogate, First>");
	CHECK("\xED\xAD\xBF\xED\xBF\xBF", 2, "<Non Private Use High Surrogate, Last> <Low Surrogate, Last>");
	CHECK("\xED\xAE\x80\xED\xB0\x80", 2, "<Private Use High Surrogate, First> <Low Surrogate, First>");
	CHECK("\xED\xAE\x80\xED\xBF\xBF", 2, "<Private Use High Surrogate, First> <Low Surrogate, Last>");
	CHECK("\xED\xAF\xBF\xED\xB0\x80", 2, "<Private Use High Surrogate, Last> <Low Surrogate, First>");
	CHECK("\xED\xAF\xBF\xED\xBF\xBF", 2, "<Private Use High Surrogate, Last> <Low Surrogate, Last>");

	CHECK("\xEF\xBF\xBE", 1, "U+FFFE (invalid)");
	CHECK("\xEF\xBF\xBF", 1, "U+FFFF (invalid)");

	return 0;
}
