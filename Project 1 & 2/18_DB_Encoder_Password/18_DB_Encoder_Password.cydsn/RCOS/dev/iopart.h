/** @file       iopart.h
 *  @brief      Io partitioning module allows access to a partition of a devIo
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       22.01.2019
 */
#ifndef FILE_IOPART_H
#define FILE_IOPART_H

#include "rcos.h"

/**
 *  @defgroup   IOPART IOPART
 *  @ingroup    DEV_IO
 *  @brief      Io partitioning module allows access to a partition of a devIo
 *  @{
 */

#ifndef FILE_IOPART_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for IOPART devices
INTERFACE const tsDevIoFuncs devIoIopartFuncs;

/// @brief  Device specific constants
typedef struct
{
    const tsDevIo *target; ///< Device that will be partitioned
    uint32_t mask;         ///< Mask of bits that will be separated
    uint8_t shift;         ///< Shift value used for placing LSB of value on the LSB of mask
} tsIopartConsts;

/** @brief  Create a IOPART device
 *  @param  _name       Name of devIo object
 *  @param  _devIo      Target devIo that will be taken a part of
 *  @param  _bitCount   Number of bits that will be taken
 *  @param  _shift      Number of empty bits at the start
 */
#define DEV_IO_IOPART_CREATE(_name, _devIo, _bitCount, _shift) \
    const tsIopartConsts _name##Consts =                       \
        {                                                      \
            &_devIo,                                           \
            ((1ul << (_bitCount)) - 1) << (_shift),            \
            (_shift),                                          \
    };                                                         \
    DEV_IO_CREATE(_name, devIoIopartFuncs, NULL, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_IOPART_H
