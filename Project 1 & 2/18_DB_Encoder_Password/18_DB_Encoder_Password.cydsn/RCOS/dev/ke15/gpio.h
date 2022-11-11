/** @file       ke15/gpio.h
 *  @brief      General Purpose I/O device driver for NXP KE15
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       5.11.2018
 */
#ifndef FILE_GPIO_H
#define FILE_GPIO_H

#include "rcos.h"
#include <fsl_gpio.h>

/**
 *  @defgroup   KE15_GPIO GPIO
 *  @ingroup    KE15_DEV_IO
 *  @brief      NXP KE15 General Purpose I/O
 *  @{
 */

#ifndef FILE_GPIO_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for GPIO devices
INTERFACE const tsDevIoFuncs devIoGpioFuncs;

/// @brief  Configuration object of a GPIO device
typedef struct
{
    uint8_t input;  ///< Pin is an input
    uint8_t output; ///< Pin is an output
    uint8_t active; ///< Activity level of the pin
} tsGpioConfig;

/// @brief  Device specific parameters
typedef struct
{
    GPIO_Type *port;     ///< Pointer to port peripheral registries
    uint8_t pin;         ///< Pin number
    uint8_t initial;     ///< Initial value of the pin if it is an output
    tsGpioConfig config; ///< Configuration of the pin
} tsGpioParams;

/** @brief  Create a GPIO object
 *  @param  _name       Name of devIo object
 *  @param	_identifier Name used as identifier in MCUXpresso Config Tool
 *  @param	_input		TRUE if input
 *  @param	_initial	Initial value to output before configuration
 *  @param  _active 	Active level for this GPIO(0:active low, 1:active high)
 */
#define DEV_IO_GPIO_CREATE(_name, _identifier /*_port, _pin*/, _input, _initial, _active) \
    tsGpioParams _name##Params =                                                          \
        {                                                                                 \
            /*INITPINS_*/ BOARD_##_identifier##_GPIO,                                     \
            /*INITPINS_*/ BOARD_##_identifier##_PIN,                                      \
            (_initial),                                                                   \
            {(_input) ? 1 : 0, (_input) ? 0 : 1, (_active) ? 1 : 0},                      \
    };                                                                                    \
    DEV_IO_CREATE(_name, devIoGpioFuncs, &_name##Params, NULL)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_GPIO_H
