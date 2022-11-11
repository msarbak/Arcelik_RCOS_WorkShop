/** @file       hc595.h
 *  @brief      8-bit serial-in, serial or parallel-out shift register with output latches; 3-state
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       2.04.2019
 */
#ifndef FILE_HC595_H
#define FILE_HC595_H

#include "rcos.h"

/**
 *  @defgroup   HC595 HC595
 *  @ingroup    DEV_IO
 *  @brief      8-bit serial-in, serial or parallel-out shift register with output latches; 3-state
 *  @warning    Uses bit banging
 *  @todo       Should use SPI
 *  @{
 */

#ifndef FILE_HC595_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for HC595 devices
INTERFACE const tsDevIoFuncs devIoHc595Funcs;

/// @brief  Device specific parameters
typedef struct
{
    uint32_t lastValue; ///< Last value placed to shiftregisters kept for devIoGet function
} tsHc595Params;

/// @brief  Device specific constants
typedef struct
{
    const tsDevIo *shcp; ///< Shift register clock input
    const tsDevIo *stcp; ///< Storage register clock input
    const tsDevIo *ds;   ///< Serial data input
    const tsDevIo *mr;   ///< Master reset (active LOW)
    const tsDevIo *oe;   ///< Output enable (active LOW)
    uint32_t actLevels;  ///< Activity levels
    uint8_t cascade;     ///< Number of devices cascaded (0..3)
                         ///< If there's a single device, "cascade" should be "0"
} tsHc595Consts;

/** @brief  Create a devIo
 *  @param  _name       Name of devIo object
 *  @param  _shcp       Shift register clock input
 *  @param  _stcp       Storage register clock input
 *  @param  _ds         Serial data input
 *  @param  _mrPtr      Master reset (active LOW)
 *  @param  _oePtr      Output enable (active LOW)
 *  @param  _initValue  Initial value for the pins
 *  @param  _actLevels  Activity levels of each pin (0:low 1:high)
 *  @param  _cascade    Number of devices cascaded (0..3)
 */
#define DEV_IO_HC595_CREATE(_name, _shcp, _stcp, _ds, _mrPtr, _oePtr, _initValue, _actLevels, _cascade) \
    tsHc595Params _name##Params =                                                                       \
        {                                                                                               \
            .lastValue = (_initValue),                                                                  \
    };                                                                                                  \
    const tsHc595Consts _name##Consts =                                                                 \
        {                                                                                               \
            .shcp      = &(_shcp),                                                                      \
            .stcp      = &(_stcp),                                                                      \
            .ds        = &(_ds),                                                                        \
            .mr        = (_mrPtr),                                                                      \
            .oe        = (_oePtr),                                                                      \
            .actLevels = ~(_actLevels), /* active low pins will be XOR'd with this value */             \
            .cascade   = (_cascade),                                                                    \
    };                                                                                                  \
    DEV_IO_CREATE(_name, devIoHc595Funcs, &_name##Params, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_HC595_H
