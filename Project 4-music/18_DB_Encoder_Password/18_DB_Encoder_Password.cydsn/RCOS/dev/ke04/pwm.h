/** @file       ke04/pwm.h
 *  @brief      PWM mode of FTM timer for NXP KE04
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       19.07.2019
 */
#ifndef FILE_PWM_H
#define FILE_PWM_H

#include "rcos.h"
#include "fsl_ftm.h"

/**
 *  @defgroup   KE04_PWM PWM
 *  @ingroup    KE04_DEV_IO
 *  @brief      PWM mode of FTM timer for NXP KE04
 *  @{
 */

#ifndef FILE_PWM_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for PWM core devices
INTERFACE const tsDevCpxFuncs devCpxPwmCoreFuncs;

/// @brief  Device specific parameters
typedef struct
{
    FTM_Type *base;                                ///< Peripheral registries base address
    ftm_config_t config;                           ///< PWM core configuration
    uint32_t freq;                                 ///< Working frequency
    const ftm_chnl_pwm_signal_param_t *chnlParams; ///< List of channel parameters
    clock_name_t srcClock;                         ///< SourceClock
    uint8_t numOfChnls;                            ///< Number of channels in chnlParams
} tsPwmCoreParams;

/** @brief  Create information for a single channel for core PWM device
 *  @param  _chnl   Channel number
 *  @param  _act    Active level
 *  @param  _duty   Duty cycle value 0..100
 */
#define PWM_CHNL(_chnl, _act, _duty) _PWM_CHNL_SUB(_chnl, _act, _duty)
#define _PWM_CHNL_SUB(_chnl, _act, _duty)                          \
    {                                                              \
        .chnlNumber       = kFTM_Chnl_##_chnl,                     \
        .level            = (_act) ? kFTM_HighTrue : kFTM_LowTrue, \
        .dutyCyclePercent = (_duty) > 100 ? 100 : (_duty),         \
    }

/** @brief  Create a devCpx as PWM core which depends on an FTM
 *  @param  _name   Name of devCpx object
 *  @param  _ftm    KE04 FTM module base address
 *  @param  _freq   Frequency of output in Hz
 *  @param  ...     Channel configurations, use PWM_CHNL
 */
#define DEV_CPX_PWM_CREATE(_name, _ftm, _freq, ...)            \
    const ftm_chnl_pwm_signal_param_t _name##Chnls[] =         \
        {                                                      \
            __VA_ARGS__,                                       \
    };                                                         \
    const tsPwmCoreParams _name##Params =                      \
        {                                                      \
            _ftm,                                              \
            {                                                  \
                .prescale          = kFTM_Prescale_Divide_1,   \
                .bdmMode           = kFTM_BdmMode_0,           \
                .pwmSyncMode       = kFTM_SoftwareTrigger,     \
                .reloadPoints      = 0,                        \
                .faultMode         = kFTM_Fault_Disable,       \
                .faultFilterValue  = 0,                        \
                .deadTimePrescale  = kFTM_Deadtime_Prescale_1, \
                .deadTimeValue     = 0,                        \
                .extTriggers       = 0,                        \
                .chnlInitState     = 0,                        \
                .chnlPolarity      = 0,                        \
                .useGlobalTimeBase = false,                    \
            },                                                 \
            _freq,                                             \
            _name##Chnls,                                      \
            kCLOCK_TimerClk,                                   \
            ARRAY_SIZE(_name##Chnls),                          \
    };                                                         \
    DEV_CPX_CREATE(_name, devCpxPwmCoreFuncs, &_name##Params, NULL)

/// @brief  Functions for PWM devices
INTERFACE const tsDevIoFuncs devIoPwmFuncs;

/// @brief  Device specific parameters
typedef struct
{
    const tsDevCpx *pwmCore; ///< devCpx PWM core that controls this channel
    uint32_t data;           ///< Last devIoPut data
    uint8_t index;           ///< Index number of target channel on PWM core
} tsPwmParams;

/** @brief  Create a devIo
 *  @param  _name   Name of devIo object
 *  @param  _core   Name of target devCpx pwm core object
 *  @param  _idx    Index number of output channel on _core device
 */
#define DEV_IO_PWM_CREATE(_name, _core, _idx) \
    tsPwmParams _name##Params =               \
        {                                     \
            &_core,                           \
            0,                                \
            _idx,                             \
    };                                        \
    DEV_IO_CREATE(_name, devIoPwmFuncs, &_name##Params, NULL)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_PWM_H
