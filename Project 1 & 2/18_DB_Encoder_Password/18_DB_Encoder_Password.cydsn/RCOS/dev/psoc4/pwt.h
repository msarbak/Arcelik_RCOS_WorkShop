/** @file       psoc4/pwt.h
 *  @brief      Header file of PWT device for CYPRESS PSOC4
 *  @copyright  (c) 2020-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Umit Sahin
 *  @author     Evren Kenanoglu
 *  @date       11.02.2020
 */
#ifndef FILE_PWT_H
#define FILE_PWT_H

/// Includes
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_PWT_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   PSOC4_PWT PWT
 *  @ingroup    PSOC4_DEV_CPX
 *  @brief      Pulse Width and Frequency Measurement
    @verbatim
                         TCPWM_BLOCK_NAME
                       ┌──────────────────┐
                       │  Timer Counter   │
                       ├──────────────────┤
        PIN_INPUT ■>─┬─│reload          ov│─■
                     │ │                un│─■
                     │ │                uc│─■
              ┌───┐  └─│capture           │
CY_CLOCK_BLOCK│_│_│────│>clock            │   ┌───┐
              └───┘    │         interrupt│───│///│CY_ISR_BlOCK_NAME
                       └──────────────────┘   └───┘

TCPWM Block Configuration:
    - Interupt Mode -> - On terminal count
                    -> + On capture/compare Count
    - Reload -> Selected Edge ("Rising Edge" selection calculates duty cycle of high part, "Falling Edge" calculates for low part)
    - Capture -> Either Edge
    @endverbatim
 */

/**
 *  @defgroup   PSOC4_PWT_CORE CORE
 *  @ingroup    PSOC4_PWT
 *  @brief      Main devCpx device of PWT which carries 2 devIo device, one for duty and another for frequency
 *  @{
 */

/// @brief  Functions for PWT devices
INTERFACE const tsDevCpxFuncs devCpxPwtCoreFuncs;

/// @brief  Generic ISR function for PWT devices
INTERFACE void pwtIsr(const tsDevCpx *device);

/// @brief  Device specific configuration
typedef struct
{
    void (*cntrStart)(void);                    ///< CYPRESS TIMER/COUNTER module Start Counter function
    void (*cntrStop)(void);                     ///< CYPRESS TIMER/COUNTER module Stop Counter function
    uint32 (*cntrReadCounter)(void);            ///< CYPRESS TIMER/COUNTER module Read Counter function
    uint32 (*cntrReadCapture)(void);            ///< CYPRESS TIMER/COUNTER module Read Capture function
    uint32 (*cntrGetIsm)(void);                 ///< CYPRESS TIMER/COUNTER module Get Interrupt Source Mask function
    void (*cntrClearInt)(uint32 interruptMask); ///< CYPRESS TIMER/COUNTER module Interrupt Clear function
    void (*isrStartEx)(cyisraddress address);   ///< CYPRESS ISR module Start function
    void (*isrStop)(void);                      ///< CYPRESS ISR module Stop function
    void (*interrupt)(void);                    ///< CYPRESS module Isr function
    uint32_t clockFreq;                         ///< Clock Frequency value in Hz
    uint32_t dutyMax;                           ///< Maximum value of Duty Cycle
} tsPwtCoreConsts;

/// @brief  Device specific parameters
typedef struct
{
    uint32_t pulse; ///< First half of the signal
    uint32_t freq;  ///< Frequency of Input Signal
    uint32_t duty;  ///< Duty Cycle of Input Signal
} tsPwtCoreParams;

/// @brief  Device child specific parameters
typedef struct
{
    const tsDevCpx *core;        ///< PWT_CORE device pointer
    tsPwtCoreParams *coreParams; ///< PWT_CORE device parameters pointer
} tsPwtChildParams;

/** @brief  Create a devCpx for PWT device
 *  @param  _name       PWT Device Name
 *  @param  _cyCounter  CYPRESS TCPWM Block Name
 *  @param  _cyIsr      CYPRESS ISR Block Name
 *  @param  _clockFreq  TCPWM Input Clock Frequency in Hz. Can be used a higher value to increase frequency resolution.
 *  @param  _dutyMax    Maximum value of Duty Cycle
 */
#define DEV_CPX_PWT_CORE_CREATE(_name, _cyCounter, _cyIsr, _clockFreq, _dutyMax) \
    const tsDevCpx _name;                                                        \
    void _name##Isr(void)                                                        \
    {                                                                            \
        pwtIsr(&_name);                                                          \
    }                                                                            \
    const tsPwtCoreConsts _name##Consts =                                        \
        {                                                                        \
            .cntrStart       = _cyCounter##_Start,                               \
            .cntrStop        = _cyCounter##_Stop,                                \
            .cntrReadCounter = _cyCounter##_ReadCounter,                         \
            .cntrReadCapture = _cyCounter##_ReadCapture,                         \
            .cntrGetIsm      = _cyCounter##_GetInterruptSourceMasked,            \
            .cntrClearInt    = _cyCounter##_ClearInterrupt,                      \
            .isrStartEx      = _cyIsr##_StartEx,                                 \
            .isrStop         = _cyIsr##_Stop,                                    \
            .interrupt       = _name##Isr,                                       \
            .clockFreq       = _clockFreq,                                       \
            .dutyMax         = _dutyMax,                                         \
    };                                                                           \
    tsPwtCoreParams _name##Params =                                              \
        {                                                                        \
            .pulse = 0,                                                          \
            .freq  = 0,                                                          \
            .duty  = 0,                                                          \
    };                                                                           \
    DEV_CPX_CREATE(_name, devCpxPwtCoreFuncs, &_name##Params, &_name##Consts)

/** @} */

/**
 *  @defgroup   PSOC4_PWT_DUTY DUTY
 *  @ingroup    PSOC4_PWT
 *  @brief      Duty cycle calculated by the PWT_CORE device
 *  @{
 */

/// @brief  Functions for a PWT_DUTY device
INTERFACE const tsDevIoFuncs devIoPwtDutyFuncs;

/** @brief  Create a PWT_DUTY devIo
 *  @param  _name   Name of devIO object
 *  @param  _devCpx Target devCpx that is a PWT_CORE
 */
#define DEV_IO_PWT_DUTY_CREATE(_name, _devCpx) \
    const tsPwtChildParams _name##Params =     \
        {                                      \
            .core       = &_devCpx,            \
            .coreParams = &_devCpx##Params,    \
    };                                         \
    DEV_IO_CREATE(_name, devIoPwtDutyFuncs, &_name##Params, NULL)

/** @} */

/**
 *  @defgroup   PSOC4_PWT_FREQ FREQ
 *  @ingroup    PSOC4_PWT
 *  @brief      Frequency calculated by the PWT_CORE device
 *  @{
 */

/// @brief  Functions for a PWT_FREQ device
INTERFACE const tsDevIoFuncs devIoPwtFreqFuncs;

/** @brief  Create a PWT_FREQ devIO
 *  @param  _name   Name of devIO object
 *  @param  _devCpx Target devCpx that is a PWT_CORE
 */
#define DEV_IO_PWT_FREQ_CREATE(_name, _devCpx) \
    const tsPwtChildParams _name##Params =     \
        {                                      \
            .core       = &_devCpx,            \
            .coreParams = &_devCpx##Params,    \
    };                                         \
    DEV_IO_CREATE(_name, devIoPwtFreqFuncs, &_name##Params, NULL)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_PWT_H
