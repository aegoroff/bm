﻿/*!
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
#include <memory.h>
#include <string.h>
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
    NHASH = 256, /* size of state hash table array */
};

typedef struct bm_hash_item {
    wchar_t key;
    size_t* shifts;
    struct bm_hash_item* next; /* next in hash table */
} bm_hash_item_t;

static bm_hash_item_t* pattern_hash[NHASH]; /* hash table of symbols */

/*!
* lookup: search for wchar_t; create if requested.
* \return returns pointer if present or created; NULL if not.
*/
static bm_hash_item_t* lookup(wchar_t key, int create) {
    int h = 0;
    bm_hash_item_t* result = NULL;

    h = key % NHASH;
    for(result = pattern_hash[h]; result != NULL; result = result->next) {
        if(result->key == key) { /* found it */
            return result;
        }
    }
    if(create) {
        result = (bm_hash_item_t *)malloc(sizeof(bm_hash_item_t));
        result->key = key;
        result->shifts = NULL;
        result->next = pattern_hash[h];
        pattern_hash[h] = result;
    }
    return result;
}

void clean() {
    bm_hash_item_t* item = NULL;
    bm_hash_item_t* old_item = NULL;
    int i = 0;

    for(; i < NHASH; ++i) {
        item = pattern_hash[i];
        if(item == NULL) {
            continue;
        }
        do {
            if(item->shifts != NULL) {
                free(item->shifts);
                item->shifts = NULL;
            }
            old_item = item;
            item = item->next;
            old_item->next = NULL;
            free(old_item);
        } while(item != NULL);
        pattern_hash[i] = NULL;
    }
}

long long search(const wchar_t* text, size_t text_length, size_t start_pos, size_t pattern_length,
                 size_t* other_shifts) {
    size_t pos = start_pos;
    size_t i = 0;
    bm_hash_item_t* item = NULL;
    size_t shift = 0;

    if(pattern_length == 0) {
        return 0;
    }

    while(pos <= text_length - pattern_length) {
        for(i = pattern_length - 1; i != -1; --i) {
            item = lookup(text[pos + i], FALSE);
            if(item == NULL) {
                pos += other_shifts[i]; // shifting
                break;
            }
            shift = item->shifts[i];
            if(shift != 0) {
                pos += shift; // shifting
                break;
            }
            if(i == 0) { // we came to the leftmost pattern character so pattern matches
                return pos; // return matching substring start index
            }
        }
    }
    return -1; // Nothing found
}

void build(const wchar_t* pattern, size_t pattern_length, size_t* other_shifts) {
    wchar_t* new_pattern = NULL;
    wchar_t* suffix = NULL;
    wchar_t* new_suffix = NULL;
    wchar_t* tmp = NULL;
    size_t max_shift = pattern_length;
    size_t ix_pattern = 0;
    size_t ix_shift = 0;
    bm_hash_item_t* item = NULL;
    BOOL is_starts_with = FALSE;
    size_t i = 0;
    size_t ix_last;

    clean();

    for(ix_pattern = 0; ix_pattern < pattern_length; ++ix_pattern) {
        lookup(pattern[ix_pattern], TRUE);
    }
    for(ix_pattern = 0; ix_pattern < pattern_length; ++ix_pattern) { // Loop uniqueue keys
        item = lookup(pattern[ix_pattern], FALSE);
        if(item->shifts != NULL) {
            continue;
        }
        item->shifts = (size_t *)malloc(sizeof(size_t) * pattern_length);
        for(ix_shift = 0; ix_shift < pattern_length; ++ix_shift) {
            item->shifts[ix_shift] = max_shift;
        }
    }
    // Calculating other shifts (filling each column from PatternLength - 2 to 0 (from right to left)
    for(ix_pattern = pattern_length - 1; ix_pattern != -1; --ix_pattern) {
        BOOL found;
        size_t num_suff = 0;
        if(suffix != NULL) {
            free(suffix);
        }
        num_suff = pattern_length - ix_pattern;
        suffix = (wchar_t *)malloc(sizeof(wchar_t) * num_suff);

        memcpy(suffix, pattern + ix_pattern + 1, sizeof(wchar_t) * num_suff);

        for(i = 0; i < num_suff - 1; ++i) {
            is_starts_with = suffix[i] == pattern[i];
            if(!is_starts_with) {
                break; // not start with
            }
        }
        if(is_starts_with) {
            max_shift = ix_pattern + 1;
        }
        other_shifts[ix_pattern] = max_shift;

        if(new_pattern != NULL) {
            free(new_pattern);
        }
        new_pattern = _wcsdup(pattern);
        new_pattern[pattern_length - 1] = 0;

        found = wcsstr(new_pattern, suffix) != NULL || num_suff == 0;
        for(i = 0; i < NHASH; ++i) {
            item = pattern_hash[i];
            if(item == NULL) {
                continue;
            }
            do {
                if(found) {
                    size_t new_suffix_len = num_suff + 1;
                    if(new_suffix != NULL) {
                        free(new_suffix);
                    }
                    // the first is key and the second id trailing zero
                    new_suffix = (wchar_t *)malloc(sizeof(wchar_t) * new_suffix_len);
                    memset(new_suffix, 0, sizeof(wchar_t) * new_suffix_len);
                    new_suffix[0] = item->key;
                    memcpy(new_suffix + 1, suffix, num_suff);
                    tmp = wcsstr(new_pattern, new_suffix);
                    if(tmp == NULL) {
                        item->shifts[ix_pattern] = max_shift;
                    } else {
                        do { // searching last occurrence
                            ix_last = (size_t)(tmp - new_pattern);
                            tmp = wcsstr(new_pattern + ix_last + 1, new_suffix);
                        } while(tmp != NULL);
                        item->shifts[ix_pattern] = ix_pattern - ix_last;
                    }
                }
                if(item->key == pattern[ix_pattern]) {
                    item->shifts[ix_pattern] = 0;
                }
                item = item->next;
            } while(item != NULL);
        }
    }

    if(new_pattern != NULL) {
        free(new_pattern);
    }
    if(suffix != NULL) {
        free(suffix);
    }
    if(new_suffix != NULL) {
        free(new_suffix);
    }
}
