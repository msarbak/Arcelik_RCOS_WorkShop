/** @file       ke14/fmo.h
 *  @brief      Frequency Modulation Output(Frequency oriented PWM)
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       9.11.2018
 *  @author     Özkan Akgül
 *  @date       14.11.2019
 */
#ifndef FILE_FMO_H
#define FILE_FMO_H

#include "rcos.h"

/**
 *  @defgroup   KE14_FMO FMO
 *  @ingroup    KE14_DEV_IO
 *  @brief      NXP MKE14 Frequency Modulation Output(Frequency oriented PWM)
 *  @{
 */

#ifndef FILE_FMO_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for FMO devices
INTERFACE const tsDevIoFuncs devIoFmoFuncs;

/// @brief  Device specific configuration
typedef struct
{
    uint32_t duty; ///< Requested duty cycle
} tsFmoConfig;

/// @brief  Device specific parameters
typedef struct
{
    tsDevIo *pwmDevice;   // pwm device configurations.
    tsDevCpx *pwmCore;
    uint32_t freq;
    uint32_t duty;  ///< Configured duty cycle
} tsFmoParams;

/** @brief  Create a FMO devIO
 *  @param  _name       Name of devIO object 
 *  @param  _freq       Output pwm frequency
 *  @param  _duty       Duty cycle of the output signal
 *  @param  _pwmCore    Device that control all pwm outputs
 *  @param  _pwmDevice  Device that pwm signals put
 */
#define DEV_IO_FMO_CREATE(_name, _freq, _duty, _pwmCore, _pwmDevice)  \
    tsFmoParams _name##Params =                                  \
        {                                                        \
            _pwmDevice,                                          \
            _pwmCore,                                            \
            (_freq),                                             \
            (_duty),                                             \
    };                                                           \
    tsDevIo _name = {&devIoFmoFuncs, &_name##Params};

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_FMO_H
