/** @file       iolink.h
 *  @brief      A simple device to access a data of type uint32_t
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       4.02.2019
 */
#ifndef FILE_IOLINK_H
#define FILE_IOLINK_H

#include "rcos.h"

/**
 *  @defgroup   IOLINK IOLINK
 *  @ingroup    DEV_IO
 *  @brief      A simple device to access a data of type uint32_t
 *  @{
 */

#ifndef FILE_IOLINK_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for IOLINK devices
INTERFACE const tsDevIoFuncs devIoIolinkFuncs;

/// @brief  Device specific constans
typedef struct
{
    void *target;  ///< Target memory address
    uint32_t size; ///< Size of target memory
} tsIolinkConsts;

/** @brief  Create an IOLINK devIo
 *  @param  _name       Name of devIo object
 *  @param  _dataPtr    Target variable address
 *  @param  _dataSize   Target variable size
 */
#define DEV_IO_IOLINK_CREATE(_name, _dataPtr, _dataSize) \
    const tsIolinkConsts _name##Consts =                 \
        {                                                \
            (_dataPtr),                                  \
            (_dataSize),                                 \
    };                                                   \
    DEV_IO_CREATE(_name, devIoIolinkFuncs, NULL, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_IOLINK_H
