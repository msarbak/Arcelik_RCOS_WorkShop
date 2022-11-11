/** @file       enc_inc_abs.h
 *  @brief      Header file of ENC_INC_ABS device
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       2.12.2019
 */
#ifndef FILE_ENC_INC_ABS_H
#define FILE_ENC_INC_ABS_H

/// Includes
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_ENC_INC_ABS_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   ENC_INC_ABS ENC_INC_ABS
 *  @ingroup    DEV_IO
 *  @brief      Converts an absolute encoder data to be like an incremental encoder.
 *  @{
 */

/// @brief  Functions for ENC_INC_ABS devices
INTERFACE const tsDevIoFuncs devIoEncIncAbsFuncs;

/// @brief  Device specific parameters
typedef struct
{
    tsTimerCallBack tcb;   ///< Timer callback structure to read absolute encoder data
    const tsDevIo *encAbs; ///< Absolute encoder data input
    uint16_t stepCount;    ///< Step count of absolute encoder
    uint16_t timeout;      ///< Timeout duration of timer callback
    uint32_t value;        ///< Internal value
    uint32_t encPos;       ///< Last read value for comparison
} tsEncIncAbsParams;

/** @brief  Create a devIo of type ENC_INC_ABS
 *  @param  _name       Name of devIo object
 *  @param  _encAbs     Target absolute encoder that will provide rotational data
 *  @param  _stepCount  Step count of absolute encoder (0..n)
 *  @param  _timeout    Timeout for refreshing data
 */
#define DEV_IO_ENC_INC_ABS_CREATE(_name, _encAbs, _stepCount, _timeout) \
    extern TIMER_CALLBACK_FUNC(encIncAbsCb);                            \
    const tsDevIo _name;                                                \
    tsEncIncAbsParams _name##Params =                                   \
        {                                                               \
            .tcb       = TIMER_CALLBACK_INIT(encIncAbsCb, &_name),      \
            .encAbs    = &_encAbs,                                      \
            .stepCount = (_stepCount),                                  \
            .timeout   = (_timeout),                                    \
            .value     = 0,                                             \
            .encPos    = 0,                                             \
    };                                                                  \
    DEV_IO_CREATE(_name, devIoEncIncAbsFuncs, &_name##Params, NULL)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_ENC_INC_ABS_H
