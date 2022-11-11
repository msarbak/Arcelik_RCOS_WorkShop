/** @file       ioduplicate.h
 *  @brief      Header file of IODUPLICATE device
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       3.12.2019
 */
#ifndef FILE_IODUPLICATE_H
#define FILE_IODUPLICATE_H

/// Includes
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_IODUPLICATE_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   IODUPLICATE IODUPLICATE
 *  @ingroup    DEV_IO
 *  @brief      A device to duplicate placed value into multiple target devIo and gets value from the first one.
 *  @{
 */

/// @brief  Functions for IODUPLICATE devices
INTERFACE const tsDevIoFuncs devIoIoduplicateFuncs;

/// @brief  Device specific constants
typedef struct
{
    const tsDevIo *const *list; ///< List of target devices for duplication
    uint16_t itemCount;         ///< Item count of list
} tsIoduplicateConsts;

/** @brief  Create a devIo
 *  @param  _name   Name of devIo object
 *  @param  ...     devIo pointers that the data will be duplicated
 */
#define DEV_IO_IODUPLICATE_CREATE(_name, ...) \
    const tsDevIo *_name##List[] =            \
        {                                     \
            __VA_ARGS__,                      \
    };                                        \
    const tsIoduplicateConsts _name##Consts = \
        {                                     \
            _name##List,                      \
            ARRAY_SIZE(_name##List),          \
    };                                        \
    DEV_IO_CREATE(_name, devIoIoduplicateFuncs, NULL, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_IODUPLICATE_H
