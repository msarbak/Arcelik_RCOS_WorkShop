/** @file       debounce.h
 *  @brief      Debounce engine for multiple tsDevIo
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       7.12.2018
 */
#ifndef FILE_DEBOUNCE_H
#define FILE_DEBOUNCE_H

#include "rcos.h"

/**
 *  @defgroup   DEBOUNCE DEBOUNCE
 *  @ingroup    DEV_CPX
 *  @brief      Debounce engine for multiple tsDevIo
 *  @{
 */

#ifndef FILE_DEBOUNCE_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for DEBOUNCE devices
INTERFACE const tsDevCpxFuncs devCpxDebounceFuncs;

/// @brief  Debounce item structure
typedef struct
{
    const tsDevIo *devIo; ///< Target devIo that will be checked
    uint32_t stable;      ///< Last stable value
    uint32_t changing;    ///< Changing into
    uint8_t count;        ///< Number of times changing value was stable
    uint8_t limit;        ///< Limit for count value
} tsDebounceItem;

/// @brief  Device specific parameters
typedef struct
{
    tsTimerCallBack timer;  ///< Timer callback to check every IO periodically
    tsDebounceItem *ioList; ///< Array of DEBOUNCE ITEMS that are going to be checked
    uint8_t ioCount;        ///< Number of IOs in the ioList
    uint8_t period;         ///< ioList is checked every period in milliseconds
} tsDebounceParams;

/** @brief  Initialize a debounce item
 *  @param  _devIo  target devIo
 *  @param  _debCnt number of times counted for stability
 */
#define DEBOUNCE_ITEM(_devIo, _debCnt) \
    {                                  \
        &(_devIo), 0, 0, 0, (_debCnt)  \
    }

/** @brief  Create a DEBOUNCE devCpx
 *  @param  _name       Name of devCpx object
 *  @param  _period     Duration of each period to check array of devIo
 *  @param  ...         Debounce items' initial values created by @ref DEBOUNCE_ITEM
 */
#define DEV_CPX_DEBOUNCE_CREATE(_name, _period, ...)               \
    extern TIMER_CALLBACK_FUNC(debounceCheck);                     \
    const tsDevCpx _name;                                          \
    tsDebounceItem _name##List[] =                                 \
        {                                                          \
            __VA_ARGS__,                                           \
    };                                                             \
    tsDebounceParams _name##Params =                               \
        {                                                          \
            .timer   = TIMER_CALLBACK_INIT(debounceCheck, &_name), \
            .ioList  = _name##List,                                \
            .ioCount = ARRAY_SIZE(_name##List),                    \
            .period  = (_period),                                  \
    };                                                             \
    DEV_CPX_CREATE(_name, devCpxDebounceFuncs, &_name##Params, NULL)

/// @brief  Functions for DEBOUNCE input devices
INTERFACE const tsDevIoFuncs devIoDebIpFuncs;

/// @brief  Device specific parameters
typedef struct
{
    const tsDevCpx *debounce; ///< Target debounce tsDevCpx object
    uint8_t idx;              ///< Index of debounce item
} tsDebIpParams;

/** @brief  Create a DEBOUNCE devIo
 *  @param  _name           Name of devIo object
 *  @param  _devDebounce    Name of tsDevCpx object that is a DEBOUNCE engine
 *  @param  _idx            Index of debounced item in DEBOUNCE engine
 */
#define DEV_IO_DEBOUNCE_CREATE(_name, _devDebounce, _idx) \
    const tsDebIpParams _name##Params =                   \
        {                                                 \
            .debounce = &(_devDebounce),                  \
            .idx      = (_idx),                           \
    };                                                    \
    DEV_IO_CREATE(_name, devIoDebIpFuncs, &_name##Params, NULL)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_DEBOUNCE_H
