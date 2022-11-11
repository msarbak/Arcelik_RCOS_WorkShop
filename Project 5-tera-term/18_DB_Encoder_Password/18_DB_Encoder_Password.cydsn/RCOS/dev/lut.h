/** @file       lut.h
 *  @brief      Header file of LUT device
 *  @copyright  (c) 2020-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       20.01.2020
 */
#ifndef FILE_LUT_H
#define FILE_LUT_H

/// Includes
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_LUT_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   LUT LUT
 *  @ingroup    DEV_IO
 *  @brief      Look-Up Table to map from a input to target output.
 *  @{
 */

/// @brief  Functions for LUT devices
INTERFACE const tsDevIoFuncs devIoLutFuncs;

/// @brief  Device specific parameters
typedef struct
{
    uint32_t lastIdx; ///< Last value put
} tsLutParams;

/// @brief  Device specific constants
typedef struct
{
    const tsDevIo *target; ///< Target device that will be driven
    const uint32_t *lut;   ///< Lookup table array pointer
    uint32_t lutSize;      ///< Lookup table size
} tsLutConsts;

/** @brief  Create a LUT devIo
 *  @param  _name   Name of devIo object
 *  @param  _target Target devIo to output lut values
 *  @param  _lut    Array of uint32_t values to be used as look-up table
 */
#define DEV_IO_LUT_CREATE(_name, _target, _lut) \
    tsLutParams _name##Params =                 \
        {                                       \
            .lastIdx = 0,                       \
    };                                          \
    const tsLutConsts _name##Consts =           \
        {                                       \
            .target  = &_target,                \
            .lut     = _lut,                    \
            .lutSize = ARRAY_SIZE(_lut),        \
    };                                          \
    DEV_IO_CREATE(_name, devIoLutFuncs, &_name##Params, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_LUT_H
