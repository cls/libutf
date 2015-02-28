/* See LICENSE file for copyright and license details. */
#ifndef RUNETYPE_H
#define RUNETYPE_H

#define nelem(x)  (sizeof (x) / sizeof *(x))

#ifdef __cplusplus
extern "C" {
#endif

int rune1cmp(const void *, const void *);
int rune2cmp(const void *, const void *);

#ifdef __cplusplus
}
#endif

#endif
