/*!
 * \brief   The file contains boyer moore search function interface
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2010-04-10
            \endverbatim
 * Copyright: (c) Alexander Egorov 2009-2010
*/

#ifndef BMSEARCH_H_
#define BMSEARCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Searches substring position in a text
 */
extern long search(const wchar_t * pText, int textLength, int startPos, int patternLength, int *pOtherShifts);

/*!
 * Builds substring shift table
 */
extern void build(const wchar_t * pattern, int patternLength, int *pOtherShifts);

/*!
 * Does necessary cleanup
 */
extern void clean();

#ifdef __cplusplus
}
#endif
#endif // BMSEARCH_H_
