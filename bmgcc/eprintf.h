/*!
 * \brief   The file contains helper functions interface
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2010-04-10
            \endverbatim
 * Copyright: (c) Alexander Egorov 2009-2010
*/

#ifndef EPRINTF_H_
#define EPRINTF_H_

#define BINARY_THOUSAND 1024

#ifdef __cplusplus
extern "C" {
#endif

extern void eprintf(char *fmt, ...);

extern char *estrdup(char *s);

extern char *progname();

extern void setprogname(char *str);

extern void eprintf(char *fmt, ...);

extern void *emalloc(size_t n);

typedef enum {
    SizeUnitBytes = 0,
    SizeUnitKBytes = 1,
    SizeUnitMBytes = 2,
    SizeUnitGBytes = 3,
    SizeUnitTBytes = 4,
    SizeUnitPBytes = 5,
    SizeUnitEBytes = 6,
    SizeUnitZBytes = 7,
    SizeUnitYBytes = 8,
    SizeUnitBBytes = 9,
    SizeUnitGPBytes = 10
} SizeUnit;

typedef struct FileSize {
	SizeUnit unit;
	// Union of either size in bytes or size it KBytes, MBytes etc.
	union {
		double size;
		unsigned long long sizeInBytes;
	} value;
} FileSize;

extern FileSize NormalizeSize(unsigned long long size);

extern void StartTimer(void);
extern void StopTimer(void);
extern double ReadElapsedTime(void);

#ifdef __cplusplus
}
#endif
#endif // EPRINTF_H_
