/** @file       uart.h
 *  @brief      Header file of UART device
 *  @copyright  (c) 2021-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Hamed Mohammadi
 *  @date       2/2/2021
 */
#ifndef FILE_UART_H
#define FILE_UART_H

/// Includes
#include "rcos.h"
#include <avr/io.h>

/// Interface definition for this file,
/// there should not be any includes after this define.
#ifndef FILE_UART_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/**
 *  @defgroup   UART UART
 *  @ingroup    DEV_COM
 *  @brief      UART device for Microchip AVR DA
 *  @{
 */

#ifndef F_CPU
#error "You need to define F_CPU as the main clock frequency for the baud rate calculation to work properly"
#endif

/// Sub macros to
#define ROUTE_USART0 USARTROUTEA
#define ROUTE_USART1 USARTROUTEA
#define ROUTE_USART2 USARTROUTEA
#define ROUTE_USART3 USARTROUTEA
#define ROUTE_USART4 USARTROUTEB
#define ROUTE_USART5 USARTROUTEB
#define UART_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)

/// @brief  Functions for UART devices
INTERFACE const tsDevComFuncs devComUartFuncs;

/// @brief  Device specific parameters
typedef struct
{
    tsDeque *rxQue; ///< Receive queue
    tsDeque *txQue; ///< Transmit queue
} tsUartParams;

/// @brief  Device specific constants
typedef struct
{
    USART_t *base;                 ///< Peripheral base address
    const uint16_t baud;           ///< Peripheral baud rate
    const USART_PMODE_t parity;    ///< Peripheral parity mode
    const USART_SBMODE_t stop;     ///< Peripheral stop bit count
    PORT_t *port;                  ///< Pointer to the port used for UART
    const uint8_t rxPin;           ///< Pin number of RX pin
    const uint8_t txPin;           ///< Pin number of TX pin
    volatile uint8_t *altRegister; ///< Alternative output selection register address
    const uint8_t altValue;        ///< Alternative register value
    const uint8_t altRegPos;       ///< Alternative register position
    volatile uint8_t *pinCtrl;     ///< Pointer to pin control register for RX
} tsUartConsts;

/** @brief  Create a devCom
 *  @param  _name           Name of devCom object
 *  @param  _uart           Name of the UART device (USART0, USART1, ...)
 *  @param  _baud           Baud rate
 *  @param  _parity         Parity (ODD, EVEN, DISABLED)
 *  @param  _stop           Stop bit count (1 or 2)
 *  @param  _alternative    Alternative pins for UART (0: main pins, 1: alternative pins)
 *  @param  _port           Name of the port used for UART pins (A, B, ...)
 *  @param  _txPin          Pin number for TX
 *  @param  _rxPin          Pin number for RX
 *  @param  _rxBufferSize   Size of receive buffer
 *  @param  _txQueueSize    Size of transmit queue
 */
#define DEV_COM_UART_CREATE(_name, _uart, _baud, _parity, _stop, _alternative, \
                            _port, _txPin, _rxPin, _rxQueueSize, _txQueueSize) \
    const tsDevCom _name;                                                      \
    ISR(_uart##_RXC_vect)                                                      \
    {                                                                          \
        uartRxcIsr(&_name);                                                    \
    }                                                                          \
    ISR(_uart##_DRE_vect)                                                      \
    {                                                                          \
        uartDreIsr(&_name);                                                    \
    }                                                                          \
    FIFO_CREATE(_name##RxQ, _rxQueueSize)                                      \
    FIFO_CREATE(_name##TxQ, _txQueueSize)                                      \
    tsUartParams _name##Params =                                               \
        {                                                                      \
            &_name##RxQ,                                                       \
            &_name##TxQ,                                                       \
    };                                                                         \
    const tsUartConsts _name##Consts =                                         \
        {                                                                      \
            &_uart,                                                            \
            (uint16_t)UART_BAUD_RATE(_baud),                                   \
            USART_PMODE_##_parity##_gc,                                        \
            USART_SBMODE_##_stop##BIT_gc,                                      \
            &PORT##_port,                                                      \
            _rxPin,                                                            \
            _txPin,                                                            \
            &PORTMUX.ROUTE_##_uart,                                            \
            _alternative,                                                      \
            PORTMUX_##_uart##_gp,                                              \
            &PORT##_port##_PIN##_rxPin##CTRL,                                  \
    };                                                                         \
    DEV_COM_CREATE(_name, devComUartFuncs, &_name##Params, &_name##Consts)

/// @brief  Functions to handle UART receive and transmit complete interrupts. DO NOT CALL OUTSIDE THE TARGET ISR
INTERFACE void uartRxcIsr(const tsDevCom *device);
INTERFACE void uartDreIsr(const tsDevCom *device);

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_UART_H
