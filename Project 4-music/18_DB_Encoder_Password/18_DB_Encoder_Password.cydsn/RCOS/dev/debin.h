/** @file       debin.h
 *  @brief      Debounce filter for input IO
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       7.01.2019
 */
#ifndef FILE_DEBIN_H
#define FILE_DEBIN_H

#include "rcos.h"

/**
 *  @defgroup   DEBIN DEBIN
 *  @ingroup    DEV_IO
 *  @brief      Debounce filter for input IO
 *  @{
 */

#ifndef FILE_DEBIN_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for DEBIN devices
INTERFACE const tsDevIoFuncs devIoDebinFuncs;

/// @brief  Device specific parameters
typedef struct
{
    const tsDevIo *target; ///< Target devIo
    uint32_t stable;       ///< Last stable value read from target
    uint32_t changing;     ///< devIo is trying to change into
    uint8_t limit;         ///< Max. value for debounce filtering
    uint8_t count;         ///< Current debouncing count
} tsDebinParams;

/** @brief  Create a devIo
 *  @param  _name   Name of devIo object
 *  @param  _devIo  Target device that will be read for debouncing
 *  @param  _limit  Count limit to decide stable value
 */
#define DEV_IO_DEBIN_CREATE(_name, _devIo, _limit) \
    tsDebinParams _name##Params =                  \
        {                                          \
            .target   = &_devIo,                   \
            .stable   = 0,                         \
            .changing = 0,                         \
            .limit    = (_limit),                  \
            .count    = (_limit),                  \
    };                                             \
    DEV_IO_CREATE(_name, devIoDebinFuncs, &_name##Params, NULL)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_DEBIN_H
