/** @file       spi.h
 *  @brief      Header file of SPI device
 *  @copyright  (c) 2021-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Hamed Mohammadi
 *  @date       2/9/2021
 */
#ifndef FILE_SPI_H
#define FILE_SPI_H

/// Includes
#include "rcos.h"
#include <avr/io.h>
#include <string.h>

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_SPI_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   SPI SPI
 *  @ingroup    DEV_COM
 *  @brief      Master Serial Peripheral Interface for Microchip AVR DA
 *  @{
 */
#define MISO_PIN (consts->mosiPin + 1)
#define SCK_PIN (consts->mosiPin + 2)

#define PORTMUX_SPI00 PORTMUX_SPI0_DEFAULT_gc
#define PORTMUX_SPI01 PORTMUX_SPI0_ALT1_gc
#define PORTMUX_SPI10 PORTMUX_SPI1_DEFAULT_gc
#define PORTMUX_SPI11 PORTMUX_SPI1_ALT1_gc

#define TX_BUFFER_SIZE 2
#define RX_BUFFER_SIZE 3

/// @brief  Functions for SPI devices
INTERFACE const tsDevComFuncs devComSpiFuncs;

/// @brief  Device specific parameters
typedef struct
{
    tsDeque *rxQue; ///< Receive queue
    tsDeque *txQue; ///< Transmit queue
} tsSpiParams;

/// @brief  Device specific constants
typedef struct
{
    SPI_t *base;            ///< Pointer for the SPI peripheral
    PORT_t *port;           ///< Selected port for SPI
    const uint8_t divider;  ///< Clock divider
    const uint8_t mode;     ///< SPI Mode
    const uint8_t mosiPin;  ///< MOSI pin number
    const uint8_t altValue; ///< Value for alternative pin selection register
} tsSpiConsts;

/** @brief  Create a devCom
 *  @param  _name           Name of devCom object
 *  @param  _base           Name of the base SPI peripheral (SPI0, SPI1, ...)
 *  @param  _mode           SPI mode of operation (0, 1, 2, 3)
 *  @param  _port           Name of the port with SPI pins on it (A, B, C, ...)
 *  @param  _mosiPin        Number of MOSI pin on the given port
 *  @param  _alt            Using default or alternative SPI pins (0: default, 1: alternative)
 *  @param  _divider        Clock divider for the SPI peripheral (4, 16, 64, 128)
 *  @param  _rxBufferSize   Size of receive buffer
 *  @param  _txBufferSize   Size of transmit buffer
 */
#define DEV_COM_SPI_CREATE(_name, _base, _mode, _port, _mosiPin, _alt, \
                           _divider, _rxBufferSize, _txBufferSize)     \
    const tsDevCom _name;                                              \
    ISR(_base##_INT_vect)                                              \
    {                                                                  \
        spiIsr(&_name);                                                \
    }                                                                  \
    FIFO_CREATE(_name##RxQ, _rxBufferSize)                             \
    FIFO_CREATE(_name##TxQ, _txBufferSize)                             \
    tsSpiParams _name##Params =                                        \
        {                                                              \
            &_name##RxQ,                                               \
            &_name##TxQ,                                               \
    };                                                                 \
    const tsSpiConsts _name##Consts =                                  \
        {                                                              \
            &_base,                                                    \
            &PORT##_port,                                              \
            SPI_PRESC_DIV##_divider##_gc,                              \
            SPI_MODE_##_mode##_gc,                                     \
            _mosiPin,                                                  \
            PORTMUX_##_base##_alt,                                     \
    };                                                                 \
    DEV_COM_CREATE(_name, devComSpiFuncs, &_name##Params, &_name##Consts)

/** @} */

/// @brief  Functions to handle SPI interrupts. DO NOT CALL OUTSIDE THE TARGET ISR
INTERFACE void spiIsr(const tsDevCom *device);

#undef INTERFACE // Should not let this roam free

#endif // FILE_SPI_H
