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

void eprintf(char *fmt, ...);

char *estrdup(char *s);

char *progname();

void setprogname(char *str);

void eprintf(char *fmt, ...);

void *emalloc(size_t n);

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

typedef struct file_size {
    SizeUnit unit;
    // Union of either size in bytes or size it KBytes, MBytes etc.
    union {
        double size;
        unsigned long long sizeInBytes;
    } value;
} file_size_t;

file_size_t NormalizeSize(unsigned long long size);

void StartTimer(void);

void StopTimer(void);

double ReadElapsedTime(void);

#ifdef __cplusplus
}
#endif
#endif // EPRINTF_H_
