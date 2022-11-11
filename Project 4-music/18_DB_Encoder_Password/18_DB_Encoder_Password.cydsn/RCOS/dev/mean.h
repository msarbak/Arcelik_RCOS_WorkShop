/** @file       mean.h
 *  @brief      Collect and calculate average value of a target devIo
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       8.01.2019
 */
#ifndef FILE_MEAN_H
#define FILE_MEAN_H

#include "rcos.h"

/**
 *  @defgroup   MEAN MEAN
 *  @ingroup    DEV_IO
 *  @brief      Collect and calculate average value of a target devIo
 *  @{
 */

#ifndef FILE_MEAN_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for MEAN devices
INTERFACE const tsDevIoFuncs devIoMeanFuncs;

/// @brief  Device specific constants
typedef struct
{
    const tsDevIo *target; ///< Target I/O device
    tsDeque *queue;        ///< Queue to store every value
    uint8_t disExt;        ///< Dismiss extreme values(min and max values are dismissed)
    uint8_t count;         ///< Number of items stored in queue
} tsMeanConsts;

/** @brief  Create a devIo
 *  @param  _name   Name of devIo object
 *  @param  _target devIo to get values from
 *  @param  _count  Number of values to collect for calculating the mean value(should be 3 at least and less than 255)
 *  @param  _disExt TRUE or FALSE
 */
#define DEV_IO_MEAN_CREATE(_name, _target, _count, _disExt)                      \
    DEQUE_CREATE(_name##Deq, (((_count) < 3) ? 3 : (_count)) * sizeof(uint32_t)) \
    const tsMeanConsts _name##Consts =                                           \
        {                                                                        \
            &_target,                                                            \
            &_name##Deq,                                                         \
            (_disExt) ? TRUE : FALSE,                                            \
            (_count),                                                            \
    };                                                                           \
    DEV_IO_CREATE(_name, devIoMeanFuncs, NULL, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_MEAN_H
