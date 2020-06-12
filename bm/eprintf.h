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

void eprintf(char* fmt, ...);

char* estrdup(char* s);

char* progname();

void setprogname(char* str);

void* emalloc(size_t n);

typedef enum {
    size_unit_bytes = 0,
    size_unit_kbytes = 1,
    size_unit_mbytes = 2,
    size_unit_gbytes = 3,
    size_unit_tbytes = 4,
    size_unit_pbytes = 5,
    size_unit_ebytes = 6,
    size_unit_zbytes = 7,
    size_unit_ybytes = 8,
    size_unit_bbytes = 9,
    size_unit_gpbytes = 10
} size_unit_t;

typedef struct file_size {
    size_unit_t unit;

    // Union of either size in bytes or size it KBytes, MBytes etc.
    union {
        double size;
        unsigned long long size_in_bytes;
    } value;
} file_size_t;

file_size_t normalize_size(unsigned long long size);

void start_timer(void);

void stop_timer(void);

double read_elapsed_time(void);

#ifdef __cplusplus
}
#endif
#endif // EPRINTF_H_
