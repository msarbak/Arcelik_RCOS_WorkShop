/** @file       iovirtual.h
 *  @brief      A simple device to access a data of type uint32_t
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       4.02.2019
 */
#ifndef FILE_IOVIRTUAL_H
#define FILE_IOVIRTUAL_H

#include "rcos.h"

/**
 *  @defgroup   IOVIRTUAL IOVIRTUAL
 *  @ingroup    DEV_IO
 *  @brief      A simple device to access a data of type uint32_t
 *  @{
 */

#ifndef FILE_IOVIRTUAL_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for IOVIRTUAL devices
INTERFACE const tsDevIoFuncs devIoIovirtualFuncs;

/** @brief      Create a devIo
 *  @details    Combine "_name" and "Value" to reach its current value
 *  @param      _name   Name of devIo object
 */
#define DEV_IO_IOVIRTUAL_CREATE(_name) \
    uint32_t _name##Value;             \
    DEV_IO_CREATE(_name, devIoIovirtualFuncs, &_name##Value, NULL)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_IOVIRTUAL_H
