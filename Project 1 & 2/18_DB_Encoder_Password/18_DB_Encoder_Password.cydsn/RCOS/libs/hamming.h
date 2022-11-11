/** @file       hamming.h
 *  @brief      Hamming code conversion for error detection and correction
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       9.05.2019
 */
#ifndef FILE_HAMMING_H
#define FILE_HAMMING_H

/** @defgroup   HAMMING HAMMING
 *  @ingroup    LIBS
 *  @brief      Functions and tools to encode or decode a data using [8,4] Hamming coding
 *  @{
 */

/*********************************************************************
 *  INCLUDES
 */
#include "libs/libs.h"
#include "libs/bits.h"

/*********************************************************************
 *  EXPORTED TYPEDEFS
 */

/*********************************************************************
 *  EXPORTED MACROS
 */

/// [7,4] Hamming code table, MSB is send first
#define HAMMING_7_4_MSB                                                                                \
    {                                                                                                  \
        0x00, 0x69, 0x2A, 0x43, 0x4C, 0x25, 0x66, 0x0F, 0x70, 0x19, 0x5A, 0x33, 0x3C, 0x55, 0x16, 0x7F \
    }

/// [8,4] Hamming code table, MSB is send first
#define HAMMING_8_4_MSB                                                                                \
    {                                                                                                  \
        0x00, 0xD2, 0x55, 0x87, 0x99, 0x4B, 0xCC, 0x1E, 0xE1, 0x33, 0xB4, 0x66, 0x78, 0xAA, 0x2D, 0xFF \
    }

/// [7,4] Hamming code table, LSB is send first
#define HAMMING_7_4_LSB                                                                                \
    {                                                                                                  \
        0x00, 0x07, 0x19, 0x1E, 0x2A, 0x2D, 0x33, 0x34, 0x4B, 0x4C, 0x52, 0x55, 0x61, 0x66, 0x78, 0x7F \
    }

/// [8,4] Hamming code table, LSB is send first
#define HAMMING_8_4_LSB                                                                                \
    {                                                                                                  \
        0x00, 0x87, 0x99, 0x1E, 0xAA, 0x2D, 0x33, 0xB4, 0x4B, 0xCC, 0xD2, 0x55, 0xE1, 0x66, 0x78, 0xFF \
    }

/** @brief      Calculate max number of data bits that can be used for a number of parity bits(for Hamming code)
 *  @details    m: parity bit count
 *              (2 ^ m - m - 1): data bit count
 *              (2 ^ m - 1): total number of bits
 *  @param      _parityCount    Number of bits used for parity
 */
#define HAMMING_MAX_DATA_BITS(_parityCount) ((1ul << (_parityCount)) - (_parityCount)-1)

/*********************************************************************
 *  INTERFACES
 */

#ifndef FILE_HAMMING_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/*********************************************************************
 *  INTERFACES: CONSTANTS
 */

/*********************************************************************
 *  INTERFACES: VARIABLES
 */

/*********************************************************************
 *  INTERFACES: FUNCTIONS
 */

/** @brief  Compare two values and calculate Hamming distance for 8-bit data
 *  @param  data1   Value of first data
 *  @param  data2   Value of second data
 *  @return Hamming distance
 */
static inline uint8_t hammingDistance(uint8_t data1, uint8_t data2)
{
    return BIT_COUNT(data1 ^ data2);
}

/** @brief  Divide an 8-bit data into two 4-bit nibbles and using Hamming code place it into two bytes
 *  @param  data    Value of the byte
 *  @return values of each nibble placed in significance
 */
INTERFACE uint16_t hammingEncodeByte(uint8_t data);

/** @brief      Process 16-bit data to get a corrected 8-bit data
 *  @param      encoded Value of the encoded data
 *  @retval     byte value of decoded data
 *  @retval     Any value outside 8-bit range is considered erroneous
 */
INTERFACE uint16_t hammingDecodeByte(uint16_t encoded);

/** @brief      Encode according to [8,4] Hamming code
 *  @param      destination Address of destination array
 *  @param      source      Address of source array
 *  @param      count       Number of bytes in source array, and words in destination array
 */
INTERFACE void hammingEncodeArray(uint16_t *destination, const uint8_t *source, uint32_t count);

/** @brief      Decode according to [8,4] Hamming code
 *  @param      destination Address of destination array
 *  @param      source      Address of source array
 *  @param      count       Number of words in source array, and bytes in destination array
 *  @retval     EXIT_SUCCESS
 *  @retval     EXIT_FAILURE At least one byte had irrecoverable error and it is left as 0x00
 */
INTERFACE uint8_t hammingDecodeArray(uint8_t *destination, const uint16_t *source, uint32_t count);

/** @brief  Encode 4-bit value into 8-bit retval
 *  @param  data    Value of 4-bit data
 *  @retval 8-bit [8,4] Hamming encoded data
 */
INTERFACE uint8_t hammingEncode(uint8_t data);

/** @brief  Decode 4-bit value from 8-bit received data
 *  @param  data    Value of received 8-bit data
 *  @retval 4-bit of data
 *  @retval 0xff if there's an error and could not be fixed
 */
INTERFACE uint8_t hammingDecode(uint8_t data);

/** @brief Adding hamming version to platform versioning
 */
INTERFACE void addHammingVersion();

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_HAMMING_H
