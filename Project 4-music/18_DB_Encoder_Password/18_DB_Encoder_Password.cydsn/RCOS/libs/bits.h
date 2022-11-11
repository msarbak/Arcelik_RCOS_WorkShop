/** @file       bits.h
 *  @brief      Bit manipulations
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       22.05.2019
 */
#ifndef FILE_BITS_H
#define FILE_BITS_H

/** @defgroup   BITS BITS
 *  @ingroup    LIBS
 *  @brief      Bit manipulations
 *  @{
 */

/*********************************************************************
 *  INCLUDES
 */
#include <stdint.h>

/*********************************************************************
 *  EXPORTED TYPEDEFS
 */

/*********************************************************************
 *  EXPORTED MACROS
 */
/// @brief  Return mask of bit with index number
#define BIT(_idx) (1ul << (_idx))

/// @brief  Return LSB index value
#define BIT_LSB(_data) find_lsb(_data)

/// @brief  Return MSB index value
#define BIT_MSB(_data) BIN_ENCODE(_data)

/// @brief  Encode MSB of data to its index number
#define BIN_ENCODE(_data) find_msb(_data)

/// @brief  Decode value to its bit representation
#define BIN_DECODE(_value) BIT(_value)

/// @brief  Count the bits of a byte
#define BIT_COUNT(_byte) bit_count(_byte)

/** @brief      Count number of bits set
 *  @details    http://www.graphics.stanford.edu/~seander/bithacks.html#CountBitsSetTable
 */
#define BIT_COUNT_LOOKUP_TABLE     \
    {                              \
        B6(0), B6(1), B6(1), B6(2) \
    }
/// Used by B4
#define B2(n) n, n + 1, n + 1, n + 2
/// Used by B6
#define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
/// Used by BIT_COUNT_LOOKUP_TABLE
#define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)

/// @brief  De Bruijn sequence used for finding MSB in a 32-bit value using bit manipulation and multiplication
#define DE_BRUIJN_MSB                                                                                                \
    {                                                                                                                        \
        0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30, 8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31 \
    }

/// @brief  De Bruijn sequence used for finding LSB in a 32-bit value using bit manipulation and multiplication
#define DE_BRUIJN_LSB                                                                                                        \
    {                                                                                                                        \
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9 \
    }

/*********************************************************************
 *  INTERFACES
 */

#ifndef FILE_BITS_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/*********************************************************************
 *  INTERFACES: CONSTANTS
 */
/// @brief  Lookup table to calculate the number of bits set in a byte
INTERFACE const uint8_t lutBitCount[256];

/// @brief  De Bruijn sequence used for finding MSB in a 32-bit value using bit manipulation and multiplication
INTERFACE const uint8_t lutDeBruijnMsb[32];

/// @brief  De Bruijn sequence used for finding LSB in a 32-bit value using bit manipulation and multiplication
INTERFACE const uint8_t lutDeBruijnLsb[32];

/*********************************************************************
 *  INTERFACES: VARIABLES
 */

/*********************************************************************
 *  INTERFACES: FUNCTIONS
 */
/** @brief  Count the bits that are set in a data
 *  @param  data    
 *  @return Set bits count
 */
static inline uint8_t bit_count(uint8_t data)
{
    return lutBitCount[data];
}

/** @brief      Find the most significant bit in a value.
 *              Also rounded down base 2 logarithm of v
 *  @details    http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogDeBruijn
 *              https://stackoverflow.com/questions/17027878/algorithm-to-find-the-most-significant-bit
 *  @param      v input value
 *  @return     Most significant bit position
 */
INTERFACE uint8_t find_msb(uint32_t v);

/** @brief      Find the least significant bit in a value
 *  @details    http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogDeBruijn
 *              http://www.goldsborough.me/bits/c++/low-level/problems/2015/10/11/23-52-02-bit_manipulation/#finding-the-lsb
 *  @param      v input value
 *  @return     Least significant bit position
 */
INTERFACE uint8_t find_lsb(uint32_t v);

/** @brief Adding bits version to platform versioning
 */
INTERFACE void addBitsVersion();

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_BITS_H
