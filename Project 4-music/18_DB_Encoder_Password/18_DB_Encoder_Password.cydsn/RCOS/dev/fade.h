/** @file       fade.h
 *  @brief      Fade in/out another devIo
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       16.11.2018
 */
#ifndef FILE_FADE_H
#define FILE_FADE_H

#include "rcos.h"

/**
 *  @defgroup   FADE FADE
 *  @ingroup    DEV_IO
 *  @brief      Fade in/out another devIo
 *  @warning    onValue must be bigger than offValue
 *  @{
 */

#ifndef FILE_FADE_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for FADE devices
INTERFACE const tsDevIoFuncs devIoFadeFuncs;

/// @brief  Device specific parameters
typedef struct
{
    tsTimerCallBack timer; ///< Timer callback object for processing
    uint32_t current;      ///< Current value of target
    uint32_t finish;       ///< Final value of target
} tsFadeParams;

/// @brief  Device specific constants
typedef struct
{
    const tsDevIo *target; ///< Target devIo to run fade function on
    uint32_t duration;     ///< Duration of each step
    uint32_t change;       ///< Change in value with each step
} tsFadeConsts;

/** @brief  Create a FADE devIo
 *  @param  _name       Name of devIo object
 *  @param  _target     Name of target devIo object
 *  @param  _change     Value of change per step
 *  @param  _duration   Duration of each step
 */
#define DEV_IO_FADE_CREATE(_name, _target, _change, _duration)   \
    extern TIMER_CALLBACK_FUNC(fadeHandler);                     \
    const tsDevIo _name;                                         \
    tsFadeParams _name##Params =                                 \
        {                                                        \
            .timer   = TIMER_CALLBACK_INIT(fadeHandler, &_name), \
            .current = 0,                                        \
            .finish  = 0,                                        \
    };                                                           \
    const tsFadeConsts _name##Consts =                           \
        {                                                        \
            .target   = &_target,                                \
            .duration = (_duration),                             \
            .change   = (_change),                               \
    };                                                           \
    DEV_IO_CREATE(_name, devIoFadeFuncs, &_name##Params, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_FADE_H
