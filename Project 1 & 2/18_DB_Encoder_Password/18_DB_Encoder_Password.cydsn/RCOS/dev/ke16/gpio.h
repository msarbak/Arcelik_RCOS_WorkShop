/** @file       ke16/gpio.h
 *  @brief      General Purpose I/O device driver for NXP KE16
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       5.11.2018
 *  @author     Özkan AKGÜL
 *  @date       18.04.2021
 */
#ifndef FILE_GPIO_H
#define FILE_GPIO_H

#include "rcos.h"
#include <fsl_gpio.h>

/**
 *  @defgroup   KE16_GPIO GPIO
 *  @ingroup    KE16_DEV_IO
 *  @brief      NXP KE16 General Purpose I/O
 *  @{
 */

#ifndef FILE_GPIO_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for GPIO devices
extern const tsDevIoFuncs devIoGpioFuncs;

typedef struct
{
    uint8_t input;
    uint8_t output;
    uint8_t active;
} tsGpioConfig;

/// @brief  Device specific parameters
typedef struct
{
    GPIO_Type *port;
    uint8_t pin;
    uint8_t initial;
    tsGpioConfig config;
} tsGpioParams;

/** @brief  Create a GPIO object
 *  @param  _name       Name of devIo object
 *  @param	_identifier Name used as identifier in MCUXpresso Config Tool
 *  @param	_input		TRUE if input
 *  @param	_initial	Initial value to output before configuration
 *  @param  _active 	Active level for this GPIO(0:active low, 1:active high)
 */
#define DEV_IO_GPIO_CREATE(_name, _identifier/*_port, _pin*/, _input, _initial, _active) \
    tsGpioParams _name##Params =                                          \
        {                                                                 \
/*INITPINS_*/BOARD_INITPINS_##_identifier##_GPIO,                      \
/*INITPINS_*/BOARD_INITPINS_##_identifier##_PIN,                       \
			(_initial),													  \
            {(_input) ? 1 : 0, (_input) ? 0 : 1, (_active) ? 1 : 0}};     \
    const tsDevIo _name = {&devIoGpioFuncs, &_name##Params};

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_GPIO_H
