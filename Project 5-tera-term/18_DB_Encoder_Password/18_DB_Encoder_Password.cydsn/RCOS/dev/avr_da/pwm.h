/** @file       pwm.h
 *  @brief      Header file of PWM device
 *  @copyright  (c) 2021-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Hamed Mohammadi
 *  @date       2/8/2021
 */
#ifndef FILE_PWM_H
#define FILE_PWM_H

/// Includes
#include "rcos.h"

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_PWM_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   PWM PWM
 *  @ingroup    DEV_CPX
 *  @brief      Pulse Width Modulation device driver for Microchip AVR DA
 *  @{
 */

/// @brief  Functions for PWM devices
INTERFACE const tsDevCpxFuncs devCpxPwmCoreFuncs; ///< Functions for PWM core devices

/// @brief  Device specific constants
typedef struct
{
    TCA_SINGLE_t *base;                    ///< Base address for the timer
    PORT_t *port;                          ///< Pointer for the used port
    volatile uint16_t *cmp;                ///< Compare value pointer
    const TCA_SINGLE_CLKSEL_t dividerMask; ///< Clock divider mask
    const uint8_t divider;                 ///< Clock divider
    const uint8_t outputMux;               ///< Multiplexer value for the selected channel
    const uint8_t pin;                     ///< Pin number of the output channel
    const uint8_t active;                  ///< Active level
    const uint8_t channelMask;             ///< Mask for the selected channel
} tsPwmCoreConsts;

/// @brief  Device specific parameters
typedef struct
{
    uint16_t freq; ///< Configured frequency
    uint8_t duty;  ///< Configured duty cycle
} tsPwmCoreParams;

/** @brief  Create a PWM core devCpx
 *  @param  _name       Name of devCPX object
 *  @param  _base       Name of the used timer / counter (TCA0, TCA1)
 *  @param  _port       Name of the output port (A, B, C, D, E, F)
 *  @param  _pin        Pin number of the output
 *  @param  _channel    Waveform Output channel number (0, 1, 2)
 *  @param  _divider    Input clock divider (1, 2, 4, 8, 16, 64, 256, 1024)
 *  @param  _active     Active level (0: active low, 1: active high)
 *  @param  _freq       Initial frequency in Hz
 *  @param  _duty       Initial duty (0 ... 100)
 */
#define DEV_CPX_PWM_CORE_CREATE(_name, _base, _port, _pin, _channel, \
                                _divider, _active, _freq, _duty)     \
    const tsPwmCoreConsts _name##Consts =                            \
        {                                                            \
            &_base.SINGLE,                                           \
            &PORT##_port,                                            \
            &_base.SINGLE.CMP##_channel,                             \
            TCA_SINGLE_CLKSEL_DIV##_divider##_gc,                    \
            _divider,                                                \
            PORTMUX_##_base##_PORT##_port##_gc,                      \
            _pin,                                                    \
            _active ? 1 : 0,                                         \
            TCA_SINGLE_CMP##_channel##EN_bm,                         \
    };                                                               \
    tsPwmCoreParams _name##Params =                                  \
        {                                                            \
            _freq,                                                   \
            _duty,                                                   \
    };                                                               \
    DEV_CPX_CREATE(_name, devCpxPwmCoreFuncs, &_name##Params, &_name##Consts)

INTERFACE const tsDevIoFuncs devIoPwmDutyFuncs; ///< Functions for PWM Duty devices

/** @brief  Create a PWM_DUTY devIO
 *  @param  _name   Name of devIO object
 *  @param  _devCpx Target devCpx that is a PWM_CORE
 */
#define DEV_IO_PWM_DUTY_CREATE(_name, _devCpx) \
    DEV_IO_CREATE(_name, devIoPwmDutyFuncs, &_devCpx##Params, &_devCpx##Consts)

INTERFACE const tsDevIoFuncs devIoPwmFreqFuncs; ///< Functions for PWM Frequency devices

/** @brief  Create a PWM_FREQ devIO
 *  @param  _name   Name of devIO object
 *  @param  _devCpx Target devCpx that is a PWM_CORE
 */
#define DEV_IO_PWM_FREQ_CREATE(_name, _devCpx) \
    DEV_IO_CREATE(_name, devIoPwmFreqFuncs, &_devCpx##Params, &_devCpx##Consts)

#undef INTERFACE // Should not let this roam free

#endif // FILE_PWM_H
