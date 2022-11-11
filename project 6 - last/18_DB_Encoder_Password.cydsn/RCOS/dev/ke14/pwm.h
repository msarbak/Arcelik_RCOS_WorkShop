/** @file       ke14/pwm.h
 *  @brief      PWM mode of FTM timer for NXP KE14
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       19.07.2019
 *  @author     Özkan AKGÜL
 *  @date       31.01.2019
 */
#ifndef FILE_PWM_H
#define FILE_PWM_H

#include "rcos.h"
#include "fsl_ftm.h"

/**
 *  @defgroup   KE14_PWM PWM
 *  @ingroup    KE14_DEV_IO
 *  @brief      PWM mode of FTM timer for NXP KE14
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
    FTM_Type *base;
    ftm_config_t config;
    uint32_t freq;
    ftm_chnl_pwm_signal_param_t *chnlParams;
    clock_name_t srcClock;
    uint8_t numOfChnls;
} tsPwmCoreParams;

/** @brief  Create information for a single channel for core PWM device
 *  @param  _chnl   Channel number
 *  @param  _act    Active level
 *  @param  _duty   Duty cycle value 0..100
 *  @param  _fEdgeDelay   use only compined mode
 */
#define PWM_CHNL(_chnl, _act, _duty, _fEdgeDelay)                  \
    {                                                              \
        .chnlNumber       = kFTM_Chnl_##_chnl,                     \
        .level            = (_act) ? kFTM_HighTrue : kFTM_LowTrue, \
        .dutyCyclePercent = (_duty) > 100 ? 100 : (_duty),         \
        .firstEdgeDelayPercent =_fEdgeDelay,                       \
    }

/** @brief  Create a devCpx as PWM core which depends on an FTM
 *  @param  _name   Name of devCpx object
 *  @param  _ftm    KE14 FTM module base address
 *  @param  _freq   Frequency of output in Hz
 *  @param  ...     Channel configurations, use PWM_CHNL
 */
#define DEV_CPX_PWM_CREATE(_name, _ftm, _freq, ...)            \
    ftm_chnl_pwm_signal_param_t _name##Chnls[] =         \
        {                                                      \
            __VA_ARGS__,                                       \
    };                                                         \
    tsPwmCoreParams _name##Params =                            \
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
            kCLOCK_CoreSysClk,                                 \
            ARRAY_SIZE(_name##Chnls),                          \
    };                                                         \
    DEV_CPX_CREATE(_name, devCpxPwmCoreFuncs, &_name##Params, NULL)

/// @brief  Functions for PWM devices
INTERFACE const tsDevIoFuncs devIoPwmFuncs;

/// @brief  Device specific parameters
typedef struct
{
    const tsDevCpx *pwmCore;
    uint8_t channel;
} tsPwmParams;

/** @brief  Create a devIo
 *  @param  _name   Name of devIo object
 *  @param  _core   Name of target devCpx pwm core object
 *  @param  _chnl   Channel number of PWM
 */
#define DEV_IO_PWM_CREATE(_name, _core, _chnl) \
    tsPwmParams _name##Params =                \
        {                                      \
            &_core,                            \
            _chnl,                             \
    };                                         \
    DEV_IO_CREATE(_name, devIoPwmFuncs, &_name##Params, NULL)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_PWM_H
