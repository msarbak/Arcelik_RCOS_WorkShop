/** @file       ke02/gpio.h
 *  @brief      General Purpose I/O device driver for NXP KE02
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
#include "fsl_gpio.h"

/**
 *  @defgroup   KE02_GPIO GPIO
 *  @ingroup    KE02_DEV_IO
 *  @brief      NXP KE02 General Purpose I/O
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
    uint8_t active;  ///< Active level of gpio 0: low, 1: high
    uint8_t output;  ///< This is an output pin 0: input, 1: output
    uint8_t initial; ///< Initial value for the pin if it is an output
} tsGpioConfig;

/// @brief  Device specific parameters
typedef struct
{
    gpio_port_num_t port; ///< Port number of the pin
    uint8_t pin;          ///< Pin number of the pin
    tsGpioConfig config;  ///< Current configuration
} tsGpioParams;

/** @brief  Create a GPIO object
 *  @param  _name       Name of devIo object
 *  @param	_identifier Name used as identifier in MCUXpresso Config Tool
 *  @param  _active     Active level for this GPIO(0:active low, 1:active high)
 *  @param	_output	    TRUE if _output
 *  @param	_initial    Initial value to output before configuration
 */
#define DEV_IO_GPIO_CREATE(_name, _identifier /*_port, _pin*/, _active, _output, _initial) \
    tsGpioParams _name##Params =                                                           \
        {                                                                                  \
            BOARD_INITPINS_##_identifier##_GPIO_PORT /*(_port)*/,                          \
            BOARD_INITPINS_##_identifier##_PIN /*(_pin)*/,                                 \
            {(_active) ? 1 : 0, (_output) ? 1 : 0, (_initial) ? 1 : 0}};                   \
    DEV_IO_CREATE(_name, devIoGpioFuncs, &_name##Params, NULL)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_GPIO_H
