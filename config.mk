# See LICENSE file for copyright and license details.

PREFIX = /usr/local

UNICODE = 7.0.0

# If -std flag is set to c11, and UTF-32 is supported, then Rune = char32_t
CFLAGS  = -pedantic -std=c89 -Iinclude -Os -Wall -Wextra
LDFLAGS = -s

CC  = cc
AWK = awk
