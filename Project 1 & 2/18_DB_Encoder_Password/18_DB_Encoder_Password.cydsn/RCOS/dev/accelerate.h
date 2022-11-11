/** @file       accelerate.h
 *  @brief      Header file of ACCELERATE device
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       25.12.2019
 */
#ifndef FILE_ACCELERATE_H
#define FILE_ACCELERATE_H

/// Includes
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_ACCELERATE_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   ACCELERATE ACCELERATE
 *  @ingroup    DEV_IO
 *  @brief      Continuously change value of target devIo depending on input value.
 *  @{
 */

/// @brief  Functions for ACCELERATE devices
INTERFACE const tsDevIoFuncs devIoAccelerateFuncs;

/// @brief  Device specific parameters
typedef struct
{
    tsTimerCallBack tcb;   ///< TimerCallBack for periodic operation
    uint32_t acceleration; ///< acceleration value
} tsAccelerateParams;

/// @brief  Device specific parameters
typedef struct
{
    const tsDevIo *target; ///< Target devIo used to drive
    uint32_t timeout;      ///< timeout period
} tsAccelerateConsts;

/** @brief  Create a devIo
 *  @param  _name       Name of devIo object
 *  @param  _target     Target devIo
 *  @param  _timeout    Value update time period
 */
#define DEV_IO_ACCELERATE_CREATE(_name, _target, _timeout)          \
    const tsDevIo _name;                                            \
    extern TIMER_CALLBACK_FUNC(accelFunc);                          \
    tsAccelerateParams _name##Params =                              \
        {                                                           \
            .tcb          = TIMER_CALLBACK_INIT(accelFunc, &_name), \
            .acceleration = 0,                                      \
    };                                                              \
    const tsAccelerateConsts _name##Consts =                        \
        {                                                           \
            .target  = &_target,                                    \
            .timeout = (_timeout),                                  \
    };                                                              \
    DEV_IO_CREATE(_name, devIoAccelerateFuncs, &_name##Params, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_ACCELERATE_H
