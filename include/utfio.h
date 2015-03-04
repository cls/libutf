/* See LICENSE file for copyright and license details. */
#ifndef UTFIO_H
#define UTFIO_H

#ifdef __cplusplus
extern "C" {
#endif

int fgetrune(Rune *, FILE *);
int fgetrunestr(Rune *, int, FILE *);
int fputrune(const Rune *, FILE *);
int fputrunestr(const Rune *, FILE *);

#ifdef __cplusplus
}
#endif

#endif
