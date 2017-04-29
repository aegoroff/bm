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

#include <io.h>
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

#define FILE_OPEN_MODE L"r, ccs=UNICODE"

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

#define BIG_FILE_FORMAT L"%.2f %s (%llu %s)" // greater or equal 1 Kb
#define SMALL_FILE_FORMAT L"%llu %s" // less then 1 Kb

wchar_t *decode(char *from);

static wchar_t *sizes[] = {
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

void PrintSize(unsigned long long size)
{
    file_size_t normalized = NormalizeSize(size);
	wprintf(normalized.unit ? BIG_FILE_FORMAT : SMALL_FILE_FORMAT,
	        normalized.value, sizes[normalized.unit], size, sizes[SizeUnitBytes]);
}

#ifdef NO_WMAIN_SUPPORT
int main(int argc, char *argv[])
{
#else
int wmain(int argc, wchar_t * argv[])
{
#endif
	FILE *in = NULL;
	size_t sz = 0;
	wchar_t *text = NULL;
	wchar_t *pattern = NULL;
	wchar_t *path = NULL;
	wchar_t *tmp = NULL;
	size_t pattern_length = 0;
    size_t *other_shifts = NULL;
	size_t text_length = 0;
	long long result = 0;
#ifdef _MSC_VER
    int  fh = 0;
#endif

	setprogname("bm.exe");

	if (argc != 3) {
		printf("\nThe usage is: %s <path to file> <pattern>\n", progname());
		return EXIT_FAILURE;
	}
#ifdef NO_WMAIN_SUPPORT
	path = decode(argv[1]);
	pattern = decode(argv[2]);
#else
	path = argv[1];
	pattern = argv[2];
#endif

#ifdef __STDC_WANT_SECURE_LIB__
	_wfopen_s(&in, path, FILE_OPEN_MODE);
#else
	in = _wfopen(path, FILE_OPEN_MODE);
#endif

	if (in == NULL) {
		wprintf(L"\nError opening file: %s Error message: ", path);
		_wperror(L"");
		goto cleanup;
	}

#ifdef _MSC_VER
    _wsopen_s(&fh, path, _O_RDONLY, _SH_DENYNO, _S_IREAD | _S_IWRITE);
    sz = _filelength(fh);
    _close( fh );
#else
    sz = _filelength(in->_file);
#endif

	wprintf(L"\nFile size is: ");
	PrintSize(sz);
	wprintf(L"\n");

	if (sz == 0) {
		goto cleanup;
	}

	if (sz > MAX_SIZE) {
		sz = MAX_SIZE;
	} else {
		sz -= BOM_LENGTH;   // BOM
	}

	text = (wchar_t *) emalloc(sz); // + trailing zero if necessary
	if (text == NULL) {
		goto cleanup;
	}
	memset(text, 0, sz);    // + trailing zero if necessary

	text_length = fread(text, sizeof(wchar_t), sz / sizeof(wchar_t), in);
	if (ferror(in)) {
		wprintf(L"\nError reading file: %s Error message: ", path);
		_wperror(L"");
		goto cleanup;
	}

	pattern_length = wcslen(pattern);

	other_shifts = (size_t *)emalloc(sizeof(size_t) * pattern_length);
	memset(other_shifts, 0, sizeof(size_t) * pattern_length);

	StartTimer();

	build(pattern, pattern_length, other_shifts);
	result = search(text, text_length, 0, pattern_length, other_shifts);

	StopTimer();

#ifdef NO_WMAIN_SUPPORT
	printf(RESULT_PATTERN_BM, argv[0], result, ReadElapsedTime());
#else
	wprintf(RESULT_PATTERN_BML, argv[0], result, ReadElapsedTime());
#endif

	StartTimer();

	tmp = wcsstr(text, pattern);
	result = (int)(tmp - text);

	StopTimer();

#ifdef NO_WMAIN_SUPPORT
	printf(RESULT_PATTERN_WCS, argv[0], result, ReadElapsedTime());
#else
	wprintf(RESULT_PATTERN_WCSL, argv[0], result, ReadElapsedTime());
#endif

cleanup:
	if (in != NULL) {
		fclose(in);
	}
	if (text != NULL) {
		free(text);
	}
	if (other_shifts != NULL && pattern_length > 0) {
		free(other_shifts);
	}
#ifdef NO_WMAIN_SUPPORT
	if (path != NULL) {
		free(path);
	}
	if (pattern != NULL && pattern_length > 0) {
		free(pattern);
	}
#endif
	clean();
	return EXIT_SUCCESS;
}

wchar_t *decode(char *from)
{
#ifdef WIN32
	int lengthWide = 0;
	size_t cchFrom = 0;
	wchar_t *result = NULL;

	cchFrom = strlen(from);

	lengthWide = MultiByteToWideChar(CP_ACP, 0, from, cchFrom, NULL, 0);
	result = (wchar_t *) emalloc(sizeof(wchar_t) * (lengthWide + 1));
	memset(result, 0, sizeof(wchar_t) * (lengthWide + 1));
	MultiByteToWideChar(CP_ACP, 0, from, cchFrom, result, lengthWide);
	return result;
#else
	return NULL;
#endif
}
