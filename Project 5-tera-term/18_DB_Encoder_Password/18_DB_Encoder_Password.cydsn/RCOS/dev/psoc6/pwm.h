/** @file       psoc6/pwm.h
 *  @brief      Pulse Width Modulation device driver for CYPRESS PSOC6
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Ali Ergun
 *  @date       3.7.2019
 */
#ifndef FILE_PWM_H
#define FILE_PWM_H

#include "rcos.h"

/**
 *  @defgroup   PSOC6_PWM PWM
 *  @ingroup    PSOC6_DEV_IO
 *  @brief      CYPRESS PSOC6 Pulse Width Modulation
 *  @{
 */

#ifndef FILE_PWM_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/** @defgroup   PSOC6_PWM_CORE CORE
 *  @brief      Core is defined as a devCpx
 *  @{
 */

INTERFACE const tsDevCpxFuncs devCpxPwmCoreFuncs; ///< Functions for PWM core devices

/// @brief  Device specific parameters
typedef struct
{
    uint32_t freq; ///< Configured frequency
    uint32_t duty; ///< Configured duty cycle
} tsPwmParams;

/// @brief  Device specific constants
typedef struct
{
    TCPWM_Type *base;                        ///< Base address of target peripheral
    uint32_t cntNum;                         ///< Counter number in the selected TCPWM
    const cy_stc_tcpwm_pwm_config_t *config; ///< Cypress peripheral configuration
    uint32_t clock;                          ///< Input clock frequency of module
    uint32_t dutyMax;                        ///< Maximum value of duty cycle
} tsPwmConsts;

/** @brief  Create a PWM core devCpx
 *  @param  _name       Name of devIO object
 *  @param  _cyName     Name of CYPRESS module
 *  @param  _clockHz    Input clock speed of module
 *  @param  _dutyMax    Maximum value for duty cycle
 *  @param  _freqHz     Initial frequency of the output signal
 *  @param  _duty       Initial duty cycle of the output signal
 */
#define DEV_CPX_PWM_CORE_CREATE(_name, _cyName, _clockHz, _dutyMax, _freqHz, _duty) \
    tsPwmParams _name##Params =                                                     \
        {                                                                           \
            .freq = (_freqHz),                                                      \
            .duty = (_duty),                                                        \
    };                                                                              \
    const tsPwmConsts _name##Consts =                                               \
        {                                                                           \
            .base    = _cyName##_HW,                                                \
            .cntNum  = _cyName##_CNT_NUM,                                           \
            .config  = &_cyName##_config,                                           \
            .clock   = (_clockHz),                                                  \
            .dutyMax = (_dutyMax),                                                  \
    };                                                                              \
    DEV_CPX_CREATE(_name, devCpxPwmCoreFuncs, &_name##Params, &_name##Consts)

/** @} */

/** @defgroup   PSOC6_PWM_DUTY DUTY
 *  @brief      Control the duty cycle of the PWM core device
 *  @{
 */

INTERFACE const tsDevIoFuncs devIoPwmDutyFuncs; ///< Functions for PWM Duty devices

/** @brief  Create a PWM_DUTY devIO
 *  @param  _name   Name of devIO object
 *  @param  _devCpx Target devCpx that is a PWM_CORE
 */
#define DEV_IO_PWM_DUTY_CREATE(_name, _devCpx) \
    DEV_IO_CREATE(_name, devIoPwmDutyFuncs, &_devCpx##Params, &_devCpx##Consts)

/** @} */

/** @defgroup   PSOC6_PWM_FREQ FREQ
 *  @brief      Control the frequency of the PWM core device
 *  @{
 */

INTERFACE const tsDevIoFuncs devIoPwmFreqFuncs; ///< Functions for PWM Frequency devices

/** @brief  Create a PWM_FREQ devIO
 *  @param  _name   Name of devIO object
 *  @param  _devCpx Target devCpx that is a PWM_CORE
 */
#define DEV_IO_PWM_FREQ_CREATE(_name, _devCpx) \
    DEV_IO_CREATE(_name, devIoPwmFreqFuncs, &_devCpx##Params, &_devCpx##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_PWM_H
