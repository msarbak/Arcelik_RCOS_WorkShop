/** @file       psoc4/gpio.h
 *  @brief      General Purpose I/O device driver for CYPRESS PSOC4
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

/**
 *  @defgroup   PSOC4_GPIO GPIO
 *  @ingroup    PSOC4_DEV_IO
 *  @brief      CYPRESS PSOC4 General Purpose I/O
 *  @{
 */

#ifndef FILE_GPIO_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for GPIO devices
INTERFACE const tsDevIoFuncs devIoGpioFuncs;

/// @brief  CYPRESS module constants
typedef struct
{
    uint8 (*read)(void);  ///< Read function for the pin
    void (*write)(uint8); ///< Write function for the pin
    uint8_t active;       ///< Activity level of the pin 0: low, 1: high
} tsGpioConsts;

/** @brief  Create a GPIO object
 *  @param  _name   Name of devIo object
 *  @param  _cyName Name of CYPRESS module
 *  @param  _active Active level for this GPIO(0:active low, 1:active high)
 */
#define DEV_IO_GPIO_CREATE(_name, _cyName, _active) \
    const tsGpioConsts _name##Consts =              \
        {                                           \
            &_cyName##_Read,                        \
            &_cyName##_Write,                       \
            (_active) ? 1 : 0,                      \
    };                                              \
    DEV_IO_CREATE(_name, devIoGpioFuncs, NULL, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_GPIO_H
