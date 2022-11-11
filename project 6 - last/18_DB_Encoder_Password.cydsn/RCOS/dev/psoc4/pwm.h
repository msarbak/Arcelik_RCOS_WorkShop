/** @file       psoc4/pwm.h
 *  @brief      Pulse Width Modulation device driver for CYPRESS PSOC4
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Ozkan AKGUL
 *  @date       1.11.2016
 */
#ifndef FILE_PWM_H
#define FILE_PWM_H

#include "rcos.h"

/**
 *  @defgroup   PSOC4_PWM PWM
 *  @ingroup    PSOC4_DEV_IO
 *  @brief      CYPRESS PSOC4 Pulse Width Modulation
 *  @{
 */

#ifndef FILE_PWM_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

INTERFACE const tsDevCpxFuncs devCpxPwmCoreFuncs; ///< Functions for PWM core devices

/// @brief  Device specific constants
typedef struct
{
    void (*start)(void);          ///< CYPRESS module Start function
    void (*stop)(void);           ///< CYPRESS module Stop function
    void (*writePeriod)(uint32);  ///< CYPRESS module WritePeriod function
    void (*writeCompare)(uint32); ///< CYPRESS module WriteCompare function
} tsPwmCoreConsts;

/// @brief  Device specific parameters
typedef struct
{
    uint32_t clock; ///< Input clock of module
    uint32_t freq;  ///< Configured frequency
    uint32_t duty;  ///< Configured duty cycle
} tsPwmCoreParams;

/// @brief  Sub device constants
typedef struct
{
    const tsDevCpx *mainDevice;
    tsPwmCoreParams *mainParams;
} tsPwmSubConsts;

/** @brief  Create a PWM core devCpx
 *  @param  _name       Name of devIO object
 *  @param  _cyName     Name of CYPRESS module
 *  @param  _clockHz    Input clock speed of module
 *  @param  _freqHz     Target frequency to modulate duty
 *  @param  _duty       Default duty cycle of the output signal
 */
#define DEV_CPX_PWM_CORE_CREATE(_name, _cyName, _clockHz, _freqHz, _duty) \
    const tsPwmCoreConsts _name##Consts =                                 \
        {                                                                 \
            _cyName##_Start,                                              \
            _cyName##_Stop,                                               \
            _cyName##_WritePeriod,                                        \
            _cyName##_WriteCompare,                                       \
    };                                                                    \
    tsPwmCoreParams _name##Params =                                       \
        {                                                                 \
            _clockHz,                                                     \
            _freqHz,                                                      \
            _duty,                                                        \
    };                                                                    \
    DEV_CPX_CREATE(_name, devCpxPwmCoreFuncs, &_name##Params, &_name##Consts)

INTERFACE const tsDevIoFuncs devIoPwmDutyFuncs; ///< Functions for PWM Duty devices

/** @brief  Create a PWM_DUTY devIO
 *  @param  _name   Name of devIO object
 *  @param  _devCpx Target devCpx that is a PWM_CORE
 */
#define DEV_IO_PWM_DUTY_CREATE(_name, _devCpx) \
    const tsPwmSubConsts _name##Consts =       \
        {                                      \
            &_devCpx,                          \
            &_devCpx##Params,                  \
    };                                         \
    DEV_IO_CREATE(_name, devIoPwmDutyFuncs, &_devCpx##Params, &_name##Consts)

INTERFACE const tsDevIoFuncs devIoPwmFreqFuncs; ///< Functions for PWM Frequency devices

/** @brief  Create a PWM_FREQ devIO
 *  @param  _name   Name of devIO object
 *  @param  _devCpx Target devCpx that is a PWM_CORE
 */
#define DEV_IO_PWM_FREQ_CREATE(_name, _devCpx) \
    const tsPwmSubConsts _name##Consts =       \
        {                                      \
            &_devCpx,                          \
            &_devCpx##Params,                  \
    };                                         \
    DEV_IO_CREATE(_name, devIoPwmFreqFuncs, &_devCpx##Params, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_PWM_H
