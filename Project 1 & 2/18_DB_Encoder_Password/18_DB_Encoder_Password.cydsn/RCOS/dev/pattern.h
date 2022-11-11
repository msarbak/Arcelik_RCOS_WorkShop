/** @file       pattern.h
 *  @brief      Output a pattern which is an array of value with timings
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       16.11.2018
 */
#ifndef FILE_PATTERN_H
#define FILE_PATTERN_H

#include "rcos.h"

/**
 *  @defgroup   PATTERN PATTERN
 *  @ingroup    DEV_IO
 *  @brief      Output a pattern which is an array of value with timings
 *  @{
 */

#ifndef FILE_PATTERN_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for PATTERN devices
INTERFACE const tsDevIoFuncs devIoPatternFuncs;

/// @brief  Pattern node structure
typedef struct
{
    uint32_t value; ///< Value to be set for this node
    uint32_t time;  ///< Amount of time to be spent in this node
} tsPatternNode;

/** @brief  Pattern node template
 *  @param  _value  Value to be set for this node
 *  @param  _time   Amount of time to be spent in this node
 */
#define PATTERN_NODE(_value, _time) \
    {                               \
        (_value), (_time)           \
    }

/// @brief  End node to indicate a pattern ending with value
#define PATTERN_NODE_END(_value) PATTERN_NODE(_value, 0)

/// @brief  Repeat node to indicate to restart this pattern array
#define PATTERN_NODE_REPEAT() PATTERN_NODE(0, (uint32_t)-1ul)

/// @brief  Device specific parameters
typedef struct
{
    tsTimerCallBack timer;     ///< Timer callback to process list of pattern nodes
    const tsPatternNode *list; ///< Array of pattern nodes that will be used
    const tsPatternNode *node; ///< Current pattern node in the list
    const tsDevIo *target;     ///< Target devIo to output pattern values
} tsPatternParams;

/// @brief  Device specific constants
typedef struct
{
    uint32_t offValue; ///< Off value for target device
} tsPatternConsts;

/** @brief  Create a PATTERN devIo
 *  @param  _name       Name of devIo object
 *  @param  _target     Target devIo to be used as output of PATTERN
 *  @param  _offValue   Off value for target device
 */
#define DEV_IO_PATTERN_CREATE(_name, _target, _offValue)              \
    extern TIMER_CALLBACK_FUNC(patternPutHandler);                    \
    const tsDevIo _name;                                              \
    tsPatternParams _name##Params =                                   \
        {                                                             \
            .timer  = TIMER_CALLBACK_INIT(patternPutHandler, &_name), \
            .list   = NULL,                                           \
            .node   = NULL,                                           \
            .target = &_target,                                       \
    };                                                                \
    const tsPatternConsts _name##Consts =                             \
        {                                                             \
            .offValue = _offValue,                                    \
    };                                                                \
    DEV_IO_CREATE(_name, devIoPatternFuncs, &_name##Params, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_PATTERN_H
