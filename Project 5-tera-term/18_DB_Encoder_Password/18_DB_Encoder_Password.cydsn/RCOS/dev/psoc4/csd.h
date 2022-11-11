/** @file       psoc4/csd.h
 *  @brief      CYPRESS PSOC4 CapSense CSD P4 v2.X
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       9.11.2018
 */
#ifndef FILE_CSD_H
#define FILE_CSD_H

#include "rcos.h"

/**
 *  @defgroup   PSOC4_CSD CSD
 *  @ingroup    PSOC4_DEV_IO
 *  @warning    Only CSD buttons are used and tested
 *  @brief      CYPRESS PSOC4 CapSense CSD P4 v2.X
 *  @{
 */

#ifndef FILE_CSD_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for CSD devices
INTERFACE const tsDevIoFuncs devIoCsdFuncs;

/// @brief  Corresponding CYPRESS component functions
typedef struct
{
    void (*start)(void);      ///< CYPRESS module Start function
    void (*stop)(void);       ///< CYPRESS module Stop function
    void (*init)(void);       ///< CYPRESS module InitializeEnabledBaselines function
    uint32 (*check)(uint32);  ///< CYPRESS module CheckIsWidgetActive function
    uint32 (*isBusy)(void);   ///< CYPRESS module IsBusy function
    void (*scanAll)(void);    ///< CYPRESS module ScanEnabledWidgets function
    void (*update)(void);     ///< CYPRESS module UpdateEnabledBaselines function
    uint32 (*debounce)(void); ///< CYPRESS module CheckIsAnyWidgetActive function
    uint8_t sensorCount;      ///< CYPRESS module _END_OF_WIDGETS_INDEX value
    uint8_t totalScanTime;    ///< CYPRESS module Configure>>Scan Order>>Total scan time
} tsCsdConsts;

/// @brief  Device specific parameters
typedef struct
{
    tsTimerCallBack timerCb; ///< Timer callback to be used for periodic operations
    uint8_t state;           ///< State of operation
} tsCsdParams;

/** @brief  Create a CSD devIO
 *  @param  _name       Name of devIO object
 *  @param  _cyName     Name of CYPRESS module
 *  @param  _totalScanTime  Total scan time presented by PSOC Creator module's Configure>>Advanced>>Scan Order>>Total scan time
 */
#define DEV_IO_CSD_CREATE(_name, _cyName, _totalScanTime) \
    extern TIMER_CALLBACK_FUNC(csdIsr);                   \
    const tsDevIo _name;                                  \
    const tsCsdConsts _name##Consts =                     \
        {                                                 \
            _cyName##_Start,                              \
            _cyName##_Stop,                               \
            _cyName##_InitializeEnabledBaselines,         \
            _cyName##_CheckIsWidgetActive,                \
            _cyName##_IsBusy,                             \
            _cyName##_ScanEnabledWidgets,                 \
            _cyName##_UpdateEnabledBaselines,             \
            _cyName##_CheckIsAnyWidgetActive,             \
            _cyName##_END_OF_WIDGETS_INDEX,               \
            (_totalScanTime),                             \
    };                                                    \
    tsCsdParams _name##Params =                           \
        {                                                 \
            TIMER_CALLBACK_INIT(csdIsr, &_name),          \
            0,                                            \
    };                                                    \
    DEV_IO_CREATE(_name, devIoCsdFuncs, &_name##Params, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_CSD_H
