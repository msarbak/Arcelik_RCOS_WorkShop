/** @file       ke04/uart.h
 *  @brief      KE04 Universal Asynchronous Receiver-Transmitter
 *  @copyright  (c) 2019-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       30.07.2019
 */
#ifndef FILE_UART_H
#define FILE_UART_H

#include "rcos.h"
#include "fsl_uart.h"

/**
 *  @defgroup   KE04_UART UART
 *  @ingroup    KE04_DEV_COM
 *  @brief      KE04 Universal Asynchronous Receiver-Transmitter
 *  @{
 */

#ifndef FILE_UART_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for UART devices
INTERFACE const tsDevComFuncs devComUartFuncs;

/// @brief  Device specific parameters
typedef struct
{
    UART_Type *base;             ///< Peripheral base address
    const uart_config_t *config; ///< Default configuration
    uint8_t *rxBuffer;           ///< Receive ring buffer area
    uint32_t rxBufferSize;       ///< Receive ring buffer size
    tsDeque *txQue;              ///< Transmit queue
    uart_handle_t handle;        ///< Handle to hold peripheral information
    uart_transfer_t xfer;        ///< Transfer information
} tsUartParams;

/** @brief  Create a devCom
 *  @param  _name       Name of devCom object
 *  @param  _base       Peripheral base address
 *  @param  _baud       Baudrate
 *  @param  _parity     Even, Odd or Disabled
 *  @param  _stop       Stop bit count: One or Two
 *  @param  _rxQueSize  Receive queue size
 *  @param  _txQueSize  Transmit queue size
 */
#define DEV_COM_UART_CREATE(_name, _base, _baud, _parity, _stop, _rxQueSize, _txQueSize)                              \
    uint8_t _name##RxBuffer[_rxQueSize] = {0};                                                                        \
    FIFO_CREATE(_name##TxQ, _txQueSize)                                                                               \
    const uart_config_t _name##Cfg =                                                                                  \
        {                                                                                                             \
            /* FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT */                                                        \
            .stopBitCount = kUART_##_stop##StopBit, /*!< Number of stop bits, 1 stop bit (default) or 2 stop bits  */ \
            .baudRate_Bps = (_baud),                /* UART baud rate  */                                             \
            .parityMode   = kUART_Parity##_parity,  /* Parity mode, disabled (default), even, odd */                  \
            .idleType     = kUART_IdleTypeStartBit, /* IDLE type select. */                                           \
            .enableTx     = 1,                      /* Enable TX */                                                   \
            .enableRx     = 1,                      /* Enable RX */                                                   \
    };                                                                                                                \
    tsUartParams _name##Params =                                                                                      \
        {                                                                                                             \
            (_base),                     /* base */                                                                   \
            &_name##Cfg,                 /* config */                                                                 \
            _name##RxBuffer,             /* rxBuffer */                                                               \
            ARRAY_SIZE(_name##RxBuffer), /* rxBufferSize */                                                           \
            &_name##TxQ,                 /* txQue */                                                                  \
            {0},                         /* handle */                                                                 \
            {0},                         /* xfer */                                                                   \
    };                                                                                                                \
    DEV_COM_CREATE(_name, devComUartFuncs, &_name##Params, NULL)

#undef INTERFACE // Should not let this roam free

/** @} */

#endif // FILE_UART_H
