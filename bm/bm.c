/*!
 * \brief   The file contains main program implementation
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2010-04-10
            \endverbatim
 * Copyright: (c) Alexander Egorov 2009-2010
*/

#include <stdio.h>

#ifdef WIN32

#include <tchar.h>
#include <windows.h>

#endif

#ifdef WIN32

#include <io.h>

#else
#include <sys/io.h>
#include <wchar.h>
#include <stdlib.h>
#include <memory.h>
#endif

#ifdef _MSC_VER
#include <sys/locking.h>
#include <share.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#include "eprintf.h"
#include "bmsearch.h"

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#define MAX_SIZE 200 * 1024 * 1024  // 200 megabytes
#define BOM_LENGTH 2    // BOM length in wchar_t

#ifdef WIN32
#define FILE_OPEN_MODE L"r, ccs=UNICODE"
#else
#define FILE_OPEN_MODE "r,ccs=UTF16LE"
#endif

#define BM "Boyer Moore"
#define BML L"Boyer Moore"
#define WCS "wcsstr"
#define WCSL L"wcsstr"

#define RESULT_PATTERN_HEAD "\n%s: "
#define RESULT_PATTERN_HEADL L"\n%s: "
#define RESULT_PATTERN_TAIL " The result is: %li Execution time: %.4f s\n"
#define RESULT_PATTERN_TAILL L" The result is: %li Execution time: %.4f s\n"

#define RESULT_PATTERN_BM RESULT_PATTERN_HEAD BM RESULT_PATTERN_TAIL
#define RESULT_PATTERN_WCS RESULT_PATTERN_HEAD WCS RESULT_PATTERN_TAIL

#define RESULT_PATTERN_BML RESULT_PATTERN_HEADL BML RESULT_PATTERN_TAILL
#define RESULT_PATTERN_WCSL RESULT_PATTERN_HEADL WCSL RESULT_PATTERN_TAILL

wchar_t* decode(char* from);

#ifdef WIN32
#define BIG_FILE_FORMAT L"%.2f %s (%llu %s)" // greater or equal 1 Kb
#define SMALL_FILE_FORMAT L"%llu %s" // less then 1 Kb
#define PRINTF wprintf
#define FSEEK64 _fseeki64
#define FTELL64 _ftelli64
static wchar_t* sizes[] = {
    L"bytes",
    L"Kb",
    L"Mb",
    L"Gb",
    L"Tb",
    L"Pb",
    L"Eb",
    L"Zb",
    L"Yb",
    L"Bb",
    L"GPb"
};
#else
#define BIG_FILE_FORMAT "%.2f %s (%llu %s)" // greater or equal 1 Kb
#define SMALL_FILE_FORMAT "%llu %s" // less then 1 Kb
#define PRINTF printf
#define FSEEK64 fseeko64
#define FTELL64 ftello64
static char* sizes[] = {
    "bytes",
    "Kb",
    "Mb",
    "Gb",
    "Tb",
    "Pb",
    "Eb",
    "Zb",
    "Yb",
    "Bb",
    "GPb"
};
#endif

void print_size(unsigned long long size) {
    file_size_t normalized = normalize_size(size);
    PRINTF(normalized.unit ? BIG_FILE_FORMAT : SMALL_FILE_FORMAT,
           normalized.unit ? normalized.value.size : normalized.value.size_in_bytes, sizes[normalized.unit], size,
           sizes[size_unit_bytes]);
}

#ifdef NO_WMAIN_SUPPORT

int main(int argc, char* argv[]) {
#else
    int wmain(int argc, wchar_t* argv[]) {
#endif
    FILE* in = NULL;
    size_t sz = 0;
    wchar_t* text = NULL;
    wchar_t* pattern = NULL;
#ifdef WIN32
    wchar_t* path = NULL;
#else
    char* path = NULL;
#endif
    wchar_t* read_result = 0;
    const size_t line_size = 1024 * 1024;

    wchar_t* tmp = NULL;
    size_t pattern_length = 0;
    size_t* other_shifts = NULL;
    size_t text_length = 0;
    long long result = 0;
#ifdef _MSC_VER
    int fh = 0;
#endif

    setprogname("bm.exe");

    if(argc != 3) {
        printf("\nThe usage is: %s <path to file> <pattern>\n", progname());
        return EXIT_FAILURE;
    }
#ifdef NO_WMAIN_SUPPORT
#ifdef WIN32
    path = decode(argv[1]);
    pattern = decode(argv[2]);
#else
    path = argv[1];
    pattern = decode(argv[2]);
#endif
#else
    path = argv[1];
    pattern = argv[2];
#endif

#ifdef WIN32
#ifdef __STDC_WANT_SECURE_LIB__
    _wfopen_s(&in, path, FILE_OPEN_MODE);
#else
    in = _wfopen(path, FILE_OPEN_MODE);
#endif
#else
    in = fopen(path, FILE_OPEN_MODE);
#endif

    if(in == NULL) {
#ifdef WIN32
        wprintf(L"\nError opening file: %s Error message: ", path);
        _wperror(L"");
#else
        printf("\nError opening file: %s Error message: ", path);
        perror("");
#endif
        goto cleanup;
    }

    FSEEK64(in, 0L, SEEK_END);
    sz = FTELL64(in);
    FSEEK64(in, 0L, SEEK_SET);

#ifdef WIN32
    wprintf(L"\nFile size is: ");
    print_size(sz);
    wprintf(L"\n");
#else
    printf("\nFile size is: ");
    print_size(sz);
    printf("\n");
#endif

    if(sz == 0) {
        goto cleanup;
    }

    if(sz > MAX_SIZE) {
        sz = MAX_SIZE;
    } else {
        sz -= BOM_LENGTH; // BOM
    }

    text = (wchar_t*) emalloc(sz); // + trailing zero if necessary
    if(text == NULL) {
        goto cleanup;
    }
    memset(text, 0, sz); // + trailing zero if necessary
    do {
        read_result = fgetws(text + text_length, line_size, in);
        if(read_result != NULL) {
            text_length += wcslen(read_result);
        }
    } while(read_result != NULL && text_length < sz / sizeof(wchar_t));

    if(ferror(in)) {
#ifdef WIN32
        wprintf(L"\nError reading file: %s Error message: ", path);
        _wperror(L"\n");
#else
        printf("\nError reading file: %s Error message: ", path);
        perror("\n");
#endif
        goto cleanup;
    }
    printf("\nRead %llu chars \n", text_length);

    pattern_length = wcslen(pattern);

    other_shifts = (size_t*) emalloc(sizeof(size_t) * pattern_length);
    memset(other_shifts, 0, sizeof(size_t) * pattern_length);

    start_timer();

    build(pattern, pattern_length, other_shifts);
    result = search(text, text_length, 0, pattern_length, other_shifts);

    stop_timer();

#ifdef NO_WMAIN_SUPPORT
    printf(RESULT_PATTERN_BM, argv[0], result, read_elapsed_time());
#else
    wprintf(RESULT_PATTERN_BML, argv[0], result, read_elapsed_time());
#endif

    start_timer();

    tmp = wcsstr(text, pattern);
    result = (int) (tmp - text);

    stop_timer();

#ifdef NO_WMAIN_SUPPORT
    printf(RESULT_PATTERN_WCS, argv[0], result, read_elapsed_time());
#else
    wprintf(RESULT_PATTERN_WCSL, argv[0], result, read_elapsed_time());
#endif

    cleanup:
    if(in != NULL) {
        fclose(in);
    }
    if(text != NULL) {
        free(text);
    }
    if(other_shifts != NULL && pattern_length > 0) {
        free(other_shifts);
    }
#ifdef NO_WMAIN_SUPPORT
    if(path != NULL) {
#ifdef WIN32
        free(path);
#endif
    }
    if(pattern != NULL && pattern_length > 0) {
        free(pattern);
    }
#endif
    clean();
    return EXIT_SUCCESS;
}

wchar_t* decode(char* from) {
    wchar_t* result = NULL;
#ifdef WIN32
    int length_wide = 0;
    size_t count_chars_from = 0;
    count_chars_from = strlen(from);
    length_wide = MultiByteToWideChar(CP_ACP, 0, from, count_chars_from, NULL, 0);
    result = (wchar_t*) emalloc(sizeof(wchar_t) * (length_wide + 1));
    memset(result, 0, sizeof(wchar_t) * (length_wide + 1));
    MultiByteToWideChar(CP_ACP, 0, from, count_chars_from, result, length_wide);
    return result;
#else
    size_t length_wide = mbstowcs(NULL, from, 0);
    result = calloc(length_wide + 1, sizeof(wchar_t));
    mbstowcs(result, from, length_wide + 1);
    return result;
#endif
}
