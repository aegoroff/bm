/*!
 * \brief   The file contains boyer moore search function implementation
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2010-04-10
            \endverbatim
 * Copyright: (c) Alexander Egorov 2009-2010
*/

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include "eprintf.h"
#include "bmsearch.h"

#ifndef BOOL
#define BOOL int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

enum {
    NHASH = 256,    /* size of state hash table array */
};


typedef struct BMHashItem BMHashItem;

struct BMHashItem {
    wchar_t key;
    int *pShifts;
    BMHashItem *next;   /* next in hash table */
};

BMHashItem *patternHash[NHASH]; /* hash table of symbols */

/*!
* lookup: search for wchar_t; create if requested.
* \return returns pointer if present or created; NULL if not.
*/
BMHashItem *lookup(wchar_t key, int create)
{
    int h = 0;
    BMHashItem *result = NULL;

    h = key % NHASH;
    for (result = patternHash[h]; result != NULL; result = result->next) {
        if (result->key == key) {   /* found it */
            return result;
        }
    }
    if (create) {
        result = (BMHashItem *) emalloc(sizeof(BMHashItem));
        result->key = key;
        result->pShifts = NULL;
        result->next = patternHash[h];
        patternHash[h] = result;
    }
    return result;
}

void clean()
{
    BMHashItem *pItem = NULL;
    BMHashItem *pItemOld = NULL;
    int i = 0;

    for (; i < NHASH; ++i) {
        pItem = patternHash[i];
        if (pItem == NULL) {
            continue;
        }
        do {
            if (pItem->pShifts != NULL) {
                free(pItem->pShifts);
                pItem->pShifts = NULL;
            }
            pItemOld = pItem;
            pItem = pItem->next;
            pItemOld->next = NULL;
            free(pItemOld);
        } while (pItem != NULL);
        patternHash[i] = NULL;
    }
}

int search(const wchar_t * pText, int textLength, int startPos, int patternLength, int *pOtherShifts)
{
    int pos = startPos;
    int i = 0;
    BMHashItem *pItem = NULL;
    int shift = 0;

    if (patternLength == 0) {
        return 0;
    }

    while (pos <= textLength - patternLength) {
        for (i = patternLength - 1; i >= 0; --i) {
            pItem = lookup(pText[pos + i], FALSE);
            if (pItem == NULL) {
                pos += pOtherShifts[i]; // shifting
                break;
            }
            shift = pItem->pShifts[i];
            if (shift != 0) {
                pos += shift;   // shifting
                break;
            }
            if (i == 0) {   // we came to the leftmost pattern character so pattern matches
                return pos; // return matching substring start index
            }
        }
    }
    return -1;  // Nothing found
}

void build(const wchar_t * pattern, int patternLength, int *pOtherShifts)
{
    wchar_t *newPattern = NULL;
    wchar_t *suffix = NULL;
    wchar_t *newSuffix = NULL;
    wchar_t *tmp = NULL;
    BOOL found = FALSE;
    int maxShift = patternLength;
    int ixPattern = 0;
    int ixShift = 0;
    BMHashItem *pItem = NULL;
    BOOL isStartsWith = FALSE;
    int i = 0;
    int ixLast = -1;
    int nSuff = 0;

    clean();

    for (ixPattern = 0; ixPattern < patternLength; ++ixPattern) {
        lookup(pattern[ixPattern], TRUE);
    }
    for (ixPattern = 0; ixPattern < patternLength; ++ixPattern) {   // Loop uniqueue keys
        pItem = lookup(pattern[ixPattern], FALSE);
        if (pItem->pShifts != NULL) {
            continue;
        }
        pItem->pShifts = (int *)emalloc(sizeof(int) * patternLength);
        for (ixShift = 0; ixShift < patternLength; ++ixShift) {
            pItem->pShifts[ixShift] = maxShift;
        }
    }
    // Calculating other shifts (filling each column from PatternLength - 2 to 0 (from right to left)
    for (ixPattern = patternLength - 1; ixPattern >= 0; --ixPattern) {
        if (suffix != NULL) {
            free(suffix);
        }
        nSuff = patternLength - ixPattern;
        suffix = (wchar_t *) emalloc(sizeof(wchar_t) * nSuff);

        memset(suffix, 0, sizeof(wchar_t) * nSuff);
        memcpy(suffix, pattern + ixPattern + 1, sizeof(wchar_t) * nSuff);

        for (i = 0; i < nSuff - 1; ++i) {
            isStartsWith = suffix[i] == pattern[i];
            if (!isStartsWith) {
                break;  // not start with
            }
        }
        if (isStartsWith) {
            maxShift = ixPattern + 1;
        }
        pOtherShifts[ixPattern] = maxShift;

        if (newPattern != NULL) {
            free(newPattern);
        }
        newPattern = _wcsdup(pattern);
        newPattern[patternLength - 1] = 0;

        found = wcsstr(newPattern, suffix) != NULL || nSuff == 0;
        for (i = 0; i < NHASH; ++i) {
            pItem = patternHash[i];
            if (pItem == NULL) {
                continue;
            }
            do {
                if (found) {
                    if (newSuffix != NULL) {
                        free(newSuffix);
                    }
                    // the first is key and the second id trailing zero
                    newSuffix = (wchar_t *) emalloc(sizeof(wchar_t) * (nSuff + sizeof(wchar_t)));
                    memset(newSuffix, 0, sizeof(wchar_t) * (nSuff + sizeof(wchar_t)));
                    newSuffix[0] = pItem->key;
                    memcpy(newSuffix + 1, suffix, nSuff);
                    tmp = wcsstr(newPattern, newSuffix);
                    if (tmp == NULL) {
                        pItem->pShifts[ixPattern] = maxShift;
                    } else {
                        do {    // searching last occurrence
                            ixLast = (int)(tmp - newPattern);
                            tmp = wcsstr(newPattern + ixLast + 1, newSuffix);
                        } while (tmp != NULL);
                        pItem->pShifts[ixPattern] = ixPattern - ixLast;
                    }
                }
                if (pItem->key == pattern[ixPattern]) {
                    pItem->pShifts[ixPattern] = 0;
                }
                pItem = pItem->next;
            } while (pItem != NULL);
        }
    }

    if (newPattern != NULL) {
        free(newPattern);
    }
    if (suffix != NULL) {
        free(suffix);
    }
    if (newSuffix != NULL) {
        free(newSuffix);
    }
}
