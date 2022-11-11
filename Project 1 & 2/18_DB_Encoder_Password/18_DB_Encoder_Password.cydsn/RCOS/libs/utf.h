/** @file       utf.h
 *  @brief      Unicode Transformation Format
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       10.09.2019
 */
#ifndef FILE_UTF_H
#define FILE_UTF_H

#include <stdint.h>

#ifndef FILE_UTF_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/** @defgroup   UNICODE UNICODE
 *  @ingroup    LIBS
 *  @brief      Unicode is a computing industry standard for the consistent encoding, representation, and handling of text expressed in most of the world's writing systems.
 *  @details     https://en.wikipedia.org/wiki/Unicode
 */

/** @defgroup   UTF8 UTF8
 *  @ingroup    UNICODE
 *  @brief      UTF-8 is a variable width character encoding capable of encoding all 1,112,064 valid code points in Unicode using one to four 8-bit bytes.
 *  @details    https://en.wikipedia.org/wiki/UTF-8
 *              Bytes   Bits    Start   Last        Byte 1      Byte 2      Byte 3      Byte 4
 *              1       7       U+0000  U+007F      0xxxxxxx
 *              2       11      U+0080  U+07FF      110xxxxx    10xxxxxx
 *              3       16      U+0800  U+FFFF      1110xxxx    10xxxxxx    10xxxxxx
 *              4       21      U+10000 U+10FFFF    11110xxx    10xxxxxx    10xxxxxx    10xxxxxx
 */

/** @brief  Find out the character size of a code point
 *  @param  codePoint Code point value
 *  @return size of corresponding character encoding
 */
INTERFACE uint8_t utf8Size(uint32_t codePoint);

/** @brief  Place a single UTF-8 encoded character into destination and return the pointer to next character
 *  @param  destination Target character buffer pointer
 *  @param  codePoint   Code point value
 *  @return pointer to next character in destination
 */
INTERFACE uint8_t *utf8Encode(uint8_t *destination, uint32_t codePoint);

/** @brief      Decode a UTF-8 character from textArr and return its value, also place the next character pointer into *nextChar
 *  @param[in]  textArr     Pointer to text array encoded with UTF-8
 *  @param[out] nextChar    Pointer to uint8_t* which will hold the pointer to the next character
 *  @return     Code point value of the encoded character
 */
INTERFACE uint32_t utf8Decode(const uint8_t *textArr, const uint8_t **nextChar);


/** @brief Adding utf version to platform versioning
 */
INTERFACE void addUtfVersion();

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_UTF_H
