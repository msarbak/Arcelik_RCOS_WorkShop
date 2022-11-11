/** @file       iocomb.h
 *  @brief      Header file of IOCOMB device
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       16.11.2018
 */
#ifndef FILE_IOCOMB_H
#define FILE_IOCOMB_H

#include "rcos.h"

/**
 *  @defgroup   IOCOMB IOCOMB
 *  @ingroup    DEV_IO
 *  @brief      Generic I/O device to combine a collection of IO devices with masks
 *  @{
 */

#ifndef FILE_IOCOMB_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for IOCOMB devices
INTERFACE const tsDevIoFuncs devIoIocombFuncs;

/// @brief  IOCOMB_ITEM object structure
typedef struct
{
    const tsDevIo *device; ///< Target IO device
    uint32_t mask;         ///< Mask that will be used to get the data
    uint8_t bitCount;      ///< Number of bits that will be connected on this device
} tsIoCombItem;

/// @brief  Device specific constants
typedef struct
{
    const tsIoCombItem *list; ///< Array of devIo
    uint8_t count;            ///< Size of list array
} tsIocombConsts;

/** @brief  Define a devIo and bitCount to be masked that will connected to IOCOMB
 *  @param  _devIo      Target devIo object
 *  @param  _bitCount   Number of bits that will be connected
 */
#define IOCOMB_ITEM(_devIo, _bitCount)               \
    {                                                \
        &_devIo, ~(-1ul << (_bitCount)), (_bitCount) \
    }

/** @brief  Create a IOCOMB devIo
 *  @param  _name   Name of devIo object
 *  @param  ...     IOCOMB_ITEM objects that are gonna be combined in this IOCOMB
 */
#define DEV_IO_IOCOMB_CREATE(_name, ...) \
    const tsIoCombItem _name##List[] =   \
        {                                \
            __VA_ARGS__,                 \
    };                                   \
    const tsIocombConsts _name##Consts = \
        {                                \
            _name##List,                 \
            ARRAY_SIZE(_name##List),     \
    };                                   \
    DEV_IO_CREATE(_name, devIoIocombFuncs, NULL, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_IOCOMB_H
