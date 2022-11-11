/** @file       pulse.h
 *  @brief      Output a pulse for a certain duration
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       16.11.2018
 */
#ifndef FILE_PULSE_H
#define FILE_PULSE_H

#include "rcos.h"

/**
 *  @defgroup   PULSE PULSE
 *  @ingroup    DEV_IO
 *  @brief      Output a pulse for a certain duration
 *  @{
 */

#ifndef FILE_PULSE_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for PULSE devices
INTERFACE const tsDevIoFuncs devIoPulseFuncs;

/// @brief  Device specific parameters
typedef struct
{
    tsTimerCallBack timer; ///< Timer callback to finish the pulse operation
} tsPulseParams;

/// @brief  Device specific constants
typedef struct
{
    const tsDevIo *target; ///< Target devIo to output the pulse
    uint32_t onValue;      ///< Active value of pulse
    uint32_t offValue;     ///< Deactive value of pulse
} tsPulseConsts;

/** @brief  Create a PULSE devIo
 *  @param  _name   Name of devIo object
 *  @param  _target Name of target devIo object
 *  @param  _on     Active value of pulse
 *  @param  _off    Deactive value of pulse
 */
#define DEV_IO_PULSE_CREATE(_name, _target, _on, _off)          \
    extern TIMER_CALLBACK_FUNC(pulseHandler);                   \
    const tsDevIo _name;                                        \
    tsPulseParams _name##Params =                               \
        {                                                       \
            .timer = TIMER_CALLBACK_INIT(pulseHandler, &_name), \
    };                                                          \
    const tsPulseConsts _name##Consts =                         \
        {                                                       \
            .target   = &(_target),                             \
            .onValue  = (_on),                                  \
            .offValue = (_off),                                 \
    };                                                          \
    DEV_IO_CREATE(_name, devIoPulseFuncs, &_name##Params, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_PULSE_H
