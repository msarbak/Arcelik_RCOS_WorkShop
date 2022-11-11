/** @file       crc.h
 *  @brief      Cyclic Redundancy Check
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author
 *  @date
 */
#ifndef FILE_CRC_H
#define FILE_CRC_H

#include <stddef.h>
#include <stdint.h>

/** @defgroup   CRC CRC
 *  @ingroup    LIBS
 *  @brief      Cyclic Redundancy Check
 *  @{
 */

/// @brief  Macro used to create a prototype for CRC calculation initalization functions
/// @param  _bit    Bit count used for crc value
/// @param  _type   Type name of calculation method
#define CRC_FUNC_PROT_INIT(_bit, _type) static inline uint##_bit##_t crc##_bit##_type##Init(void)

/// @brief  Macro used to create a prototype for CRC calculation addData functions
/// @param  _bit    Bit count used for crc value
/// @param  _type   Type name of calculation method
#define CRC_FUNC_PROT_ADD(_bit, _type) static inline uint##_bit##_t crc##_bit##_type##AddData(uint##_bit##_t currentCrc, uint8_t data)

/// @brief  Macro used to create a prototype for CRC calculation finishing functions
/// @param  _bit    Bit count used for crc value
/// @param  _type   Type name of calculation method
#define CRC_FUNC_PROT_FINISH(_bit, _type) static inline uint##_bit##_t crc##_bit##_type##Finish(uint##_bit##_t currentCrc)

/** @brief  Macro used to define a function to calculate the CRC value of an array for a specific type
 *  @param  _bit    Size of CRC data in number of bits used
 *  @param  _type   Type of CRC calculation method
 */
#define CRC_FUNC_ARRAY(_bit, _type)                                                                                \
    static inline uint##_bit##_t crc##_bit##_type##Array(uint##_bit##_t currentCrc, uint8_t *array, size_t length) \
    {                                                                                                              \
        /* uint##_bit##_t currentCrc = crc##_bit##_type##Init(); */                                                \
                                                                                                                   \
        for (; length; length--)                                                                                   \
        {                                                                                                          \
            currentCrc = crc##_bit##_type##AddData(currentCrc, *array++);                                          \
        }                                                                                                          \
                                                                                                                   \
        return currentCrc; /* crc##_bit##_type##Finish(currentCrc); */                                             \
    }

/** @} */

#ifndef FILE_CRC_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/** @defgroup   CRC_8_MAXIM CRC_8_MAXIM
 *  @ingroup    CRC
 *  @brief      CRC-8-Maxim method that uses polynomial x^8 + x^5 + x^4 + x^0 (0x31)
 *  @{
 */
INTERFACE const uint8_t crc8MaximTable[256]; ///< CRC-8-Maxim lookup table

/// @fn     crc8MaximInit
/// @brief  CRC-8-Maxim init function
/// @return Initial value of crc
CRC_FUNC_PROT_INIT(8, Maxim)
{
    return 0;
}

/// @fn     crc8MaximAddData
/// @brief  CRC-8-Maxim add data function
/// @param  currentCrc  Current value of crc
/// @param  data        Data value that will be added to crc
/// @return New value of crc
CRC_FUNC_PROT_ADD(8, Maxim)
{
    return crc8MaximTable[currentCrc ^ data];
}

/// @fn     crc8MaximFinish
/// @brief  CRC-8-Maxim finish function
/// @param  currentCrc  Current value of crc
/// @return Final value of crc
CRC_FUNC_PROT_FINISH(8, Maxim)
{
    return currentCrc;
}

/// @fn     crc8MaximArray
/// @brief  CRC-8-Maxim array calculator
/// @param  currentCrc  Current value of crc
/// @param  array       Array of data that will be added to crc
/// @param  length      Length of array
/// @return New value of crc
CRC_FUNC_ARRAY(8, Maxim)

/** @} */

/** @defgroup   CRC_16_CCITT CRC_16_CCITT
 *  @ingroup    CRC
 *  @brief      CRC-16-CCITT method that uses polynomial x^16 + x^12 + x^5 + x^0 (0x1021)
 *  @{
 */
INTERFACE const uint16_t crc16CcittTable[256]; ///< CRC-16-CCITT lookup table

/// @fn     crc16CcittInit
/// @brief  CRC-16-CCITT init function
/// @return Initial value of crc
CRC_FUNC_PROT_INIT(16, Ccitt)
{
    return 0xffff;
}

/// @fn     crc16CcittAddData
/// @brief  CRC-16-CCITT add data function
/// @param  currentCrc  Current value of crc
/// @param  data        Data value that will be added to crc
/// @return New value of crc
CRC_FUNC_PROT_ADD(16, Ccitt)
{
    return (currentCrc << 8) ^ crc16CcittTable[(currentCrc >> 8) ^ data];
}

/// @fn     crc16CcittFinish
/// @brief  CRC-16-CCITT finish function
/// @param  currentCrc  Current value of crc
/// @return Final value of crc
CRC_FUNC_PROT_FINISH(16, Ccitt)
{
    return currentCrc;
}

/// @fn     crc16CcittArray
/// @brief  CRC-16-CCITT array calculator
/// @param  currentCrc  Current value of crc
/// @param  array       Array of data that will be added to crc
/// @param  length      Length of array
/// @return New value of crc
CRC_FUNC_ARRAY(16, Ccitt)

/** @} */

/** @defgroup   CRC_16_CCITT_ZERO CRC_16_CCITT_ZERO
 *  @ingroup    CRC
 *  @brief      CRC-16-CCITT method that uses polynomial x^16 + x^12 + x^5 + x^0 (0x1021) and 0 as initial value
 *  @{
 */

/// @fn     crc16CcittZeroInit
/// @brief  CRC-16-CCITT-ZERO init function
/// @return Initial value of crc
CRC_FUNC_PROT_INIT(16, CcittZero)
{
    return 0;
}

/// @fn     crc16CcittZeroAddData
/// @brief  CRC-16-CCITT-ZERO add data function
/// @param  currentCrc  Current value of crc
/// @param  data        Data value that will be added to crc
/// @return New value of crc
CRC_FUNC_PROT_ADD(16, CcittZero)
{
    return (currentCrc << 8) ^ crc16CcittTable[(currentCrc >> 8) ^ data];
}

/// @fn     crc16CcittZeroFinish
/// @brief  CRC-16-CCITT-ZERO finish function
/// @param  currentCrc  Current value of crc
/// @return Final value of crc
CRC_FUNC_PROT_FINISH(16, CcittZero)
{
    return currentCrc;
}

/// @fn     crc16CcittZeroArray
/// @brief  CRC-16-CCITT-ZERO array calculator
/// @param  currentCrc  Current value of crc
/// @param  array       Array of data that will be added to crc
/// @param  length      Length of array
/// @return New value of crc
CRC_FUNC_ARRAY(16, CcittZero)

/** @} */

/** @brief Adding crc version to platform versioning
 */
INTERFACE void addCrcVersion();

#undef INTERFACE // Should not let this roam free

#endif // FILE_CRC_H
