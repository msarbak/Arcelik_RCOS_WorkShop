/** @file       psoc6/capsense.h
 *  @brief      CYPRESS PSOC6 CapSense v3.X
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Ali Ergun
 *  @author     Deniz Fer
 *  @date       28.11.2019
 */
#ifndef FILE_CAPSENSE_H
#define FILE_CAPSENSE_H

#include "rcos.h"

/**
 *  @defgroup   PSOC6_CAPSENSE CAPSENSE
 *  @ingroup    PSOC6_DEV_IO
    @warning    Only CSD buttons are used and tested
 *  @brief      CYPRESS PSOC6 CapSense v3.X
 *  @{
 */

#ifndef FILE_CAPSENSE_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for CAPSENSE devices
INTERFACE const tsDevIoFuncs devIoCapsenseFuncs;

typedef struct
{
} tsCapsenseCyParams;

/// @brief  Device specific parameters
typedef struct
{
    tsTimerCallBack timerCb; ///< Timer callback to be used for periodic operations
} tsCapsenseParams;

/// @brief  Device specific constants
typedef struct
{
    cystatus (*start)(void);                  ///< CYPRESS module Start function
    cystatus (*stop)(void);                   ///< CYPRESS module Stop function
    uint32 (*isBusy)(void);                   ///< CYPRESS module IsBusy function
    cystatus (*processAllWidgets)(void);      ///< CYPRESS module ProcessAllWidgets function
    cystatus (*scanAllWidgets)(void);         ///< CYPRESS module ScanAllWidgets function
    uint32 (*isSensorActive)(uint32, uint32); ///< CYPRESS module IsSensorActive function
    uint8_t widgetCount;                      ///< CYPRESS module TOTAL_WIDGETS value
    uint8_t totalScanTime;                    ///< CYPRESS module Configure>>Advanced>>Scan Order>>Total scan time
} tsCapsenseConsts;

/** @brief  Create a CAPSENSE devIo
 *  @param  _name           Name of devIO object
 *  @param  _cyName         Name of CYPRESS module
 *  @param  _totalScanTime  Total scan time presented by PSOC Creator module's Configure>>Advanced>>Scan Order>>Total scan time
 */
#define DEV_IO_CAPSENSE_CREATE(_name, _cyName, _totalScanTime) \
    extern TIMER_CALLBACK_FUNC(capsenseIsr);                   \
    const tsDevIo _name;                                      \
    tsCapsenseParams _name##Params =                           \
        {                                                      \
            TIMER_CALLBACK_INIT(capsenseIsr, &_name),          \
    };                                                         \
    const tsCapsenseConsts _name##Consts =                     \
        {                                                      \
            _cyName##_Start,                                   \
            _cyName##_Stop,                                    \
            _cyName##_IsBusy,                                  \
            _cyName##_ProcessAllWidgets,                       \
            _cyName##_ScanAllWidgets,                          \
            _cyName##_IsSensorActive,                          \
            _cyName##_TOTAL_WIDGETS,                           \
            (_totalScanTime),                                  \
    };                                                         \
    DEV_IO_CREATE(_name, devIoCapsenseFuncs, &_name##Params, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_CAPSENSE_H
