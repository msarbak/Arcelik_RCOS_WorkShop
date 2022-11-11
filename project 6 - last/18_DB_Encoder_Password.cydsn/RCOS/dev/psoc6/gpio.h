/** @file       psoc6/gpio.h
 *  @brief      General Purpose I/O device driver for CYPRESS PSOC6
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Ali Ergun
 *  @date       28.06.2019
 */
#ifndef FILE_GPIO_H
#define FILE_GPIO_H

#include "rcos.h"

/**
 *  @defgroup   PSOC6_GPIO GPIO
 *  @ingroup    PSOC6_DEV_IO
 *  @brief      CYPRESS PSOC6 General Purpose I/O
 *  @{
 */

#ifndef FILE_GPIO_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for GPIO devices
INTERFACE const tsDevIoFuncs devIoGpioFuncs;

/// @brief  Device specific constants
typedef struct
{
    GPIO_PRT_Type *port; ///< Cypress port registries pointer
    uint32_t pin;        ///< Pin number
    uint8_t active;      ///< Active level
} tsGpioConsts;

/** @brief  Create a GPIO object
 *  @param  _name   Name of devIo object
 *  @param  _cyName Name of CYPRESS module
 *  @param  _active Active level for this GPIO(0:active low, 1:active high)
 */
#define DEV_IO_GPIO_CREATE(_name, _cyName, _active) \
    const tsGpioConsts _name##Consts =              \
        {                                           \
            _cyName##_PORT,                         \
            _cyName##_NUM,                          \
            (_active) ? 1 : 0,                      \
    };                                              \
    DEV_IO_CREATE(_name, devIoGpioFuncs, NULL, &_name##Consts)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_GPIO_H
