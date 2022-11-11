/** @file       apos.h
 *  @brief      Analog Position - Convert specific ranges of an input devIo value to enumeration
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       11.01.2019
 */
#ifndef FILE_APOS_H
#define FILE_APOS_H

#include "rcos.h"

/**
 *  @defgroup   APOS APOS
 *  @ingroup    DEV_IO
 *  @brief      Analog Position - Convert specific ranges of an input devIo value to enumeration
 *  @{
 */

#ifndef FILE_APOS_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for APOS devices
INTERFACE const tsDevIoFuncs devIoAposFuncs;

/// @brief  Range definition of a position
typedef struct
{
    uint32_t minimum; ///< Minimum value that is acceptable for this position
    uint32_t maximum; ///< Maximum value that is acceptable for this position
} tsAposRange;

/// @brief  Device specific constants
typedef struct
{
    const tsDevIo *input;     ///< devIo for getting the analog value
    const tsAposRange *table; ///< Table used for position conversion
    uint8_t tableSize;        ///< Size of table
} tsAposConsts;

/** @brief      APOS Range table item definition to be used as arguments for DEV_IO_APOS_CREATE.
 *  @warning    Range table items' enumerations start with 1 and increses in the order of placement.
 *  @param  _n  minimum value of this range
 *  @param  _x  maximum value of this range
 */
#define APOS_RANGE(_n, _x) \
    {                      \
        (_n), (_x)         \
    }

/** @brief      Create a devIo
 *  @warning    Get function returns 0 in case of unmatched input value
 *  @param      _name   Name of devIo object
 *  @param      _devIo  Target ADC device
 *  @param      ...     List of defined ranges using APOS_RANGE
 */
#define DEV_IO_APOS_CREATE(_name, _devIo, ...) \
    const tsAposRange _name##Table[] =         \
        {                                      \
            __VA_ARGS__,                       \
    };                                         \
    const tsAposConsts _name##Consts =         \
        {                                      \
            &_devIo,                           \
            _name##Table,                      \
            ARRAY_SIZE(_name##Table),          \
    };                                         \
    DEV_IO_CREATE(_name, devIoAposFuncs, NULL, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_APOS_H
