/** @file       twi.h
 *  @brief      Header file of TWI device
 *  @copyright  (c) 2021-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Hamed Mohammadi
 *  @date       17/2/2021
 */
#ifndef FILE_TWI_H
#define FILE_TWI_H

/// Includes
#include "rcos.h"
#include <avr/io.h>

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_TWI_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   TWI TWI
 *  @ingroup    DEV_COM
 *  @brief      TWI device for Microchip AVR DA
 *  @{
 */

#ifndef F_CPU
#error "You need to define F_CPU as the main clock frequency for the baud rate calculation to work properly"
#endif

/// Sub macros to
#define TWI_BAUD(F_SCL, T_RISE) (uint8_t)((((((float)F_CPU / (float)(F_SCL)) - 10 - (((float)(F_CPU) * (T_RISE)) / 1000000.0))) / 2))

/// @brief  Functions for TWI devices
INTERFACE const tsDevComFuncs devComTwiFuncs;

/// @brief  Device specific parameters
typedef struct
{
    uint8_t *buffer;       ///< Xfer buffer
    uint8_t length;        ///< Xfer buffer length
    uint8_t currentIndex;  ///< Xfer buffer currentIndex
    uint8_t slaveAddress;  ///< Slave Address
    uint8_t repeatSend;    ///< Repeat send without resend start condition
} tsTwiParams;

/// @brief  Device specific constants
typedef struct
{
    TWI_t *base;               ///< Peripheral base address
    const uint8_t baud;        ///< Peripheral baud rate
    const uint8_t altPosition; ///< Alternative output bit position
    const uint8_t altValue;    ///< Alternative register value
} tsTwiConsts;

/** @brief  Create a devCom
 *  @param  _name           Name of devCom object
 *  @param  _base           Name of the TWI device (TWI0, TWI1, ...)
 *  @param  _baud           Baud rate in Hz
 *  @param  _alternative    Alternative pins for TWI (0: main pins, 1: alternative 1, ...)
 */
#define DEV_COM_TWI_CREATE(_name, _base, _baud, _alternative) \
    const tsDevCom _name;                                     \
    ISR(_base##_TWIM_vect)                                    \
    {                                                         \
        twiIsr(&_name);                                       \
    }                                                         \
    tsTwiParams _name##Params =                               \
        {                                                     \
            NULL,                                             \
            0,                                                \
            0,                                                \
            0,                                                \
            FALSE,                                            \
    };                                                        \
    const tsTwiConsts _name##Consts =                         \
        {                                                     \
            &_base,                                           \
            TWI_BAUD(_baud, 0.3),                             \
            PORTMUX_##_base##_gp,                             \
            _alternative,                                     \
    };                                                        \
    DEV_COM_CREATE(_name, devComTwiFuncs, &_name##Params, &_name##Consts)

/// @brief  Functions to handle TWI receive and transmit complete interrupts. DO NOT CALL OUTSIDE THE TARGET ISR
INTERFACE void twiIsr(const tsDevCom *device);

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_TWI_H
