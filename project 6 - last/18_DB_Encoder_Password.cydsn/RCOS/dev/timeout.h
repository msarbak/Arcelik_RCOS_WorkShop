/** @file       timeout.h
 *  @brief      Header file of TIMEOUT device
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       26.12.2019
 */
#ifndef FILE_TIMEOUT_H
#define FILE_TIMEOUT_H

/// Includes
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_TIMEOUT_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   TIMEOUT TIMEOUT
 *  @ingroup    DEV_IO
 *  @brief      Timeout device counts down a value for each millisecond.
 *  @details    PUT is used for changing the countdown value, INIT is used for 
 *              starting the count down, DEINIT is used as pause, current value
 *              can be read by GET
 *  @{
 */

/// @brief  Functions for TIMEOUT devices
INTERFACE const tsDevIoFuncs devIoTimeoutFuncs;

/// @brief  Device specific parameters
typedef struct
{
    tsTimerCallBack tcb; ///< timerCallBack structure used to periodically decrease value
    uint32_t value;      ///< Current value of the timeout device
} tsTimeoutParams;

/** @brief  Create a devIo
 *  @param  _name   Name of devIo object
 */
#define DEV_IO_TIMEOUT_CREATE(_name)                              \
    extern TIMER_CALLBACK_FUNC(timeoutTcbFunc);                   \
    const tsDevIo _name;                                          \
    tsTimeoutParams _name##Params =                               \
        {                                                         \
            .tcb   = TIMER_CALLBACK_INIT(timeoutTcbFunc, &_name), \
            .value = 0,                                           \
    };                                                            \
    DEV_IO_CREATE(_name, devIoTimeoutFuncs, &_name##Params, NULL)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_TIMEOUT_H
