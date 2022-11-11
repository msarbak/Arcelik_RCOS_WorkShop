/** @file       gpio.h
 *  @brief      General Purpose I/O device driver for Microchip AVR DA
 *  @copyright  (c) 2021-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Hamed Mohammadi
 *  @date       1/28/2021
 */
#ifndef FILE_GPIO_H
#define FILE_GPIO_H

/// Includes
#include "rcos.h"
#include <avr/io.h>

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_GPIO_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   AVR_GPIO GPIO
 *  @ingroup    AVR_DEV_IO
 *  @brief      AVR DA General Purpose I/O
 *  @{
 */

/// Pin direction macros
#define INPUT 0
#define OUTPUT 1

/// @brief  Functions for GPIO devices
INTERFACE const tsDevIoFuncs devIoGpioFuncs;

/// @brief  Device specific constants
typedef struct
{
    PORT_t *port;            ///< Pointer to the used port
    const uint8_t pin;       ///< Pin number on the port
    const uint8_t direction; ///< Pin direction 0: input, 1: output
    const uint8_t active;    ///< Activity level of the pin 0: low, 1: high
} tsGpioConsts;

/** @brief  Create a devIo
 *  @param  _name       Name of devIo object
 *  @param  _port       Name of the port (A, B, C, D, E, F)
 *  @param  _pin        Pin number
 *  @param  _direction  Pin direction (INPUT, OUTPUT)
 *  @param  _active     Active level for this GPIO(0:active low, 1:active high)
 */
#define DEV_IO_GPIO_CREATE(_name, _port, _pin, _direction, _active) \
    const tsGpioConsts _name##Consts =                              \
        {                                                           \
            &PORT##_port,                                           \
            _pin,                                                   \
            (_direction) ? 1 : 0,                                   \
            (_active) ? 1 : 0,                                      \
    };                                                              \
    DEV_IO_CREATE(_name, devIoGpioFuncs, NULL, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_GPIO_H
