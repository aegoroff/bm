/*!
 * \brief   The file contains helper functions implementation
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2010-04-10
            \endverbatim
 * Copyright: (c) Alexander Egorov 2009-2010
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <malloc.h>
#include <time.h>
#ifdef WIN32
#include <windows.h>
#endif
#include "eprintf.h"

static char *name = NULL;

static double span = 0.0;

#ifdef WIN32
static LARGE_INTEGER freq = { 0 };
static LARGE_INTEGER time1 = { 0 };
static LARGE_INTEGER time2 = { 0 };

#else
static clock_t c0 = 0;
static clock_t c1 = 0;
#endif

char *estrdup(char *s)
{
    char *t = NULL;
    size_t len = 0;

    if (s == NULL) {
        return t;
    }

    len = strlen(s) + 1;
    t = (char *)malloc(len);
    if (t == NULL) {
        eprintf("estrdup(\"%.20s\") failed:", s);
        return t;
    }
#ifdef __STDC_WANT_SECURE_LIB__
    strcpy_s(t, len, s);
#else
    strcpy(t, s);
#endif
    return t;
}

char *progname()
{
    return name;
}

void setprogname(char *str)
{
    name = estrdup(str);
}

void eprintf(char *fmt, ...)
{
    va_list args = NULL;
    fflush(stdout);
    if (progname() != NULL) {
#ifdef __STDC_WANT_SECURE_LIB__
        fprintf_s(stderr, "%s: ", progname());
#else
        fprintf(stderr, "%s: ", progname());
#endif
    }
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    if (fmt[0] != '\0' && fmt[strlen(fmt - 1)] == ':') {
#ifdef __STDC_WANT_SECURE_LIB__
        fprintf_s(stderr, " %s", strerror(errno));
#else
        fprintf(stderr, " %s", strerror(errno));
#endif
    }
    fprintf(stderr, "\n");
    exit(2);
}

void *emalloc(size_t n)
{
    void *p = NULL;
    p = malloc(n);
    if (p == NULL) {
        eprintf("malloc of %u bytes failed:", n);
    }
    return p;
}

FileSize NormalizeSize(unsigned long long size)
{
    FileSize result = { 0 };
    result.unit = size == 0 ? SizeUnitBytes : floor(log(size) / log(BINARY_THOUSAND));
    if (result.unit == SizeUnitBytes) {
        result.value.sizeInBytes = size;
    } else {
        result.value.size = size / pow(BINARY_THOUSAND, floor(result.unit));
    }
    return result;
}

void StartTimer(void)
{
#ifdef WIN32
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&time1);
#else
    c0 = clock();
#endif
}

void StopTimer(void)
{
#ifdef WIN32
    QueryPerformanceCounter(&time2);
    span = (double)(time2.QuadPart - time1.QuadPart) / (double)freq.QuadPart;
#else
    c1 = clock();
    span = (double)(c1 - c0) / (double)CLOCKS_PER_SEC;
#endif
}

double ReadElapsedTime(void)
{
    return span;
}