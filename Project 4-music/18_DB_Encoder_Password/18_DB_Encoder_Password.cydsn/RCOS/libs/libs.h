/**
 *  @file       libs.h
 *  @brief      Main includes and defines(mostly tools that does not take up space)
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       1.11.2018
 */
#ifndef FILE_LIBS_H
#define FILE_LIBS_H

#include <stddef.h>
#include <stdint.h>

/** @defgroup   LIBS LIBS
 *  @ingroup    RCoS
 *  @brief      Generic datastructures and other helpful functions and definitions some of which are also used by the core
 *  @{
 */

typedef enum
{
    TRUE  = (1 == 1),
    FALSE = !TRUE
} teBool;

#ifndef ENABLE
#define ENABLE 1
#endif
#ifndef DISABLE
#define DISABLE 0
#endif

/// Return element count of array
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(_arr) (sizeof(_arr) / sizeof((_arr)[0]))
#endif

/// Size of member of a struct
#define SIZE_OF_MEMBER(_type, _member)  (sizeof(((_type *)NULL)->_member))

/// Return maximum of two variables
#ifndef MAX
#define MAX(_x, _y) ((_x) < (_y) ? (_y) : (_x))
#endif
/// Return minimum of two variables
#ifndef MIN
#define MIN(_x, _y) ((_x) < (_y) ? (_x) : (_y))
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS ((uint8_t)0ul)
#endif
#ifndef EXIT_FAILURE
#define EXIT_FAILURE ((uint8_t)-1ul)
#endif

/// Used to remove "unused variable" warning from compiler output
#ifndef UNUSED
#if 0
#define UNUSED(x)                                     \
    do                                                \
    {                                                 \
        if (0)                                        \
        {                                             \
            const void *temp __attribute__((unused)); \
            temp = (void *)x;                         \
        }                                             \
    } while (0)
#elif 1
#define UNUSED(x)  \
    do             \
    {              \
        (void)(x); \
    } while (0)
#endif
#endif

/// Parity bit: number of 1's in data is even or odd
#define EVEN    (0)
#define ODD     (1)

/// SPI clock phase and polarity value of a certain mode
#define SPI_CPHA(_mode) ((_mode)&0x01)
#define SPI_CPOL(_mode) (((_mode)&0x02) ? 1 : 0)

/** @brief      Calculate parity value of a single byte
 *  @details    http://www.graphics.stanford.edu/~seander/bithacks.html#ParityParallel
 *  @param      data   Value of the byte
 *  @retval     EVEN even number of bits are set
 *  @retval     ODD odd number of bits are set
 */
static inline uint8_t parity(uint8_t data)
{
    data ^= data >> 4;
    data &= 0xf;
    return (0x6996 >> data) & 1;
}

/** @brief      Swap the values of two variables
 *  @details    http://www.graphics.stanford.edu/~seander/bithacks.html#SwappingValuesSubAdd
 *  @param      a first variable
 *  @param      b second variable
 */
#define SWAP(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))

/** @defgroup   MACRO MACRO
 *  @brief      Advanced macro definitions
 *  @{
 */

/// @brief  Concatenation of two arguments
#define _CONCAT(x, y) x##y
#define CONCAT(x, y) _CONCAT(x, y)

/// SUB macro for COUNT_VARARGS, do not use otherwise
#define _GET_NTH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, N, ...) N
/// @brief      Argument counter for variadic macros
/// @warning    Counts up to 32
#define COUNT_VARARGS(...)  _GET_NTH_ARG(__VA_ARGS__, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

/// @brief  Used for combaining variable number of arguments into a single one for other macros
#define VARGS(...) __VA_ARGS__

/** @} */

/** @defgroup   GRAY GRAY
 *  @brief      Gray Encoding
 *  @details    https://en.wikipedia.org/wiki/Gray_code
 *  @{
 */

/** @brief  This function converts an unsigned binary number to reflected binary Gray code.
 *  @param  bin Binary value to be encoded
 *  @return Corresponding gray code
 */
static inline uint32_t grayEncode(uint32_t bin)
{
    return bin ^ (bin >> 1);
}

/** @brief      This function converts a reflected binary Gray code number to a binary number.
 *  @details    Each Gray code bit is exclusive-ored with all more significant bits.
 *  @param      gray    Encoded value
 *  @return     Corresponding binary value
 */
static inline uint32_t grayDecode(uint32_t gray)
{
    unsigned int mask = gray >> 1;
    while (mask != 0)
    {
        gray = gray ^ mask;
        mask = mask >> 1;
    }
    return gray;
}

/** @brief      A more efficient version for Gray codes 32 bits or fewer through the use of SWAR (SIMD within a register) techniques.
 *  @details    It implements a parallel prefix XOR function.  The assignment statements can be in any order.
 *              This function can be adapted for longer Gray codes by adding steps.
 *              A 4-bit variant changes a binary number (abcd)2 to (abcd)2 ^ (00ab)2,
 *              then to (abcd)2 ^ (00ab)2 ^ (0abc)2 ^ (000a)2.
 *  @param      gray    Encoded value
 *  @return     Corresponding binary value
 */
static inline uint32_t grayDecode32(uint32_t gray)
{
    gray = gray ^ (gray >> 16);
    gray = gray ^ (gray >> 8);
    gray = gray ^ (gray >> 4);
    gray = gray ^ (gray >> 2);
    gray = gray ^ (gray >> 1);
    return gray;
}

/** @} */

/** @defgroup   BCD BCD
 *  @brief      Binary Coded Decimal
 *  @details    https://en.wikipedia.org/wiki/Binary-coded_decimal
 *  @{
 */

/** @brief  Convert Binary Coded Decimal to Binary for 16-bit
 *  @param  bcd Value in BCD
 *  @return Value in decimal
 */
static inline uint16_t bcdDecode16(uint16_t bcd)
{
    uint32_t retVal = 0, coeff = 1;

    while(bcd)
    {
        retVal += (bcd & 0xf) * coeff;
        coeff *= 10;
        bcd >>= 4;
    }

    return retVal;
}

/** @brief      Convert binary value to Binary Coded Decimal for 16-bit
 *  @warning    Max value can be used is limited by 9999
 *  @details    https://en.wikipedia.org/wiki/Double_dabble
 *  @param      bin Value to be converted
 *  @return     Value in BCD
 */
static inline uint16_t bcdEncode16(uint16_t bin)
{
    uint32_t buffer = bin;

    for(int idx = 0; idx < 16; idx++)
    {
        if(buffer & ((uint32_t)0xc << 16)) buffer += ((uint32_t)0x3 << 16);
        if(buffer & ((uint32_t)0xc << 20)) buffer += ((uint32_t)0x3 << 20);
        if(buffer & ((uint32_t)0xc << 24)) buffer += ((uint32_t)0x3 << 24);
        if(buffer & ((uint32_t)0xc << 28)) buffer += ((uint32_t)0x3 << 28);
        buffer <<= 1;
    }

    return buffer >> 16;
}

/** @brief  Convert Binary Coded Decimal to Binary for 8-bit
 *  @param  bcd Value in BCD
 *  @return Value in decimal
 */
static inline uint8_t bcdDecode8(uint8_t bcd)
{
    return bcd - ((bcd >> 4) * 6);
    // return ((bcd >> 4) * 10) + (bcd & 0x0f);
}

/** @brief      Convert binary value to Binary Coded Decimal for 8-bit
 *  @warning    Max value can be used is limited by 99
 *  @param      bin Value to be converted
 *  @return     Value in BCD
 */
static inline uint8_t bcdEncode8(uint8_t bin)
{
    return ((bin / 10) * 6) + bin;
    // return ((bin / 10) << 4) + (bin % 10);
}

/** @} */

/** @defgroup   SEVSEG SEVSEG
 *  @brief      Seven segment support
 *  @{
 */

/// Set _idx bit if _x is true
#define SEVSEGSUB(_x,_idx)      ((_x)? 1ul << (_idx) : 0ul)
/// Orientation of segments
#define SEVSEG(a,b,c,d,e,f,g)   (SEVSEGSUB(a, 0) | SEVSEGSUB(b, 1) | SEVSEGSUB(c, 2) | SEVSEGSUB(d, 3) | SEVSEGSUB(e, 4) | SEVSEGSUB(f, 5) | SEVSEGSUB(g, 6))

#define SEVSEG_0                SEVSEG(1,1,1,1,1,1,0)   ///< '0'
#define SEVSEG_1                SEVSEG(0,1,1,0,0,0,0)   ///< '1'
#define SEVSEG_2                SEVSEG(1,1,0,1,1,0,1)   ///< '2'
#define SEVSEG_3                SEVSEG(1,1,1,1,0,0,1)   ///< '3'
#define SEVSEG_4                SEVSEG(0,1,1,0,0,1,1)   ///< '4'
#define SEVSEG_4rv              SEVSEG(0,1,0,0,1,1,1)   ///< '~4'   reverse vertical
#define SEVSEG_5                SEVSEG(1,0,1,1,0,1,1)   ///< '5'
#define SEVSEG_6                SEVSEG(1,0,1,1,1,1,1)   ///< '6'
#define SEVSEG_7                SEVSEG(1,1,1,0,0,0,0)   ///< '7'
#define SEVSEG_7rv              SEVSEG(1,0,0,0,1,1,0)   ///< '~7'   reverse vertical
#define SEVSEG_7rh              SEVSEG(1,0,0,0,1,1,0)   ///< '!7'   reverse horizontal
#define SEVSEG_8                SEVSEG(1,1,1,1,1,1,1)   ///< '8'
#define SEVSEG_9                SEVSEG(1,1,1,1,0,1,1)   ///< '9'
#define SEVSEG_9rv              SEVSEG(1,1,0,1,1,1,1)   ///< '~9'   reverse vertical
#define SEVSEG_9rh              SEVSEG(1,1,1,1,1,0,1)   ///< '!9'   reverse horizontal

#define SEVSEG_A                SEVSEG(1,1,1,0,1,1,1)   ///< 'A'
#define SEVSEG_B                SEVSEG(0,0,1,1,1,1,1)   ///< 'b'
#define SEVSEG_C                SEVSEG(1,0,0,1,1,1,0)   ///< 'C'
#define SEVSEG_Crh              SEVSEG(1,1,1,1,0,0,0)   ///< '~C'   reverse horizontal
#define SEVSEG_Cs               SEVSEG(0,0,0,1,1,0,1)   ///< 'c'    small
#define SEVSEG_Csrh             SEVSEG(0,0,1,1,0,0,1)   ///< '~c'   small reverse horizontal
#define SEVSEG_D                SEVSEG(0,1,1,1,1,0,1)   ///< 'd'
#define SEVSEG_E                SEVSEG(1,0,0,1,1,1,1)   ///< 'E'
#define SEVSEG_F                SEVSEG(1,0,0,0,1,1,1)   ///< 'F'
#define SEVSEG_G                SEVSEG(1,0,1,1,1,1,0)   ///< 'G'
#define SEVSEG_Gs               SEVSEG(1,1,1,1,0,1,1)   ///< 'g'    small
#define SEVSEG_H                SEVSEG(0,1,1,0,1,1,1)   ///< 'H'
#define SEVSEG_Hs               SEVSEG(0,0,1,0,1,1,1)   ///< 'h'    small
#define SEVSEG_Hsrh             SEVSEG(0,1,1,0,1,0,1)   ///< '~h'   small reverse horizontal
#define SEVSEG_I                SEVSEG(0,0,0,0,1,1,0)   ///< 'I'
#define SEVSEG_Is               SEVSEG(0,0,0,0,1,0,0)   ///< 'i_'   small left aligned
#define SEVSEG_Isrh             SEVSEG(0,0,1,0,0,0,0)   ///< '_i'   small reverse horizontal
#define SEVSEG_J                SEVSEG(0,1,1,1,0,0,0)   ///< 'J'
#define SEVSEG_K                SEVSEG(0,0,0,0,0,0,0)
#define SEVSEG_L                SEVSEG(0,0,0,1,1,1,0)   ///< 'L'
#define SEVSEG_Lrh              SEVSEG(0,1,1,1,0,0,0)   ///< '!L'   reverse horizontal
#define SEVSEG_M                SEVSEG(0,0,0,0,0,0,0)
#define SEVSEG_n                SEVSEG(0,0,1,0,1,0,1)   ///< 'n'
#define SEVSEG_O                SEVSEG(1,1,1,1,1,1,0)   ///< 'O'
#define SEVSEG_Os               SEVSEG(0,0,1,1,1,0,1)   ///< 'o'    small
#define SEVSEG_P                SEVSEG(1,1,0,0,1,1,1)   ///< 'P'
#define SEVSEG_Q                SEVSEG(1,1,1,0,0,1,1)   ///< 'q'
#define SEVSEG_R                SEVSEG(0,0,0,0,1,0,1)   ///< 'r'
#define SEVSEG_S                SEVSEG(1,0,1,1,0,1,1)   ///< 'S'
#define SEVSEG_T                SEVSEG(0,0,0,1,1,1,1)   ///< 't'
#define SEVSEG_U                SEVSEG(0,1,1,1,1,1,0)   ///< 'U'
#define SEVSEG_Urv              SEVSEG(1,1,1,0,1,1,0)   ///< '!U'   reverse vertical
#define SEVSEG_Us               SEVSEG(0,0,1,1,1,0,0)   ///< 'u'    small
#define SEVSEG_Usrv             SEVSEG(1,1,0,0,0,1,0)   ///< '!u'   small reverse vertical
#define SEVSEG_V                SEVSEG(0,0,0,0,0,0,0)
#define SEVSEG_W                SEVSEG(0,0,0,0,0,0,0)
#define SEVSEG_X                SEVSEG(0,0,0,0,0,0,0)
#define SEVSEG_Y                SEVSEG(0,1,1,1,0,1,1)   ///< 'Y'
#define SEVSEG_Z                SEVSEG(0,0,0,0,0,0,0)

#define SEVSEG_SPACE            SEVSEG(0,0,0,0,0,0,0)   ///< ' '
#define SEVSEG_FULL             SEVSEG(1,1,1,1,1,1,1)   ///< '8'

#define SEVSEG_OVER             SEVSEG(1,0,0,0,0,0,0)   ///< '^'
#define SEVSEG_MINUS            SEVSEG(0,0,0,0,0,0,1)   ///< '-'
#define SEVSEG_UNDER            SEVSEG(0,0,0,1,0,0,0)   ///< '_'
#define SEVSEG_EQU              SEVSEG(0,0,0,1,0,0,1)   ///< '='
#define SEVSEG_EQUrv            SEVSEG(1,0,0,0,0,0,1)   ///< '='    reverse vertical
#define SEVSEG_DLINE_UP         SEVSEG(1,0,0,0,0,0,1)   ///< '='
#define SEVSEG_DLINE_DWN        SEVSEG(0,0,0,1,0,0,1)   ///< '='    reverse vertical
#define SEVSEG_TRIPLE           SEVSEG(1,0,0,1,0,0,1)   ///< '3E'

#define SEVSEG_PLUSl            SEVSEG(0,0,0,0,1,1,1)   ///< '|-'   aligned left
#define SEVSEG_PLUSr            SEVSEG(0,1,1,0,0,0,1)   ///< '-|'   aligned right

/// @brief  Combine 4 seven segment values starting with highest segment
#define SEVSEG_COMBINE(_s3, _s2, _s1, _s0) (((uint32_t)(_s3) << 21) | ((uint32_t)(_s2) << 14) | ((uint32_t)(_s1) << 7) | (_s0))

/// @brief  Hexadecimal value list for a single digit seven segment 
#define SEVSEG_HEX_LIST                                                             \
    SEVSEG_0, SEVSEG_1, SEVSEG_2, SEVSEG_3, SEVSEG_4, SEVSEG_5, SEVSEG_6, SEVSEG_7, \
    SEVSEG_8, SEVSEG_9, SEVSEG_A, SEVSEG_B, SEVSEG_C, SEVSEG_D, SEVSEG_E, SEVSEG_F

/** @defgroup   ANIMATION ANIMATION
 *  @brief      Pattern node definitions of some common animations
 *  @{
 */
/// Seven Segment Rotation animation parts
#define ANIMATION_SEVSEG_TR          SEVSEG(1,1,0,0,0,0,0)   ///< Animation sequence 1 Top Right
#define ANIMATION_SEVSEG_R           SEVSEG(0,1,1,0,0,0,0)   ///< Animation sequence 2 Right
#define ANIMATION_SEVSEG_BR          SEVSEG(0,0,1,1,0,0,0)   ///< Animation sequence 3 Bottom Right
#define ANIMATION_SEVSEG_B           SEVSEG(0,0,0,1,0,0,0)   ///< Animation sequence 4 Bottom
#define ANIMATION_SEVSEG_BL          SEVSEG(0,0,0,1,1,0,0)   ///< Animation sequence 5 Bottom Left
#define ANIMATION_SEVSEG_L           SEVSEG(0,0,0,0,1,1,0)   ///< Animation sequence 6 Left
#define ANIMATION_SEVSEG_TL          SEVSEG(1,0,0,0,0,1,0)   ///< Animation sequence 7 Top Left
#define ANIMATION_SEVSEG_T           SEVSEG(1,0,0,0,0,0,0)   ///< Animation sequence 8 Top

/// @brief  Animation pattern of 1 segment of download cycle
#define ANIMATION_PATTERN_CYCLE_1(_period)                               \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0, 0, ANIMATION_SEVSEG_TR), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0, 0, ANIMATION_SEVSEG_R ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0, 0, ANIMATION_SEVSEG_BR), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0, 0, ANIMATION_SEVSEG_BL), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0, 0, ANIMATION_SEVSEG_L ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0, 0, ANIMATION_SEVSEG_TL), _period)

/// @brief  Animation pattern of 2 segments of download cycle
#define ANIMATION_PATTERN_CYCLE_2(_period)                                                  \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0, 0,                    ANIMATION_SEVSEG_TR), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0, 0,                    ANIMATION_SEVSEG_R ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0, 0,                    ANIMATION_SEVSEG_BR), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0, ANIMATION_SEVSEG_B,   ANIMATION_SEVSEG_B ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0, ANIMATION_SEVSEG_BL,  0                  ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0, ANIMATION_SEVSEG_L,   0                  ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0, ANIMATION_SEVSEG_TL,  0                  ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0, ANIMATION_SEVSEG_T,   ANIMATION_SEVSEG_T ), _period)

/// @brief  Animation pattern of 3 segments of download cycle
#define ANIMATION_PATTERN_CYCLE_3(_period)                                                                  \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0,                   0,                  ANIMATION_SEVSEG_TR), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0,                   0,                  ANIMATION_SEVSEG_R ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0,                   0,                  ANIMATION_SEVSEG_BR), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0,                   ANIMATION_SEVSEG_B, ANIMATION_SEVSEG_B ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, ANIMATION_SEVSEG_B,  ANIMATION_SEVSEG_B, 0                  ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, ANIMATION_SEVSEG_BL, 0,                  0                  ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, ANIMATION_SEVSEG_L,  0,                  0                  ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, ANIMATION_SEVSEG_TL, 0,                  0                  ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, ANIMATION_SEVSEG_T,  ANIMATION_SEVSEG_T, 0                  ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0, 0,                   ANIMATION_SEVSEG_T, ANIMATION_SEVSEG_T ), _period)

/// @brief  Animation pattern of 4 segments of download cycle
#define ANIMATION_PATTERN_CYCLE_4(_period)                                                                                  \
    PATTERN_NODE(SEVSEG_COMBINE(0,                   0,                  0,                  ANIMATION_SEVSEG_TR), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0,                   0,                  0,                  ANIMATION_SEVSEG_R ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0,                   0,                  0,                  ANIMATION_SEVSEG_BR), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0,                   0,                  ANIMATION_SEVSEG_B, ANIMATION_SEVSEG_B ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0,                   ANIMATION_SEVSEG_B, ANIMATION_SEVSEG_B, 0                  ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(ANIMATION_SEVSEG_B,  ANIMATION_SEVSEG_B, 0,                  0                  ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(ANIMATION_SEVSEG_BL, 0,                  0,                  0                  ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(ANIMATION_SEVSEG_L,  0,                  0,                  0                  ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(ANIMATION_SEVSEG_TL, 0,                  0,                  0                  ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(ANIMATION_SEVSEG_T,  ANIMATION_SEVSEG_T, 0,                  0                  ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0,                   ANIMATION_SEVSEG_T, ANIMATION_SEVSEG_T, 0                  ), _period), \
    PATTERN_NODE(SEVSEG_COMBINE(0,                   0,                  ANIMATION_SEVSEG_T, ANIMATION_SEVSEG_T ), _period)
/** @} */

/** @} */

/** @defgroup   LIBS_MORSE MORSE
 *  @brief      Pattern node definitions of morse code characters
 *  @details
 *              dot = 1 unit
 *              dash = 3 units
 *              between symbols = 1 unit
 *              between letters = 3 units (1 after last symbol is included)
 *              between words = 7 units (1 after last symbol, 2 after last letter and 4 as space are included)
 *  @{
 */

#define MORSE_PATTERN_DOT(_unit)    \
    PATTERN_NODE(1, ((_unit) * 1)), \
    PATTERN_NODE(0, ((_unit) * 1))

#define MORSE_PATTERN_DASH(_unit)   \
    PATTERN_NODE(1, ((_unit) * 3)), \
    PATTERN_NODE(0, ((_unit) * 1))

#define MORSE_PATTERN_CHAR_END(_unit) \
    PATTERN_NODE(0, ((_unit) * 2))

#define MORSE_PATTERN_SPACE(_unit) \
    PATTERN_NODE(0, ((_unit) * 4))

#define MORSE_PATTERN_A(_unit) \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_B(_unit) \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_C(_unit) \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_D(_unit) \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_E(_unit) \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_F(_unit) \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_G(_unit) \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_H(_unit) \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_I(_unit) \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_J(_unit) \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_K(_unit) \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_L(_unit) \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_M(_unit) \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_N(_unit) \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_O(_unit) \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_P(_unit) \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_Q(_unit) \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_R(_unit) \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_S(_unit) \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_T(_unit) \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_U(_unit) \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_V(_unit) \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_W(_unit) \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_X(_unit) \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_Y(_unit) \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_Z(_unit) \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_0(_unit) \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_1(_unit) \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_2(_unit) \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_3(_unit) \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_4(_unit) \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_5(_unit) \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_6(_unit) \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_7(_unit) \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_8(_unit) \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

#define MORSE_PATTERN_9(_unit) \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DASH(_unit), \
    MORSE_PATTERN_DOT(_unit),  \
    MORSE_PATTERN_CHAR_END(_unit)

/** @} */

/** @defgroup   BRAILLE BRAILLE
 *  @brief      Braille alphabet for the blind
 *  @details    https://en.wikipedia.org/wiki/Braille
 *              https://www.pharmabraille.com/pharmaceutical-braille/the-braille-alphabet/
 *  @code
 *              ┌─┬─┐
 *              │1│4│
 *              ├─┼─┤
 *              │2│5│
 *              ├─┼─┤
 *              │3│6│
 *              └─┴─┘
 *  @endcode
 *  @{
 */

/// Set _idx bit if _x is true
#define _BRAILLE_DOT(_x,_idx)      ((_x)? 1ul << (_idx) : 0ul)

/// Orientation of Braille dots
#define _BRAILLE_DOTS(d1, d2, d3, d4, d5, d6)      \
    (_BRAILLE_DOT(d1, 0) | _BRAILLE_DOT(d4, 3) | \
     _BRAILLE_DOT(d2, 1) | _BRAILLE_DOT(d5, 4) | \
     _BRAILLE_DOT(d3, 2) | _BRAILLE_DOT(d6, 5))

// Numeric sequence

/// 0 _
/// _ _
/// _ _
#define _BRAILLE_s1  _BRAILLE_DOTS(1,0,0,0,0,0)

/// 0 _
/// 0 _
/// _ _
#define _BRAILLE_s2  _BRAILLE_DOTS(1,2,0,0,0,0)

/// 0 0
/// _ _
/// _ _
#define _BRAILLE_s3  _BRAILLE_DOTS(1,0,0,4,0,0)

/// 0 0
/// _ 0
/// _ _
#define _BRAILLE_s4  _BRAILLE_DOTS(1,0,0,4,5,0)

/// 0 _
/// _ 0
/// _ _
#define _BRAILLE_s5  _BRAILLE_DOTS(1,0,0,0,5,0)

/// 0 0
/// 0 _
/// _ _
#define _BRAILLE_s6  _BRAILLE_DOTS(1,2,0,4,0,0)

/// 0 0
/// 0 0
/// _ _
#define _BRAILLE_s7  _BRAILLE_DOTS(1,2,0,4,5,0)

/// 0 _
/// 0 0
/// _ _
#define _BRAILLE_s8  _BRAILLE_DOTS(1,2,0,0,5,0)

/// _ 0
/// 0 _
/// _ _
#define _BRAILLE_s9  _BRAILLE_DOTS(0,2,0,4,0,0)

/// _ 0
/// 0 0
/// _ _
#define _BRAILLE_s0  _BRAILLE_DOTS(0,2,0,4,5,0)

// Shift right sequences

/// _ 0
/// _ _
/// _ _
#define _BRAILLE_sr1 _BRAILLE_DOTS(0,0,0,4,0,0)

/// _ 0
/// _ 0
/// _ _
#define _BRAILLE_sr2 _BRAILLE_DOTS(0,0,0,4,5,0)

// Decades

/// _ _
/// _ _
/// _ _
#define _BRAILLE_d1  _BRAILLE_DOTS(0,0,0,0,0,0)

/// _ _
/// _ _
/// 0 _
#define _BRAILLE_d2  _BRAILLE_DOTS(0,0,3,0,0,0)

/// _ _
/// _ _
/// 0 0
#define _BRAILLE_d3  _BRAILLE_DOTS(0,0,3,0,0,6) 

/// _ _
/// _ _
/// _ 0
#define _BRAILLE_d4  _BRAILLE_DOTS(0,0,0,0,0,6)

// 5th decade is a shift down operation

/// Get braille of a decade and sequence
#define _BRAILLE_DS(_dec, _seq) (_BRAILLE_d##_dec | _BRAILLE_s##_seq)

/** @defgroup   BRAILLE_1 GRADE_1
 *  @brief      A letter-by-letter transcription used for basic literacy
 *  @{
 */

// Alphabet
#define BRAILLE_A   _BRAILLE_DS(1, 1)
#define BRAILLE_B   _BRAILLE_DS(1, 2)
#define BRAILLE_C   _BRAILLE_DS(1, 3)
#define BRAILLE_D   _BRAILLE_DS(1, 4)
#define BRAILLE_E   _BRAILLE_DS(1, 5)
#define BRAILLE_F   _BRAILLE_DS(1, 6)
#define BRAILLE_G   _BRAILLE_DS(1, 7)
#define BRAILLE_H   _BRAILLE_DS(1, 8)
#define BRAILLE_I   _BRAILLE_DS(1, 9)
#define BRAILLE_J   _BRAILLE_DS(1, 0)
#define BRAILLE_K   _BRAILLE_DS(2, 1)
#define BRAILLE_L   _BRAILLE_DS(2, 2)
#define BRAILLE_M   _BRAILLE_DS(2, 3)
#define BRAILLE_N   _BRAILLE_DS(2, 4)
#define BRAILLE_O   _BRAILLE_DS(2, 5)
#define BRAILLE_P   _BRAILLE_DS(2, 6)
#define BRAILLE_Q   _BRAILLE_DS(2, 7)
#define BRAILLE_R   _BRAILLE_DS(2, 8)
#define BRAILLE_S   _BRAILLE_DS(2, 9)
#define BRAILLE_T   _BRAILLE_DS(2, 0)
#define BRAILLE_U   _BRAILLE_DS(3, 1)
#define BRAILLE_V   _BRAILLE_DS(3, 2)
#define BRAILLE_W   _BRAILLE_DS(4, 0)
#define BRAILLE_X   _BRAILLE_DS(3, 3)
#define BRAILLE_Y   _BRAILLE_DS(3, 4)
#define BRAILLE_Z   _BRAILLE_DS(3, 5)

// Numbers
#define BRAILLE_1   _BRAILLE_s1
#define BRAILLE_2   _BRAILLE_s2
#define BRAILLE_3   _BRAILLE_s3
#define BRAILLE_4   _BRAILLE_s4
#define BRAILLE_5   _BRAILLE_s5
#define BRAILLE_6   _BRAILLE_s6
#define BRAILLE_7   _BRAILLE_s7
#define BRAILLE_8   _BRAILLE_s8
#define BRAILLE_9   _BRAILLE_s9
#define BRAILLE_0   _BRAILLE_s0

// Formatting

/// _ _
/// _ _
/// _ 0
#define BRAILLE_CAPITAL _BRAILLE_DOTS(0,0,0,0,0,1)
/// _ 0
/// _ 0
/// 0 0
#define BRAILLE_NUMBER  _BRAILLE_DOTS(0,0,1,1,1,1)

// Punctuation

/// _ _
/// 0 _
/// _ _
#define BRAILLE_COMMA       _BRAILLE_DOTS(0,1,0,0,0,0)
/// _ _
/// 0 _
/// 0 _
#define BRAILLE_SEMICOLON   _BRAILLE_DOTS(0,1,1,0,0,0)
/// _ _
/// _ _
/// 0 _
#define BRAILLE_APOSTROPHE  _BRAILLE_DOTS(0,0,1,0,0,0)
/// _ _
/// _ _
/// 0 0
#define BRAILLE_COLON       _BRAILLE_DOTS(0,1,0,0,1,0)
/// _ _
/// _ _
/// 0 0
#define BRAILLE_HYPHEN      _BRAILLE_DOTS(0,0,1,0,0,1)
/// _ 0
/// _ _
/// _ 0
#define BRAILLE_DECIMAL_PNT _BRAILLE_DOTS(0,0,0,1,0,1)
/// _ _
/// 0 0
/// _ 0
#define BRAILLE_FULL_STOP   _BRAILLE_DOTS(0,1,0,0,1,1)
/// _ _
/// 0 0
/// _ 0
#define BRAILLE_PERIOD      _BRAILLE_DOTS(0,1,0,0,1,1)
/// _ _
/// 0 0
/// 0 _
#define BRAILLE_EXCLAMATION _BRAILLE_DOTS(0,1,1,0,1,0)
/// _ _
/// 0 _
/// 0 0
#define BRAILLE_QUESTION    _BRAILLE_DOTS(0,1,1,0,0,1)
/// _ _
/// 0 _
/// 0 0
#define BRAILLE_QUOTE_OPEN  _BRAILLE_DOTS(0,1,1,0,0,1)
/// _ _
/// _ 0
/// 0 0
#define BRAILLE_QUOTE_CLOSE _BRAILLE_DOTS(0,0,1,0,1,1)
/// _ _
/// 0 0
/// 0 0
#define BRAILLE_BRACKET     _BRAILLE_DOTS(0,1,1,0,1,1)
/// _ _
/// 0 0
/// 0 0
#define BRAILLE_PARENTHESES _BRAILLE_DOTS(0,1,1,0,1,1)
/// _ 0
/// _ _
/// 0 _
#define BRAILLE_SLASH       _BRAILLE_DOTS(0,0,1,1,0,0)

/** @} */

/** @defgroup   BRAILLE_2 GRADE_2
 *  @brief      An addition of abbreviations and contractions
 *  @{
 */

#define BRAILLE_ST      _BRAILLE_DS(2, r1)
#define BRAILLE_AR      _BRAILLE_DS(2, r2)

#define BRAILLE_AND     _BRAILLE_DS(3, 6)
#define BRAILLE_FOR     _BRAILLE_DS(3, 7)
#define BRAILLE_OF      _BRAILLE_DS(3, 8)
#define BRAILLE_THE     _BRAILLE_DS(3, 9)
#define BRAILLE_WITH    _BRAILLE_DS(3, 0)

#define BRAILLE_CH      _BRAILLE_DS(4, 1)
#define BRAILLE_GH      _BRAILLE_DS(4, 2)
#define BRAILLE_SH      _BRAILLE_DS(4, 3)
#define BRAILLE_TH      _BRAILLE_DS(4, 4)
#define BRAILLE_WH      _BRAILLE_DS(4, 5)
#define BRAILLE_ED      _BRAILLE_DS(4, 6)
#define BRAILLE_ER      _BRAILLE_DS(4, 7)
#define BRAILLE_OU      _BRAILLE_DS(4, 8)
#define BRAILLE_OW      _BRAILLE_DS(4, 9)

// #define BRAILLE_A
#define BRAILLE_BUT         BRAILLE_B
#define BRAILLE_CAN         BRAILLE_C
#define BRAILLE_DO          BRAILLE_D
#define BRAILLE_EVERY       BRAILLE_E
#define BRAILLE_FROM        BRAILLE_F
#define BRAILLE_GO          BRAILLE_G
#define BRAILLE_HAVE        BRAILLE_H
// #define BRAILLE_I
#define BRAILLE_JUST        BRAILLE_J
#define BRAILLE_KNOWLEDGE   BRAILLE_K
#define BRAILLE_LIKE        BRAILLE_L
#define BRAILLE_MORE        BRAILLE_M
#define BRAILLE_NOT         BRAILLE_N
#define BRAILLE_PEOPLE      BRAILLE_P
#define BRAILLE_QUITE       BRAILLE_Q
#define BRAILLE_RATHER      BRAILLE_R
#define BRAILLE_SO          BRAILLE_S
#define BRAILLE_THAT        BRAILLE_T
#define BRAILLE_STILL       BRAILLE_ST
#define BRAILLE_US          BRAILLE_U
#define BRAILLE_VERY        BRAILLE_V
#define BRAILLE_IT          BRAILLE_X
#define BRAILLE_YOU         BRAILLE_Y
#define BRAILLE_AS          BRAILLE_Z
#define BRAILLE_CHILD       BRAILLE_CH
#define BRAILLE_SHALL       BRAILLE_SH
#define BRAILLE_THIS        BRAILLE_TH
#define BRAILLE_WHICH       BRAILLE_WH
#define BRAILLE_OUT         BRAILLE_OU
#define BRAILLE_WILL        BRAILLE_W

/** @} */

/** @defgroup   BRAILLE_3 GRADE_3
 *  @brief      Various non-standardized personal stenography
 *  @{
 */

/** @} */

/** @} end of BRAILLE */

/** @defgroup   MUSIC MUSIC
 *  @brief      Pattern nodes corresponding to musical notations
 *  @{
 */

/** @defgroup   TEMPO TEMPO
 *  @brief      Speed adjustment for music.
 *  @details    Information taken from https://en.wikipedia.org/wiki/Tempo
 *  @{
 */
#define TEMPO_Larghissimo       (24) ///< very, very slow (24 bpm and under)
#define TEMPO_Grave             (35) ///< very slow (25–45 bpm)
#define TEMPO_Largo             (50) ///< broadly (40–60 bpm)
#define TEMPO_Lento             (52) ///< slowly (45–60 bpm)
#define TEMPO_Larghetto         (63) ///< rather broadly (60–66 bpm)
#define TEMPO_Adagio            (71) ///< slowly with great expression[8] (66–76 bpm)
#define TEMPO_Adagietto         (75) ///< slower than andante (72–76 bpm) or slightly faster than adagio (70–80 bpm)
#define TEMPO_Andante           (92) ///< at a walking pace (76–108 bpm)
#define TEMPO_Andantino         (94) ///< slightly faster than andante (although, in some cases, it can be taken to mean slightly slower than andante) (80–108 bpm)
#define TEMPO_MarciaModerato    (84) ///< moderately, in the manner of a march[9][10] (83–85 bpm)
#define TEMPO_AndanteModerato   (102) ///< between andante and moderato (thus the name) (92–112 bpm)
#define TEMPO_Moderato          (114) ///< at a moderate speed (108–120 bpm)
#define TEMPO_Allegretto        (116) ///< by the mid 19th century, moderately fast (112–120 bpm); see paragraph above for earlier usage
#define TEMPO_AllegroModerato   (118) ///< close to, but not quite allegro (116–120 bpm)
#define TEMPO_Allegro           (138) ///< fast, quickly, and bright (120–156 bpm) (molto allegro is slightly faster than allegro, but always in its range)
#define TEMPO_Vivace            (166) ///< lively and fast (156–176 bpm)
#define TEMPO_Vivacissimo       (174) ///< very fast and lively (172–176 bpm)
#define TEMPO_Allegrissimo      (174) ///< very fast (172–176 bpm)
#define TEMPO_AllegroVivace     (174) ///< very fast (172–176 bpm)
#define TEMPO_Presto            (184) ///< very, very fast (168–200 bpm)
#define TEMPO_Prestissimo       (200) ///< even faster than presto (200 bpm and over)

#define NOTE_TIME(_tempo) ((uint32_t)60000 / _tempo)
/** @} */

/** @defgroup   NOTES_SOLFEGE NOTES_SOLFEGE
 *  @ingroup    MUSIC
 *  @brief      Musical Notes solfege (in Hz) NOTE_octave_notePitch(flat/natural/sharp)
 *  @details    Information taken from https://pages.mtu.edu/~suits/notefreqs.html
 *  @{
 */
#define NOTE_4_FAf  (329)
#define NOTE_4_FA   (349)
#define NOTE_4_FAs  (370)
#define NOTE_4_SOLf (370)
#define NOTE_4_SOL  (392)
#define NOTE_4_SOLs (415)
#define NOTE_4_LAf  (415)
#define NOTE_4_LA   (440)
#define NOTE_4_LAs  (466)
#define NOTE_4_SIf  (466)
#define NOTE_4_SI   (493)
#define NOTE_4_SIs  (523)
#define NOTE_5_DOf  (493)
#define NOTE_5_DO   (523)
#define NOTE_5_DOs  (554)
#define NOTE_5_REf  (554)
#define NOTE_5_RE   (587)
#define NOTE_5_REs  (622)
#define NOTE_5_MIf  (622)
#define NOTE_5_MI   (659)
#define NOTE_5_MIs  (698)
#define NOTE_5_FAf  (659)
#define NOTE_5_FA   (698)
#define NOTE_5_FAs  (739)
#define NOTE_5_SOLf (739)
#define NOTE_5_SOL  (783)
#define NOTE_5_SOLs (783)
#define NOTE_5_LAf  (830)
#define NOTE_5_LA   (880)
#define NOTE_5_LAs  (932)
#define NOTE_5_SIf  (932)
#define NOTE_5_SI   (987)
#define NOTE_5_SIs  (1046)
#define NOTE_6_DOf  (987)
#define NOTE_6_DO   (1046)
#define NOTE_6_DOs  (1108)
#define NOTE_6_REf  (1108)
#define NOTE_6_RE   (1174)
#define NOTE_6_REs  (1244)
#define NOTE_6_MIf  (1244)
#define NOTE_6_MI   (1318)
#define NOTE_6_MIs  (1396)
#define NOTE_6_FAf  (1318)
#define NOTE_6_FA   (1396)
#define NOTE_6_FAs  (1479)
#define NOTE_6_SOLf (1479)
#define NOTE_6_SOL  (1567)
#define NOTE_6_SOLs (1661)
#define NOTE_6_LAf  (1661)
#define NOTE_6_LA   (1760)
#define NOTE_6_LAs  (1864)
#define NOTE_6_SIf  (1864)
#define NOTE_6_SI   (1975)
#define NOTE_6_SIs  (2093)
#define NOTE_7_DOf  (1975)
#define NOTE_7_DO   (2093)
#define NOTE_7_DOs  (2217)
#define NOTE_7_REf  (2217)
#define NOTE_7_RE   (2349)
#define NOTE_7_REs  (2489)
#define NOTE_7_MIf  (2489)
#define NOTE_7_MI   (2637)
#define NOTE_7_MIs  (2793)
#define NOTE_7_FAf  (2637)
#define NOTE_7_FA   (2793)
#define NOTE_7_FAs  (2959)
#define NOTE_7_SOLf (2959)
#define NOTE_7_SOL  (3135)
#define NOTE_7_SOLs (3322)
#define NOTE_7_LAf  (3322)
#define NOTE_7_LA   (3520)
#define NOTE_7_LAs  (3729)
#define NOTE_7_SIf  (3729)
#define NOTE_7_SI   (3951)
#define NOTE_7_SIs  (4186)
#define NOTE_8_DOf  (3951)
#define NOTE_8_DO   (4186)
#define NOTE_8_DOs  (4434)
#define NOTE_8_REf  (4434)
#define NOTE_8_RE   (4698)
#define NOTE_8_REs  (4978)
#define NOTE_8_MIf  (4978)
#define NOTE_8_MI   (5274)
#define NOTE_8_MIs  (5587)
#define NOTE_8_FAf  (5274)
#define NOTE_8_FA   (5587)
#define NOTE_8_FAs  (5919)
#define NOTE_8_SOLf (5919)
#define NOTE_8_SOL  (6271)
/** @} */

/** @defgroup   NOTES_LATIN NOTES_LATIN
 *  @brief      Musical Notes latin (in Hz) NOTE_octave_notePitch(flat/natural/sharp)
 *  @details    Information taken from https://pages.mtu.edu/~suits/notefreqs.html
 *  @{
 */
#define NOTE_4_Ff  (329)
#define NOTE_4_F   (349)
#define NOTE_4_Fs  (370)
#define NOTE_4_Gf  (370)
#define NOTE_4_G   (392)
#define NOTE_4_Gs  (415)
#define NOTE_4_Af  (415)
#define NOTE_4_A   (440)
#define NOTE_4_As  (466)
#define NOTE_4_Bf  (466)
#define NOTE_4_B   (493)
#define NOTE_4_Bs  (523)
#define NOTE_5_Cf  (493)
#define NOTE_5_C   (523)
#define NOTE_5_Cs  (554)
#define NOTE_5_Df  (554)
#define NOTE_5_D   (587)
#define NOTE_5_Ds  (622)
#define NOTE_5_Ef  (622)
#define NOTE_5_E   (659)
#define NOTE_5_Es  (698)
#define NOTE_5_Ff  (659)
#define NOTE_5_F   (698)
#define NOTE_5_Fs  (739)
#define NOTE_5_Gf  (739)
#define NOTE_5_G   (783)
#define NOTE_5_Gs  (783)
#define NOTE_5_Af  (830)
#define NOTE_5_A   (880)
#define NOTE_5_As  (932)
#define NOTE_5_Bf  (932)
#define NOTE_5_B   (987)
#define NOTE_5_Bs  (1046)
#define NOTE_6_Cf  (987)
#define NOTE_6_C   (1046)
#define NOTE_6_Cs  (1108)
#define NOTE_6_Df  (1108)
#define NOTE_6_D   (1174)
#define NOTE_6_Ds  (1244)
#define NOTE_6_Ef  (1244)
#define NOTE_6_E   (1318)
#define NOTE_6_Es  (1396)
#define NOTE_6_Ff  (1318)
#define NOTE_6_F   (1396)
#define NOTE_6_Fs  (1479)
#define NOTE_6_Gf  (1479)
#define NOTE_6_G   (1567)
#define NOTE_6_Gs  (1661)
#define NOTE_6_Af  (1661)
#define NOTE_6_A   (1760)
#define NOTE_6_As  (1864)
#define NOTE_6_Bf  (1864)
#define NOTE_6_B   (1975)
#define NOTE_6_Bs  (2093)
#define NOTE_7_Cf  (1975)
#define NOTE_7_C   (2093)
#define NOTE_7_Cs  (2217)
#define NOTE_7_Df  (2217)
#define NOTE_7_D   (2349)
#define NOTE_7_Ds  (2489)
#define NOTE_7_Ef  (2489)
#define NOTE_7_E   (2637)
#define NOTE_7_Es  (2793)
#define NOTE_7_Ff  (2637)
#define NOTE_7_F   (2793)
#define NOTE_7_Fs  (2959)
#define NOTE_7_Gf  (2959)
#define NOTE_7_G   (3135)
#define NOTE_7_Gs  (3322)
#define NOTE_7_Af  (3322)
#define NOTE_7_A   (3520)
#define NOTE_7_As  (3729)
#define NOTE_7_Bf  (3729)
#define NOTE_7_B   (3951)
#define NOTE_7_Bs  (4186)
#define NOTE_8_Cf  (3951)
#define NOTE_8_C   (4186)
#define NOTE_8_Cs  (4434)
#define NOTE_8_Df  (4434)
#define NOTE_8_D   (4698)
#define NOTE_8_Ds  (4978)
#define NOTE_8_Ef  (4978)
#define NOTE_8_E   (5274)
#define NOTE_8_Es  (5587)
#define NOTE_8_Ff  (5274)
#define NOTE_8_F   (5587)
#define NOTE_8_Fs  (5919)
#define NOTE_8_Gf  (5919)
#define NOTE_8_G   (6271)
/** @} */

/// @brief  List of notes of an octave
/// @param  _o Octave number
#define NOTES_OF(_o) NOTE_##_o##_DO, NOTE_##_o##_RE, NOTE_##_o##_MI, NOTE_##_o##_FA, NOTE_##_o##_SOL, NOTE_##_o##_LA, NOTE_##_o##_SI

/// @brief  Create a silent pattern node for rest
/// @param  _length Length of rest in inverse (1: 1/1, 2: 1/2, 4: 1/4, 8: 1/8, etc...)
#define MUSIC_REST(_length) PATTERN_NODE(0, (NOTE_TIME(M_TEMPO) / (_length)))

/** @brief      Create a pattern node with pitch and length
 *  @details    Must define M_TEMPO before using this macro
 *  @code
 *      const tsPatternNode melody[] =
 *      {
 *          #define M_TEMPO     TEMPO_Larghetto
 *          MUSIC_NOTE(6, DO, 4),
 *          MUSIC_NOTE(6, MI, 4),
 *          PATTERN_NODE_END(0)
 *      };
 *  @endcode
 *  @param      _oct        Octave of current note
 *  @param      _notePitch  Contains the note and pitch (Cs, MI, LAf, etc...)
 *  @param      _length     Length of note in inverse (1: 1/1, 2: 1/2, 4: 1/4, 8: 1/8, etc...)
 */
#define MUSIC_NOTE(_oct, _notePitch, _length) _M_NOTE(_oct, _notePitch, M_TEMPO, _length)
#define _M_NOTE(_oct, _note, _tempo, _length) PATTERN_NODE(NOTE_##_oct##_##_note, (NOTE_TIME(_tempo) / (_length)) - (NOTE_TIME(_tempo) / (32))), MUSIC_REST(32)

/** @defgroup   MELODIES MELODIES
 *  @brief      Predefined melodies
 *  @{
 */
/// @brief  Melody for brand BEKO advised with TEMPO_Lento
#define MELODY_TELVE_BEKO \
    MUSIC_REST(4),        \
    MUSIC_REST(4),        \
    MUSIC_NOTE(5, Af, 4), \
    MUSIC_NOTE(5, Bf, 4), \
    /**/                  \
    MUSIC_NOTE(6, C, 8),  \
    MUSIC_REST(8),        \
    MUSIC_NOTE(4, F, 8),  \
    MUSIC_NOTE(4, F, 8),  \
    MUSIC_REST(8),        \
    MUSIC_NOTE(4, F, 8),  \
    MUSIC_NOTE(4, F, 8),  \
    MUSIC_REST(8),        \
    /**/                  \
    MUSIC_REST(4),        \
    MUSIC_REST(4),        \
    MUSIC_NOTE(4, F, 4),  \
    MUSIC_NOTE(5, Ef, 4), \
    /**/                  \
    MUSIC_NOTE(5, C, 4),  \
    MUSIC_REST(4),        \
    MUSIC_REST(2)

/// @brief  Melody for brand ARCELIK advised with TEMPO_MarciaModerato
#define MELODY_TELVE_ARCELIK \
    MUSIC_NOTE(6, E, 4),     \
    MUSIC_NOTE(6, E, 4),     \
    MUSIC_NOTE(6, E, 4),     \
    MUSIC_REST(4),           \
    MUSIC_NOTE(6, E, 4),     \
    MUSIC_NOTE(6, A, 2),     \
    MUSIC_REST(4)
/** @} */

/** @} end of MUSIC */

/** @defgroup   ASCII ASCII
 *  @brief      ASCII table characters
 *  @{
 */
#define ASCII_NUL   (0x00) // null
#define ASCII_SOH   (0x01) // start of heading
#define ASCII_STX   (0x02) // start of text
#define ASCII_ETX   (0x03) // end of text
#define ASCII_EOT   (0x04) // end of transmission
#define ASCII_ENQ   (0x05) // enquiry
#define ASCII_ACK   (0x06) // acknowledge
#define ASCII_BEL   (0x07) // bell
#define ASCII_BS    (0x08) // backspace
#define ASCII_TAB   (0x09) // horizontal tab
#define ASCII_LF    (0x0a) // line feed, new line
#define ASCII_VT    (0x0b) // vertical tab
#define ASCII_FF    (0x0c) // form feed, new page
#define ASCII_CR    (0x0d) // carriage return
#define ASCII_SO    (0x0e) // shift out
#define ASCII_SI    (0x0f) // shift in
#define ASCII_DLE   (0x10) // data link escape
#define ASCII_DC1   (0x11) // device control 1
#define ASCII_DC2   (0x12) // device control 2
#define ASCII_DC3   (0x13) // device control 3
#define ASCII_DC4   (0x14) // device control 4
#define ASCII_NAK   (0x15) // negative acknowledge
#define ASCII_SYN   (0x16) // synchronous idle
#define ASCII_ETB   (0x17) // end of transmission block
#define ASCII_CAN   (0x18) // cancel
#define ASCII_EM    (0x19) // end of medium
#define ASCII_SUB   (0x1a) // substitute
#define ASCII_ESC   (0x1b) // escape
#define ASCII_FS    (0x1c) // file separator
#define ASCII_GS    (0x1d) // group separator
#define ASCII_RS    (0x1e) // record separator
#define ASCII_US    (0x1f) // unit separator
#define ASCII_DEL   (0x7f) // delete
/** @} */

/** @defgroup   TERMINAL TERMINAL
 *  @brief      Special escape sequence codes: (0x1b) + (0x5b) '[' + code
 *  @{
 */
#define TERMINAL_CTRL(_char)                (_char - 'A' + 1) // Use capital letters only!!!
#define TERMINAL_CURSOR_UP(_count)          "\x1b[" #_count "A"
#define TERMINAL_CURSOR_DOWN(_count)        "\x1b[" #_count "B"
#define TERMINAL_CURSOR_RIGHT(_count)       "\x1b[" #_count "C"
#define TERMINAL_CURSOR_LEFT(_count)        "\x1b[" #_count "D"
#define TERMINAL_CURSOR_POS(_row, _column)  "\x1b[" #_row ";" #_column "H"
#define TERMINAL_ERASE_DISPLAY              "\x1b[2J"
#define TERMINAL_SET_FG_COLOR(_no)          "\x1b[3" #_no "m" // 0..9
#define TERMINAL_SET_BG_COLOR(_no)          "\x1b[4" #_no "m" // 0..9
#define TERMINAL_PRINT_SCREEN               "\x1b[i"
#define TERMINAL_PRINT_LINE                 "\x1b[1i"
#define TERMINAL_PRINT_START_LOG            "\x1b[5i"
#define TERMINAL_PRINT_STOP_LOG             "\x1b[4i"
/** @} */

#include "bits.h"
#include "debug.h"
#include "ticket.h"
#include "version.h"

/** @} */

#endif // FILE_LIBS_H
